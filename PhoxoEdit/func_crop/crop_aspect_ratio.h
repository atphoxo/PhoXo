#pragma once

namespace crop
{
    class CropAspectRatio
    {
    private:
        // Aspect ratio (e.g. 16:9, 4:3, 1:1). Default 0 means ratio is not locked.
        double   m_width = 0;
        double   m_height = 0;

    public:
        bool IsLocked() const
        {
            return (m_width > 0) && (m_height > 0);
        }

        double Value() const
        {
            assert(IsLocked());
            return IsLocked() ? (m_width / m_height) : 0;
        }

        void Unlock()
        {
            m_width = m_height = 0;
        }

        void Lock(int width, int height)
        {
            m_width = width;
            m_height = height;
            assert(IsLocked());
        }

        // 目标：返回一个矩形尺寸，使其至少覆盖 canvas（>= canvas），并且保持比例。
        CRect FitCanvas(CSize canvas_size) const
        {
            assert(IsLocked());
            double   t = (std::max)(canvas_size.cx / m_width, canvas_size.cy / m_height);

            // t * m_width >= canvas_width
            // t * m_height >= canvas_height
            CSize   objsize{ (int)ceil(t * m_width), (int)ceil(t * m_height) };
            return phoxo::Utils::CalculateFitWindow(objsize, CRect({}, canvas_size));
        }

        // Maps a 2D mouse drag into an effective vertical delta when resizing
        // a crop rectangle corner with a locked aspect ratio.
        double MapDragToVertical(GPointF delta) const
        {
            assert(IsLocked());
            return (delta.X + delta.Y) * m_height / (m_width + m_height);
        }
    };
}
