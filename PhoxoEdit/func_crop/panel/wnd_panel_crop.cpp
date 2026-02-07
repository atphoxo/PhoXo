#include "pch.h"
#include "local.h"
#include "wnd_panel_crop.h"
#include "../tool_crop.h"
using namespace crop;

namespace
{
    enum
    {
        ID_CROP_FREE = 3000,
        ID_CROP_ORIGINAL = 3001,
        ID_CROP_1_1 = 3002,
        ID_CROP_16_9 = 3003,
        ID_CROP_3_2 = 3004,
        ID_CROP_4_3 = 3005,
        ID_CROP_9_16 = 3006,
        ID_CROP_2_3 = 3007,

        IDC_CROP_WIDTH = 3100,
        IDC_CROP_HEIGHT = 3101,
        ID_KEEP_ASPECT = 3201, // 保持宽高比按钮

        ID_APPLY_CROP = 3200,  // 应用裁剪
        ID_CANCEL_CROP = 3300, // 取消裁剪

        ID_POST_UPDATE_KEEP_ASPECT = 4000,
        ID_CROP_EXPAND_HOLDER = 6000,
    };

    int RatioButtonSvgId(UINT id)
    {
        switch (id)
        {
            case ID_CROP_FREE:      return IDSVG_CROP_FREE;
            case ID_CROP_ORIGINAL:  return IDSVG_CROP_ORIGINAL;
            case ID_CROP_1_1:       return IDSVG_CROP_1_1;
            case ID_CROP_16_9:      return IDSVG_CROP_16_9;
            case ID_CROP_3_2:       return IDSVG_CROP_3_2;
            case ID_CROP_4_3:       return IDSVG_CROP_4_3;
            case ID_CROP_9_16:      return IDSVG_CROP_9_16;
            case ID_CROP_2_3:       return IDSVG_CROP_2_3;
            default:                return 0;
        }
    }

    void SetTextAndTooltip(BCGImageButton& btn, int key)
    {
        LanguageTextGroup   text(PanelCropText(key));
        btn.SetWindowText(text[0]);
        btn.SetTooltip(text[1]);
    }

    DWORD DockStyle()
    {
        return (theConfig.m_panel_dock == PanelDock::Right) ? CBRS_RIGHT : CBRS_LEFT;
    }
}

BEGIN_MESSAGE_MAP(WndPanelCrop, CBCGPDialogBar)
    ON_COMMAND_RANGE(ID_CROP_FREE, ID_CROP_2_3, OnRatioButton)
    ON_COMMAND(ID_KEEP_ASPECT, OnKeepAspect)
    ON_COMMAND(ID_CANCEL_CROP, OnCancelCrop)
    ON_COMMAND(ID_POST_UPDATE_KEEP_ASPECT, OnPostUpdateKeepAspect)
    ON_EN_KILLFOCUS(IDC_CROP_WIDTH, OnWidthEditKillFocus)
    ON_EN_KILLFOCUS(IDC_CROP_HEIGHT, OnHeightEditKillFocus)
    ON_UPDATE_COMMAND_UI(ID_KEEP_ASPECT, OnEnableIfCanvasValid)
    ON_UPDATE_COMMAND_UI(IDC_CROP_WIDTH, OnEnableIfCanvasValid)
    ON_UPDATE_COMMAND_UI(IDC_CROP_HEIGHT, OnEnableIfCanvasValid)
    ON_UPDATE_COMMAND_UI(ID_APPLY_CROP, OnEnableIfCropValid)
    ON_UPDATE_COMMAND_UI(ID_CANCEL_CROP, OnEnableIfCanvasValid)
    ON_UPDATE_COMMAND_UI_RANGE(ID_CROP_FREE, ID_CROP_2_3, OnEnableIfCanvasValid)
END_MESSAGE_MAP()

WndPanelCrop::WndPanelCrop()
{
    EnableVisualManagerStyle();

    // 构造的时候不能设置text and tip
    for (int id = ID_CROP_FREE; id <= ID_CROP_2_3; id++)
    {
        auto&   btn = AddImageButton(id);
        btn.m_bTopImage = true;
        btn.LoadSvgWithDpi(RatioButtonSvgId(id));
    }
    AddImageButton(ID_KEEP_ASPECT);
    AddImageButton(ID_CANCEL_CROP).LoadSvgWithDpi(IDSVG_CROP_CANCEL);

    auto&   btn = AddImageButton(ID_APPLY_CROP);
    btn.m_always_default_status = true;
    btn.LoadSvgWithDpi(IDSVG_CROP_APPLY, ThemeMode::InverseBCG);
}

void WndPanelCrop::Create(CWnd* parent)
{
    CBCGPDialogBar::Create(NULL, parent,
        FALSE,                             // bHasGripper，是否显示抓手
        MAKEINTRESOURCE(IDD_PANEL_CROP),   // 对话框资源 ID
        WS_VISIBLE | WS_CHILD | DockStyle() | CBRS_HIDE_INPLACE, // left/right dock
        ID_PANEL_CROP_ROTATE,
        CBRS_BCGP_REGULAR_TABS,
        CBRS_BCGP_AUTOHIDE
    );

    SetTextAndTooltip(*m_image_buttons[ID_CROP_FREE], 1);
    SetTextAndTooltip(*m_image_buttons[ID_CROP_ORIGINAL], 2);
    m_image_buttons[ID_APPLY_CROP]->SetWindowText(PanelCropText(3));
    m_image_buttons[ID_CANCEL_CROP]->SetTooltip(PanelCropText(21));
    InitSizeEdit();
    UpdateKeepAspectButton();

    m_shape_panel.Create(this, ID_CROP_EXPAND_HOLDER);
}

BCGImageButton& WndPanelCrop::AddImageButton(int id)
{
    auto [it, _] = m_image_buttons.try_emplace(id, make_unique<BCGImageButton>());
    return *it->second;
}

void WndPanelCrop::UpdateKeepAspectButton()
{
    LanguageTextGroup   stat(PanelCropText(5));
    auto&   btn = *m_image_buttons[ID_KEEP_ASPECT];
    btn.SetTooltip(m_lock_aspect ? stat[0] : stat[1], PanelCropText(4), TRUE);
    btn.SetImageEx(PhoxoUtils::LoadSvgWithDpi(m_lock_aspect ? IDSVG_CROP_LINK : IDSVG_CROP_UNLINK, PhoxoUtils::GetIconColor()));
    btn.Invalidate();
}

void WndPanelCrop::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);

    for (const auto& [id, btn] : m_image_buttons)
    {
        DDX_Control(pDX, id, *btn);
    }
    DDX_Radio(pDX, ID_CROP_FREE, m_ratio_index);
    DDX_Control(pDX, IDC_CROP_WIDTH, m_width_edit);
    DDX_Control(pDX, IDC_CROP_HEIGHT, m_height_edit);
    // keep aspect
    DDX_Check(pDX, ID_KEEP_ASPECT, m_lock_aspect);
}

void WndPanelCrop::OnObserveEvent(ObservedEvent& event)
{
    if (event.m_type == (int)AppEvent::CropRectChanged)
    {
        UpdateSizeEdit();
    }
    else if (event.m_type == (int)AppEvent::CanvasReloaded)
    {
        OnEventCanvasReloaded();
    }
}

void WndPanelCrop::OnEventCanvasReloaded()
{
    UpdateSizeEdit();

    m_ratio_index = 0;
    m_lock_aspect = FALSE; ASSERT(!ToolCrop::s_aspect_ratio.IsLocked());
    UpdateKeepAspectButton();
    UpdateData(FALSE);
}

void WndPanelCrop::OnPostUpdateKeepAspect()
{
    UpdateKeepAspectButton();
}

void WndPanelCrop::OnEnableIfCanvasValid(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(theRuntime.GetCurrentCanvas() != NULL);
}

void WndPanelCrop::OnEnableIfCropValid(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(theRuntime.GetCurrentCanvas() && ToolCrop::HasCropRect());
}

namespace
{
    void ApplyRatioForID(UINT id, CSize canvas_size)
    {
        CSize   sz;
        switch (id)
        {
            case ID_CROP_ORIGINAL: sz = canvas_size; break;
            case ID_CROP_1_1: sz = { 1, 1 }; break;
            case ID_CROP_16_9: sz = { 16, 9 }; break;
            case ID_CROP_3_2: sz = { 3, 2 }; break;
            case ID_CROP_4_3: sz = { 4, 3 }; break;
            case ID_CROP_9_16: sz = { 9, 16 }; break;
            case ID_CROP_2_3: sz = { 2, 3 }; break;
        }
        ToolCrop::ApplyCropAspectRatio(sz.cx, sz.cy);
    }
}

void WndPanelCrop::OnRatioButton(UINT id)
{
    if ((m_ratio_index == (int)(id - ID_CROP_FREE)) && ToolCrop::HasCropRect())
        return; // no change

    auto   canvas = theRuntime.GetCurrentCanvas();
    if (!canvas)
        return;

    UpdateData();
    m_lock_aspect = (id != ID_CROP_FREE);
    if (!m_lock_aspect)
        ToolCrop::s_aspect_ratio.Unlock();
    UpdateData(FALSE);
    UpdateKeepAspectButton();

    if (id == ID_CROP_FREE)
    {
        if (!ToolCrop::HasCropRect())
        {
            ToolCrop::SetCropOnCanvas(CRect({}, canvas->Size()));
        }
        return;
    }

    ApplyRatioForID(id, canvas->Size());
}

void WndPanelCrop::OnKeepAspect()
{
    UpdateData();
    if (!m_lock_aspect)
    {
        m_ratio_index = 0;
        ToolCrop::s_aspect_ratio.Unlock();
    }
    else
    {
        if (ToolCrop::HasCropRect())
        {
            CRect   rc = ToolCrop::s_crop_on_canvas;
            ToolCrop::s_aspect_ratio.Lock(rc.Width(), rc.Height()); // lock current ratio
        }
        else
        {
            m_lock_aspect = FALSE; // 没有crop rect可锁定，保持原状态
        }
    }
    UpdateData(FALSE);

    PostMessage(WM_COMMAND, ID_POST_UPDATE_KEEP_ASPECT); // 在这里更新tip会闪烁，post后处理
}

void WndPanelCrop::OnCancelCrop()
{
    ToolCrop::SetCropOnCanvas(CRect());
}
