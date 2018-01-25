// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include <string>
#include <utility>
#include "controls\MyListCtrl.h"
#include "controls\MyButton.h"
#include "controls\MyComboBox.h"
#include "controls\MyEdit.h"
#include "controls\MyStatic.h"
#include "controls\MyStatusBarCtrl.h"
#include "controls\MySpinButtonCtrl.h"
#include "controls\MyProgressCtrl.h"
#include "controls\MyDialogEx.h"
#include "utilities\StringHelper.h"
#include "utilities\Thread.h"
#include "utilities\TimeCount.h"
#include "xml\XmlDoc.h"
#include "worker\WorkerContext.h"
#include "worker\Worker.h"

#define ITEM_COLUMN_NAME    0
#define ITEM_COLUMN_INPUT   1
#define ITEM_COLUMN_SIZE    2
#define ITEM_COLUMN_OUTPUT  3
#define ITEM_COLUMN_PRESET  4
#define ITEM_COLUMN_OPTIONS 5
#define ITEM_COLUMN_TIME    6
#define ITEM_COLUMN_STATUS  7

namespace app
{
    class CMainDlg;

    typedef struct tagMainDlgDropContext
    {
        CMainDlg *pDlg = nullptr;
        HDROP hDrop = nullptr;
        HANDLE hThread = nullptr;
        DWORD dwThreadID = -1;
        volatile bool bHandled = true;
    } MainDlgDropContext;

    class CMainDlg : public controls::CMyDialogEx
    {
        DECLARE_DYNAMIC(CMainDlg)
    public:
        CMainDlg(CWnd* pParent = nullptr);
        virtual ~CMainDlg();
        enum { IDD = IDD_DIALOG_MAIN };
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
        MainDlgDropContext m_DD;
        config::CConfiguration m_Config;
    public:
        util::CThread m_WorkerThread;
        worker::IWorkerContext* pWorkerContext;
        worker::CWorker m_Worker;
    public:
        int nEdtItem;
        int nEdtSubItem;
        CString szEdtText;
        controls::CMyEdit m_EdtItem;
    public:
        controls::CMyStatic m_GrpOutput;
        controls::CMyStatic m_StcPreset;
        controls::CMyStatic m_StcFormat;
        controls::CMyStatic m_StcThreads;
        controls::CMyButton m_StcOutPath;
        controls::CMyComboBox m_CmbPresets;
        controls::CMyComboBox m_CmbFormat;
        controls::CMyComboBox m_CmbOutPath;
        controls::CMyListCtrl m_LstInputItems;
        controls::CMyButton m_BtnBrowse;
        controls::CMyEdit m_EdtThreads;
        controls::CMySpinButtonCtrl m_SpinThreads;
        controls::CMyProgressCtrl m_Progress;
        controls::CMyButton m_BtnConvert;
        controls::CMyStatusBarCtrl m_StatusBar;
    public:
        afx_msg void OnClose();
        afx_msg void OnDestroy();
        afx_msg void OnDropFiles(HDROP hDropInfo);
        afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
        afx_msg void OnLvnGetdispinfoListItems(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnOdfindListItems(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg LRESULT OnListItemChaged(WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT OnNotifyFormat(WPARAM wParam, LPARAM lParam);
        afx_msg void OnNMClickListItems(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnEnKillfocusEditItem();
        afx_msg void OnLvnKeydownListInputItems(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnNMRclickListInputItems(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnLvnItemchangingListInputItems(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnLvnItemchangedListInputItems(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnCbnSelchangeComboPresets();
        afx_msg void OnCbnSelchangeComboFormat();
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
        afx_msg void OnEditToggleSelected();
        afx_msg void OnEditSelectAll();
        afx_msg void OnEditSelectNone();
        afx_msg void OnEditInvertSelection();
        afx_msg void OnEditOpen();
        afx_msg void OnEditExplore();
        afx_msg void OnActionConvert();
        afx_msg void OnOptionsConfigurePresets();
        afx_msg void OnOptionsConfigureFormat();
        afx_msg void OnOptionsConfigureTools();
        afx_msg void OnOptionsDeleteSource();
        afx_msg void OnOptionsShutdownWindows();
        afx_msg void OnOptionsDoNotSave();
        afx_msg void OnOptionsDeleteOnErrors();
        afx_msg void OnOptionsStopOnErrors();
        afx_msg void OnOptionsHideConsole();
        afx_msg void OnOptionsEnsureVisible();
        afx_msg void OnOptionsFindDecoder();
        afx_msg void OnOptionsValidateFiles();
        afx_msg void OnOptionsOverwriteFiles();
        afx_msg void OnOptionsDownloadTools();
        afx_msg void OnLanguageDefault();
        afx_msg void OnLanguageChange(UINT nID);
        afx_msg void OnHelpWebsite();
        afx_msg void OnHelpAbout();
    public:
        bool SearchFolderForLanguages(CString szPath);
        void InitLanguageMenu();
        void SetLanguage();
        void GetItems();
        void SetItems();
        void GetOptions();
        void SetOptions();
        bool IsItemSelected(int nItem);
        void SelectItem(int nItem);
        void DeselectItem(int nItem);
        void MakeItemVisible(int nItem);
        void ToggleItem(int nItem);
        int AddToItems(const std::wstring& szPath);
        bool AddToList(const std::wstring& szPath);
        void RedrawItem(int nItem);
        void RedrawItem(int nStart, int nEnd);
        void ShowEdtItem();
        void UpdateEdtItem(BOOL bUpdateText);
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
    public:
        bool LoadOptions(const std::wstring& szFileXml);
        bool LoadOptions(xml::XmlDocumnent &doc);
        bool SaveOptions(const std::wstring& szFileXml);
        bool LoadFormats(const std::wstring& szFileXml);
        bool LoadFormats(xml::XmlDocumnent &doc);
        bool SaveFormats(const std::wstring& szFileXml);
        bool LoadFormat(const std::wstring& szFileXml);
        bool LoadFormat(xml::XmlDocumnent &doc);
        bool SaveFormat(const std::wstring& szFileXml);
        bool LoadPresets(const std::wstring& szFileXml);
        bool LoadPresets(xml::XmlDocumnent &doc);
        bool SavePresets(const std::wstring& szFileXml);
        bool LoadTools(const std::wstring& szFileXml);
        bool LoadTools(xml::XmlDocumnent &doc);
        bool SaveTools(const std::wstring& szFileXml);
        bool LoadTool(const std::wstring& szFileXml);
        bool LoadTool(xml::XmlDocumnent &doc);
        bool LoadItems(const std::wstring& szFileXml);
        bool LoadItems(xml::XmlDocumnent &doc);
        bool SaveItems(const std::wstring& szFileXml);
        bool LoadLanguage(const std::wstring& szFileXml);
        bool LoadLanguage(xml::XmlDocumnent &doc);
    };
}
