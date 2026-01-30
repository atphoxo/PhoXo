#include "pch.h"
#include "shape_page.h"
#include "local.h"
#include "../tool_crop.h"

namespace
{
    enum
    {
        IDC_SHAPE_RECT = 3000,
        IDC_SHAPE_ROUND_RECT = 3001,
        IDC_SHAPE_CIRCLE = 3002,

        IDC_ROUND_RECT_TEXT = 4000,
        IDC_ROUND_RECT_NUMBER = 4001,
        IDC_ROUND_RECT_SLIDER = 4002,
    };
}

_PHOXO_NAMESPACE(crop)

IMPLEMENT_DYNCREATE(ShapePage, CBCGPDialog)

BEGIN_MESSAGE_MAP(ShapePage, CBCGPDialog)
    ON_WM_HSCROLL()
    ON_COMMAND_RANGE(IDC_SHAPE_RECT, IDC_SHAPE_CIRCLE, OnSelectShape)
END_MESSAGE_MAP()

ShapePage::ShapePage()
{
    EnableVisualManagerStyle();
    m_shape_index = (int)ToolCrop::s_crop_shape;
}

void ShapePage::InitRoundnessSlider()
{
    m_roundness.SetRange(0, 100);
    m_roundness.EnableProgressMode();
    m_roundness.SetPos(ToolCrop::s_roundness);
    m_roundness.m_bDrawFocus = FALSE;
    m_roundness.SetPositionOnClick();
    UpdateRoundnessValue();
}

BOOL ShapePage::OnInitDialog()
{
    __super::OnInitDialog();

    BCGImageButton::ButtonInfo   buttons[] =
    {
        { &m_rect, IDSVG_CROP_SHAPE_RECT, 15 },
        { &m_round_rect, IDSVG_CROP_SHAPE_ROUND_RECT, 16 },
        { &m_circle, IDSVG_CROP_SHAPE_CIRCLE, 17 },
    };
    InitButtons(buttons);
    SetDlgItemText(IDC_ROUND_RECT_TEXT, PanelCropText(20));

    InitRoundnessSlider();
    UpdateRoundRectControlsVisibility();
    return TRUE;
}

void ShapePage::UpdateRoundnessValue()
{
    SetDlgItemText(IDC_ROUND_RECT_NUMBER, FCString::From(ToolCrop::s_roundness));
}

void ShapePage::UpdateRoundRectControlsVisibility()
{
    bool   show = (m_shape_index == 1);
    for (int id : { IDC_ROUND_RECT_TEXT, IDC_ROUND_RECT_NUMBER, IDC_ROUND_RECT_SLIDER })
    {
        GetDlgItem(id)->ShowWindow(show ? SW_SHOW : SW_HIDE);
    }
}

void ShapePage::DoDataExchange(CDataExchange * pDX)
{
    __super::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SHAPE_RECT, m_rect);
    DDX_Control(pDX, IDC_SHAPE_ROUND_RECT, m_round_rect);
    DDX_Control(pDX, IDC_SHAPE_CIRCLE, m_circle);
    DDX_Radio(pDX, IDC_SHAPE_RECT, m_shape_index);
    DDX_Control(pDX, IDC_ROUND_RECT_SLIDER, m_roundness);
}

void ShapePage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    __super::OnHScroll(nSBCode, nPos, pScrollBar);

    ToolCrop::s_roundness = m_roundness.GetPos();
    UpdateRoundnessValue();
    theRuntime.InvalidateView();
}

void ShapePage::OnSelectShape(UINT id)
{
    UpdateData();

    auto   sel = (CropShape)(id - IDC_SHAPE_RECT);
    if (sel != ToolCrop::s_crop_shape)
    {
        ToolCrop::s_crop_shape = sel;
        theRuntime.InvalidateView();
    }
    UpdateRoundRectControlsVisibility();
}

_PHOXO_NAMESPACE_END
