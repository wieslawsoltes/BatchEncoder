// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "AboutDlg.h"

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAboutDlg::IDD, pParent)
{

}

CAboutDlg::~CAboutDlg()
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDC_STATIC_TEXT_APP_NAME, m_StcMainAppName);
    DDX_Control(pDX, IDC_STATIC_TEXT_WEBSITE, m_StcWebsite);
    DDX_Control(pDX, IDC_STATIC_TEXT_EMAIL, m_StcEmail);
    DDX_Control(pDX, IDC_STATIC_LICENSE, m_StcLicense);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_StcMainAppName.SetBold(true);

    m_StcMainAppName.SetWindowText(MAIN_APP_NAME_VER);
    m_StcWebsite.SetWindowText(MAIN_APP_WEBSITE);
    m_StcWebsite.SetTargetUrl(MAIN_APP_WEBSITE);

    CString szEmail;
    szEmail.Format(_T("mailto:%s"), MAIN_APP_EMAIL);
    m_StcEmail.SetWindowText(MAIN_APP_EMAIL);
    m_StcEmail.SetTargetUrl(szEmail);

    return TRUE;
}

void CAboutDlg::OnOK()
{
    CDialog::OnOK();
}

void CAboutDlg::OnCancel()
{
    CDialog::OnCancel();
}

void CAboutDlg::OnClose()
{
    CDialog::OnClose();
}

void CAboutDlg::OnBnClickedOk()
{
    OnOK();
}
