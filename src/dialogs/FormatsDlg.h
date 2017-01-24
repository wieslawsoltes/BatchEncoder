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
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
    bool bShowGridLines;
    volatile bool bUpdate;
    int nSelectedFormat;
    CFormatsList m_Formats;
public:
    CMyStatic m_GrpPipes;
    CMyStatic m_StcPath;
    CMyStatic m_StcTemplate;
    CMyStatic m_StcProgress;
    CMyListCtrl m_LstFormats;
    CMyEdit m_EdtPath;
    CMyEdit m_EdtTemplate;
    CMyEdit m_EdtFunction;
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyButton m_BtnRemoveAll;
    CMyButton m_BtnRemove;
    CMyButton m_BtnAdd;
    CMyButton m_BtnMoveUp;
    CMyButton m_BtnMoveDown;
    CMyButton m_BtnUpdate;
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnBrowse;
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
    void ShowGridlines(bool bShow);
    void AddToList(CFormat &format, int nItem);
    void InsertFormatsToListCtrl();
    void UpdateFields(CFormat &format);
    void ListSelectionChange();
    void LoadFormats(CString szFileXml);
    void SaveFormats(CString szFileXml);
    bool BrowseForPath(CString szDefaultFName, CEdit *pEdit, int nID);
    bool BrowseForFunction(CString szDefaultFName, CEdit *pEdit, int nID);
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLvnItemchangedListFormats(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonRemoveAllFormats();
    afx_msg void OnBnClickedButtonRemoveFormat();
    afx_msg void OnBnClickedButtonAddFormat();
    afx_msg void OnBnClickedButtonFormatUp();
    afx_msg void OnBnClickedButtonFormatDown();
    afx_msg void OnBnClickedButtonUpdateFormat();
    afx_msg void OnBnClickedCheckPipesInput();
    afx_msg void OnBnClickedCheckPipesOutput();
    afx_msg void OnEnChangeEditFormatPath();
    afx_msg void OnEnChangeEditFormatTemplate();
    afx_msg void OnEnChangeEditFormatFunction();
    afx_msg void OnBnClickedButtonLoadFormats();
    afx_msg void OnBnClickedButtonSaveFormats();
    afx_msg void OnBnClickedButtonBrowsePath();
    afx_msg void OnBnClickedButtonBrowseProgress();
    afx_msg void OnClose();
};
