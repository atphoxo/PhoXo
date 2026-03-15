#pragma once

namespace crop
{
    class RotatePage : public CBCGPDialog
    {
    private:
        BCGImageButton   m_cw, m_ccw, m_mirror, m_flip;

    public:
        RotatePage()
        {
            EnableVisualManagerStyle();
        }

    private:
        void LoadButtons();
        BOOL OnInitDialog() override;
        void DoDataExchange(CDataExchange* pDX) override;

        afx_msg LRESULT OnChangeVisualManager(WPARAM wp, LPARAM lp);
        DECLARE_MESSAGE_MAP()
        DECLARE_DYNCREATE(RotatePage)
    };
}
