#pragma once

class ToolBase
{
public:
    virtual ~ToolBase() = default;

    //----------------------------------------
    // Tool lifecycle
    //----------------------------------------
//     virtual void OnEnterTool() {} // Not used yet, will add when needed
//     virtual void OnLeaveTool() {}

    virtual std::wstring_view GetToolName() const = 0;
    virtual void OnCanvasReloaded() {} // Canvas changed

    //----------------------------------------
    // Cursor: cannot return NULL
    //----------------------------------------
    virtual HCURSOR GetToolCursor(const ViewportContext& ctx)
    {
        return ::LoadCursor(NULL, IDC_ARROW);
    }

    //----------------------------------------
    // Mouse events
    //----------------------------------------
    virtual void OnLButtonDown(const ViewportContext& ctx, UINT nFlags, CPoint point) {}
    virtual void OnLButtonUp(const ViewportContext& ctx, UINT nFlags, CPoint point) {}
    virtual void OnMouseMove(const ViewportContext& ctx, UINT nFlags, CPoint point) {}

    //----------------------------------------
    // Keyboard events
    //----------------------------------------
    virtual bool OnKeyDown(UINT nChar, UINT nFlags) { return false; }

    // Called when capture changes
    virtual void OnCaptureChanged() {}

    //----------------------------------------
    // Draw tool overlay (guides / selection box / mask, etc.)
    //----------------------------------------
    virtual void OnDrawToolOverlay(HDC hdc, const ViewportContext& ctx) {}

    //----------------------------------------
    // Context menu
    //----------------------------------------
    virtual void OnContextMenu(CPoint point) {}
};
