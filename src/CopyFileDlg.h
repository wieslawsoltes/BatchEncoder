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

#pragma once

#include "afxwin.h"
#include "afxcmn.h"

class CCopyFileDlg : public CDialog
{
    DECLARE_DYNAMIC(CCopyFileDlg)
public:
    CCopyFileDlg(CWnd* pParent = NULL);
    virtual ~CCopyFileDlg();
    enum { IDD = IDD_DIALOG_COPYFILE };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
protected:
    virtual void OnCancel();
    virtual void OnOK();
public:
    CStatic m_StcTextFrom;
    CStatic m_StcTextTo;
    CProgressCtrl m_PrgCopy;
    CButton m_BtnCancel;
private:
    HANDLE hThread;
    DWORD dwThreadId;
public:
    CString szInFile;
    CString szOutFile;
public:
    bool bNoFiles;
    volatile bool bStop;
public:
    volatile int nProgress;
public:
    void GetInFilePath();
    void GetOutFilePath();
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonCopyStop();
};
