#include "pch.h"
#include "wnd_panel_crop_rotate.h"

namespace
{
    enum
    {
        ID_CROP_BEGIN = 2999,
        ID_CROP_FREE = 3000,
        ID_CROP_ORIGINAL = 3001,
        ID_CROP_1_1 = 3002,
        ID_CROP_16_9 = 3003,
        ID_CROP_3_2 = 3004,
        ID_CROP_4_3 = 3005,
        ID_CROP_9_16 = 3006,
        ID_CROP_2_3 = 3007,
        ID_CROP_END = 3010,

        ID_APPLY_CROP = 3050,
        ID_KEEP_ASPECT = 3051,

        IDC_CROP_WIDTH = 3100,
        IDC_CROP_HEIGHT = 3101,

        IDC_PRESETS_COMBO = 3200,

        ID_POST_UPDATE_KEEP_ASPECT = 4000,
    };

    CString LoadText(PCWSTR key)
    {
        return LanguageText::Get(L"panel_crop", key);
    }

    FCColor IconColor(ThemeMode theme = ThemeMode::FollowBCG)
    {
        using enum ThemeMode;
        if (theme == FollowBCG)
            theme = CBCGPVisualManager::GetInstance()->IsDarkTheme() ? Dark : Light;
        else if (theme == InverseBCG)
            theme = CBCGPVisualManager::GetInstance()->IsDarkTheme() ? Light : Dark;
        return FCColor{ (theme == Dark) ? 0xEEEEEE : 0x333333 };
    }
}

BEGIN_MESSAGE_MAP(WndPanelCropRotate, CBCGPDialogBar)
    ON_COMMAND_RANGE(ID_CROP_BEGIN, ID_CROP_END, OnRatioButton)
    ON_COMMAND(ID_KEEP_ASPECT, OnKeepAspect)
    ON_COMMAND(ID_POST_UPDATE_KEEP_ASPECT, OnPostUpdateKeepAspect)
END_MESSAGE_MAP()

WndPanelCropRotate::WndPanelCropRotate()
{
    std::pair<int, int>   buttons[] =
    {
        { ID_CROP_FREE,     IDSVG_CROP_FREE },
        { ID_CROP_ORIGINAL, IDSVG_CROP_ORIGINAL },
        { ID_CROP_1_1,      IDSVG_CROP_1_1 },
        { ID_CROP_16_9,     IDSVG_CROP_16_9 },
        { ID_CROP_3_2,      IDSVG_CROP_3_2 },
        { ID_CROP_4_3,      IDSVG_CROP_4_3 },
        { ID_CROP_9_16,     IDSVG_CROP_9_16 },
        { ID_CROP_2_3,      IDSVG_CROP_2_3 },
    };

    for (auto [id, svg] : buttons)
    {
        auto&   btn = AddImageButton(id);
        btn.m_bTopImage = true;
        btn.SetImageEx(PhoxoUtils::LoadSvgWithDpi(svg, { 32,32 }, IconColor()));
    }

    AddImageButton(ID_KEEP_ASPECT);

    auto&   btn = AddImageButton(ID_APPLY_CROP);
    btn.m_always_default_status = true;
    btn.SetIcon(PhoxoUtils::LoadSvgWithDpi(IDSVG_CROP_APPLY, { 18,18 }, IconColor(ThemeMode::InverseBCG)));

    m_width_edit.SetVerticalAlignment(TA_CENTER);
    m_height_edit.SetVerticalAlignment(TA_CENTER);
}

void WndPanelCropRotate::Create(CWnd* parent)
{
    DWORD   dock = (theConfig.m_panel_dock == PanelDock::Right) ? CBRS_RIGHT : CBRS_LEFT;
    CBCGPDialogBar::Create(NULL, parent,
        FALSE,                                  // bHasGripper，是否显示抓手
        MAKEINTRESOURCE(IDD_PANEL_CROP_ROTATE),   // 对话框资源 ID
        WS_VISIBLE | WS_CHILD | dock | CBRS_HIDE_INPLACE, // right dock
        ID_PANEL_CROP_ROTATE,
        CBRS_BCGP_REGULAR_TABS,
        CBRS_BCGP_AUTOHIDE
    );
    InitSizeEdit();

    m_image_buttons[ID_CROP_FREE]->SetWindowText(LoadText(L"1"));
    m_image_buttons[ID_CROP_ORIGINAL]->SetWindowText(LoadText(L"2"));
    m_image_buttons[ID_APPLY_CROP]->SetWindowText(LoadText(L"3"));
    UpdateKeepAspectButton();

    m_presets_combo.SetItemHeight(-1, DPICalculator::Cast(20));
    ResetPresetsCombo();

    EnableVisualManagerStyle(TRUE);
}

BCGImageButton& WndPanelCropRotate::AddImageButton(UINT id)
{
    auto [it, _] = m_image_buttons.try_emplace(id, make_unique<BCGImageButton>());
    return *it->second;
}

void WndPanelCropRotate::InitSizeEdit()
{
    for (auto ctrl : { &m_width_edit, &m_height_edit })
    {
        ctrl->SetLimitText(6);
        ctrl->SetVerticalAlignment(TA_CENTER);
    }
    ResetSizeEdit();
}

void WndPanelCropRotate::ResetSizeEdit()
{
    LanguageTextGroup   text(L"panel_crop", L"size");
    for (int idx = 0; auto ctrl : { &m_width_edit, &m_height_edit })
    {
        ctrl->SetWindowText(L"");
        ctrl->SetPrompt(text[idx++]);
    }
}

void WndPanelCropRotate::UpdateKeepAspectButton()
{
    LanguageTextGroup   stat(L"panel_crop", L"5");
    UINT   svg = m_keep_aspect ? IDSVG_CROP_LINK : IDSVG_CROP_UNLINK;

    auto&   btn = *m_image_buttons[ID_KEEP_ASPECT];
    btn.SetTooltip(m_keep_aspect ? stat[0] : stat[1], LoadText(L"4"), TRUE);
    btn.SetImageEx(PhoxoUtils::LoadSvgWithDpi(svg, { 20,20 }, IconColor()));
    btn.Invalidate();
}

void WndPanelCropRotate::ResetPresetsCombo()
{
    m_presets_combo.SetPrompt(LoadText(L"preset"));
}

void WndPanelCropRotate::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);

    for (auto& [id, btn] : m_image_buttons)
    {
        DDX_Control(pDX, id, *btn);
    }
    DDX_Radio(pDX, ID_CROP_FREE, m_ratio_index);
    DDX_Control(pDX, IDC_PRESETS_COMBO, m_presets_combo);
    DDX_Control(pDX, IDC_CROP_WIDTH, m_width_edit);
    DDX_Control(pDX, IDC_CROP_HEIGHT, m_height_edit);
    // keep aspect
    DDX_Check(pDX, ID_KEEP_ASPECT, m_keep_aspect);
}

void WndPanelCropRotate::OnPostUpdateKeepAspect()
{
    UpdateKeepAspectButton();
}

void WndPanelCropRotate::OnRatioButton(UINT id)
{
    bool   want_keep_aspect = (id != ID_CROP_FREE);
    if (want_keep_aspect != (bool)m_keep_aspect)
    {
        UpdateData();
        m_keep_aspect = !m_keep_aspect;
        UpdateKeepAspectButton();
        UpdateData(FALSE);
    }
}

void WndPanelCropRotate::OnKeepAspect()
{
    UpdateData();
    if (!m_keep_aspect)
        m_ratio_index = 0; // free
    UpdateData(FALSE);

    PostMessage(WM_COMMAND, ID_POST_UPDATE_KEEP_ASPECT); // 在这里更新tip会闪烁，post后处理
}
