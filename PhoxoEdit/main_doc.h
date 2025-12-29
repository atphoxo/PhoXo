#pragma once

class CMainDoc : public CDocument
{
public:
    unique_ptr<Canvas>   m_canvas;

private:
    BOOL OnNewDocument() override;
    BOOL OnOpenDocument(LPCTSTR filepath) override;
    BOOL OnSaveDocument(LPCTSTR filepath) override;

    afx_msg void OnUpdateFileSave(CCmdUI* ui);
    DECLARE_MESSAGE_MAP()
    DECLARE_DYNCREATE(CMainDoc)
};
