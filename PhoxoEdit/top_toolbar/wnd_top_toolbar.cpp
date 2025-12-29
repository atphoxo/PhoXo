#include "pch.h"
#include "PhoxoEdit.h"
#include "wnd_top_toolbar.h"
#include "zoom_slider_mapper.h"

namespace
{
    enum class ButtonText
    {
        Show,
        Hide
    };

    enum
    {
        IMAGE_OPEN = 0,
        IMAGE_NEW = 1,
        IMAGE_PASTE = 2,
        IMAGE_ZOOM_OUT = 4,
        IMAGE_ZOOM_IN = 5,
        IMAGE_FIT_VIEW = 6,
        IMAGE_ACTUAL = 7,
        IMAGE_SCANNER = 8,
        IMAGE_DELETE = 9,
        IMAGE_SAVE = 10,
        IMAGE_SAVE_AS = 11,
        IMAGE_COPY = 12,
        IMAGE_PRINT = 13,
    };

    auto CreateButton(UINT id, int image_index, ButtonText show_text = ButtonText::Show)
    {
        LanguageTextGroup   text(L"TOPBAR", id);
        CString   label = (text.size() == 3) ? text[2] : text[0];
        auto   btn = new CBCGPRibbonButton(
            id,
            label,
            image_index,
            -1,
            FALSE,
            (show_text == ButtonText::Show) ? CBCGPRibbonButton::RibbonSimplifiedOnScreenFull : CBCGPRibbonButton::RibbonSimplifiedOnScreenCompact);

        // 只有一项就是没有tip
        if (text.size() >= 2)
        {
            btn->SetToolTipText(text[0]);
            btn->SetDescription(text[1]);
        }
        return btn;
    }

    auto CreateNewButton()
    {
        auto   btn = CreateButton(ID_CREATE_IMAGE, IMAGE_NEW, ButtonText::Show);
        btn->AddSubItem(CreateButton(ID_NEW_FROM_CLIPBOARD, IMAGE_PASTE));
        btn->AddSubItem(CreateButton(ID_NEW_FROM_SCANNER, IMAGE_SCANNER));
        return btn;
    }

    auto CreateOpenButton()
    {
        auto   btn = CreateButton(ID_FILE_OPEN, IMAGE_OPEN);
        btn->SetDefaultCommand();

        // Recent Files 子菜单
        btn->AddSubItem(new CBCGPRibbonRecentFilesList(theConfig.AppText(L"recent")));
        btn->AddSubItem(new CBCGPRibbonSeparator());
        btn->AddSubItem(CreateButton(ID_CLEAR_RECENT_FILES, IMAGE_DELETE)); // 清除按钮
        return btn;
    }

    auto CreateSaveButton()
    {
        auto   btn = CreateButton(ID_FILE_SAVE, IMAGE_SAVE);
        btn->SetDefaultCommand();
        btn->AddSubItem(CreateButton(ID_FILE_SAVE_AS, IMAGE_SAVE_AS));
        btn->AddSubItem(new CBCGPRibbonSeparator());
        btn->AddSubItem(CreateButton(ID_EDIT_COPY, IMAGE_COPY));
        btn->AddSubItem(CreateButton(ID_FILE_PRINT, IMAGE_PRINT));
        return btn;
    }

    auto CreateFitViewButton()
    {
        auto   btn = CreateButton(ID_TOP_ZOOM_FIT_WINDOW, IMAGE_FIT_VIEW, ButtonText::Hide);
        btn->SetDefaultCommand();
        btn->AddSubItem(CreateButton(ID_TOP_ZOOM_ACTUAL, IMAGE_ACTUAL));
        return btn;
    }

    auto CreateZoomSlider()
    {
        auto   ctl = new CBCGPRibbonSlider(ID_TOP_ZOOM_SLIDER, 100);
        ctl->EnableProgressMode(TRUE);
        ctl->SetRange(0, ZoomSliderMapper::slider_range_max());
        return ctl;
    }

    auto CreateZoomRatioCombobox()
    {
        auto   ctl = new CBCGPRibbonComboBox(ID_TOP_ZOOM_COMBO_LIST, TRUE, 52);
        ctl->SetDropDownHeight(400); // 为了不显示滚动条
        ctl->EnableDropDownListResize(FALSE);
        ctl->SetLimitText(7);
        for (auto iter : { 10, 25, 50, 75, 100, 150, 200, 400, 800 })
        {
            CString   t;
            t.Format(L"%d%%", iter);
            ctl->AddItem(t);
        }
        return ctl;
    }
}

WndTopToolbar::WndTopToolbar()
    : m_zoom_combobox{ CreateZoomRatioCombobox() }
    , m_zoom_slider{ CreateZoomSlider() }
{
    //SetQuickAccessToolbarVisible(FALSE); // 不显示左上角那几个QAT
    //HideSingleTab(TRUE); // 就一个 Category，隐藏
}

void WndTopToolbar::Create(CWnd* parent)
{
    __super::Create(parent);
    EnableKeyTips(FALSE); // 禁止:按住Alt出现字符

    auto   category = AddCategory(L"Top", IDSVG_TOP_TOOLBAR_ICONS, 0);
    AddFileGroup(*category->AddPanel(L""));
    AddZoomGroup(*category->AddPanel(L""));
    AddUndoGroup(*category->AddPanel(L""));

    SetSimplifiedMode(TRUE);
}

double WndTopToolbar::GetComboDisplayedRatio() const
{
    CString   s = m_zoom_combobox->GetEditText();
    s.Remove(L'%');
    return _wtof(s) / 100.0;
}

double WndTopToolbar::GetRatioFromSlider() const
{
    return ZoomSliderMapper::RatioAtSlider(m_zoom_slider->GetPos());
}

void WndTopToolbar::AddFileGroup(CBCGPRibbonPanel& panel)
{
    panel.Add(CreateNewButton());
    panel.Add(CreateOpenButton());
    panel.Add(CreateSaveButton());
}

void WndTopToolbar::AddZoomGroup(CBCGPRibbonPanel& panel)
{
    panel.Add(CreateFitViewButton());
    panel.Add(m_zoom_combobox);
    panel.Add(CreateButton(ID_TOP_ZOOM_OUT, IMAGE_ZOOM_OUT, ButtonText::Hide));
    panel.Add(m_zoom_slider);
    panel.Add(CreateButton(ID_TOP_ZOOM_IN, IMAGE_ZOOM_IN, ButtonText::Hide));
}

void WndTopToolbar::AddUndoGroup(CBCGPRibbonPanel& panel)
{
    CBCGPRibbonUndoButton* pBtn1 = new CBCGPRibbonUndoButton(5343, _T("Undo"), 4, -1, L"11", L"22", L"33", CBCGPRibbonButton::RibbonSimplifiedOnScreenFull);
    panel.Add(pBtn1);
}

void WndTopToolbar::OnZoomRatioChanged(ZoomChangedBy sender)
{
    auto   canvas = theApp.GetCurrentCanvas();
    if (!canvas)
        return;

    const double   ratio = canvas->ZoomRatio();

    // 更新combobox，来自自身也更新，用户可能胡乱输入
    if (m_zoom_combobox->GetEditCtrl())
    {
        CString   s;
        s.Format(L"%d%%", (int)lround(ratio * 100));
        m_zoom_combobox->SetEditText(s);
    }

    // 更新slider
    if (sender != ZoomChangedBy::TopSlider)
    {
        m_zoom_slider->SetPos(ZoomSliderMapper::NearestSliderPos(ratio));
    }
}

void WndTopToolbar::OnObserveEvent(ObservedEvent& event)
{
    if (event.m_type == (int)AppEvent::ZoomRatioChanged)
    {
        OnZoomRatioChanged((ZoomChangedBy)event.m_param);
    }
}
