#include "pch.h"
#include "page_expander.h"
#include "transform_page.h"
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

BEGIN_MESSAGE_MAP(PageExpander, CBCGPExpanderCtrl)
    ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
END_MESSAGE_MAP()

void PageExpander::Create(CWnd* parent, UINT placeholder_id)
{
    CreateOnPlaceHolder(parent, placeholder_id, ID_PANEL_CROP_EXT_PAGES);

    m_group_ids[0] = AddDialog(PanelCropText(11), IDD_PANEL_CROP_ROTATE_PAGE, RUNTIME_CLASS(TransformPage));
    m_group_ids[1] = AddDialog(PanelCropText(10), IDD_PANEL_CROP_SHAPE_PAGE, RUNTIME_CLASS(ShapePage));
    LoadGroupIcon();
}

void PageExpander::LoadGroupIcon()
{
    SetGroupIcon(m_group_ids[0], BuildGroupIcon(IDSVG_GROUP_ROTATE), TRUE);
    SetGroupIcon(m_group_ids[1], BuildGroupIcon(IDSVG_GROUP_SHAPE), TRUE);
}

LRESULT PageExpander::OnChangeVisualManager(WPARAM wp, LPARAM lp)
{
    LoadGroupIcon();
    return __super::OnChangeVisualManager(wp, lp);
}

_PHOXO_NAMESPACE_END
