#pragma once

class WndTopToolbar : public CBCGPRibbonBar,
                      public IEventObserverBase
{
private:
    CBCGPRibbonComboBox   * m_zoom_combobox{};
    CBCGPRibbonSlider   * m_zoom_slider{};

public:
    WndTopToolbar();
    void Create(CWnd* parent);

    double GetComboDisplayedRatio() const;
    double GetRatioFromSlider() const;

private:
    void AddFileGroup(CBCGPRibbonPanel& panel);
    void AddZoomGroup(CBCGPRibbonPanel& panel);
    void AddUndoGroup(CBCGPRibbonPanel& panel);

    void OnZoomRatioChanged(ZoomChangedBy sender);
    void OnObserveEvent(ObservedEvent& event) override;
};
