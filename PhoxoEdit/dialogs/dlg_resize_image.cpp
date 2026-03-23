#include "pch.h"
#include "dlg_resize_image.h"

namespace
{
    enum
    {
        IDC_WIDTH_TEXT = 1000,
        IDC_WIDTH_INPUT = 1010,
        IDC_HEIGHT_TEXT = 1001,
        IDC_HEIGHT_INPUT = 1011,
        IDC_WIDTH_PERCENT = 1012,
        IDC_HEIGHT_PERCENT = 1013,
        IDC_KEEP_ASPECT_RATIO = 1020,
    };
}

BEGIN_MESSAGE_MAP(DlgResizeImage, CBCGPDialog)
    ON_EN_CHANGE(IDC_WIDTH_INPUT, OnChangeWidth)
    ON_EN_CHANGE(IDC_HEIGHT_INPUT, OnChangeHeight)
    ON_EN_CHANGE(IDC_WIDTH_PERCENT, OnChangeWidthPercent)
    ON_EN_CHANGE(IDC_HEIGHT_PERCENT, OnChangeHeightPercent)
    ON_COMMAND(IDC_KEEP_ASPECT_RATIO, OnKeepRatio)
END_MESSAGE_MAP()

DlgResizeImage::DlgResizeImage(CSize original_size)
    : CBCGPDialog(IDD_RESIZE_IMAGE)
    , m_original_size(original_size)
{
    m_width = original_size.cx;
    m_height = original_size.cy;
}

void DlgResizeImage::EnableOKButton()
{
    INT64   bytes = m_width * (INT64)m_height * 4;
    BOOL   enable = (bytes && (bytes < 3000000000)); // < 3Gb
    ::EnableWindow(::GetDlgItem(m_hWnd, IDOK), enable);
}

BOOL DlgResizeImage::OnInitDialog()
{
    __super::OnInitDialog();

    SendDlgItemMessage(IDC_WIDTH_INPUT, EM_SETLIMITTEXT, 6, 0);
    SendDlgItemMessage(IDC_HEIGHT_INPUT, EM_SETLIMITTEXT, 6, 0);
    SendDlgItemMessage(IDC_WIDTH_PERCENT, EM_SETLIMITTEXT, 4, 0);
    SendDlgItemMessage(IDC_HEIGHT_PERCENT, EM_SETLIMITTEXT, 4, 0);

    LanguageTextGroup   text(L"DlgResize", L"t");
    SetWindowText(text[0]);
    SetDlgItemText(IDC_WIDTH_TEXT, text[1]);
    SetDlgItemText(IDC_HEIGHT_TEXT, text[2]);
    SetDlgItemText(IDC_KEEP_ASPECT_RATIO, text[3]);
    SetDlgItemText(IDOK, theConfig.AppText(L"ok"));
    SetDlgItemText(IDCANCEL, theConfig.AppText(L"cancel"));

    BOOL   NCArea = !globalData.bIsWindows11;
    EnableVisualManagerStyle(TRUE, NCArea); // Use Win32 to set caption for Windows 11 and later
    BCGUtils::SyncCaptionWithVisualManager(m_hWnd);

    if (auto edit = (CEdit*)GetDlgItem(IDC_WIDTH_INPUT))
    {
        edit->SetFocus();
        edit->SetSel(0, -1);
    }
    return FALSE;
}

void DlgResizeImage::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);
    FCWnd::DDX_Text_NoTip(pDX, IDC_WIDTH_INPUT, m_width, 0);
    FCWnd::DDX_Text_NoTip(pDX, IDC_HEIGHT_INPUT, m_height, 0);
    FCWnd::DDX_Text_NoTip(pDX, IDC_WIDTH_PERCENT, m_width_percent, 0);
    FCWnd::DDX_Text_NoTip(pDX, IDC_HEIGHT_PERCENT, m_height_percent, 0);
    DDX_Check(pDX, IDC_KEEP_ASPECT_RATIO, m_keep_ratio);
    EnableOKButton();
}

void DlgResizeImage::OnChangeWidth()
{
    UpdateData();
    double   ratio = m_width / (double)m_original_size.cx;
    m_width_percent = lround(100 * ratio);
    if (m_keep_ratio)
    {
        m_height_percent = m_width_percent;
        m_height = (int)(ratio * m_original_size.cy);
    }
    UpdateData(FALSE);
}

void DlgResizeImage::OnChangeHeight()
{
    UpdateData();
    double   ratio = m_height / (double)m_original_size.cy;
    m_height_percent = lround(100 * ratio);
    if (m_keep_ratio)
    {
        m_width_percent = m_height_percent;
        m_width = (int)(ratio * m_original_size.cx);
    }
    UpdateData(FALSE);
}

void DlgResizeImage::OnChangeWidthPercent()
{
    UpdateData();
    double   ratio = m_width_percent / (double)100;
    m_width = (int)(m_original_size.cx * ratio);
    if (m_keep_ratio)
    {
        m_height_percent = m_width_percent;
        m_height = (int)(ratio * m_original_size.cy);
    }
    UpdateData(FALSE);
}

void DlgResizeImage::OnChangeHeightPercent()
{
    UpdateData();
    double   ratio = m_height_percent / (double)100;
    m_height = (int)(m_original_size.cy * ratio);
    if (m_keep_ratio)
    {
        m_width_percent = m_height_percent;
        m_width = (int)(ratio * m_original_size.cx);
    }
    UpdateData(FALSE);
}

void DlgResizeImage::OnKeepRatio()
{
    OnChangeWidth();
}
