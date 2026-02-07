#include "pch.h"
#include "page_expander.h"
#include "rotate_page.h"
#include "shape_page.h"
#include "local.h"

namespace
{
    HICON BuildGroupIcon(int svg_res_id)
    {
        FCResource   res(svg_res_id, L"SVG");
        FCImage   img = phoxo::ImageHandler::Make(res.LoadSvgWithDpi(), WICNormal32bpp);
        FCColor   clr = PhoxoUtils::GetIconColor();
        phoxo::ImageFastPixel::FillRGBKeepAlpha(img, clr);
        return FCIcon::CreateIcon(img);
    }
}

_PHOXO_NAMESPACE(crop)

void PageExpander::Create(CWnd* parent, UINT placeholder_id)
{
    CreateOnPlaceHolder(parent, placeholder_id, ID_PANEL_CROP_EXT_PAGES);

    int   group = AddDialog(PanelCropText(11), IDD_PANEL_CROP_ROTATE_PAGE, RUNTIME_CLASS(RotatePage));
    SetGroupIcon(group, BuildGroupIcon(IDSVG_GROUP_ROTATE), TRUE);

    group = AddDialog(PanelCropText(10), IDD_PANEL_CROP_SHAPE_PAGE, RUNTIME_CLASS(ShapePage));
    SetGroupIcon(group, BuildGroupIcon(IDSVG_GROUP_SHAPE), TRUE);
}

_PHOXO_NAMESPACE_END
