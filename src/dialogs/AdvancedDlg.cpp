// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\XmlConfiguration.h"
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
    DDX_Control(pDX, IDC_STATIC_THREAD_COUNT, m_StcThreads);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDC_EDIT_THREADCOUNT, m_EdtThreads);
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
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
