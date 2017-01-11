// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "AdvancedDlg.h"
#include ".\advanceddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const int g_nColorControlId[NUM_PROGRAM_COLORS] =
{
    IDC_COLOR_0,
    IDC_COLOR_1,
    IDC_COLOR_2,
    IDC_COLOR_3,
    IDC_COLOR_4,
    IDC_COLOR_5,
    IDC_COLOR_6,
    IDC_COLOR_7,
    IDC_COLOR_8
};

IMPLEMENT_DYNAMIC(CAdvancedDlg, CDialog)
CAdvancedDlg::CAdvancedDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAdvancedDlg::IDD, pParent)
{
    nThreadPriorityIndex = 3;
    nProcessPriorityIndex = 1;
    bDeleteOnError = true;
    bStopOnErrors = false;
    szLogFileName = MAIN_APP_LOG;
    nLogEncoding = 2;

    for (int i = 0; i < NUM_PROGRAM_COLORS; i++)
        m_Color[i] = RGB(0xFF, 0xFF, 0xFF);
}

CAdvancedDlg::~CAdvancedDlg()
{

}

void CAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDC_COMBO_PRIORITY_THREAD, m_CmbThread);
    DDX_Control(pDX, IDC_COMBO_PRIORITY_PROCESS, m_CmbProcess);
    DDX_Control(pDX, IDC_EDIT_CONSOLE_LOG_FILE, m_EdtLog);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_LOG, m_BtnBrowse);
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOG, OnBnClickedButtonBrowseLog)
    ON_STN_CLICKED(IDC_COLOR_0, OnStnClickedColor0)
    ON_STN_CLICKED(IDC_COLOR_1, OnStnClickedColor1)
    ON_STN_CLICKED(IDC_COLOR_2, OnStnClickedColor2)
    ON_STN_CLICKED(IDC_COLOR_3, OnStnClickedColor3)
    ON_STN_CLICKED(IDC_COLOR_4, OnStnClickedColor4)
    ON_STN_CLICKED(IDC_COLOR_5, OnStnClickedColor5)
    ON_STN_CLICKED(IDC_COLOR_6, OnStnClickedColor6)
    ON_STN_CLICKED(IDC_COLOR_7, OnStnClickedColor7)
    ON_STN_CLICKED(IDC_COLOR_8, OnStnClickedColor8)
END_MESSAGE_MAP()

BOOL CAdvancedDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    for (int i = 0; i < NUM_THREAD_PRIORITY; i++)
        this->m_CmbThread.AddString(g_szThreadPriority[i]);

    SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRIORITY_THREAD);

    for (int i = 0; i < NUM_PROCESS_PRIORITY; i++)
        this->m_CmbProcess.AddString(g_szProcessPriority[i]);

    SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRIORITY_THREAD);

    this->SetAdvSettings();

    return TRUE;
}

void CAdvancedDlg::OnPaint()
{
    CPaintDC dc(this);
    OnPaintProc();
}

HWND CAdvancedDlg::GetHWND(int nID)
{
    return this->GetDlgItem(nID)->GetSafeHwnd();
}

void CAdvancedDlg::PaintRect(int nID, COLORREF cr)
{
    HWND hWnd;
    HDC hDC;
    CRect rc;
    HBRUSH hBrush;
    HPEN hPen;

    hWnd = GetDlgItem(nID)->GetSafeHwnd();
    if (hWnd)
    {
        hDC = ::GetDC(hWnd);
        ::GetClientRect(hWnd, rc);
        hBrush = ::CreateSolidBrush(cr);
        hPen = ::CreatePen(PS_SOLID, 1, cr);

        ::SelectObject(hDC, hBrush);
        ::SelectObject(hDC, hPen);

        ::Rectangle(hDC, 0, 0, rc.Width(), rc.Height());

        ::ReleaseDC(hWnd, hDC);
        ::DeleteObject(hBrush);
        ::DeleteObject(hPen);
    }
}

void CAdvancedDlg::OnPaintProc(void)
{
    for (int i = 0; i < NUM_PROGRAM_COLORS; i++)
        PaintRect(g_nColorControlId[i], m_Color[i]);
}

COLORREF CAdvancedDlg::ChangeTheColor(COLORREF cr)
{
    CColorDialog cd(cr, CC_FULLOPEN, this);
    INT_PTR nRet = cd.DoModal();
    if (nRet == IDOK)
        return cd.GetColor();
    else
        return cr;
}

void CAdvancedDlg::GetAdvSettings()
{
    nThreadPriorityIndex = this->m_CmbThread.GetCurSel();
    nProcessPriorityIndex = this->m_CmbProcess.GetCurSel();

    bDeleteOnError = (this->IsDlgButtonChecked(IDC_CHECK_OPTION_DELETE) == BST_CHECKED) ? true : false;
    bStopOnErrors = (this->IsDlgButtonChecked(IDC_CHECK_OPTION_ERROR) == BST_CHECKED) ? true : false;

    m_EdtLog.GetWindowText(szLogFileName);

    int nCheckID = this->GetCheckedRadioButton(IDC_RADIO_ENCODING_ANSI, IDC_RADIO_ENCODING_UTF8);

    if (nCheckID == IDC_RADIO_ENCODING_ANSI)
        nLogEncoding = 0;
    else if (nCheckID == IDC_RADIO_ENCODING_UNICODE)
        nLogEncoding = 1;
    else if (nCheckID == IDC_RADIO_ENCODING_UTF8)
        nLogEncoding = 2;
    else
        nLogEncoding = 2;
}

void CAdvancedDlg::SetAdvSettings()
{
    this->m_CmbThread.SetCurSel(nThreadPriorityIndex);
    this->m_CmbProcess.SetCurSel(nProcessPriorityIndex);

    this->CheckDlgButton(IDC_CHECK_OPTION_DELETE,
        (bDeleteOnError == true) ? BST_CHECKED : BST_UNCHECKED);

    this->CheckDlgButton(IDC_CHECK_OPTION_ERROR,
        (bStopOnErrors == true) ? BST_CHECKED : BST_UNCHECKED);

    m_EdtLog.SetWindowText(szLogFileName);

    switch (nLogEncoding)
    {
    case 0:
        this->CheckRadioButton(IDC_RADIO_ENCODING_ANSI,
            IDC_RADIO_ENCODING_UTF8,
            IDC_RADIO_ENCODING_ANSI);
        break;
    case 1:
        this->CheckRadioButton(IDC_RADIO_ENCODING_ANSI,
            IDC_RADIO_ENCODING_UTF8,
            IDC_RADIO_ENCODING_UNICODE);
        break;
    case 2:
        this->CheckRadioButton(IDC_RADIO_ENCODING_ANSI,
            IDC_RADIO_ENCODING_UTF8,
            IDC_RADIO_ENCODING_UTF8);
        break;
    default:
        this->CheckRadioButton(IDC_RADIO_ENCODING_ANSI,
            IDC_RADIO_ENCODING_UTF8,
            IDC_RADIO_ENCODING_UTF8);
        break;
    };
}

int CAdvancedDlg::GetTheThreadPriority(void)
{
    return(g_nThreadPriority[this->m_CmbThread.GetCurSel()]);
}

DWORD CAdvancedDlg::GetTheProcessPriority(void)
{
    return(g_dwProcessPriority[this->m_CmbProcess.GetCurSel()]);
}

void CAdvancedDlg::OnOK()
{
    GetAdvSettings();
    CDialog::OnOK();
}

void CAdvancedDlg::OnCancel()
{
    CDialog::OnCancel();
}

void CAdvancedDlg::OnClose()
{
    CDialog::OnClose();
}

void CAdvancedDlg::OnDestroy()
{
    CDialog::OnDestroy();
}

void CAdvancedDlg::OnBnClickedButtonBrowseLog()
{
    CFileDialog fd(FALSE, _T("log"), szLogFileName,
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Log Files (*.log)|*.log|Txt Files (*.txt)|*.txt|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        szLogFileName = fd.GetPathName();
        m_EdtLog.SetWindowText(szLogFileName);
    }
}

void CAdvancedDlg::ClickedOnColorRect(int nID)
{
    COLORREF cr_new;
    cr_new = this->ChangeTheColor(m_Color[nID]);
    if (cr_new != m_Color[nID])
        m_Color[nID] = cr_new;

    this->PaintRect(g_nColorControlId[nID], m_Color[nID]);
}

void CAdvancedDlg::OnStnClickedColor0()
{
    this->ClickedOnColorRect(0);
}

void CAdvancedDlg::OnStnClickedColor1()
{
    this->ClickedOnColorRect(1);
}

void CAdvancedDlg::OnStnClickedColor2()
{
    this->ClickedOnColorRect(2);
}

void CAdvancedDlg::OnStnClickedColor3()
{
    this->ClickedOnColorRect(3);
}

void CAdvancedDlg::OnStnClickedColor4()
{
    this->ClickedOnColorRect(4);
}

void CAdvancedDlg::OnStnClickedColor5()
{
    this->ClickedOnColorRect(5);
}

void CAdvancedDlg::OnStnClickedColor6()
{
    this->ClickedOnColorRect(6);
}

void CAdvancedDlg::OnStnClickedColor7()
{
    this->ClickedOnColorRect(7);
}

void CAdvancedDlg::OnStnClickedColor8()
{
    this->ClickedOnColorRect(8);
}
