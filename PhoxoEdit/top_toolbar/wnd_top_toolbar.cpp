#include "pch.h"
#include "PhoxoEdit.h"
#include "wnd_top_toolbar.h"
#include "zoom_slider_mapper.h"
#include "undo_button.h"
#include "theme_menu.h"

namespace
{
    enum class ButtonText
    {
        Show,
        Hide
    };

    enum
    {
        IMAGE_OPEN = 0, // 图标顺序和svg一致
        IMAGE_NEW,
        IMAGE_PASTE,
        IMAGE_ZOOM_OUT,
        IMAGE_ZOOM_IN,
        IMAGE_FIT_VIEW,
        IMAGE_ACTUAL,
        IMAGE_SCANNER,
        IMAGE_DELETE,
        IMAGE_SAVE,
        IMAGE_SAVE_AS,
        IMAGE_COPY,
        IMAGE_PRINT,
        IMAGE_UNDO,
        IMAGE_REDO,
        IMAGE_SETTINGS,
        IMAGE_THEMES,
        IMAGE_MENU,
        IMAGE_ABOUT,
    };

    CString LoadText(PCWSTR key) { return LanguageText::Get(L"TOPBAR", key); }
    CString LoadText(int key) { return LoadText(FCString::From(key)); }

    auto CreateButton(UINT id, int image_index, ButtonText show_text = ButtonText::Show)
    {
        LanguageTextSplitter   text(LoadText(id));
        const CString   label = text.Next();
        auto   btn = new CBCGPRibbonButton(
            id,
            label,
            image_index,
            -1,
            FALSE,
            (show_text == ButtonText::Show) ? CBCGPRibbonButton::RibbonSimplifiedOnScreenFull : CBCGPRibbonButton::RibbonSimplifiedOnScreenCompact);

        // 只有一项就是没有tip
        if (CString tip = text.Next(); !tip.IsEmpty())
        {
            btn->SetToolTipText(label);
            btn->SetDescription(tip);
        }
        return btn;
    }

    auto CreateNewButton()
    {
        auto   btn = CreateButton(ID_CREATE_IMAGE, IMAGE_NEW);
        btn->AddSubItem(CreateButton(ID_NEW_FROM_CLIPBOARD, IMAGE_PASTE));
        btn->AddSubItem(CreateButton(ID_NEW_FROM_SCANNER, IMAGE_SCANNER));
        return btn;
    }

    auto CreateOpenButton()
    {
        auto   btn = CreateButton(ID_FILE_OPEN, IMAGE_OPEN);
        btn->SetDefaultCommand();

        // Recent Files 子菜单
        btn->AddSubItem(new CBCGPRibbonRecentFilesList(LanguageText::Get(L"file", L"recent")));
        btn->AddSubItem(new CBCGPRibbonSeparator());
        btn->AddSubItem(CreateButton(ID_CLEAR_RECENT_FILES, IMAGE_DELETE)); // 清除按钮
        return btn;
    }

    auto CreateSaveButton()
    {
        auto   btn = CreateButton(ID_FILE_SAVE_AS, IMAGE_SAVE_AS);
        btn->SetDefaultCommand();
        btn->AddSubItem(CreateButton(ID_FILE_SAVE, IMAGE_SAVE));
        btn->AddSubItem(new CBCGPRibbonSeparator());
        btn->AddSubItem(CreateButton(ID_EDIT_COPY, IMAGE_COPY));
        btn->AddSubItem(CreateButton(ID_FILE_PRINT, IMAGE_PRINT));
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

    auto CreateUndoButton()
    {
        LanguageTextSplitter   undostr(LoadText(L"undo"));
        CString   str0 = undostr.Next();
        CString   str1 = undostr.Next();
        CString   str2 = undostr.Next();

        auto   btn = new topbar::UndoButton(ID_EDIT_UNDO, L"", IMAGE_UNDO, -1, str0, str1, str2, CBCGPRibbonButton::RibbonSimplifiedOnScreenFull);
        LanguageTextSplitter   text(LoadText(btn->GetID()));
        btn->SetToolTipText(text.Next());
        btn->SetDescription(text.Next());
        return btn;
    }

    auto CreateThemeMenu()
    {
        topbar::ThemeMenu   menu(LoadText(L"theme"));
        menu.LoadMenuWithTranslate(IDR_MENU_THEMES);

        auto   btn = CreateButton(ID_APP_THEMES, IMAGE_THEMES, ButtonText::Hide);
        btn->SetMenu(menu.GetSafeHmenu(), FALSE, FALSE, FALSE, FALSE);
        return btn;
    }
}

WndTopToolbar::WndTopToolbar()
    : m_zoom_combobox{ CreateZoomRatioCombobox() }
    , m_zoom_slider{ CreateZoomSlider() }
    , m_undo_button{ CreateUndoButton() }
{
    SetQuickAccessToolbarVisible(FALSE); // 不显示左上角那几个QAT
    HideSingleTab(TRUE); // 就一个 Category，隐藏
}

void WndTopToolbar::Create(CWnd* parent)
{
    __super::Create(parent);
    EnableKeyTips(FALSE); // Disable key tips (Alt shortcuts display)

    auto   category = AddCategory(L"Top", IDSVG_TOP_TOOLBAR_ICONS, 0);
    AddFileGroup(*category->AddPanel(L""));
    AddZoomGroup(*category->AddPanel(L""));
    AddUndoGroup(*category->AddPanel(L""));
    AddAppGroup(*category->AddPanel(L""));

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
    panel.Add(CreateButton(ID_TOP_ZOOM_FIT_WINDOW, IMAGE_FIT_VIEW, ButtonText::Hide));
    panel.Add(CreateButton(ID_TOP_ZOOM_ACTUAL, IMAGE_ACTUAL, ButtonText::Hide));
    panel.Add(m_zoom_combobox);
    panel.Add(CreateButton(ID_TOP_ZOOM_OUT, IMAGE_ZOOM_OUT, ButtonText::Hide));
    panel.Add(m_zoom_slider);
    panel.Add(CreateButton(ID_TOP_ZOOM_IN, IMAGE_ZOOM_IN, ButtonText::Hide));
}

void WndTopToolbar::AddUndoGroup(CBCGPRibbonPanel& panel)
{
    panel.Add(m_undo_button);
    panel.Add(CreateButton(ID_EDIT_REDO, IMAGE_REDO, ButtonText::Hide));
}

void WndTopToolbar::AddAppGroup(CBCGPRibbonPanel& panel)
{
    panel.Add(CreateThemeMenu());
    panel.Add(CreateButton(ID_APP_SETTINGS, IMAGE_SETTINGS, ButtonText::Hide));
    panel.Add(CreateButton(ID_APP_ABOUT, IMAGE_ABOUT, ButtonText::Hide));

    /*auto   btn = CreateButton(ID_TOPBAR_MENU, IMAGE_MENU, ButtonText::Hide);
    btn->SetDefaultCommand(FALSE);
    btn->AddSubItem(CreateButton(ID_APP_SETTINGS, IMAGE_SETTINGS));
    btn->SetMenu((HMENU)NULL, FALSE, FALSE, FALSE, FALSE);
    panel.Add(btn);*/
}

void WndTopToolbar::OnZoomRatioChanged(ZoomChangedBy sender)
{
    auto   canvas = theRuntime.GetCurrentCanvas();
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
