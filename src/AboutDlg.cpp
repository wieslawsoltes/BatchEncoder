//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "AboutDlg.h"
#include ".\aboutdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
    m_StcWebsite.SetBold(false);
    m_StcEmail.SetBold(false);
    m_StcLicense.SetBold(false);

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
