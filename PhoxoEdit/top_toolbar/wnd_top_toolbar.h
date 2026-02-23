#pragma once

class WndTopToolbar : public CBCGPRibbonBar,
                      public IEventObserverBase
{
private:
    CBCGPRibbonComboBox*   m_zoom_combobox{};
    CBCGPRibbonSlider*   m_zoom_slider{};
    CBCGPRibbonUndoButton*   m_undo_button{};

public:
    WndTopToolbar();
    void Create(CWnd* parent);

    double GetComboDisplayedRatio() const;
    double GetRatioFromSlider() const;

    int UndoCount() const { return (std::max)(1, m_undo_button->GetActionNumber()); }

private:
    void AddFileGroup(CBCGPRibbonPanel& panel);
    void AddZoomGroup(CBCGPRibbonPanel& panel);
    void AddUndoGroup(CBCGPRibbonPanel& panel);

    void OnZoomRatioChanged(ZoomChangedBy sender);
    void OnObserveEvent(ObservedEvent& event) override;
};
