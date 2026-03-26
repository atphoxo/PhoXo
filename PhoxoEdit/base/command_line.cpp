#include "pch.h"
#include "command_line.h"

void CommandLine::ParseParam(PCWSTR pszParam, BOOL bFlag, BOOL bLast)
{
    if (bFlag)
        m_cmds.emplace_back(pszParam);
    else
        m_params.emplace_back(pszParam);
}

CString CommandLine::GetStartupOpenFile() const
{
    if (m_params.size() == 1 && m_cmds.empty())
        return m_params.front();
    return GetParamFor(L"open");
}

HWND CommandLine::GetFromPhoXoSeeHwnd() const
{
    CString   s = GetParamFor(L"fromsee");
    return s.IsEmpty() ? NULL : (HWND)_wtoi64(s);
}

CString CommandLine::GetParamFor(CString cmd) const
{
    if (auto it = std::ranges::find(m_cmds, cmd); it != m_cmds.end())
    {
        try
        {
            return m_params.at(it - m_cmds.begin());
        }
        catch (const std::out_of_range&) { assert(false); }
    }
    return L"";
}

void CommandLine::NotifyEditorReady() const
{
    HWND   seewnd = GetFromPhoXoSeeHwnd();
    if (!seewnd)
        return;

    CString   name;
    name.Format(L"PhoXoEditReady_%I64u", (UINT64)seewnd);

    if (CHandle event{ ::OpenEventW(EVENT_MODIFY_STATE, FALSE, name) })
    {
        ::SetEvent(event);
    }
}
