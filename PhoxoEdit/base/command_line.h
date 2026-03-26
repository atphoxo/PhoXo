#pragma once

class CommandLine : public CCommandLineInfo
{
private:
    std::deque<CString>   m_cmds;
    std::deque<CString>   m_params;

public:
    void ParseParam(PCWSTR pszParam, BOOL bFlag, BOOL bLast) override;

    CString GetStartupOpenFile() const;
    HWND GetFromPhoXoSeeHwnd() const;
    CString GetParamFor(CString cmd) const;

    void NotifyEditorReady() const;
};
