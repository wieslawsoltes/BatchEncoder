// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "..\controls\Controls.h"
#include "..\utilities\TimeCount.h"
#include "..\Configuration.h"
#include "..\XMLDocumentW.h"

#define WM_TRAY (WM_USER + 0x10)

#define IDC_STATUSBAR 1500

#define ADD_ITEM_MEMORY_AND_CONTROL 2

typedef struct TNewItemData
{
    CString szPath;
    CString szName;
    CString szOutExt;
    int nOutPreset;
    bool bChecked;
    CString szTime;
    CString szStatus;

} NewItemData, *PNewItemData;

class CBatchEncoderDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CBatchEncoderDlg)
public:
    CBatchEncoderDlg(CWnd* pParent = NULL);
    virtual ~CBatchEncoderDlg();
    enum { IDD = IDD_BATCHENCODER_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
protected:
    HICON m_hIcon;
    afx_msg HCURSOR OnQueryDragIcon();
protected:
    afx_msg void OnPaint();
protected:
    virtual BOOL OnInitDialog();
protected:
    DECLARE_MESSAGE_MAP()
protected:
    virtual void OnOK();
    virtual void OnCancel();
public:
    bool CreateBatchFile(CString szFileName);
public:
    CMyStatusBarCtrl m_StatusBar;
public:
    void UpdateStatusBar();
protected:
    CProgressCtrl m_Progress;
public:
    HACCEL m_hAccel;
public:
    BOOL PreTranslateMessage(MSG* pMsg);
public:
    CString szMainConfigFile;
public:
    CConfiguration m_Config;
public:
    HANDLE hThread;
    DWORD dwThreadID;
    volatile bool bRunning;
    volatile int nProgressCurrent;
    bool bSameAsSourceEdit;
public:
    CEdit m_EdtOutPath;
public:
    CProgressCtrl m_FileProgress;
public:
    CMyComboBox m_CmbPresets;
    CMyComboBox m_CmbFormat;
public:
    CItemListCtrl m_LstInputItems;
public:
    CMyStatic m_StcPreset;
    CMyStatic m_StcFormat;
public:
    CButton m_ChkOutPath;
public:
    CMyButton m_BtnConvert;
    CMyButton m_BtnBrowse;
public:
    bool WorkerCallback(int nProgress,
        bool bFinished,
        bool bError = false,
        double fTime = 0.0,
        int nIndex = -1);
public:
    int InsertToMemoryList(NewItemData &nid);
    void InsertToControlList(int nItem);
    bool InsertToList(NewItemData &nid);
public:
    void HandleDropFiles(HDROP hDropInfo);
public:
    void UpdateFormatAndPreset();
public:
    void ResetOutput();
    void ResetConvertionTime();
    void ResetConvertionStatus();
public:
    void SearchFolderForFiles(CString szFile, const bool bRecurse);
public:
    LPTSTR GetMenuItemCheck(int nID);
    void SetMenuItemCheck(int nID, LPTSTR bChecked);
public:
    bool GridlinesVisible();
    void ShowGridlines(bool bShow);
private:
    void InitNewItemData(NewItemData &nid);
public:
    void LoadItems(tinyxml2::XMLElement *pItemsElem);
    void SaveItems(CXMLDocumentW &doc, tinyxml2::XMLElement *pItemsElem);
public:
    void GetOptions();
    void SetOptions();
public:
    bool LoadList(CString szFileXml);
    bool SaveList(CString szFileXml);
public:
    bool LoadConfigFile(CString szFileXml);
    bool SaveConfigFile(CString szFileXml);
public:
    afx_msg void LoadUserConfig();
    afx_msg void SaveUserConfig();
public:
    void UpdateOutputComboBoxes();
public:
    void EnableUserInterface(BOOL bEnable = TRUE);
public:
    void EnableTrayIcon(bool bEnable = true, bool bModify = false);
    void ShowProgressTrayIcon(int nProgress);
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
public:
    afx_msg LRESULT OnTrayIconMsg(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTrayMenuExit();
public:
    afx_msg LRESULT OnListItemChaged(WPARAM wParam, LPARAM lParam);
protected:
    afx_msg LRESULT OnNotifyFormat(WPARAM wParam, LPARAM lParam);
public:
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnDropFiles(HDROP hDropInfo);
public:
    afx_msg void OnBnClickedCheckOutPath();
public:
    afx_msg void OnBnClickedButtonConvert();
    afx_msg void OnBnClickedButtonBrowsePath();
public:
    afx_msg void OnCbnSelchangeComboPresets();
    afx_msg void OnCbnSelchangeComboFormat();
public:
    afx_msg void OnLvnKeydownListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnNMRclickListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnEnChangeEditOutPath();
    afx_msg void OnEnSetFocusEditOutPath();
    afx_msg void OnEnKillFocusEditOutPath();
public:
    afx_msg void OnFileLoadList();
    afx_msg void OnFileSaveList();
    afx_msg void OnFileClearList();
    afx_msg void OnFileCreateBatchFile();
    afx_msg void OnFileExit();
public:
    afx_msg void OnEditAddFiles();
    afx_msg void OnEditClear();
    afx_msg void OnEditRemoveChecked();
    afx_msg void OnEditRemoveUnchecked();
    afx_msg void OnEditCheckSelected();
    afx_msg void OnEditUncheckSelected();
    afx_msg void OnEditRename();
    afx_msg void OnEditOpen();
    afx_msg void OnEditExplore();
    afx_msg void OnEditCrop();
    afx_msg void OnEditSelectNone();
    afx_msg void OnEditInvertSelection();
    afx_msg void OnEditRemove();
    afx_msg void OnEditAddDir();
    afx_msg void OnEditSelectAll();
    afx_msg void OnEditResetOutput();
    afx_msg void OnEditResetTime();
public:
    afx_msg void OnViewShowGridLines();
public:
    afx_msg void OnActionConvert();
public:
    afx_msg void OnOptionsStayOnTop();
    afx_msg void OnOptionsShowTrayIcon();
    afx_msg void OnOptionsLogConsoleOutput();
    afx_msg void OnOptionsShowLog();
    afx_msg void OnOptionsDeleteLog();
    afx_msg void OnOptionsDeleteSourceFileWhenDone();
    afx_msg void OnOptionsShutdownWhenFinished();
    afx_msg void OnOptionsDoNotSave();
    afx_msg void OnOptionsForceConsoleWindow();
    afx_msg void OnOptionsAdvanced();
    afx_msg void OnOptionsConfigurePresets();
    afx_msg void OnOptionsConfigureFormat();
public:
    afx_msg void OnHelpWebsite();
    afx_msg void OnHelpAbout();
};
