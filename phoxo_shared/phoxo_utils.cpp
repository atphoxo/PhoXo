#include "pch.h"
#include "phoxo_app_define.h"
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

namespace
{
    void WIAPopPrintModalDialog(PCWSTR image_file)
    {
        try
        {
            WIA::ICommonDialogPtr   printDlg;
            printDlg.CreateInstance(L"WIA.CommonDialog");

            WIA::IVectorPtr   imgList;
            imgList.CreateInstance(L"WIA.Vector");
            _variant_t   n1(image_file);
            imgList->Add(&n1, 0);

            _variant_t   n2(imgList.GetInterfacePtr());
            printDlg->ShowPhotoPrintingWizard(&n2);
        }
        catch (_com_error&) { ASSERT(FALSE); }
    }

    void print_image_thread(BSTR param)
    {
        AutoComInitializer   COM_init;
        CComBSTR   path;
        path.Attach(param);
        WIAPopPrintModalDialog(path);
        ::DeleteFile(path);
    }
}

void PhoxoUtils::CreatePrintImageThread(CString imagefile)
{
    std::thread   t(print_image_thread, imagefile.AllocSysString());
    t.detach();
}

FCImage PhoxoUtils::LoadSvgWithDpi(UINT res_id, std::optional<FCColor> fill_color)
{
    FCResource   svg(res_id, L"SVG");
    FCImage   bmp = phoxo::ImageHandler::Make(svg.LoadSvgWithDpi(), WICPremultiplied32bpp);
    if (fill_color)
    {
        phoxo::ImageFastPixel::FillRGBAndPremultiply(bmp, *fill_color);
    }
    return bmp;
}

FCColor PhoxoUtils::GetIconColor(ThemeMode theme)
{
    using enum ThemeMode;
#ifdef __BCGCBPRO_H
    if (theme == FollowBCG)
        theme = CBCGPVisualManager::GetInstance()->IsDarkTheme() ? Dark : Light;
    else if (theme == InverseBCG)
        theme = CBCGPVisualManager::GetInstance()->IsDarkTheme() ? Light : Dark;
#endif
    return FCColor{ (theme == Dark) ? 0xEEEEEE : 0x333333 };
}

void PhoxoUtils::EnableWndDragDrop(HWND wnd)
{
    ChangeWindowMessageFilterEx(wnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
    ChangeWindowMessageFilterEx(wnd, 0x0049, MSGFLT_ALLOW, NULL); // WM_COPYGLOBALDATA: allow low-privilege Explorer to drag files in when app is running with high privileges
    ::DragAcceptFiles(wnd, TRUE);
}
