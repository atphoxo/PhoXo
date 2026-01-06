#pragma once

class PhoxoUtils
{
public:
    // 保存d:\ 如果1.jpg存在则返回d:\1(1).jpg，依次类推
    static CString GetUniqueFilename(const CString& folder, const CString& dest_filename);

    static bool WIAIsCanScanImage();
    static void WIAPopScanImageDialog(PCWSTR savefile);

    // original_size是写在SVG里的尺寸
    static FCImage LoadSvgWithDpi(UINT res_id, CSize original_size, std::optional<FCColor> fill_color = std::nullopt);
};
