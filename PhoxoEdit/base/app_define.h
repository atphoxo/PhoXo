#pragma once

// ---------------------------------------------------------------------------
// 本程序消息，不要用 enum class，否则 ON_MESSAGE 需要强制转换
// ---------------------------------------------------------------------------
enum
{
    MSG_POST_LOAD_FIRST = (WM_APP + 500),
};

enum class AppEvent
{
    ZoomRatioChanged,
};

enum class ZoomChangedBy
{
    TopCombobox,
    TopSlider,
    InitView,
    ZoomButton,
    MouseWheel,
};

enum class ZoomDirection
{
    ZoomIn,
    ZoomOut
};

enum class RenderPreference
{
    Quality = 0, // 高质量：Box 平均
    Speed = 1    // 高速：最近邻采样
};
