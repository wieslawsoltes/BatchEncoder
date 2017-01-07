//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

/* $Id: BatchEncoderDlg.h 437 2008-03-24 12:15:45Z wisodev $ */

#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "ResizeDialog.h"
#include "LListFiles.h"
#include "LListPresets.h"
#include "TimeCount.h"
#include "FileListCtrl.h"
#include "DropList.h"
#include "MyStatic.h"
#include "MyButton.h"
#include "MyStatusBarCtrl.h"
#include "Histogram.h"
#include "LameHistogram.h"
#include "ConversionStatus.h"
#include "TransMove.h"
#include "NewItemData.h"

#define WM_TRAY (WM_USER + 0x10)

#define IDC_STATUSBAR 1500
#define IDC_HISTOGRAM 1600
#define IDC_CNVSTATUS 1700

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
    CTransMove m_TransMove;
protected:
    DECLARE_MESSAGE_MAP()
protected:
    virtual void OnOK();
    virtual void OnCancel();
public:
    bool CreateBatchFile(CString szFileName, bool bUseListCtrl = true);
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
    PBATCHENCODER_OPTIONS m_pFo;
public:
    CString szMainConfigFile;
public:
    CLListFiles m_FileList;
public:
    CString szFormatTemplate[NUM_FORMAT_NAMES];
    CString szFormatPath[NUM_FORMAT_NAMES];
    bool bFormatInput[NUM_FORMAT_NAMES];
    bool bFormatOutput[NUM_FORMAT_NAMES];
    CString szFormatFunction[NUM_FORMAT_NAMES];
public:
    CLListPresets m_ListPresets[NUM_PRESET_FILES];
public:
    CString szPresetsFile[NUM_PRESET_FILES];
public:
    HANDLE hThread;
    DWORD dwThreadID;
    volatile bool bRunning;
    volatile int nProgressCurrent;
    bool bSameAsSourceEdit;
public:
    int nThreadPriorityIndex;
    int nProcessPriorityIndex;
    bool bDeleteOnError;
    bool bStopOnErrors;
    CString szLogFileName;
    int nLogEncoding;
public:
    bool bForceConsoleWindow;
public:
    CString szBrowsePath[NUM_BROWSE_PATH];
public:
    int nCurSel[NUM_OUTPUT_EXT];
public:
    CEdit m_EdtOutPath;
public:
    CProgressCtrl m_FileProgress;
public:
    CDropList m_CmbPresets;
    CDropList m_CmbFormat;
public:
    CFileListCtrl m_LstInputFiles;
public:
    CMyStatic m_StcPreset;
    CMyStatic m_StcFormat;
public:
    CButton m_ChkOutPath;
public:
    CMyButton m_BtnConvert;
    CMyButton m_BtnBrowse;
public:
    CString szPresetsWndResize;
    CString szPresetsListColumns;
    CString szFormatsWndResize;
    CString szFormatsListColumns;
public:
    CString GetEncoderExe(int nOutputFormat);
    CString GetDecoderExe(int nIntputFormat);
public:
    CString GetEncoderOpt(int nOutputFormat, int nPreset);
    CString GetDecoderOpt(int nIntputFormat, int nPreset);
public:
    bool WorkerCallback(int nProgress, 
        bool bFinished, 
        bool bError = false, 
        double fTime = 0.0, 
        int nIndex = -1);
public:
    void HistogramCallback(PLAME_ENC_HISTOGRAM plehData);
    CHistogram m_Histogram;
public:
    CConversionStatus m_CnvStatus;
public:
    bool InsertToList(NewItemData &Iid);
public:
    void HandleDropFiles(HDROP hDropInfo);
public:
    void UpdateFormatAndPreset();
public:
    void ResetOutput();
    void ResetConvertionTime();
    void ResetConvertionStatus();
public:
    void SearchFolderForFiles(CString szFile, 
        const bool bRecurse,
        const TCHAR *szOutExt = NULL,
        const int nPreset = -1);
public:
    CString BrowseForSettings();
public:
    LPTSTR GetMenuItemCheck(int nID);
    void SetMenuItemCheck(int nID, LPTSTR bChecked);
public:
    bool GridlinesVisible();
    void ShowGridlines(bool bShow);
public:
    bool LoadList(CString szFileXml, bool bAddToListCtrl = true);
    bool SaveList(CString szFileXml, bool bUseListCtrl = true);
public:
    bool LoadSettings();
    bool SaveSettings();
public:
    afx_msg void LoadUserSettings();
    afx_msg void SaveUserSettings();
    afx_msg void LoadDefaultSettings();
public:
    bool LoadPresets(CString szPresetsFName, CLListPresets *m_ListPresets);
    CLListPresets *GetCurrentPresetsList(void);
public:
    void FillPresetComboBox(CLListPresets *m_ListPresets = NULL, int nSelIndex = -1);
    void UpdateOutputComboBoxes(int nSelFormatIndex = -1, int nSelPresetIndex = -1);
public:
    void EnableUserInterface(BOOL bEnable = TRUE);
public:
    bool bShowTrayIcon;
public:
    void EnableTrayIcon(bool bEnable = true, bool bModify = false);
    void ShowProgressTrayIcon(int nProgress);
public:
    bool bIsHistogramVisible;
    bool bIsCnvStatusVisible;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
public:
    afx_msg LRESULT OnTrayIconMsg(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTrayMenuExit();
public:
    afx_msg LRESULT OnListItemChaged(WPARAM wParam, LPARAM lParam);
protected:
    afx_msg LRESULT OnNotifyFormat(WPARAM wParam,LPARAM lParam);
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
    afx_msg void OnShowHistogram();
    afx_msg void OnShowCnvStatus();
public:
    afx_msg void OnViewStartWithExtendedProgress();
    afx_msg void OnViewToogleExtendedProgress();
    afx_msg void OnViewToogleHistogramWindow();
    afx_msg void OnViewShowGridLines();
public:
    afx_msg void OnActionConvert();
public:
    afx_msg void OnOptionsStayOnTop();
    afx_msg void OnOptionsShowTrayIcon();
    afx_msg void OnOptionsDeleteSourceFileWhenDone();
    afx_msg void OnOptionsShowLog();
    afx_msg void OnOptionsDeleteLog();
    afx_msg void OnOptionsLogConsoleOutput();
    afx_msg void OnOptionsDoNotSave();
    afx_msg void OnOptionsConfigurePresets();
    afx_msg void OnOptionsConfigureFormat();
    afx_msg void OnOptionsShutdownWhenFinished();
    afx_msg void OnOptionsForceConsoleWindow();
    afx_msg void OnOptionsAdvanced();
public:
    afx_msg void OnHelpHelp();
    afx_msg void OnHelpWebsite();
    afx_msg void OnHelpAbout();
public:
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg UINT OnNcHitTest(CPoint point);
};
