// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "controls\MyListCtrl.h"
#include "controls\MyButton.h"
#include "controls\MyComboBox.h"
#include "controls\MyEdit.h"
#include "controls\MyStatic.h"
#include "controls\MyDialogEx.h"
#include "configuration\FormatsList.h"
#include "configuration\Configuration.h"

#define TOOL_COLUMN_NAME      0
#define TOOL_COLUMN_URL       1

class CToolsDlg;

typedef struct tagToolsDlgDropContext
{
    CToolsDlg *pDlg = NULL;
    HDROP hDrop = NULL;
    HANDLE hThread = NULL;
    DWORD dwThreadID = -1;
    volatile bool bHandled = true;
} ToolsDlgDropContext;

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
    ToolsDlgDropContext m_DD;
    CConfiguration *pConfig;
    CString szToolsDialogResize;
    CString szToolsListColumns;
    volatile bool bUpdate;
    int nSelectedFormat;
    CFormatsList m_Formats;
public:
    CMyStatic m_StcName;
    CMyStatic m_StcPlatform;
    CMyStatic m_StcFormats;
    CMyStatic m_StcUrl;
    CMyStatic m_StcFile;
    CMyStatic m_StcExtract;
    CMyStatic m_StcPath;
    CMyListCtrl m_LstTools;
    CMyEdit m_EdtName;
    CMyEdit m_EdtPlatform;
    CMyEdit m_EdtFormats;
    CMyEdit m_EdtUrl;
    CMyEdit m_EdtFile;
    CMyEdit m_EdtExtract;
    CMyEdit m_EdtPath;
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyButton m_BtnImport;
    CMyButton m_BtnExport;
    CMyButton m_BtnDuplicate;
    CMyButton m_BtnRemoveAll;
    CMyButton m_BtnRemove;
    CMyButton m_BtnAdd;
    CMyButton m_BtnMoveUp;
    CMyButton m_BtnMoveDown;
    CMyButton m_BtnUpdate;
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnEditFormats;
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLvnItemchangedListTools(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonImport();
    afx_msg void OnBnClickedButtonExport();
    afx_msg void OnBnClickedButtonDuplicate();
    afx_msg void OnBnClickedButtonRemoveAllTools();
    afx_msg void OnBnClickedButtonRemoveTool();
    afx_msg void OnBnClickedButtonAddTool();
    afx_msg void OnBnClickedButtonToolUp();
    afx_msg void OnBnClickedButtonToolDown();
    afx_msg void OnBnClickedButtonUpdateTool();
    afx_msg void OnEnChangeEditToolName();
    afx_msg void OnEnChangeEditToolPlatform();
    afx_msg void OnEnChangeEditToolFormats();
    afx_msg void OnEnChangeEditToolUrl();
    afx_msg void OnEnChangeEditToolFile();
    afx_msg void OnEnChangeEditToolExtract();
    afx_msg void OnEnChangeEditToolPath();
    afx_msg void OnBnClickedButtonLoadTools();
    afx_msg void OnBnClickedButtonSaveTools();
    afx_msg void OnBnClickedButtonEditFormats();
    afx_msg void OnClose();
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
    void SetLanguage();
    void AddToList(CFormat &format, int nItem);
    void InsertFormatsToListCtrl();
    void HandleDropFiles(HDROP hDropInfo);
    void UpdateFields(CFormat &format);
    void ListSelectionChange();
    void LoadTool(CString szFileXml);
    void SaveTool(CString szFileXml, CFormat &format);
    void LoadTools(CString szFileXml);
    void SaveTools(CString szFileXml);
};
