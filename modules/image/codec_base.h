#pragma once

class CCodecBase
{
public:
    CSize   m_image_size; // 派生类构造必须写入

    virtual ~CCodecBase() {}

    virtual IWICBitmapPtr CreateWICBitmap(IWICImagingFactory* factory) = 0;
};
