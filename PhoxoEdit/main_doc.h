#pragma once

class CMainDoc : public CDocument
{
private:
    unique_ptr<Canvas>   m_canvas;

public:
    Canvas* GetCanvas() const { return m_canvas.get(); }

    void Execute(unique_ptr<phoxo::Command>&& cmd, phoxo::IProgressListener* progress = nullptr);

private:
    BOOL OnNewDocument() override;
    BOOL OnOpenDocument(LPCTSTR filepath) override;
    BOOL OnSaveDocument(LPCTSTR filepath) override;

    afx_msg void OnUndo();
    afx_msg void OnUpdateUndo(CCmdUI* ui);
    afx_msg void OnRedo();
    afx_msg void OnUpdateRedo(CCmdUI* ui);

    afx_msg void OnUpdateFileSave(CCmdUI* ui);
    DECLARE_MESSAGE_MAP()
    DECLARE_DYNCREATE(CMainDoc)
};
