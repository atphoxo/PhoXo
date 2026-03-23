#pragma once

#ifdef __BCGCBPRO_H
class BCGUtils
{
public:
    static void SyncCaptionWithVisualManager(HWND wnd);
    static bool ForwardAccelerator(MSG* msg);
};
#endif
