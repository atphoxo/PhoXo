#include "pch.h"
#include "transform_page.h"
#include "local.h"
#include "main_doc.h"
#include "../tool_crop.h"
#include "./dialogs/dlg_resize_image.h"

namespace
{
    enum
    {
        ID_ROTATE_CW = 3300,  // 顺时针 90°
        ID_ROTATE_CCW = 3301, // 逆时针 90°
        ID_FLIP_HORZ = 3302,  // 左右翻转
        ID_FLIP_VERT = 3303,  // 上下翻转
    };
}

_PHOXO_NAMESPACE(crop)

IMPLEMENT_DYNCREATE(TransformPage, CBCGPDialog)

BEGIN_MESSAGE_MAP(TransformPage, CBCGPDialog)
    ON_COMMAND(ID_RESIZE_IMAGE, OnResizeImage)
    ON_COMMAND(ID_FLIP_HORZ, OnFlipHorz)
    ON_COMMAND(ID_FLIP_VERT, OnFlipVert)
    ON_COMMAND(ID_ROTATE_CW, OnRotateCW)
    ON_COMMAND(ID_ROTATE_CCW, OnRotateCCW)
    ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
END_MESSAGE_MAP()

void TransformPage::LoadButtons()
{
    BCGImageButton::ButtonInfo    buttons[] =
    {
        { &m_cw,     IDSVG_ROTATE_CW, 6 },
        { &m_ccw,    IDSVG_ROTATE_CCW, 7 },
        { &m_flip,   IDSVG_FLIP_VERT, 8 },
        { &m_mirror, IDSVG_FLIP_HORZ, 9 },
        { &m_resize, IDSVG_RESIZE, 30 },
    };
    InitButtons(buttons);
}

BOOL TransformPage::OnInitDialog()
{
    __super::OnInitDialog();
    LoadButtons();
    return TRUE;
}

void TransformPage::DoDataExchange(CDataExchange * pDX)
{
    __super::DoDataExchange(pDX);
    DDX_Control(pDX, ID_ROTATE_CW, m_cw);
    DDX_Control(pDX, ID_ROTATE_CCW, m_ccw);
    DDX_Control(pDX, ID_FLIP_HORZ, m_mirror);
    DDX_Control(pDX, ID_FLIP_VERT, m_flip);
    DDX_Control(pDX, ID_RESIZE_IMAGE, m_resize);
}

LRESULT TransformPage::OnChangeVisualManager(WPARAM wp, LPARAM lp)
{
    LoadButtons();
    return __super::OnChangeVisualManager(wp, lp);
}

void TransformPage::OnResizeImage()
{
    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        DlgResizeImage   dlg(canvas->Size());
        if (dlg.DoModal() == IDOK)
        {
            ToolCrop::SetCropOnCanvas(CRect());
            theRuntime.GetActiveDoc()->Execute(make_unique<phoxo::CmdResizeImage>(*canvas, dlg.GetScaledSize(), LanguageText::Get(L"cmd", L"resize")));
        }
    }
}

void TransformPage::OnFlipHorz()
{
    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        ToolCrop::SetCropOnCanvas(CRect());
        theRuntime.GetActiveDoc()->Execute(make_unique<phoxo::CmdFlipHorizontalCanvas>(*canvas, LanguageText::Get(L"cmd", L"fliph")));
    }
}

void TransformPage::OnFlipVert()
{
    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        ToolCrop::SetCropOnCanvas(CRect());
        theRuntime.GetActiveDoc()->Execute(make_unique<phoxo::CmdFlipVerticalCanvas>(*canvas, LanguageText::Get(L"cmd", L"flipv")));
    }
}

void TransformPage::OnRotateCW()
{
    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        ToolCrop::SetCropOnCanvas(CRect());
        theRuntime.GetActiveDoc()->Execute(make_unique<phoxo::CmdRotate90Canvas>(*canvas, LanguageText::Get(L"cmd", L"r90")));
        AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_TOP_ZOOM_FIT_WINDOW);
    }
}

void TransformPage::OnRotateCCW()
{
    if (auto canvas = theRuntime.GetCurrentCanvas())
    {
        ToolCrop::SetCropOnCanvas(CRect());
        theRuntime.GetActiveDoc()->Execute(make_unique<phoxo::CmdRotate270Canvas>(*canvas, LanguageText::Get(L"cmd", L"r270")));
        AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_TOP_ZOOM_FIT_WINDOW);
    }
}

_PHOXO_NAMESPACE_END
