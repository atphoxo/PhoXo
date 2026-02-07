#pragma once

namespace crop
{
    class PageExpander : public CBCGPExpanderCtrl
    {
    public:
        void Create(CWnd* parent, UINT placeholder_id);
    };
}
