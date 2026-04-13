#include "pch.h"
#include "local.h"
#include "wnd_panel_crop.h"
#include "../tool_crop.h"
using namespace crop;

namespace
{
    int GetEditInt(const CWnd& wnd)
    {
        CString   str;
        wnd.GetWindowText(str);
        int   v = StrToInt(str);
        return (v > 0) ? v : 0;
    }

    bool HandleOversize(int width, int height, CSize canvas_size)
    {
        if (width > canvas_size.cx || height > canvas_size.cy)
        {
            // 尺寸超出canvas，居中显示
            ToolCrop::ApplyCropAspectRatio(width, height);
            return true;
        }
        return false;
    }
}

void WndPanelCrop::InitSizeEdit()
{
    for (auto ctrl : { &m_width_edit, &m_height_edit })
    {
        ctrl->SetLimitText(6);
        ctrl->SetVerticalAlignment(TA_CENTER);
    }
    UpdateSizeEdit();
}

void WndPanelCrop::UpdateSizeEdit()
{
    if (ToolCrop::HasCropRect())
    {
        CRect   rc = ToolCrop::s_crop_on_canvas;
        m_width_edit.SetWindowText(FCString::From(rc.Width()));
        m_height_edit.SetWindowText(FCString::From(rc.Height()));
    }
    else
    {
        m_width_edit.SetWindowText(L"");
        m_width_edit.SetPrompt(PanelCropText(60));
        m_height_edit.SetWindowText(L"");
        m_height_edit.SetPrompt(PanelCropText(61));
    }
}

void WndPanelCrop::ApplyCropSizeFromSingleEdit(HWND edit_ctrl)
{
    auto   canvas = g_runtime.GetCurrentCanvas();
    if (!canvas)
        return;

    CRect   rc = ToolCrop::s_crop_on_canvas;
    if (edit_ctrl == m_width_edit)
    {
        if (int width = GetEditInt(m_width_edit))
        {
            rc.right = rc.left + width;
            if (ToolCrop::s_aspect_ratio.IsLocked())
            {
                int   height = (std::max)(1, (int)(width / ToolCrop::s_aspect_ratio.Value()));
                rc.bottom = rc.top + height;
                if (HandleOversize(width, height, canvas->Size()))
                    return;
            }
            else
            {
                if (rc.Height() == 0)
                    rc.bottom = rc.top + width; // 没有选区设置正方形
            }
        }
    }
    else
    {
        if (int height = GetEditInt(m_height_edit))
        {
            rc.bottom = rc.top + height;
            if (ToolCrop::s_aspect_ratio.IsLocked())
            {
                int   width = (std::max)(1, (int)(height * ToolCrop::s_aspect_ratio.Value()));
                rc.right = rc.left + width;
                if (HandleOversize(width, height, canvas->Size()))
                    return;
            }
            else
            {
                if (rc.Width() == 0)
                    rc.right = rc.left + height;
            }
        }
    }

    // 推回canvas内
    FCWnd::MoveRectInside(rc, canvas->Size());

    // 必须调用，如果用户输入无效值或空，则恢复显示当前值
    ToolCrop::SetCropOnCanvas(rc);
}

void WndPanelCrop::OnWidthEditKillFocus()
{
    ApplyCropSizeFromSingleEdit(m_width_edit);
}

void WndPanelCrop::OnHeightEditKillFocus()
{
    ApplyCropSizeFromSingleEdit(m_height_edit);
}

BOOL WndPanelCrop::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        HWND   focus = ::GetFocus();
        if (focus == m_width_edit)
        {
            ApplyCropSizeFromSingleEdit(m_width_edit);
            m_width_edit.SetSel(0, -1);
            return TRUE;
        }
        else if (focus == m_height_edit)
        {
            ApplyCropSizeFromSingleEdit(m_height_edit);
            m_height_edit.SetSel(0, -1);
            return TRUE;
        }
    }
    return __super::PreTranslateMessage(pMsg);
}
