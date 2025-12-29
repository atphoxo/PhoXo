#pragma once

#ifdef __BCGCBPRO_H
class PhoXoScrollViewBase : public CBCGPScrollView
{
protected:
    // ¿½±´mfc´úÂëÐÞ¸Ä
    void SetScrollSizes(int nMapMode, SIZE sizeTotal, const SIZE& sizePage = sizeDefault, const SIZE& sizeLine = sizeDefault);
    void ScrollToDevicePosition(POINT ptDev);
    void UpdateBars();
    BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll) override;
    void ScrollHorizByWheel(short zDelta);
    void GetScrollBarSizes(CSize& sizeSb);
    BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
    void GetScrollBarState(CSize sizeClient, CSize& needSb, CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

    BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) override;
    afx_msg int OnCreate(LPCREATESTRUCT lcs);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return FALSE; }
	DECLARE_MESSAGE_MAP()
    DECLARE_DYNAMIC(PhoXoScrollViewBase)
};
#endif
