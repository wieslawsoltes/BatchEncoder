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
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
protected:
    HICON m_hIcon;
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnPaint();
public:
    virtual BOOL OnInitDialog();
public:
    CFormatsList m_Formats;
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
    bool bShowGridLines;
public:
    CMyListCtrl m_LstFormats;
    CMyEdit m_EdtPath;
    CMyEdit m_EdtTemplate;
    CMyEdit m_EdtProgress;
    CMyStatic m_GrpPipes;
    CMyStatic m_StcPath;
    CMyStatic m_StcTemplate;
    CMyStatic m_StcProgress;
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnBrowse;
    CMyButton m_BtnChange;
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
    void ShowGridlines(bool bShow);
    void AddToList(CFormat &format, int nItem);
    void InsertFormatsToListCtrl();
    void UpdateFormatsFromListCtrl();
    void LoadFormats(CString szFileXml);
    void SaveFormats(CString szFileXml);
    void UpdateEditableFields(void);
    bool BrowseForCliExe(CString szDefaultFName, CEdit *pEdit, int nID);
    bool BrowseForProgress(CString szDefaultFName, CEdit *pEdit, int nID);
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonLoadConfig();
    afx_msg void OnBnClickedButtonSaveConfig();
    afx_msg void OnBnClickedButtonFdBrowse();
    afx_msg void OnBnClickedButtonFdBrowseProgress();
    afx_msg void OnBnClickedButtonFdUpdatePreset();
    afx_msg void OnLvnItemchangedEditFdFormats(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditFdCliPath();
    afx_msg void OnEnChangeEditFdCliTemplate();
    afx_msg void OnEnChangeEditFdCliProgress();
    afx_msg void OnBnClickedCheckFdPipesInput();
    afx_msg void OnBnClickedCheckFdPipesOutput();
    afx_msg void OnClose();
};
