// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the WINRTTOOLS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// WINRTTOOLS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WINRTTOOLS_EXPORTS
#define WINRTTOOLS_API __declspec(dllexport)
#else
#define WINRTTOOLS_API __declspec(dllimport)
#endif

extern "C"
{
    // 必须单独开个线程来调用此函数
    WINRTTOOLS_API int OcrImageFile(PCWSTR filepath, BSTR* result);
}
