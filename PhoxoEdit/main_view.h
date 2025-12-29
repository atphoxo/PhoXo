#pragma once
#include "main_view_welcome.h"

class CMainView : public PhoXoScrollViewBase
{
private:
    unique_ptr<main_view::WelcomeRender>   m_welcome;

private:
    Canvas* GetCanvas() const;
    GPointF ViewToCanvas(CPoint view_pt) const;
    void UpdateZoomRatio(double new_ratio, ZoomChangedBy from, std::optional<CPoint> view_anchor = std::nullopt);

	void OnDraw(CDC* pDC) override;
    void OnInitialUpdate() override;
    void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;

    afx_msg void OnTopZoomSlider();
    afx_msg void OnTopZoomRatioComboList();
    afx_msg void OnTopZoomActual();
    afx_msg void OnTopZoomFitWindow();
    afx_msg void OnTopZoomInOut(UINT id);
    afx_msg void OnFilePrint();
    afx_msg void OnEditCopy();
    afx_msg void OnUpdateIfCanvasValid(CCmdUI* ui);
    afx_msg void OnContextMenu(CWnd*, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CMainView)
};
