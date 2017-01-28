// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "..\controls\Controls.h"
#include "..\Configuration.h"

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
protected:
    virtual void OnOK();
    virtual void OnCancel();
public:
    COptions m_Options;
public:
    CMyStatic m_GrpMisc;
    CMyStatic m_StcThreads;
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyEdit m_EdtThreads;
public:
    void GetAdvancedOptions();
    void SetAdvancedOptions();
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
};
