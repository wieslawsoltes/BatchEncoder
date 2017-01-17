// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "..\controls\Controls.h"
#include "..\Configuration.h"
#include "..\XmlConfiguration.h"

class CFormatsDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CFormatsDlg)
public:
    CFormatsDlg(CWnd* pParent = NULL);
    virtual ~CFormatsDlg();
    enum { IDD = IDD_DIALOG_FORMATS };
protected:
    HICON m_hIcon;
    afx_msg HCURSOR OnQueryDragIcon();
protected:
    afx_msg void OnPaint();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
public:
    CFormatsList m_Formats;
public:
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
public:
    CListCtrl m_LstFormats;
public:
    CEdit m_EdtPath;
    CEdit m_EdtTemplate;
    CEdit m_EdtProgress;
public:
    CMyStatic m_GrpPipes;
    CMyStatic m_StcPath;
    CMyStatic m_StcTemplate;
    CMyStatic m_StcProgress;
public:
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnBrowse;
    CMyButton m_BtnChange;
public:
    bool bShowGridLines;
    void ShowGridlines(bool bShow);
public:
    void InsertFormatsToListCtrl();
    void UpdateFormatsFromListCtrl();
public:
    void LoadFormatsFile(CString szFileXml);
    void SaveFormatsFile(CString szFileXml);
public:
    void UpdateEditableFields(void);
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonLoadConfig();
    afx_msg void OnBnClickedButtonSaveConfig();
    afx_msg void OnBnClickedButtonFdBrowse();
    afx_msg void OnBnClickedButtonFdBrowseProgress();
    afx_msg void OnBnClickedButtonFdUpdatePreset();
public:
    bool BrowseForCliExe(CString szDefaultFName, CEdit *pEdit, int nID);
    bool BrowseForProgress(CString szDefaultFName, CEdit *pEdit, int nID);
public:
    afx_msg void OnLvnItemchangedEditFdFormats(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnEnChangeEditFdCliPath();
    afx_msg void OnEnChangeEditFdCliTemplate();
    afx_msg void OnEnChangeEditFdCliProgress();
public:
    afx_msg void OnBnClickedCheckFdPipesInput();
    afx_msg void OnBnClickedCheckFdPipesOutput();
public:
    afx_msg void OnClose();
};
