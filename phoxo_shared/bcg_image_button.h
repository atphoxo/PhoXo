#pragma once

#ifdef __BCGCBPRO_H
class BCGImageButton : public CBCGPButton
{
private:
    FCImage   m_icon; // BCG操纵image非常不方便，用我们自己的图像绘制
public:
    bool   m_always_default_status = false; // 一直保持按钮背景高亮状态

public:
    BCGImageButton()
    {
        m_bDrawFocus = FALSE;
    }

    void SetIcon(FCImage&& icon)
    {
        m_icon = std::move(icon);
    }

    // 设置bcg的image
    void SetImageEx(const FCImage& img)
    {
        ASSERT(img.IsPremultiplied());
        ClearImages(FALSE);
        m_sizeImage = img.Size();
        m_Image.SetImageSize(m_sizeImage);
        m_Image.AddImage(img, TRUE);
    }

private:
    void CalculatePos(const CRect& rect, CSize text_size, CRect& text_rect, CRect& img_rect, UINT& uiDTFlags) const
    {
        // 图片先放中间，后面根据位置调整
        img_rect = phoxo::Utils::CalculateFitWindow(m_icon.Size(), rect);

        if (!text_size.cx)
        {
            text_rect = rect;
            return;
        }

        if (m_bTopImage)
        {
            uiDTFlags = (uiDTFlags & ~DT_VCENTER) | DT_BOTTOM; // 底部对齐

            text_rect = rect;
            text_rect.bottom -= DPICalculator::Cast(3);

            img_rect.MoveToY(rect.bottom - m_icon.Height() - DPICalculator::Cast(20));
        }
        else
        {
            uiDTFlags = (uiDTFlags & ~DT_CENTER) | DT_LEFT; // 左对齐

            int   gap = DPICalculator::Cast(6); // icon和文字之间间隔
            int   total = m_icon.Width() + gap + text_size.cx;
            int   half = (rect.Width() - total) / 2;
            img_rect.MoveToX(rect.left + half);

            text_rect = rect;
            text_rect.left = img_rect.right + gap;
        }
    }

    void OnDrawText(CDC* pDC, const CRect& rect, const CString& strText, UINT uiDTFlags, UINT uiState) override
    {
        if (!m_icon)
        {
            __super::OnDrawText(pDC, rect, strText, uiDTFlags, uiState);
            return;
        }

        CRect   text_rc, img_rc;
        CalculatePos(rect, pDC->GetTextExtent(strText), text_rc, img_rc, uiDTFlags);
        __super::OnDrawText(pDC, text_rc, strText, uiDTFlags, uiState);

        // draw image
        phoxo::ImageDrawer::Draw(*pDC, img_rc.TopLeft(), m_icon);
    }

    BOOL IsDefaultButton(BOOL bCheckForOtherButtonFocus) const override
    {
        return m_always_default_status ? TRUE : __super::IsDefaultButton(bCheckForOtherButtonFocus);
    }
};
#endif
