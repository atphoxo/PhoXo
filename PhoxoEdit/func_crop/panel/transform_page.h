#pragma once

namespace crop
{
    class TransformPage : public CBCGPDialog
    {
    private:
        BCGImageButton   m_cw, m_ccw, m_mirror, m_flip, m_resize;

    public:
        TransformPage()
        {
            EnableVisualManagerStyle();
        }

    private:
        void LoadButtons();
        BOOL OnInitDialog() override;
        void DoDataExchange(CDataExchange* pDX) override;

        BOOL PreTranslateMessage(MSG* pMsg) override
        {
            if (BCGUtils::ForwardAccelerator(pMsg))
                return TRUE;
            return __super::PreTranslateMessage(pMsg);
        }

        afx_msg void OnResizeImage();
        afx_msg void OnFlipHorz();
        afx_msg void OnFlipVert();
        afx_msg void OnRotateCW();
        afx_msg void OnRotateCCW();
        afx_msg LRESULT OnChangeVisualManager(WPARAM wp, LPARAM lp);
        DECLARE_MESSAGE_MAP()
        DECLARE_DYNCREATE(TransformPage)
    };
}
