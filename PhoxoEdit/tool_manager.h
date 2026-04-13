#pragma once
#include "tool_base.h"

class ToolManager : public FCSingleton<ToolManager>
{
public:
    ToolBase* GetActiveTool() const { return m_activeTool.get(); }

    void ActivateTool(std::wstring_view tool_name);
    void Shutdown();

private:
    unique_ptr<ToolBase>   m_activeTool;
};

#define g_tool_manager ToolManager::GetInstance()
