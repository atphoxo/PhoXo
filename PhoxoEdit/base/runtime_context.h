#pragma once
class CMainDoc;
class CMainView; // forward declare

class RuntimeContext : public FCSingleton<RuntimeContext>
{
public:
    RuntimeContext();

    CBrush   m_canvas_back;
    CBrush   m_accent_brush;

    CMainView* GetActiveView() const;
    CMainDoc* GetActiveDoc() const;
    void InvalidateView() const;
    Canvas* GetCurrentCanvas() const; // If a canvas exists, an active view and document must exist
    void OnThemeChanged();
};

#define theRuntime RuntimeContext::GetInstance()
