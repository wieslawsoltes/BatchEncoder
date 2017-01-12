// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "afxwin.h"
#include "DropList.h"
#include "MyButton.h"

class CAdvancedDlg : public CDialog
{
    DECLARE_DYNAMIC(CAdvancedDlg)
public:
    CAdvancedDlg(CWnd* pParent = NULL);
    virtual ~CAdvancedDlg();
    enum { IDD = IDD_DIALOG_ADVANCED };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
public:
    CMyButton m_BtnCancel;
    CMyButton m_BtnOK;
public:
    CDropList m_CmbThread;
    CDropList m_CmbProcess;
public:
    CEdit m_EdtLog;
    CButton m_BtnBrowse;
public:
    COptions m_Options;
public:
    int GetTheThreadPriority(void);
    DWORD GetTheProcessPriority(void);
public:
    void GetAdvSettings();
    void SetAdvSettings();
protected:
    virtual void OnOK();
    virtual void OnCancel();
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
public:
    afx_msg void OnBnClickedButtonBrowseLog();
};
