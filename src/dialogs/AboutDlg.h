// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "..\controls\Controls.h"

class CAboutDlg : public CDialog
{
    DECLARE_DYNAMIC(CAboutDlg)
public:
    CAboutDlg(CWnd* pParent = NULL);
    virtual ~CAboutDlg();
    enum { IDD = IDD_DIALOG_ABOUT };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
protected:
    virtual void OnOK();
    virtual void OnCancel();
public:
    CMyButton m_BtnOK;
    CMyStatic m_StcMainAppName;
    CHyperlink m_StcWebsite;
    CHyperlink m_StcEmail;
    CMyStatic m_StcLicense;
public:
    afx_msg void OnClose();
    afx_msg void OnBnClickedOk();
};
