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
#include "CopyFileDlg.h"
#include ".\copyfiledlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCopyFileDlg, CDialog)
CCopyFileDlg::CCopyFileDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCopyFileDlg::IDD, pParent)
{
    bNoFiles = false;
    bStop = false;

    nProgress = 0;

    szInFile = _T("");
    szOutFile = _T("");
}

CCopyFileDlg::~CCopyFileDlg()
{

}

void CCopyFileDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_TEXT_FROM, m_StcTextFrom);
    DDX_Control(pDX, IDC_STATIC_TEXT_TO, m_StcTextTo);
    DDX_Control(pDX, IDC_PROGRESS_COPY_FILE, m_PrgCopy);
    DDX_Control(pDX, IDC_BUTTON_COPY_STOP, m_BtnCancel);
}

BEGIN_MESSAGE_MAP(CCopyFileDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_COPY_STOP, OnBnClickedButtonCopyStop)
END_MESSAGE_MAP()

CCopyFileDlg *pDlgCopy = NULL;

bool ProgressCallback(int nProgress)
{
    if(pDlgCopy != NULL)
    {
        pDlgCopy->nProgress = nProgress;
        pDlgCopy->m_PrgCopy.SetPos(nProgress);
        return pDlgCopy->bStop;
    }
    return true;
}

DWORD WINAPI CopyWorkThread(LPVOID lpParam)
{
    CCopyFileDlg *pDlg = (CCopyFileDlg *) lpParam;
    if(pDlg == NULL)
        return (DWORD) (-1);

    bool bRet = ::CopyOneFile(pDlg->szInFile, pDlg->szOutFile, ProgressCallback);
    if(bRet == false)
    {
        if(pDlg->bStop == false)
            pDlg->MessageBox(_T("Failed To Copy File!"), _T("ERROR"), MB_OK | MB_ICONERROR);

        Sleep(100);
        pDlg->EndDialog(0);
        return (DWORD) (0);
    }
    else
    {
        Sleep(100);
        pDlg->EndDialog(0);
        return (DWORD) (1);
    }
}

BOOL CCopyFileDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if(bNoFiles == true)
    {
        bNoFiles = false;

        this->GetInFilePath();
        if(szInFile.GetLength() <= 0)
        {
            this->EndDialog(0);
            return TRUE;
        }

        this->GetOutFilePath();
        if(szOutFile.GetLength() <= 0)
        {
            this->EndDialog(0);
            return TRUE;
        }
    }   

    if((szInFile.GetLength() <= 0) || (szOutFile.GetLength() <= 0))
    {
        this->EndDialog(0);
        return TRUE;
    }

    if(szInFile.CompareNoCase(szOutFile) == 0)
    {
        this->EndDialog(0);
        return TRUE;
    }

    pDlgCopy = this;
    m_PrgCopy.SetRange32(0, 100);
    m_PrgCopy.SetPos(0);

    m_StcTextFrom.SetWindowText(szInFile);
    m_StcTextTo.SetWindowText(szOutFile);

    dwThreadId = 0L;
    hThread = ::CreateThread(NULL, 0, CopyWorkThread, this, CREATE_SUSPENDED, &dwThreadId);
    if(hThread == NULL)
    {
        this->MessageBox(_T("Failed To Copy File!"), _T("ERROR"), MB_OK | MB_ICONERROR);
        this->EndDialog(0);
        return TRUE;
    }
    else
    {
        ::ResumeThread(hThread);
    }
    return TRUE;
}

void CCopyFileDlg::GetInFilePath()
{
    CFileDialog fd(TRUE, _T(""), _T(""), 
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER, 
        _T("All Files|*.*||"), this);

    if(fd.DoModal() == IDOK)
        szInFile = fd.GetPathName();
    else
        szInFile = _T("");
}

void CCopyFileDlg::GetOutFilePath()
{
    CFileDialog fd(FALSE, _T(""), ::GetFileName(szInFile), 
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT, 
        _T("All Files|*.*||"), this);

    if(fd.DoModal() == IDOK)
        szOutFile = fd.GetPathName();
    else
        szOutFile = _T("");
}

void CCopyFileDlg::OnCancel()
{
    // CDialog::OnCancel();
}

void CCopyFileDlg::OnOK()
{
    // CDialog::OnOK();
}

void CCopyFileDlg::OnClose()
{
    CDialog::OnClose();

    CDialog::OnOK();
}

void CCopyFileDlg::OnDestroy()
{
    CDialog::OnDestroy();
}

void CCopyFileDlg::OnBnClickedButtonCopyStop()
{
    // ::TerminateThread(hThread, 0);
    bStop = true;

    // this->OnClose();
}
