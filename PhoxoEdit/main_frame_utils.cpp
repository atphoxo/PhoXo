#include "pch.h"
#include "main_frame_utils.h"

CPoint CMainFrameUtils::GetMinMainWndSize()
{
    CRect   rc;
    SystemParametersInfo(SPI_GETWORKAREA, sizeof(RECT), rc, 0);
    rc.DeflateRect(200, 150);

    int   x = (std::min)(DPICalculator::Cast(300), rc.Width()); // 如果屏幕很小的情况下做个保护
    int   y = (std::min)(DPICalculator::Cast(400), rc.Height());
    return { x, y };
}
