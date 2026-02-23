#include "pch.h"
#include "runtime_context.h"
#include "../main_view.h"

namespace
{
    void CreateCanvasGridBack(CBrush& br)
    {
        using namespace Gdiplus;
        phoxo::effect::CheckerFill   eff(FCColor(Color::Silver), FCColor(Color::White), 16);
        FCImage   img;
        img.Create(32, 32);
        img.ApplyEffect(eff);
        br.Attach(::CreatePatternBrush(img));
    }
}

RuntimeContext::RuntimeContext()
{
    CreateCanvasGridBack(m_canvas_back);
    m_accent_brush.CreateSolidBrush(CBCGPVisualManager::GetInstance()->GetDefaultAccentColor());
}

CMainView* RuntimeContext::GetActiveView() const
{
    auto   main = (CFrameWnd*)AfxGetMainWnd();
    return main ? (CMainView*)main->GetActiveView() : nullptr;
}

CMainDoc* RuntimeContext::GetActiveDoc() const
{
    auto   view = GetActiveView();
    return view ? (CMainDoc*)view->GetDocument() : nullptr;
}

void RuntimeContext::InvalidateView() const
{
    if (auto view = GetActiveView())
    {
        view->Invalidate();
    }
}

Canvas* RuntimeContext::GetCurrentCanvas() const
{
    auto   view = GetActiveView();
    return view ? view->GetCanvas() : nullptr;
}
