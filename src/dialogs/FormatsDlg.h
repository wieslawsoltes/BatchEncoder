// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "..\controls\MyListCtrl.h"
#include "..\controls\MyButton.h"
#include "..\controls\MyComboBox.h"
#include "..\controls\MyEdit.h"
#include "..\controls\MyStatic.h"
#include "..\controls\ResizeDialog.h"
#include "..\configuration\FormatsList.h"
#include "..\configuration\Configuration.h"

#define FORMAT_COLUMN_NAME      0
#define FORMAT_COLUMN_TEMPLATE  1

class CFormatsDlg;

typedef struct tagFormatsDlgDropContext
{
    CFormatsDlg *pDlg = NULL;
    HDROP hDrop = NULL;
    HANDLE hThread = NULL;
    DWORD dwThreadID = -1;
    volatile bool bHandled = true;
} FormatsDlgDropContext;

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
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
public:
    virtual BOOL OnInitDialog();
public:
    FormatsDlgDropContext m_DD;
    CConfiguration *pConfig;
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
    volatile bool bUpdate;
    int nSelectedFormat;
    CFormatsList m_Formats;
public:
    CMyStatic m_GrpPipes;
    CMyStatic m_GrpTypes;
    CMyStatic m_StcId;
    CMyStatic m_StcName;
    CMyStatic m_StcExtension;
    CMyStatic m_StcFormats;
    CMyStatic m_StcCode;
    CMyStatic m_StcDefault;
    CMyStatic m_StcPath;
    CMyStatic m_StcTemplate;
    CMyStatic m_StcProgress;
    CMyListCtrl m_LstFormats;
    CMyComboBox m_CmbDefault;
    CMyEdit m_EdtId;
    CMyEdit m_EdtName;
    CMyEdit m_EdtExtension;
    CMyEdit m_EdtFormats;
    CMyEdit m_EdtCode;
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
    CMyButton m_BtnEditPresets;
    CMyButton m_BtnBrowsePath;
    CMyButton m_BtnBrowseFunction;
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLvnItemchangedListFormats(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonRemoveAllFormats();
    afx_msg void OnBnClickedButtonRemoveFormat();
    afx_msg void OnBnClickedButtonAddFormat();
    afx_msg void OnBnClickedButtonFormatUp();
    afx_msg void OnBnClickedButtonFormatDown();
    afx_msg void OnBnClickedButtonUpdateFormat();
    afx_msg void OnBnClickedRadioTypeEncoder();
    afx_msg void OnBnClickedRadioTypeDecoder();
    afx_msg void OnBnClickedCheckPipesInput();
    afx_msg void OnBnClickedCheckPipesOutput();
    afx_msg void OnEnChangeEditFormatId();
    afx_msg void OnEnChangeEditFormatName();
    afx_msg void OnEnChangeEditFormatExtension();
    afx_msg void OnEnChangeEditFormatFormats();
    afx_msg void OnCbnSelchangeComboDefault();
    afx_msg void OnEnChangeEditFormatPath();
    afx_msg void OnEnChangeEditFormatTemplate();
    afx_msg void OnEnChangeEditFormatFunction();
    afx_msg void OnBnClickedButtonLoadFormats();
    afx_msg void OnBnClickedButtonSaveFormats();
    afx_msg void OnBnClickedButtonEditPresets();
    afx_msg void OnBnClickedButtonBrowsePath();
    afx_msg void OnBnClickedButtonBrowseProgress();
    afx_msg void OnClose();
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
    void SetLanguage();
    void AddToList(CFormat &format, int nItem);
    void InsertFormatsToListCtrl();
    void HandleDropFiles(HDROP hDropInfo);
    void UpdateFields(CFormat &format);
    void UpdateDefaultComboBox(CFormat &format);
    void ListSelectionChange();
    void LoadFormats(CString szFileXml);
    void SaveFormats(CString szFileXml);
    bool BrowseForPath(CString szDefaultFName, CEdit *pEdit, int nID);
    bool BrowseForFunction(CString szDefaultFName, CEdit *pEdit, int nID);
};
