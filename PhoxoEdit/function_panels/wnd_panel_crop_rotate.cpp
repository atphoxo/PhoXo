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

        IDC_CROP_WIDTH = 3100,
        IDC_CROP_HEIGHT = 3101,

        IDC_PRESETS_COMBO = 3200,
    };

    CString LoadText(PCWSTR key)
    {
        return LanguageText::Get(L"panel_crop", key);
    }

    class RatioButton : public CBCGPButton
    {
    public:
        //CBCGPVisualManager::GetInstance()->IsDarkTheme()

        void OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState) override
        {
            CRect   rc = rect;
            rc.top = 0;
            //rc.bottom -= globalUtils.ScaleByDPI(5, this);

            uiDTFlags = (uiDTFlags & ~DT_VCENTER) | DT_BOTTOM;

            __super::OnDrawText(pDC, rc, strText, uiDTFlags, uiState);
        }
    };
}

BEGIN_MESSAGE_MAP(WndPanelCropRotate, CBCGPDialogBar)
    ON_COMMAND_RANGE(ID_CROP_BEGIN, ID_CROP_END, OnRatioButton)
END_MESSAGE_MAP()

WndPanelCropRotate::WndPanelCropRotate()
{
    for (int id = ID_CROP_FREE; id <= ID_CROP_2_3; ++id)
    {
        auto   btn = make_unique<RatioButton>();
        btn->m_bTopImage = TRUE;
        btn->m_bDrawFocus = FALSE;
        btn->m_bHighlightChecked = TRUE;
        m_ratio_buttons.try_emplace(id, std::move(btn));
    }
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

    m_presets_combo.SetItemHeight(-1, globalUtils.ScaleByDPI(20, this));
    ResetPresetsCombo();

    m_ratio_buttons[ID_CROP_FREE]->SetImage(IDSVG_CROP_FREE);
    m_ratio_buttons[ID_CROP_ORIGINAL]->SetImage(IDSVG_CROP_ORIGINAL);
    m_ratio_buttons[ID_CROP_1_1]->SetImage(IDSVG_CROP_1_1);
    m_ratio_buttons[ID_CROP_16_9]->SetImage(IDSVG_CROP_16_9);
    m_ratio_buttons[ID_CROP_3_2]->SetImage(IDSVG_CROP_3_2);
    m_ratio_buttons[ID_CROP_4_3]->SetImage(IDSVG_CROP_4_3);
    m_ratio_buttons[ID_CROP_9_16]->SetImage(IDSVG_CROP_9_16);
    m_ratio_buttons[ID_CROP_2_3]->SetImage(IDSVG_CROP_2_3);

    // EnableDocking(CBRS_ALIGN_RIGHT);

    EnableVisualManagerStyle(TRUE);
}

void WndPanelCropRotate::InitSizeEdit()
{
    m_size_edit_font.CreatePointFont(100, L"Segoe UI");

    for (auto ctrl : { &m_width_edit, &m_height_edit })
    {
        ctrl->SetFont(&m_size_edit_font);
        ctrl->SetLimitText(6);
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

void WndPanelCropRotate::ResetPresetsCombo()
{
    m_presets_combo.SetPrompt(LoadText(L"preset"));
}

void WndPanelCropRotate::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);

    for (auto& [id, btn] : m_ratio_buttons)
    {
        DDX_Control(pDX, id, *btn);
    }
    DDX_Radio(pDX, ID_CROP_FREE, m_ratio_index);
    DDX_Control(pDX, IDC_PRESETS_COMBO, m_presets_combo);
    DDX_Control(pDX, IDC_CROP_WIDTH, m_width_edit);
    DDX_Control(pDX, IDC_CROP_HEIGHT, m_height_edit);
}

void WndPanelCropRotate::OnRatioButton(UINT id)
{
    Sleep(0);
}
