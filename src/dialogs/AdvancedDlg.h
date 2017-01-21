// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "..\controls\Controls.h"
#include "..\Configuration.h"
#include "..\XmlConfiguration.h"

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
    CMyButton m_BtnCancel;
    CMyButton m_BtnOK;
    CEdit m_EdtLog;
    CButton m_BtnBrowse;
public:
    void GetAdvancedOptions();
    void SetAdvancedOptions();
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonBrowseLog();
};
