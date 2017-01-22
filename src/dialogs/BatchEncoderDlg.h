// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "..\controls\Controls.h"
#include "..\utilities\TimeCount.h"
#include "..\Configuration.h"
#include "..\XmlConfiguration.h"
#include "..\worker\WorkThread.h"

class CBatchEncoderDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CBatchEncoderDlg)
public:
    CBatchEncoderDlg(CWnd* pParent = NULL);
    virtual ~CBatchEncoderDlg();
    enum { IDD = IDD_BATCHENCODER_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
protected:
    HICON m_hIcon;
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnPaint();
public:
    virtual BOOL OnInitDialog();
protected:
    virtual void OnOK();
    virtual void OnCancel();
protected:
    HACCEL m_hAccel;
    BOOL PreTranslateMessage(MSG* pMsg);
public:
    CString szConfigFile;
    CConfiguration m_Config;
    WorkerContext* pWorkerContext;
    bool bSameAsSourceEdit;
public:
    CMyStatic m_StcPreset;
    CMyStatic m_StcFormat;
    CMyComboBox m_CmbPresets;
    CMyComboBox m_CmbFormat;
    CItemListCtrl m_LstInputItems;
    CButton m_ChkOutPath;
    CMyButton m_BtnBrowse;
    CEdit m_EdtOutPath;
    CProgressCtrl m_Progress;
    CMyButton m_BtnConvert;
    CMyStatusBarCtrl m_StatusBar;
public:
    void UpdateStatusBar();
    int InsertToMemoryList(CString szPath);
    bool AddToList(CString szPath);
    void AddToList(CItem &item, int nItem);
    void HandleDropFiles(HDROP hDropInfo);
    void UpdateFormatAndPreset();
    void ResetOutput();
    void ResetConvertionTime();
    void ResetConvertionStatus();
    void SearchFolderForFiles(CString szFile, const bool bRecurse);
    bool GridlinesVisible();
    void ShowGridlines(bool bShow);
    void GetItems();
    void SetItems();
    void GetOptions();
    void SetOptions();
    bool LoadItems(CString szFileXml);
    bool SaveItems(CString szFileXml);
    bool LoadConfiguration(CString szFileXml);
    bool SaveConfiguration(CString szFileXml);
    void UpdateFormatComboBox();
    void UpdatePresetComboBox();
    void EnableUserInterface(BOOL bEnable = TRUE);
    void EnableTrayIcon(bool bEnable = true, bool bModify = false);
    void ShowProgressTrayIcon(int nProgress);
    void StartConvert();
    void FinishConvert();
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnTrayIconMsg(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTrayMenuExit();
    afx_msg LRESULT OnListItemChaged(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnNotifyFormat(WPARAM wParam, LPARAM lParam);
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg void OnBnClickedCheckOutPath();
    afx_msg void OnBnClickedButtonConvert();
    afx_msg void OnBnClickedButtonBrowsePath();
    afx_msg void OnCbnSelchangeComboPresets();
    afx_msg void OnCbnSelchangeComboFormat();
    afx_msg void OnLvnKeydownListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRclickListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkListInputFiles(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditOutPath();
    afx_msg void OnEnSetFocusEditOutPath();
    afx_msg void OnEnKillFocusEditOutPath();
    afx_msg void LoadConfiguration();
    afx_msg void SaveConfiguration();
    afx_msg void OnFileLoadList();
    afx_msg void OnFileSaveList();
    afx_msg void OnFileClearList();
    afx_msg void OnFileExit();
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
    afx_msg void OnViewShowGridLines();
    afx_msg void OnActionConvert();
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
    afx_msg void OnHelpWebsite();
    afx_msg void OnHelpAbout();
};

class CBatchEncoderWorkerContext : public WorkerContext
{
    CTimeCount timeCount;
    CBatchEncoderDlg *pDlg;
public:
    CBatchEncoderWorkerContext(CConfiguration* pConfig, CBatchEncoderDlg* pDlg) : WorkerContext(pConfig)
    {
        this->pDlg = pDlg;
    }
public:
    void Init();
    void Next(int nIndex);
    void Done();
    bool Callback(int nIndex, int nProgress, bool bFinished, bool bError = false, double fTime = 0.0);
    void Status(int nIndex, CString szTime, CString szStatus);
};
