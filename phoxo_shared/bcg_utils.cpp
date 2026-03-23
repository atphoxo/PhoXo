#include "pch.h"
#include "bcg_utils.h"
#include "phoxo_utils.h"

#ifdef __BCGCBPRO_H

namespace
{
    bool GetDialogCaptionColors(COLORREF& bk, COLORREF& text)
    {
        if (auto mgr = dynamic_cast<CBCGPVisualManagerVS2012*>(CBCGPVisualManager::GetInstance()))
        {
            if (auto br = mgr->GetNcCaptionBrush(TRUE, FALSE))
            {
                LOGBRUSH   info{};
                br->GetLogBrush(&info);
                if (info.lbStyle == BS_SOLID)
                {
                    bk = info.lbColor;
                    text = mgr->GetNcCaptionTextColor(TRUE);
                    return true;
                }
            }
        }
        ASSERT(false);
        return false;
    }
}

void BCGUtils::SyncCaptionWithVisualManager(HWND wnd)
{
    if (!globalData.bIsWindows11)
        return;

    // Using BCG custom drawing; system animation will be lost
    COLORREF   bk, text;
    if (GetDialogCaptionColors(bk, text))
    {
        globalData.SetWindowCaptionColor(wnd, bk);
        globalData.SetWindowTextColor(wnd, text);
    }
}

bool BCGUtils::ForwardAccelerator(MSG* msg)
{
    if (msg->message >= WM_KEYFIRST && msg->message <= WM_KEYLAST)
    {
        return AfxGetMainWnd()->PreTranslateMessage(msg);
    }
    return false;
}

#endif
