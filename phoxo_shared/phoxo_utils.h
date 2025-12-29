#pragma once

class PhoxoUtils
{
public:
    // 保存d:\ 如果1.jpg存在则返回d:\1(1).jpg，依次类推
    static CString GetUniqueFilename(const CString& folder, const CString& dest_filename);

    static bool WIAIsCanScanImage();
    static void WIAPopScanImageDialog(PCWSTR savefile);
};
