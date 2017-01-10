// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

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
