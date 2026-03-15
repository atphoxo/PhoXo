#pragma once

namespace crop
{
    class PageExpander : public CBCGPExpanderCtrl
    {
    private:
        int   m_group_ids[2]{};

    public:
        void Create(CWnd* parent, UINT placeholder_id);

    private:
        void LoadGroupIcon();
        afx_msg LRESULT OnChangeVisualManager(WPARAM wp, LPARAM lp);
        DECLARE_MESSAGE_MAP()
    };
}
