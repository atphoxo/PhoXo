#pragma once

class PanelManager
{
private:
    std::map<UINT, unique_ptr<CBCGPDockingControlBar>>   m_func_panel;
    CBCGPDockingControlBar*   m_current_panel = nullptr;
    std::map<UINT, UINT>   m_panel_tab_map;

public:
    UINT CurrentTabID() const;

    void OnClickTab(CBCGPFrameWnd& main_wnd, UINT tab_id);
    CBCGPDockingControlBar* CreatePanel(CWnd* parent, UINT tab_id);
};
