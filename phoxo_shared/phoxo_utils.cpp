#include "pch.h"
#import <wiaaut.dll>
#include "phoxo_utils.h"

CString PhoxoUtils::GetUniqueFilename(const CString& folder, const CString& dest_filename)
{
    assert(folder.Right(1) == L"\\");

    CString   ext = PathFindExtension(dest_filename); // ".xxx"
    CPath   name(folder + dest_filename);
    name.RemoveExtension();

    for (int i = 0; i < 50; i++)
    {
        CString   ret;
        if (i == 0)
            ret = (PCWSTR)name + ext;
        else
            ret.Format(L"%s(%d)%s", (PCWSTR)name, i, (PCWSTR)ext);

        if (!PathFileExists(ret))
            return ret;
    }
    return L"";
}

bool PhoxoUtils::WIAIsCanScanImage()
{
    WIA::IDeviceManagerPtr   dm;
    dm.CreateInstance(L"WIA.DeviceManager");
    if (dm)
    {
        if (auto infos = dm->GetDeviceInfos())
        {
            return infos->GetCount() > 0;
        }
    }
    return false;
}

void PhoxoUtils::WIAPopScanImageDialog(PCWSTR savefile)
{
    try
    {
        WIA::ICommonDialogPtr   dlg;
        dlg.CreateInstance(L"WIA.CommonDialog");

        WIA::IImageFilePtr   img = dlg->ShowAcquireImage(WIA::UnspecifiedDeviceType,
            WIA::UnspecifiedIntent,
            WIA::MaximizeQuality,
            "{00000000-0000-0000-0000-000000000000}",
            VARIANT_TRUE, VARIANT_TRUE, VARIANT_FALSE);

        if (img)
            img->SaveFile(savefile);
    }
    catch (_com_error&) { assert(false); }
}

FCImage PhoxoUtils::LoadSvgWithDpi(UINT res_id, CSize original_size, std::optional<FCColor> fill_color)
{
    FCResource   svg(res_id, L"SVG");
    return svg.LoadSvgWithDpi(original_size, fill_color);
}
