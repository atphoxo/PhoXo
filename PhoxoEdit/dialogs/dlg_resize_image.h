#pragma once

class DlgResizeImage : public CBCGPDialog
{
private:
    const CSize   m_original_size;
    int   m_width;
    int   m_height;
    int   m_width_percent = 100;
    int   m_height_percent = 100;
    int   m_keep_ratio = TRUE;

public:
    DlgResizeImage(CSize original_size);
    CSize GetScaledSize() const { return CSize(m_width, m_height); }

private:
    void EnableOKButton();
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg void OnChangeWidth();
    afx_msg void OnChangeHeight();
    afx_msg void OnChangeWidthPercent();
    afx_msg void OnChangeHeightPercent();
    afx_msg void OnKeepRatio();
    DECLARE_MESSAGE_MAP()
};
