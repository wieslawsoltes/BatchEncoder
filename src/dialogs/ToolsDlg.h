// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "controls\MyDialogEx.h"

class CToolsDlg;

class CToolsDlg : public CMyDialogEx
{
    DECLARE_DYNAMIC(CToolsDlg)
public:
    CToolsDlg(CWnd* pParent = NULL);
    virtual ~CToolsDlg();
    enum { IDD = IDD_DIALOG_TOOLS };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
protected:
    HICON m_hIcon;
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
public:
    virtual BOOL OnInitDialog();
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnClose();
};
