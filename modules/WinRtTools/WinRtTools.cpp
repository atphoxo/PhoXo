#include "pch.h"
#include "framework.h"
#include "WinRtTools.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>
#include <winrt/Windows.Foundation.Collections.h>
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Ocr;
using namespace Windows::Storage;

#pragma comment(lib, "windowsapp")

namespace
{
    struct com_apartment
    {
        com_apartment()
        {
            winrt::init_apartment(winrt::apartment_type::multi_threaded);
        }

        ~com_apartment()
        {
            winrt::uninit_apartment();
        }
    };
}

int OcrImageFile(PCWSTR filepath, BSTR* result)
{
    try
    {
        com_apartment   auto_com;

        auto ocrEngine = OcrEngine::TryCreateFromUserProfileLanguages();
        if (!ocrEngine)
            return 2;

        auto file = StorageFile::GetFileFromPathAsync(filepath).get();
        auto stream = file.OpenAsync(FileAccessMode::Read).get();
        auto decoder = BitmapDecoder::CreateAsync(stream).get();
        auto bitmap = decoder.GetSoftwareBitmapAsync(BitmapPixelFormat::Bgra8, BitmapAlphaMode::Premultiplied).get();

        auto resultOcr = ocrEngine.RecognizeAsync(bitmap).get();

        // 保存每行文本
        CString text;
        for (auto const& line : resultOcr.Lines())
        {
            text += line.Text().c_str();  // 每行文本
            text += L"\r\n";
        }

        *result = text.AllocSysString();
        return 0;
    }
    catch (...) { return 3; }
}
