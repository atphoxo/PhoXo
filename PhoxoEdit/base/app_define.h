#pragma once

// ---------------------------------------------------------------------------
// 1. 本程序消息，不要用 enum class，否则 ON_MESSAGE 需要强制转换
// 2. 凡是定义了enum就不要改了，会保存到注册表
// ---------------------------------------------------------------------------
enum
{
    MSG_POST_LOAD_FIRST = (WM_APP + 500),
    MSG_POST_CANVAS_RELOADED = (WM_APP + 501),
};

enum class PanelDock
{
    Right = 0, // Default dock to right side
    Left = 1,
};

enum class ZoomChangedBy
{
    TopCombobox,
    TopSlider,
    InitView,
    ZoomButton,
    MouseWheel,
    Other,
};

enum class ZoomDirection
{
    ZoomIn,
    ZoomOut,
};

// Index corresponds to the order of UI buttons
enum class CropShape
{
    Rectangle = 0,
    RoundedRect = 1,
    Ellipse = 2,
};

enum class RenderPreference
{
    Quality = 0, // High quality: Box averaging
    Speed = 1,   // High speed: Nearest neighbor sampling
};

namespace AppDefine
{
    inline constexpr PCWSTR  CONFIG_REGISTRY = LR"(Software\PhoXo\edit)";
    inline constexpr PCWSTR  LANG_REPLACE = L"\u25A2";
}

enum class AppEvent
{
    ZoomRatioChanged,
    CanvasReloaded,
    CropRectChanged,

//     CommandExecuted,
//     UndoExecuted,
//     RedoExecuted,

    CropUndoCompleted,
};
