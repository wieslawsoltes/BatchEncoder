// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "..\controls\Controls.h"
#include "..\utilities\TimeCount.h"
#include "..\Configuration.h"
#include "..\worker\WorkThread.h"

class CBatchEncoderDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CBatchEncoderDlg)
public:
    CBatchEncoderDlg(CWnd* pParent = NULL);
    virtual ~CBatchEncoderDlg();
    enum { IDD = IDD_DIALOG_BATCHENCODER };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
protected:
    HICON m_hIcon;
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
public:
    virtual BOOL OnInitDialog();
protected:
    virtual void OnOK();
    virtual void OnCancel();
protected:
    HACCEL m_hAccel;
    BOOL PreTranslateMessage(MSG* pMsg);
public:
    CString szOptionsFile;
    CString szFormatsFile;
    CString szItemsFile;
    CConfiguration m_Config;
    CWorkerContext* pWorkerContext;
    bool bSameAsSourceEdit;
public:
    CMyStatic m_StcPreset;
    CMyStatic m_StcFormat;
    CMyStatic m_StcThreads;
    CMyComboBox m_CmbPresets;
    CMyComboBox m_CmbFormat;
    CMyListCtrl m_LstInputItems;
    CButton m_ChkOutPath;
    CMyButton m_BtnBrowse;
    CMyEdit m_EdtOutPath;
    CMyEdit m_EdtThreads;
    CProgressCtrl m_Progress;
    CMyButton m_BtnConvert;
    CMyStatusBarCtrl m_StatusBar;
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnDropFiles(HDROP hDropInfo);
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg LRESULT OnListItemChaged(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnNotifyFormat(WPARAM wParam, LPARAM lParam);
    afx_msg void OnLvnKeydownListInputItems(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRclickListInputItems(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangingListInputItems(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLvnItemchangedListInputItems(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeComboPresets();
    afx_msg void OnCbnSelchangeComboFormat();
    afx_msg void OnBnClickedCheckOutPath();
    afx_msg void OnEnSetFocusEditOutPath();
    afx_msg void OnEnKillFocusEditOutPath();
    afx_msg void OnBnClickedButtonBrowsePath();
    afx_msg void OnBnClickedButtonConvert();
    afx_msg void OnFileLoadList();
    afx_msg void OnFileSaveList();
    afx_msg void OnFileClearList();
    afx_msg void OnFileExit();
    afx_msg void OnEditAddFiles();
    afx_msg void OnEditAddDir();
    afx_msg void OnEditRename();
    afx_msg void OnEditResetTime();
    afx_msg void OnEditResetOutput();
    afx_msg void OnEditRemove();
    afx_msg void OnEditCrop();
    afx_msg void OnEditRemoveChecked();
    afx_msg void OnEditRemoveUnchecked();
    afx_msg void OnEditCheckSelected();
    afx_msg void OnEditUncheckSelected();
    afx_msg void OnEditSelectAll();
    afx_msg void OnEditSelectNone();
    afx_msg void OnEditInvertSelection();
    afx_msg void OnEditOpen();
    afx_msg void OnEditExplore();
    afx_msg void OnActionConvert();
    afx_msg void OnOptionsConfigurePresets();
    afx_msg void OnOptionsConfigureFormat();
    afx_msg void OnOptionsDeleteSource();
    afx_msg void OnOptionsShutdownWindows();
    afx_msg void OnOptionsDoNotSave();
    afx_msg void OnOptionsDeleteOnErrors();
    afx_msg void OnOptionsStopOnErrors();
    afx_msg void OnHelpWebsite();
    afx_msg void OnHelpAbout();
public:
    void GetItems();
    void SetItems();
    void GetOptions();
    void SetOptions();
    bool LoadOptions(CString szFileXml);
    bool SaveOptions(CString szFileXml);
    bool LoadFormats(CString szFileXml);
    bool SaveFormats(CString szFileXml);
    bool LoadItems(CString szFileXml);
    bool SaveItems(CString szFileXml);
    int AddToItems(CString szPath);
    bool AddToList(CString szPath);
    void AddToList(CItem &item, int nItem);
    void HandleDropFiles(HDROP hDropInfo);
    void SearchFolderForFiles(CString szFile, const bool bRecurse);
    void UpdateFormatComboBox();
    void UpdatePresetComboBox();
    void UpdateFormatAndPreset();
    void ResetOutput();
    void ResetConvertionTime();
    void ResetConvertionStatus();
    void UpdateStatusBar();
    void EnableUserInterface(BOOL bEnable = TRUE);
    void StartConvert();
    void FinishConvert();
};

class CBatchEncoderWorkerContext : public CWorkerContext
{
    CTimeCount timeCount;
    CBatchEncoderDlg *pDlg;
public:
    CBatchEncoderWorkerContext(CConfiguration* pConfig, CBatchEncoderDlg* pDlg) : CWorkerContext(pConfig)
    {
        this->pDlg = pDlg;
    }
public:
    void Init();
    void Next(int nItemId);
    void Done();
    bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false);
    void Status(int nItemId, CString szTime, CString szStatus);
};
