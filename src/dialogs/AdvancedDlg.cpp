// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "AdvancedDlg.h"

IMPLEMENT_DYNAMIC(CAdvancedDlg, CDialog)
CAdvancedDlg::CAdvancedDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAdvancedDlg::IDD, pParent)
{

}

CAdvancedDlg::~CAdvancedDlg()
{

}

void CAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_GROUP_MISC, m_GrpMisc);
    DDX_Control(pDX, IDC_STATIC_GROUP_LOG, m_GrpLog);
    DDX_Control(pDX, IDC_STATIC_LOG_ENCODING, m_StcEncoding);
    DDX_Control(pDX, IDC_STATIC_THREAD_COUNT, m_StcThreads);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDC_EDIT_CONSOLE_LOG_FILE, m_EdtLog);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_LOG, m_BtnBrowse);
    DDX_Control(pDX, IDC_EDIT_THREADCOUNT, m_EdtThreads);
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOG, OnBnClickedButtonBrowseLog)
END_MESSAGE_MAP()

BOOL CAdvancedDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    this->SetAdvancedOptions();

    return TRUE;
}

void CAdvancedDlg::GetAdvancedOptions()
{
    m_Options.bDeleteOnError = (this->IsDlgButtonChecked(IDC_CHECK_OPTION_DELETE) == BST_CHECKED) ? true : false;
    m_Options.bStopOnErrors = (this->IsDlgButtonChecked(IDC_CHECK_OPTION_ERROR) == BST_CHECKED) ? true : false;

    m_EdtLog.GetWindowText(m_Options.szLogFileName);

    int nCheckID = this->GetCheckedRadioButton(IDC_RADIO_ENCODING_ANSI, IDC_RADIO_ENCODING_UTF8);

    if (nCheckID == IDC_RADIO_ENCODING_ANSI)
        m_Options.nLogEncoding = 0;
    else if (nCheckID == IDC_RADIO_ENCODING_UNICODE)
        m_Options.nLogEncoding = 1;
    else if (nCheckID == IDC_RADIO_ENCODING_UTF8)
        m_Options.nLogEncoding = 2;
    else
        m_Options.nLogEncoding = 2;

    CString szThreadCount;
    m_EdtThreads.GetWindowText(szThreadCount);
    m_Options.nThreadCount = _tstoi(szThreadCount);
}

void CAdvancedDlg::SetAdvancedOptions()
{
    this->CheckDlgButton(IDC_CHECK_OPTION_DELETE,
        (m_Options.bDeleteOnError == true) ? BST_CHECKED : BST_UNCHECKED);

    this->CheckDlgButton(IDC_CHECK_OPTION_ERROR,
        (m_Options.bStopOnErrors == true) ? BST_CHECKED : BST_UNCHECKED);

    m_EdtLog.SetWindowText(m_Options.szLogFileName);

    switch (m_Options.nLogEncoding)
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

    CString szThreadCount;
    szThreadCount.Format(_T("%d\0"), m_Options.nThreadCount);
    m_EdtThreads.SetWindowText(szThreadCount);
}

void CAdvancedDlg::OnOK()
{
    GetAdvancedOptions();
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
    CFileDialog fd(FALSE, _T("log"), m_Options.szLogFileName,
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Log Files (*.log)|*.log|Txt Files (*.txt)|*.txt|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        m_Options.szLogFileName = fd.GetPathName();
        m_EdtLog.SetWindowText(m_Options.szLogFileName);
    }
}
