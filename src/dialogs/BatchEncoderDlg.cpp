// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "..\StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "..\worker\WorkThread.h"
#include "BatchEncoderDlg.h"
#include "PresetsDlg.h"
#include "AboutDlg.h"
#include "FormatsDlg.h"
#include "AdvancedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct TDRAGANDDROP
{
    CBatchEncoderDlg *pDlg;
    HDROP hDropInfo;
} DRAGANDDROP, *PDRAGANDDROP;

static volatile bool bHandleDrop = true;
static HANDLE hDDThread;
static DWORD dwDDThreadID;
static DRAGANDDROP m_DDParam;

#define IDC_FOLDERTREE          0x3741
#define IDC_TITLE               0x3742
#define IDC_STATUSTEXT          0x3743
#define IDC_CHECK_RECURSE       0x3744
#define IDC_BROWSE_NEW_FOLDER   0x3746

static CString szLastBrowse;
static CString szLastBrowseAddDir;
static WNDPROC lpOldWindowProc;
static bool bRecurseChecked = true;
static HWND hWndBtnRecurse = NULL;
static HWND hWndStaticText = NULL;

int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    TCHAR szPath[MAX_PATH + 1] = _T("");
    wsprintf(szPath, _T("%s\0"), szLastBrowse);

    if ((uMsg == BFFM_INITIALIZED))
        ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath);

    return(0);
}

LRESULT CALLBACK BrowseDlgWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_COMMAND)
    {
        if ((HIWORD(wParam) == BN_CLICKED) && ((HWND)lParam == hWndBtnRecurse))
        {
            if (::SendMessage(hWndBtnRecurse, BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED)
                bRecurseChecked = true;
            else
                bRecurseChecked = false;
        }
    }
    return ::CallWindowProc(lpOldWindowProc, hWnd, uMsg, wParam, lParam);
}

int CALLBACK BrowseCallbackAddDir(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    TCHAR szPath[MAX_PATH + 1] = _T("");
    wsprintf(szPath, _T("%s\0"), szLastBrowseAddDir);

    if (uMsg == BFFM_INITIALIZED)
    {
        HWND hWndTitle = NULL;
        HFONT hFont;
        RECT rc, rcTitle, rcTree, rcWnd;

        hWndTitle = ::GetDlgItem(hWnd, IDC_TITLE);

        ::GetWindowRect(hWndTitle, &rcTitle);
        ::GetWindowRect(::GetDlgItem(hWnd, IDC_FOLDERTREE), &rcTree);
        ::GetWindowRect(hWnd, &rcWnd);

        rc.top = 8;
        rc.left = rcTree.left - rcWnd.left;
        rc.right = rcTree.right - rcTree.left;
        rc.bottom = (rcTitle.bottom - rcTitle.top) + 8;

        hWndBtnRecurse = ::CreateWindowEx(0,
            _T("BUTTON"),
            _T("Recurse subdirectories"),
            BS_CHECKBOX | BS_AUTOCHECKBOX | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
            rc.left, rc.top,
            rc.right - rc.left, rc.bottom - rc.top,
            hWnd,
            NULL, NULL, NULL);
        if (hWndBtnRecurse != NULL)
        {
            ::ShowWindow(hWndTitle, SW_HIDE);
            ::ShowWindow(::GetDlgItem(hWnd, IDC_STATUSTEXT), SW_HIDE);

            if (bRecurseChecked == true)
                ::SendMessage(hWndBtnRecurse, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
            else
                ::SendMessage(hWndBtnRecurse, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);

            // disable warnings 4311 and 4312
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

            lpOldWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)BrowseDlgWindowProc);
            ::ShowWindow(hWndBtnRecurse, SW_SHOW);

            // enable warnings 4311 and 4312
#pragma warning(pop)

            hFont = (HFONT) ::SendMessage(hWnd, WM_GETFONT, 0, 0);
            ::SendMessage(hWndBtnRecurse, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
        }

        ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath);
    }
    return(0);
}

int CALLBACK BrowseCallbackOutPath(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    TCHAR szPath[MAX_PATH + 1] = _T("");
    wsprintf(szPath, _T("%s\0"), szLastBrowse);

    if (uMsg == BFFM_INITIALIZED)
    {
        TCHAR szText[256] = _T("");
        HWND hWndTitle = NULL;
        HFONT hFont;
        RECT rc, rcTitle, rcTree, rcWnd;

        hWndTitle = ::GetDlgItem(hWnd, IDC_TITLE);

        ::GetWindowText(hWndTitle, szText, 256);

        ::GetWindowRect(hWndTitle, &rcTitle);
        ::GetWindowRect(::GetDlgItem(hWnd, IDC_FOLDERTREE), &rcTree);
        ::GetWindowRect(hWnd, &rcWnd);

        rc.top = 8;
        rc.left = rcTree.left - rcWnd.left;
        rc.right = rcTree.right - rcTree.left;
        rc.bottom = (rcTitle.bottom - rcTitle.top) + 8;

        hWndStaticText = ::CreateWindowEx(0,
            _T("STATIC"),
            szText,
            SS_CENTERIMAGE | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
            rc.left, rc.top,
            rc.right - rc.left, rc.bottom - rc.top,
            hWnd,
            NULL, NULL, NULL);
        if (hWndStaticText != NULL)
        {
            ::ShowWindow(hWndTitle, SW_HIDE);
            ::ShowWindow(::GetDlgItem(hWnd, IDC_STATUSTEXT), SW_HIDE);

            // disable warnings 4311 and 4312
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

            lpOldWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)BrowseDlgWindowProc);
            ::ShowWindow(hWndStaticText, SW_SHOW);

            // enable warnings 4311 and 4312
#pragma warning(pop)

            hFont = (HFONT) ::SendMessage(hWnd, WM_GETFONT, 0, 0);
            ::SendMessage(hWndStaticText, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
        }

        ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath);
    }
    return(0);
}

DWORD WINAPI DragAndDropThread(LPVOID lpParam)
{
    PDRAGANDDROP m_ThreadParam = (PDRAGANDDROP)lpParam;

    m_ThreadParam->pDlg->HandleDropFiles(m_ThreadParam->hDropInfo);
    bHandleDrop = true;

    return ::CloseHandle(hDDThread);
}

IMPLEMENT_DYNAMIC(CBatchEncoderDlg, CDialog)
CBatchEncoderDlg::CBatchEncoderDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CBatchEncoderDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    szMainConfigFile = ::GetExeFilePath() + MAIN_APP_CONFIG;
    bShowTrayIcon = false;

    szPresetsWndResize = _T("");
    szPresetsListColumns = _T("");
    szFormatsWndResize = _T("");
    szFormatsListColumns = _T("");

    m_Config.m_Options.nThreadPriorityIndex = 3;
    m_Config.m_Options.nProcessPriorityIndex = 1;
    m_Config.m_Options.bDeleteOnError = true;
    m_Config.m_Options.bStopOnErrors = false;
    m_Config.m_Options.szLogFileName = MAIN_APP_LOG;
    m_Config.m_Options.nLogEncoding = 2;

    bForceConsoleWindow = false;
}

CBatchEncoderDlg::~CBatchEncoderDlg()
{

}

void CBatchEncoderDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_WORK, m_FileProgress);
    DDX_Control(pDX, IDC_COMBO_PRESETS, m_CmbPresets);
    DDX_Control(pDX, IDC_COMBO_FORMAT, m_CmbFormat);
    DDX_Control(pDX, IDC_EDIT_INPUT_ITEMS, m_LstInputItems);
    DDX_Control(pDX, IDC_CHECK_OUT_PATH, m_ChkOutPath);
    DDX_Control(pDX, IDC_EDIT_OUT_PATH, m_EdtOutPath);
    DDX_Control(pDX, IDC_STATIC_TEXT_PRESET, m_StcPreset);
    DDX_Control(pDX, IDC_STATIC_TEXT_FORMAT, m_StcFormat);
    DDX_Control(pDX, IDC_BUTTON_RUN, m_BtnConvert);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_PATH, m_BtnBrowse);
}

BEGIN_MESSAGE_MAP(CBatchEncoderDlg, CResizeDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_DROPFILES()
    ON_WM_HELPINFO()
    ON_WM_SIZE()
    ON_MESSAGE(WM_TRAY, OnTrayIconMsg)
    ON_MESSAGE(WM_ITEMCHANGED, OnListItemChaged)
    ON_COMMAND(ID_TRAYMENU_EXIT, OnTrayMenuExit)
    ON_NOTIFY(LVN_KEYDOWN, IDC_EDIT_INPUT_ITEMS, OnLvnKeydownListInputFiles)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDIT_INPUT_ITEMS, OnLvnItemchangedListInputFiles)
    ON_NOTIFY(NM_RCLICK, IDC_EDIT_INPUT_ITEMS, OnNMRclickListInputFiles)
    ON_NOTIFY(NM_DBLCLK, IDC_EDIT_INPUT_ITEMS, OnNMDblclkListInputFiles)
    ON_CBN_SELCHANGE(IDC_COMBO_PRESETS, OnCbnSelchangeComboPresets)
    ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnCbnSelchangeComboFormat)
    ON_BN_CLICKED(IDC_BUTTON_RUN, OnBnClickedButtonConvert)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_PATH, OnBnClickedButtonBrowsePath)
    ON_BN_CLICKED(IDC_CHECK_OUT_PATH, OnBnClickedCheckOutPath)
    ON_EN_CHANGE(IDC_EDIT_OUT_PATH, OnEnChangeEditOutPath)
    ON_EN_SETFOCUS(IDC_EDIT_OUT_PATH, OnEnSetFocusEditOutPath)
    ON_EN_KILLFOCUS(IDC_EDIT_OUT_PATH, OnEnKillFocusEditOutPath)
    ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
    ON_COMMAND(ID_FILE_LOADLIST, OnFileLoadList)
    ON_COMMAND(ID_FILE_SAVELIST, OnFileSaveList)
    ON_COMMAND(ID_FILE_CLEARLIST, OnFileClearList)
    ON_COMMAND(ID_FILE_CREATE_BATCH_FILE, OnFileCreateBatchFile)
    ON_COMMAND(ID_FILE_EXIT, OnFileExit)
    ON_COMMAND(ID_EDIT_ADDFILES, OnEditAddFiles)
    ON_COMMAND(ID_EDIT_REMOVECHECKED, OnEditRemoveChecked)
    ON_COMMAND(ID_EDIT_REMOVEUNCHECKED, OnEditRemoveUnchecked)
    ON_COMMAND(ID_EDIT_CHECKSELECTED, OnEditCheckSelected)
    ON_COMMAND(ID_EDIT_UNCHECKSELECTED, OnEditUncheckSelected)
    ON_COMMAND(ID_EDIT_RENAME, OnEditRename)
    ON_COMMAND(ID_EDIT_OPEN, OnEditOpen)
    ON_COMMAND(ID_EDIT_EXPLORE, OnEditExplore)
    ON_COMMAND(ID_EDIT_CROP, OnEditCrop)
    ON_COMMAND(ID_EDIT_SELECTNONE, OnEditSelectNone)
    ON_COMMAND(ID_EDIT_INVERTSELECTION, OnEditInvertSelection)
    ON_COMMAND(ID_EDIT_REMOVE, OnEditRemove)
    ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
    ON_COMMAND(ID_EDIT_RESETOUTPUT, OnEditResetOutput)
    ON_COMMAND(ID_EDIT_RESETTIME, OnEditResetTime)
    ON_COMMAND(ID_EDIT_ADDDIR, OnEditAddDir)
    ON_COMMAND(ID_VIEW_SHOWGRIDLINES, OnViewShowGridLines)
    ON_COMMAND(ID_ACTION_CONVERT, OnActionConvert)
    ON_COMMAND(ID_OPTIONS_STAYONTOP, OnOptionsStayOnTop)
    ON_COMMAND(ID_OPTIONS_SHOWTRAYICON, OnOptionsShowTrayIcon)
    ON_COMMAND(ID_OPTIONS_LOGCONSOLEOUTPUT, OnOptionsLogConsoleOutput)
    ON_COMMAND(ID_OPTIONS_SHOWLOGLIST, OnOptionsShowLog)
    ON_COMMAND(ID_OPTIONS_DELETELOG, OnOptionsDeleteLog)
    ON_COMMAND(ID_OPTIONS_DELETESOURCEFILEWHENDONE, OnOptionsDeleteSourceFileWhenDone)
    ON_COMMAND(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, OnOptionsShutdownWhenFinished)
    ON_COMMAND(ID_OPTIONS_DO_NOT_SAVE, OnOptionsDoNotSave)
    ON_COMMAND(ID_OPTIONS_FORCECONSOLEWINDOW, OnOptionsForceConsoleWindow)
    ON_COMMAND(ID_OPTIONS_ADVANCED, OnOptionsAdvanced)
    ON_COMMAND(ID_OPTIONS_CONFIGUREPRESETS, OnOptionsConfigurePresets)
    ON_COMMAND(ID_OPTIONS_CONFIGUREFORMAT, OnOptionsConfigureFormat)
    ON_COMMAND(ID_HELP_WEBSITE, OnHelpWebsite)
    ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
    ON_COMMAND(ID_ACCELERATOR_CTRL_L, OnFileLoadList)
    ON_COMMAND(ID_ACCELERATOR_CTRL_S, OnFileSaveList)
    ON_COMMAND(ID_ACCELERATOR_CTRL_E, OnFileClearList)
    ON_COMMAND(ID_ACCELERATOR_CTRL_B, OnFileCreateBatchFile)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_L, LoadUserSettings)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_S, SaveUserSettings)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_D, LoadDefaultSettings)
    ON_COMMAND(ID_ACCELERATOR_F3, OnEditResetTime)
    ON_COMMAND(ID_ACCELERATOR_F4, OnEditResetOutput)
    ON_COMMAND(ID_ACCELERATOR_ALT_F4, OnFileExit)
    ON_COMMAND(ID_ACCELERATOR_F5, OnEditAddFiles)
    ON_COMMAND(ID_ACCELERATOR_F6, OnEditAddDir)
    ON_COMMAND(ID_ACCELERATOR_F2, OnEditRename)
    ON_COMMAND(ID_ACCELERATOR_CTRL_A, OnEditSelectAll)
    ON_COMMAND(ID_ACCELERATOR_CTRL_N, OnEditSelectNone)
    ON_COMMAND(ID_ACCELERATOR_CTRL_I, OnEditInvertSelection)
    ON_COMMAND(ID_ACCELERATOR_CTRL_O, OnEditOpen)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_O, OnEditExplore)
    ON_COMMAND(ID_ACCELERATOR_SHIFT_PLUS, OnEditCheckSelected)
    ON_COMMAND(ID_ACCELERATOR_SHIFT_MINUS, OnEditUncheckSelected)
    ON_COMMAND(ID_ACCELERATOR_CTRL_PLUS, OnEditRemoveChecked)
    ON_COMMAND(ID_ACCELERATOR_CTRL_MINUS, OnEditRemoveUnchecked)
    ON_COMMAND(ID_ACCELERATOR_CTRL_G, OnViewShowGridLines)
    ON_COMMAND(ID_ACCELERATOR_F9, OnBnClickedButtonConvert)
    ON_COMMAND(ID_ACCELERATOR_F10, OnOptionsStayOnTop)
    ON_COMMAND(ID_ACCELERATOR_CTRL_X, OnOptionsDoNotSave)
    ON_COMMAND(ID_ACCELERATOR_F7, OnOptionsConfigurePresets)
    ON_COMMAND(ID_ACCELERATOR_F8, OnOptionsConfigureFormat)
    ON_COMMAND(ID_ACCELERATOR_CTRL_Q, OnOptionsShutdownWhenFinished)
    ON_COMMAND(ID_ACCELERATOR_F12, OnOptionsLogConsoleOutput)
    ON_COMMAND(ID_ACCELERATOR_CTRL_F12, OnOptionsShowLog)
    ON_COMMAND(ID_ACCELERATOR_SHIFT_F12, OnOptionsDeleteLog)
    ON_COMMAND(ID_ACCELERATOR_F11, OnOptionsShowTrayIcon)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_F, OnOptionsForceConsoleWindow)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_A, OnOptionsAdvanced)
    ON_COMMAND(ID_ACCELERATOR_CTRL_D, OnOptionsDeleteSourceFileWhenDone)
END_MESSAGE_MAP()

BOOL CBatchEncoderDlg::OnInitDialog()
{
    CResizeDialog::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // create status-bar control
    VERIFY(m_StatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP,
        CRect(0, 0, 0, 0),
        this,
        IDC_STATUSBAR));

    int nStatusBarParts[2] = { 100, -1 };
    m_StatusBar.SetParts(2, nStatusBarParts);

    // load accelerators
    m_hAccel = ::LoadAccelerators(::GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDR_ACCELERATOR_BATCHENCODER));

    if (bShowTrayIcon == true)
        this->EnableTrayIcon(true);

    // handle the OnNotifyFormat message (WM_NOTIFYFORMAT)
#ifdef _UNICODE
    m_LstInputItems.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

    // enable or disable '<< same as source file >>' option
    bSameAsSourceEdit = true;

    // main dialog title with version number
    this->SetWindowText(MAIN_APP_NAME);

    // output file format combo-box 
    for (int i = 0; i < NUM_PRESET_FILES; i++)
        m_CmbFormat.InsertString(i, g_szNames[i]);

    ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_FORMAT);

    // set flag to non running state
    bRunning = false;

    // clear progress status
    m_FileProgress.SetRange(0, 100);
    m_FileProgress.SetPos(0);

    // update list style
    DWORD dwExStyle = m_LstInputItems.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
    m_LstInputItems.SetExtendedStyle(dwExStyle);

    // insert ListCtrl columns
    m_LstInputItems.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 165);
    m_LstInputItems.InsertColumn(1, _T("Input"), LVCFMT_LEFT, 50);
    m_LstInputItems.InsertColumn(2, _T("Size (bytes)"), LVCFMT_LEFT, 80);
    m_LstInputItems.InsertColumn(3, _T("Output"), LVCFMT_LEFT, 50);
    m_LstInputItems.InsertColumn(4, _T("Preset#"), LVCFMT_LEFT, 55);
    m_LstInputItems.InsertColumn(5, _T("Time"), LVCFMT_LEFT, 90);
    m_LstInputItems.InsertColumn(6, _T("Status"), LVCFMT_LEFT, 85);

    // set ListCtrl columns order
    /*
    INT nOrder[7] = { 0, 1, 2, 3, 4, 5, 6 };

    this->m_LstInputItems.GetHeaderCtrl()->SetOrderArray(7, nOrder);
    this->m_LstInputItems.Invalidate();
    */

    // set to bold Convert/Stop button font style
    m_BtnConvert.SetBold(true);

    // hide ProgressBar when not running conversion process
    this->m_FileProgress.ShowWindow(SW_HIDE);

    // enable files/dirs drag & drop for dialog
    this->DragAcceptFiles(TRUE);

    // setup resize anchors
    AddAnchor(IDC_STATIC_GROUP_OUTPUT, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_STATIC_TEXT_FORMAT, TOP_LEFT);
    AddAnchor(IDC_COMBO_FORMAT, TOP_LEFT);
    AddAnchor(IDC_STATIC_TEXT_PRESET, TOP_LEFT);
    AddAnchor(IDC_COMBO_PRESETS, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_EDIT_INPUT_ITEMS, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_CHECK_OUT_PATH, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_OUT_PATH, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_BROWSE_PATH, BOTTOM_RIGHT);
    AddAnchor(IDC_PROGRESS_WORK, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_RUN, BOTTOM_RIGHT);
    AddAnchor(IDC_STATUSBAR, BOTTOM_LEFT, BOTTOM_RIGHT);

    this->LoadConfigFile();
    this->UpdateStatusBar();

    // ResetMinTrackSize();
    // ResetMaxTrackSize();
    // ResetMaximizedRect();

    return TRUE;
}

BOOL CBatchEncoderDlg::PreTranslateMessage(MSG* pMsg)
{
    // translate here all accelerators, because by default they are not translated
    if (m_hAccel != NULL)
    {
        if (::TranslateAccelerator(this->GetSafeHwnd(), m_hAccel, pMsg))
            return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

bool CBatchEncoderDlg::CreateBatchFile(CString szFileName)
{
    // TODO: handle command-line options (params)

    // NOTE: this function use same logic as WorkThread(...)

    char *szPrefix = "@echo off\r\n@setlocal\r\n";
    char *szPostfix = "@endlocal\r\n@pause\r\n";
    char *szPreDel = "@del /F /Q \"";
    char *szPostDel = "\"\r\n";
    char *szShutdown = "@shutdown -t 30 -s\r\n";
    char *szLineEnd = "\r\n";

    ::UpdatePath();

    CFile fp;
    if (fp.Open(szFileName, fp.modeCreate | fp.modeReadWrite) == FALSE)
        return false;

    fp.Write(szPrefix, (UINT)strlen(szPrefix));

    bool bDeleteAfterconversion = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_BYCOMMAND) == MF_CHECKED;
    
    bool bOutPath = this->m_ChkOutPath.GetCheck() == BST_CHECKED;
    CString szOutPath;

    this->m_EdtOutPath.GetWindowText(szOutPath);

    int nItems = this->m_LstInputItems.GetItemCount();
    for (int i = 0; i < nItems; i++)
    {
        if (this->m_LstInputItems.GetCheck(i) == FALSE)
            continue;

        int nProcessingMode = -1;
        int nIntputFormat = m_Config.m_Items.GetItemInFormat(i);
        int nOutputFormat = m_Config.m_Items.GetItemOutFormat(i);
        int nPreset = m_Config.m_Items.GetItemOutPreset(i);

        CString szInputFile = m_Config.m_Items.GetItemFilePath(i);

        if (bOutPath == false)
        {
            szOutPath = szInputFile;
            CString szToRemove = m_Config.m_Items.GetFileName(szInputFile);
            int nNewLenght = szOutPath.GetLength() - szToRemove.GetLength();
            szOutPath.Truncate(nNewLenght);
        }

        CString szDecoderExePath;
        CString szDecoderOptions;

        szDecoderExePath = this->GetDecoderExe(nIntputFormat);
        GetFullPathName(szDecoderExePath);

        szDecoderOptions = this->GetDecoderOpt(nIntputFormat, -1);

        CString szName = m_Config.m_Items.GetItemFileName(i);

        CString szEncoderExePath;
        CString szEncoderOptions;

        szEncoderExePath = this->GetEncoderExe(nOutputFormat);
        GetFullPathName(szEncoderExePath);

        szEncoderOptions = this->GetEncoderOpt(nOutputFormat, nPreset);

        szName = szName + _T(".") + m_Config.m_Items.GetItemOutExt(i).MakeLower();

        CString szOutputFile;

        if (szOutPath.GetLength() >= 1)
        {
            if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
                szOutputFile = szOutPath + szName;
            else
                szOutputFile = szOutPath + _T("\\") + szName;
        }
        else
        {
            szOutputFile = szName;
        }

        nProcessingMode = 1;
        if (nIntputFormat == 0)
            nProcessingMode = 0;

        if (nProcessingMode == 1)
            nProcessingMode = 2;

        if (nOutputFormat == 0)
        {
            bool bNeedResampling = (szEncoderOptions.GetLength() > 0) ? true : false;

            if ((nIntputFormat == 0) && (bNeedResampling == false))
                nProcessingMode = 0;

            if ((nIntputFormat == 0) && (bNeedResampling == true))
                nProcessingMode = 0;

            if ((nIntputFormat > 0) && (bNeedResampling == false))
                nProcessingMode = 1;

            if ((nIntputFormat > 0) && (bNeedResampling == true))
                nProcessingMode = 2;
        }

        CString csExecute;
        bool bDecode = false;
        int nTool = -1;

        CString szOrgInputFile = szInputFile;
        CString szOrgOutputFile = szOutputFile;

        if (nProcessingMode == 2)
            szOutputFile = szOutputFile + _T(".wav");

        if ((nProcessingMode == 1) || (nProcessingMode == 2))
        {
            csExecute = m_Config.m_Formats[(NUM_OUTPUT_EXT + nIntputFormat - 1)].szTemplate;
            csExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
            csExecute.Replace(_T("$EXE"), szDecoderExePath);
            csExecute.Replace(_T("$OPTIONS"), szDecoderOptions);
            csExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
            csExecute.Replace(_T("$INFILE"), szInputFile);
            csExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
            csExecute.Replace(_T("$OUTFILE"), szOutputFile);

            bDecode = true;
            nTool = (NUM_OUTPUT_EXT + nIntputFormat - 1);

            CUtf8String szBuffUtf8;
            char *szCommandLine = szBuffUtf8.Create(csExecute);
            fp.Write(szCommandLine, (UINT)strlen(szCommandLine));
            szBuffUtf8.Clear();

            fp.Write(szLineEnd, (UINT)strlen(szLineEnd));

            if (bDeleteAfterconversion == true)
            {
                fp.Write(szPreDel, (UINT)strlen(szPreDel));

                CUtf8String szBuffUtf8;
                char *szDelFile = szBuffUtf8.Create(szOrgInputFile);
                fp.Write(szDelFile, (UINT)strlen(szDelFile));
                szBuffUtf8.Clear();

                fp.Write(szPostDel, (UINT)strlen(szPostDel));
            }
        }

        if (nProcessingMode == 2)
        {
            szInputFile = szOutputFile;
            szOrgInputFile = szOutputFile;
            szOutputFile = szOrgOutputFile;
        }

        if ((nProcessingMode == 0) || (nProcessingMode == 2))
        {            
            csExecute = m_Config.m_Formats[nOutputFormat].szTemplate;
            csExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
            csExecute.Replace(_T("$EXE"), szEncoderExePath);
            csExecute.Replace(_T("$OPTIONS"), szEncoderOptions);
            csExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
            csExecute.Replace(_T("$INFILE"), szInputFile);
            csExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
            csExecute.Replace(_T("$OUTFILE"), szOutputFile);

            bDecode = false;
            nTool = nOutputFormat;

            CUtf8String szBuffUtf8;
            char *szCommandLine = szBuffUtf8.Create(csExecute);
            fp.Write(szCommandLine, (UINT)strlen(szCommandLine));
            szBuffUtf8.Clear();

            fp.Write(szLineEnd, (UINT)strlen(szLineEnd));

            if ((bDeleteAfterconversion == true) || (nProcessingMode == 2))
            {
                fp.Write(szPreDel, (UINT)strlen(szPreDel));

                CUtf8String szBuffUtf8;
                char *szDelFile = szBuffUtf8.Create(szOrgInputFile);
                fp.Write(szDelFile, (UINT)strlen(szDelFile));
                szBuffUtf8.Clear();

                fp.Write(szPostDel, (UINT)strlen(szPostDel));
            }
        }
    }

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_BYCOMMAND) == MF_CHECKED)
        fp.Write(szShutdown, (UINT)strlen(szShutdown));

    fp.Write(szPostfix, (UINT)strlen(szPostfix));
    fp.Close();

    return true;
}

void CBatchEncoderDlg::UpdateStatusBar()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        CString szText;
        szText.Format(_T("%d %s"),
            nCount,
            (nCount > 1) ? _T("Items") : _T("Item"));

        VERIFY(m_StatusBar.SetText(szText, 0, 0));
    }
    else
    {
        VERIFY(m_StatusBar.SetText(_T("No Files"), 0, 0));
        VERIFY(m_StatusBar.SetText(_T(""), 1, 0));
    }
}

void CBatchEncoderDlg::EnableTrayIcon(bool bEnable, bool bModify)
{
    NOTIFYICONDATA tnd;
    HICON hIconExit;
    hIconExit = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME));

    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = this->GetSafeHwnd();
    tnd.uID = 0x1000;
    tnd.hIcon = hIconExit;
    tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    tnd.uCallbackMessage = WM_TRAY;

    lstrcpy(tnd.szTip, _T("BatchEncoder"));

    if ((bEnable == true) && (bModify == false))
    {
        Shell_NotifyIcon(NIM_ADD, &tnd);
        this->bShowTrayIcon = true;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHOWTRAYICON, MF_CHECKED);
    }
    else if ((bEnable == true) && (bModify == true))
    {
        Shell_NotifyIcon(NIM_MODIFY, &tnd);
    }
    else
    {
        // delete tray icon only if exist
        if (this->bShowTrayIcon == true)
        {
            Shell_NotifyIcon(NIM_DELETE, &tnd);
            this->bShowTrayIcon = false;
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHOWTRAYICON, MF_UNCHECKED);
        }
    }
}
void CBatchEncoderDlg::ShowProgressTrayIcon(int nProgress)
{
    if (this->bShowTrayIcon == false)
        return;

    static const int nIconsIdCount = 12;
    static const int nIconId[nIconsIdCount] =
    {
        IDI_ICON_PROGRESS_01,
        IDI_ICON_PROGRESS_02,
        IDI_ICON_PROGRESS_03,
        IDI_ICON_PROGRESS_04,
        IDI_ICON_PROGRESS_05,
        IDI_ICON_PROGRESS_06,
        IDI_ICON_PROGRESS_07,
        IDI_ICON_PROGRESS_08,
        IDI_ICON_PROGRESS_09,
        IDI_ICON_PROGRESS_10,
        IDI_ICON_PROGRESS_11,
        IDI_ICON_PROGRESS_12 
    };

    int nIndex = (nIconsIdCount * nProgress) / 100;
    HICON hIconProgress = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(nIconId[nIndex]));

    NOTIFYICONDATA tnd;
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = this->GetSafeHwnd();
    tnd.uID = 0x1000;
    tnd.hIcon = hIconProgress;
    tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    tnd.uCallbackMessage = WM_TRAY;

    TCHAR szText[64];

    _stprintf(szText, _T("%d%%"), nProgress);
    _tcscpy(tnd.szTip, szText);

    Shell_NotifyIcon(NIM_MODIFY, &tnd);
}

void CBatchEncoderDlg::OnSize(UINT nType, int cx, int cy)
{
    CResizeDialog::OnSize(nType, cx, cy);

    if ((bShowTrayIcon == true) && (nType == SIZE_MINIMIZED))
    {
        ShowWindow(SW_HIDE);
        InvalidateRect(NULL, FALSE);
    }
}

LRESULT CBatchEncoderDlg::OnTrayIconMsg(WPARAM wParam, LPARAM lParam)
{
    UINT uID = (UINT)wParam;
    UINT uMouseMsg = (UINT)lParam;

    if (bShowTrayIcon == false)
        return(0);

    if (uMouseMsg == WM_RBUTTONDOWN)
    {
        if (bRunning == true)
            return(0);

        CMenu menu;
        if (!menu.LoadMenu(IDR_MENU_TRAY))
            return(0);

        CMenu* pSubMenu = menu.GetSubMenu(0);
        if (!pSubMenu)
            return(0);

        // set first menu item font to bold
        ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

        CPoint mouse;
        GetCursorPos(&mouse);
        ::SetForegroundWindow(this->GetSafeHwnd());
        ::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0, this->GetSafeHwnd(), NULL);
        ::PostMessage(this->GetSafeHwnd(), WM_NULL, 0, 0);
    }
    else if (uMouseMsg == WM_LBUTTONDOWN)
    {
        if (this->IsWindowVisible() == FALSE)
        {
            ShowWindow(SW_SHOW);
            ShowWindow(SW_RESTORE);
            SetFocus();
            SetActiveWindow();
        }
        else
        {
            ShowWindow(SW_MINIMIZE);
        }
    }
    else if (uMouseMsg == WM_LBUTTONDBLCLK)
    {
        if (bRunning == true)
            return(0);

        // ...
    }

    return(0);
}

LRESULT CBatchEncoderDlg::OnListItemChaged(WPARAM wParam, LPARAM lParam)
{
    INT nIndex = (INT)wParam;
    LPTSTR szText = (LPTSTR)lParam;

    // update item data
    if ((nIndex >= 0) && szText != NULL)
        m_Config.m_Items.SetItemFileName(szText, nIndex);

    return(0);
}

void CBatchEncoderDlg::OnTrayMenuExit()
{
    if (bShowTrayIcon == true)
        this->OnClose();
}

LRESULT CBatchEncoderDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
{
    // required for ClistView control to receive notifications messages
#ifdef _UNICODE
    return NFR_UNICODE;
#else
    return NFR_ANSI;
#endif
}

void CBatchEncoderDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CResizeDialog::OnPaint();
    }
}

HCURSOR CBatchEncoderDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

CString CBatchEncoderDlg::GetDecoderExe(int nIntputFormat)
{
    return m_Config.m_Formats[(NUM_OUTPUT_EXT + nIntputFormat - 1)].szPath;
}

CString CBatchEncoderDlg::GetDecoderOpt(int nIntputFormat, int nPreset)
{
    CString szRet = _T("");

    // NOTE: presets for decoder are not supported

    return szRet;
}

CString CBatchEncoderDlg::GetEncoderExe(int nOutputFormat)
{
    return m_Config.m_Formats[nOutputFormat].szPath;
}

CString CBatchEncoderDlg::GetEncoderOpt(int nOutputFormat, int nPreset)
{
    return m_Config.m_Presets[nOutputFormat].GetPresetOptions(nPreset);
}

bool CBatchEncoderDlg::LoadPresets(CString szPresetsFName, CPresetsList *m_ListPresets)
{
    CXMLDocumentW doc;
    if (doc.LoadFileW(szPresetsFName) == true)
    {
        // root: Presets
        tinyxml2::XMLElement *pRootElem = doc.FirstChildElement();
        if (!pRootElem)
            return false;

        // check for "Presets"
        const char *szRoot = pRootElem->Value();
        const char *szRootName = "Presets";
        if (strcmp(szRootName, szRoot) != 0)
            return false;

        // remove all presets from list
        m_ListPresets->RemoveAllNodes();

        // fill list with new presets
        int nIndex = 0;
        tinyxml2::XMLElement *pFilesNode = pRootElem->FirstChildElement("Preset");
        for (pFilesNode; pFilesNode; pFilesNode = pFilesNode->NextSiblingElement())
        {
            const char *pszName = pFilesNode->Attribute("name");
            const char *pszOptions = pFilesNode->Attribute("options");

            CString szNameData = GetConfigString(pszName);
            CString szOptionsData = GetConfigString(pszOptions);

            if ((pszName != NULL) && (pszOptions != NULL))
            {
                m_ListPresets->InsertNode(szNameData);
                m_ListPresets->SetPresetOptions(szOptionsData, nIndex);
                nIndex++;
            }
        }

        return true;
    }

    return false;
}

void CBatchEncoderDlg::FillPresetComboBox(CPresetsList *m_ListPresets, int nSelIndex)
{
    if (m_ListPresets != NULL)
    {
        int nSize = m_ListPresets->GetSize();

        // check if we have some presets
        if (nSize == 0)
        {
            // reset presets ComboBox
            this->m_CmbPresets.ResetContent();

            // insert Error Message
            this->m_CmbPresets.AddString(_T("Not Available"));

            return;
        }

        // reset ComboBox content
        this->m_CmbPresets.ResetContent();

        // insert all preset names to combo-box
        for (int i = 0; i < nSize; i++)
        {
            CString szName = m_ListPresets->GetPresetName(i);
            this->m_CmbPresets.InsertString(i, szName);
        }

        // resize ComboBox only once
        static bool bInitPresetsCombo = false;
        if (bInitPresetsCombo == false)
        {
            ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRESETS);
            bInitPresetsCombo = true;
        }

        // select default item in presets ComboBox
        if ((nSelIndex < 0) || (nSelIndex > nSize - 1))
            this->m_CmbPresets.SetCurSel(0); // ERROR, select first item
        else
            this->m_CmbPresets.SetCurSel(nSelIndex);
    }
}

CPresetsList *CBatchEncoderDlg::GetCurrentPresetsList(void)
{
    int nSelFormatIndex = this->m_CmbFormat.GetCurSel();

    if ((nSelFormatIndex >= 0) && (nSelFormatIndex < NUM_PRESET_FILES))
        return &m_Config.m_Presets[nSelFormatIndex];
    else
        return NULL; // ERROR
}

void CBatchEncoderDlg::UpdateOutputComboBoxes(int nSelFormatIndex, int nSelPresetIndex)
{
    // check the format index
    if (this->m_CmbFormat.GetCount() < nSelFormatIndex || nSelFormatIndex < 0)
    {
        this->m_CmbFormat.SetCurSel(0);
        this->m_CmbPresets.SetCurSel(0);

        return; // ERROR
    }

    if ((nSelFormatIndex >= 0) && (nSelFormatIndex < NUM_PRESET_FILES))
    {
        this->FillPresetComboBox(&m_Config.m_Presets[nSelFormatIndex], nSelPresetIndex);
    }
    else
    {
        // ERROR
        this->m_CmbPresets.ResetContent();
        this->m_CmbPresets.InsertString(0, _T("Default"));
        this->m_CmbPresets.SetCurSel(0);
    }

    this->m_CmbFormat.SetCurSel(nSelFormatIndex);
}

LPTSTR CBatchEncoderDlg::GetMenuItemCheck(int nID)
{
    return (this->GetMenu()->GetMenuState(nID, MF_BYCOMMAND) == MF_CHECKED) ? _T("true") : _T("false");
}

void CBatchEncoderDlg::SetMenuItemCheck(int nID, LPTSTR bChecked)
{
    // NOTE: bChecked IS _T("true") OR _T("false")
    this->GetMenu()->CheckMenuItem(nID, (lstrcmp(_T("true"), bChecked) == 0) ? MF_CHECKED : MF_UNCHECKED);
}

bool CBatchEncoderDlg::GridlinesVisible()
{
    DWORD dwExStyle = m_LstInputItems.GetExtendedStyle();
    if (dwExStyle & LVS_EX_GRIDLINES)
        return true;
    else
        return false;
}

void CBatchEncoderDlg::ShowGridlines(bool bShow)
{
    DWORD dwExStyle = m_LstInputItems.GetExtendedStyle();
    if (bShow == true)
    {
        dwExStyle |= LVS_EX_GRIDLINES;
        m_LstInputItems.SetExtendedStyle(dwExStyle);
        this->GetMenu()->CheckMenuItem(ID_VIEW_SHOWGRIDLINES, MF_CHECKED);
    }
    else
    {
        // check if we have grid-lines on
        if (dwExStyle & LVS_EX_GRIDLINES)
        {
            dwExStyle = dwExStyle ^ LVS_EX_GRIDLINES;
            m_LstInputItems.SetExtendedStyle(dwExStyle);
            this->GetMenu()->CheckMenuItem(ID_VIEW_SHOWGRIDLINES, MF_UNCHECKED);
        }
    }
}

void CBatchEncoderDlg::InitNewItemData(NewItemData &nid)
{
    nid.nAction = ADD_ITEM_NONE;
    nid.nItem = -1;
    nid.szPath = _T("");
    nid.szName = _T("");
    nid.szOutExt = _T("");
    nid.nOutPreset = -1;
    nid.bChecked = TRUE;
    nid.szTime = _T("");
    nid.szStatus = _T("");
}

void CBatchEncoderDlg::LoadItems(tinyxml2::XMLElement *pItemsElem)
{
    tinyxml2::XMLElement *pItemElem = pItemsElem->FirstChildElement("Item");
    for (pItemElem; pItemElem; pItemElem = pItemElem->NextSiblingElement())
    {
        char *pszAttribPath = (char*)pItemElem->Attribute("path");
        char *pszAttribChecked = (char*)pItemElem->Attribute("checked");
        char *pszAttribName = (char*)pItemElem->Attribute("name");
        char *pszAttribInExt = (char*)pItemElem->Attribute("input");
        char *pszAttribSize = (char*)pItemElem->Attribute("size");
        char *pszAttribOutExt = (char*)pItemElem->Attribute("output");
        char *pszAttribOutPreset = (char*)pItemElem->Attribute("preset");
        char *pszAttribTime = (char*)pItemElem->Attribute("time");
        char *pszAttribStatus = (char*)pItemElem->Attribute("status");

        if (pszAttribPath == NULL
            || pszAttribChecked == NULL
            || pszAttribName == NULL
            || pszAttribInExt == NULL
            || pszAttribSize == NULL
            || pszAttribOutExt == NULL
            || pszAttribOutPreset == NULL
            || pszAttribTime == NULL
            || pszAttribStatus == NULL)
        {
            continue;
        }

        NewItemData nid;
        this->InitNewItemData(nid);

        nid.nAction = ADD_ITEM_MEMORY_AND_CONTROL;
        nid.szPath = GetConfigString(pszAttribPath);
        nid.nItem = -1;
        nid.szName = GetConfigString(pszAttribName);
        nid.szOutExt = GetConfigString(pszAttribOutExt);
        nid.nOutPreset = stoi(GetConfigString(pszAttribOutPreset));
        nid.bChecked = (GetConfigString(pszAttribChecked).Compare(_T("true")) == 0) ? TRUE : FALSE;
        nid.szTime = GetConfigString(pszAttribTime);
        nid.szStatus = GetConfigString(pszAttribStatus);

        this->InsertToList(nid);
    }
}

void CBatchEncoderDlg::SaveItems(CXMLDocumentW &doc, tinyxml2::XMLElement *pItemsElem)
{
    int nItems = this->m_LstInputItems.GetItemCount();
    for (int i = 0; i < nItems; i++)
    {
        tinyxml2::XMLElement *pItemElem = doc.NewElement("Item");
        pItemsElem->LinkEndChild(pItemElem);

        CUtf8String szBuffUtf8;

        char *pszAttribPath = szBuffUtf8.Create(m_Config.m_Items.GetItemFilePath(i));
        pItemElem->SetAttribute("path", pszAttribPath);
        szBuffUtf8.Clear();

        char *pszAttribChecked = szBuffUtf8.Create((this->m_LstInputItems.GetCheck(i) == TRUE) ? _T("true") : _T("false"));
        pItemElem->SetAttribute("checked", pszAttribChecked);
        szBuffUtf8.Clear();

        char *pszAttribName = szBuffUtf8.Create(m_Config.m_Items.GetItemFileName(i));
        pItemElem->SetAttribute("name", pszAttribName);
        szBuffUtf8.Clear();

        char *pszAttribInExt = szBuffUtf8.Create(m_Config.m_Items.GetItemInExt(i));
        pItemElem->SetAttribute("input", pszAttribInExt);
        szBuffUtf8.Clear();

        CString szSize;
        szSize.Format(_T("%I64d"), m_Config.m_Items.GetItemFileSize(i));
        char *pszAttribSize = szBuffUtf8.Create(szSize);
        pItemElem->SetAttribute("size", pszAttribSize);
        szBuffUtf8.Clear();

        char *pszAttribOutExt = szBuffUtf8.Create(m_Config.m_Items.GetItemOutExt(i));
        pItemElem->SetAttribute("output", pszAttribOutExt);
        szBuffUtf8.Clear();

        CString szOutPreset;
        szOutPreset.Format(_T("%d"), m_Config.m_Items.GetItemOutPreset(i));
        char *pszAttribOutPreset = szBuffUtf8.Create(szOutPreset);;
        pItemElem->SetAttribute("preset", pszAttribOutPreset);
        szBuffUtf8.Clear();

        char *pszAttribTime = szBuffUtf8.Create(this->m_LstInputItems.GetItemText(i, 5));
        pItemElem->SetAttribute("time", pszAttribTime);
        szBuffUtf8.Clear();

        char *pszAttribStatus = szBuffUtf8.Create(this->m_LstInputItems.GetItemText(i, 6));
        pItemElem->SetAttribute("status", pszAttribStatus);
        szBuffUtf8.Clear();
    }
}

bool CBatchEncoderDlg::LoadConfigFile()
{
    ::UpdatePath();

    CXMLDocumentW doc;
    if (doc.LoadFileW(szMainConfigFile) == false)
        return false;

    tinyxml2::XMLElement *pRootElem = doc.FirstChildElement();
    if (!pRootElem)
        return false;

    // root: "Configuration"
    const char *szRoot = pRootElem->Value();
    const char *szRootName = "Configuration";
    if (strcmp(szRootName, szRoot) != 0)
        return false;

    // root: Settings
    tinyxml2::XMLElement *pSettingsElem = pRootElem->FirstChildElement("Settings");
    CString szSetting[NUM_PROGRAM_SETTINGS];
    for (int i = 0; i < NUM_PROGRAM_SETTINGS; i++)
    {
        tinyxml2::XMLElement *pSettingElem = pSettingsElem->FirstChildElement(g_szSettingsTags[i]);
        if (pSettingElem)
        {
            const char *tmpBuff = pSettingElem->GetText();
            szSetting[i] = GetConfigString(tmpBuff);
        }
        else
        {
            szSetting[i] = _T("");
        }
    }

    // root: Presets
    tinyxml2::XMLElement *pPresetsElem = pRootElem->FirstChildElement("Presets");
    for (int i = 0; i < NUM_PRESET_FILES; i++)
    {
        tinyxml2::XMLElement *pPresetElem = pPresetsElem->FirstChildElement(g_szPresetTags[i]);
        if (pPresetElem)
        {
            const char *tmpBuff = pPresetElem->GetText();
            m_Config.szPresetsFile[i] = GetConfigString(tmpBuff);
        }
        else
        {
            m_Config.szPresetsFile[i] = g_szPresetFiles[i];
        }
    }

    // root: Formats
    // NOTE: 
    // same code as in CFormatsDlg::OnBnClickedButtonLoadConfig()
    // only FirstChild("Formats").FirstChild("Format") is different

    tinyxml2::XMLElement *pFormatsElem = pRootElem->FirstChildElement("Formats");
    tinyxml2::XMLElement *pFormatElem = pFormatsElem->FirstChildElement("Format");
    for (pFormatElem; pFormatElem; pFormatElem = pFormatElem->NextSiblingElement())
    {
        int nFormat = -1;

        const char *pszName = pFormatElem->Attribute("name");
        if (pszName != NULL)
        {
            CString szBuff = GetConfigString(pszName);

            nFormat = ::GetFormatId(szBuff);

            // check if this is valid format name
            if ((nFormat < 0) || (nFormat >= NUM_FORMAT_NAMES))
            {
                // invalid format Id
                continue;
            }

            m_Config.m_Formats[nFormat].szName = szBuff;
        }
        else
        {
            // unknown or invalid format
            continue;
        }

        const char *pszTemplate = pFormatElem->Attribute("template");
        if (pszTemplate != NULL)
        {
            m_Config.m_Formats[nFormat].szTemplate = GetConfigString(pszTemplate);
        }

        const char *pszPipesInput = pFormatElem->Attribute("input");
        if (pszPipesInput != NULL)
        {
            CString szBuff = GetConfigString(pszPipesInput);
            if (szBuff.CompareNoCase(_T("true")) == 0)
                m_Config.m_Formats[nFormat].bInput = true;
            else
                m_Config.m_Formats[nFormat].bInput = false;
        }

        const char *pszPipesOutput = pFormatElem->Attribute("output");
        if (pszPipesOutput != NULL)
        {
            CString szBuff = GetConfigString(pszPipesOutput);
            if (szBuff.CompareNoCase(_T("true")) == 0)
                m_Config.m_Formats[nFormat].bOutput = true;
            else
                m_Config.m_Formats[nFormat].bOutput = false;
        }

        const char *pszFunction = pFormatElem->Attribute("function");
        if (pszFunction != NULL)
        {
            m_Config.m_Formats[nFormat].szFunction = GetConfigString(pszFunction);
        }

        const char *tmpBuff = pFormatElem->GetText();
        m_Config.m_Formats[nFormat].szPath = GetConfigString(tmpBuff);
    }

    /*
    // TODO: Add to CSettings
    // this is special case for m_Config.m_Browse.szBrowsePath[4]
    // check for out-path if not presets set to default value
    if (m_Config.m_Browse.szBrowsePath[4].Compare(_T("")) != 0)
    {
        this->m_EdtOutPath.SetWindowText(m_Config.m_Browse.szBrowsePath[4]);
        szLastBrowse = m_Config.m_Browse.szBrowsePath[4];
    }
    else
    {
        CString szBuff = ::GetExeFilePath();
        m_EdtOutPath.SetWindowText(szBuff);
        szLastBrowse = szBuff;
        m_Config.m_Browse.szBrowsePath[4] = szBuff;
    }
    */
    
    // root: Items
    this->OnFileClearList();

    tinyxml2::XMLElement *pItemsElem = pRootElem->FirstChildElement("Items");
    this->LoadItems(pItemsElem);

    // handle loaded settings
    int nSelFormatIndex = 0;

    // 0
    // Description: get for each output format selected preset index
    if (szSetting[0].Compare(_T("")) != 0)
    {
        CString resToken;
        int curPos = 0;
        int nIndex = 0;

        resToken = szSetting[0].Tokenize(_T(" "), curPos);
        while (resToken != _T("") && nIndex <= (NUM_OUTPUT_EXT - 1))
        {
            nCurSel[nIndex] = stoi(resToken);
            resToken = szSetting[0].Tokenize(_T(" "), curPos);
            nIndex++;
        }
    }
    else
    {
        ZeroMemory(&nCurSel, sizeof(int) * NUM_OUTPUT_EXT);
    }

    // 1
    // Description: get selected format in formats combo-box
    if (szSetting[1].Compare(_T("")) != 0)
    {
        nSelFormatIndex = stoi(szSetting[1]);
        if (nSelFormatIndex < 0)
            nSelFormatIndex = 0;
    }
    else
    {
        // by default select 2nd format
        nSelFormatIndex = 0;
    }

    // 2
    // Description: output path check-box state
    if (szSetting[2].Compare(_T("")) != 0)
    {
        if (szSetting[2].Compare(_T("true")) == 0)
        {
            m_ChkOutPath.SetCheck(BST_CHECKED);
        }
        else
        {
            m_BtnBrowse.EnableWindow(FALSE);
            m_EdtOutPath.EnableWindow(FALSE);
        }
    }
    else
    {
        m_BtnBrowse.EnableWindow(FALSE);
        m_EdtOutPath.EnableWindow(FALSE);
    }

    // 3
    // Description: debug check-box state
    if (szSetting[3].Compare(_T("")) != 0)
    {
        if (szSetting[3].Compare(_T("true")) == 0)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_CHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_UNCHECKED);
    }
    else
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_UNCHECKED);
    }

    // 4
    // Description: delete source file after successful conversion
    if (szSetting[4].Compare(_T("")) != 0)
    {
        if (szSetting[4].Compare(_T("true")) == 0)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_CHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_UNCHECKED);
    }
    else
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_UNCHECKED);
    }

    // 5
    // Description: make main dialog window on top of all other desktop windows
    if (szSetting[5].Compare(_T("")) != 0)
    {
        if (szSetting[5].Compare(_T("true")) == 0)
        {
            this->SetWindowPos(CWnd::FromHandle(HWND_TOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_STAYONTOP, MF_CHECKED);
        }
        else
        {
            this->SetWindowPos(CWnd::FromHandle(HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_STAYONTOP, MF_UNCHECKED);
        }
    }

    // 6
    // Description: enables recursed flag in browse dialog
    if (szSetting[6].Compare(_T("")) != 0)
    {
        if (szSetting[6].Compare(_T("true")) == 0)
            ::bRecurseChecked = true;
        else
            ::bRecurseChecked = false;
    }
    else
    {
        ::bRecurseChecked = true;
    }

    // 7
    // Description: set main window rectangle and position
    if (szSetting[7].Compare(_T("")) != 0)
    {
        this->SetWindowRectStr(szSetting[7]);
    }

    // 8
    // Description: width of each column in ItemList
    if (szSetting[8].Compare(_T("")) != 0)
    {
        int nColWidth[7];
        if (_stscanf(szSetting[8], _T("%d %d %d %d %d %d %d"),
            &nColWidth[0],
            &nColWidth[1],
            &nColWidth[2],
            &nColWidth[3],
            &nColWidth[4],
            &nColWidth[5],
            &nColWidth[6]) == 7)
        {
            for (int i = 0; i < 7; i++)
                m_LstInputItems.SetColumnWidth(i, nColWidth[i]);
        }
    }

    // 9
    // Description: show grid-lines in ListCtrl
    if (szSetting[9].Compare(_T("")) != 0)
    {
        if (szSetting[9].Compare(_T("true")) == 0)
            ShowGridlines(true);
        else
            ShowGridlines(false);
    }
    else
    {
        ShowGridlines(true);
    }

    // 10
    // Description: show program icon in system tray
    if (szSetting[10].Compare(_T("")) != 0)
    {
        if (szSetting[10].Compare(_T("true")) == 0)
        {
            this->EnableTrayIcon(true);
            bShowTrayIcon = true;
        }
        else
        {
            this->EnableTrayIcon(false);
            bShowTrayIcon = false;
        }
    }
    else
    {
        this->EnableTrayIcon(false);
        bShowTrayIcon = false;
    }

    // 11
    // Description: do not save setting on exit
    if (szSetting[11].Compare(_T("")) != 0)
    {
        if (szSetting[11].Compare(_T("true")) == 0)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_CHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);
    }
    else
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);
    }

    // 12
    // Description: presets window rectangle and position
    if (szSetting[12].Compare(_T("")) != 0)
    {
        this->szPresetsWndResize = szSetting[12];
    }

    // 13
    // Description: width of each column in PresetsList
    if (szSetting[13].Compare(_T("")) != 0)
    {
        this->szPresetsListColumns = szSetting[13];
    }

    // 14
    // Description: formats window rectangle and position
    if (szSetting[14].Compare(_T("")) != 0)
    {
        this->szFormatsWndResize = szSetting[14];
    }

    // 15
    // Description: width of each column in FormatsList
    if (szSetting[15].Compare(_T("")) != 0)
    {
        this->szFormatsListColumns = szSetting[15];
    }

    // 16
    // Description: encoder/decoder thread priority index
    if (szSetting[16].Compare(_T("")) != 0)
    {
        m_Config.m_Options.nThreadPriorityIndex = stoi(szSetting[16]);
    }
    else
    {
        m_Config.m_Options.nThreadPriorityIndex = 3;
    }

    // 17
    // Description: encoder/decoder process priority index
    if (szSetting[17].Compare(_T("")) != 0)
    {
        m_Config.m_Options.nProcessPriorityIndex = stoi(szSetting[17]);
    }
    else
    {
        m_Config.m_Options.nProcessPriorityIndex = 1;
    }

    // 18
    // Description: delete output file on error
    if (szSetting[18].Compare(_T("")) != 0)
    {
        if (szSetting[18].Compare(_T("true")) == 0)
            m_Config.m_Options.bDeleteOnError = true;
        else
            m_Config.m_Options.bDeleteOnError = false;
    }
    else
    {
        m_Config.m_Options.bDeleteOnError = true;
    }

    // 19
    // Description: stop conversion process on error
    if (szSetting[19].Compare(_T("")) != 0)
    {
        if (szSetting[19].Compare(_T("true")) == 0)
            m_Config.m_Options.bStopOnErrors = true;
        else
            m_Config.m_Options.bStopOnErrors = false;
    }
    else
    {
        m_Config.m_Options.bStopOnErrors = false;
    }

    // 20
    // Description: log filename for console output
    if (szSetting[20].Compare(_T("")) != 0)
    {
        m_Config.m_Options.szLogFileName = szSetting[20];
    }
    else
    {
        m_Config.m_Options.szLogFileName = MAIN_APP_LOG;
    }

    // 21
    // Description: encoding of data stored in logfile
    if (szSetting[21].Compare(_T("")) != 0)
    {
        m_Config.m_Options.nLogEncoding = stoi(szSetting[21]);
    }
    else
    {
        m_Config.m_Options.nLogEncoding = 2;
    }

    // 22
    // Description: force console window instead of conversion progress
    if (szSetting[22].Compare(_T("")) != 0)
    {
        if (szSetting[22].Compare(_T("true")) == 0)
        {
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_CHECKED);
            this->bForceConsoleWindow = true;
        }
        else
        {
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_UNCHECKED);
            this->bForceConsoleWindow = false;
        }
    }
    else
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_UNCHECKED);
        this->bForceConsoleWindow = false;

    }

    // remove all items from current preset
    this->m_CmbPresets.ResetContent();

    // clear presets lists
    for (int i = 0; i < NUM_PRESET_FILES; i++)
        m_Config.m_Presets[i].RemoveAllNodes();

    // load all presets configuration files
    for (int i = 0; i < NUM_PRESET_FILES; i++)
    {
        if (this->LoadPresets(m_Config.szPresetsFile[i], &m_Config.m_Presets[i]) == false)
            return false;
    }

    // update output ComboBox'es depending on selected format
    this->UpdateOutputComboBoxes(nSelFormatIndex, nCurSel[nSelFormatIndex]);

    return true;
}

bool CBatchEncoderDlg::SaveConfigFile()
{
    // save all settings to file
    CXMLDocumentW doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    doc.LinkEndChild(decl);

    // root: Configuration
    tinyxml2::XMLElement *pRootElem = doc.NewElement("Configuration");
    doc.LinkEndChild(pRootElem);

    // root: Settings
    tinyxml2::XMLElement *pSettingsElem = doc.NewElement("Settings");
    pRootElem->LinkEndChild(pSettingsElem);

    CString szSetting[NUM_PROGRAM_SETTINGS];

    // 0
    szSetting[0] = _T("");
    for (int i = 0; i < NUM_OUTPUT_EXT; i++)
    {
        CString szTemp;
        szTemp.Format(_T("%d"), nCurSel[i]);
        szSetting[0] += szTemp;

        if (i < (NUM_OUTPUT_EXT - 1))
            szSetting[0] += _T(" ");
    }

    // 1
    szSetting[1].Format(_T("%d\0"), this->m_CmbFormat.GetCurSel());

    // 2
    szSetting[2] = (this->m_ChkOutPath.GetCheck() == BST_CHECKED) ? _T("true") : _T("false");

    // 3
    szSetting[3] = this->GetMenuItemCheck(ID_OPTIONS_LOGCONSOLEOUTPUT);

    // 4
    szSetting[4] = this->GetMenuItemCheck(ID_OPTIONS_DELETESOURCEFILEWHENDONE);

    // 5
    szSetting[5] = this->GetMenuItemCheck(ID_OPTIONS_STAYONTOP);

    // 6
    szSetting[6] = (::bRecurseChecked == true) ? _T("true") : _T("false");

    // 7
    szSetting[7] = this->GetWindowRectStr();

    // 8
    int nColWidth[7];
    for (int i = 0; i < 7; i++)
        nColWidth[i] = m_LstInputItems.GetColumnWidth(i);
    szSetting[8].Format(_T("%d %d %d %d %d %d %d"),
        nColWidth[0],
        nColWidth[1],
        nColWidth[2],
        nColWidth[3],
        nColWidth[4],
        nColWidth[5],
        nColWidth[6]);

    // 9
    szSetting[9] = this->GetMenuItemCheck(ID_VIEW_SHOWGRIDLINES);

    // 10
    szSetting[10] = this->GetMenuItemCheck(ID_OPTIONS_SHOWTRAYICON);

    // 11
    szSetting[11] = this->GetMenuItemCheck(ID_OPTIONS_DO_NOT_SAVE);

    // 12
    szSetting[12] = szPresetsWndResize;

    // 13
    szSetting[13] = szPresetsListColumns;

    // 14
    szSetting[14] = szFormatsWndResize;

    // 15
    szSetting[15] = szFormatsListColumns;

    // 16
    szSetting[16].Format(_T("%d"), m_Config.m_Options.nThreadPriorityIndex);

    // 17
    szSetting[17].Format(_T("%d"), m_Config.m_Options.nProcessPriorityIndex);

    // 18
    szSetting[18] = (m_Config.m_Options.bDeleteOnError == true) ? _T("true") : _T("false");

    // 19
    szSetting[19] = (m_Config.m_Options.bStopOnErrors == true) ? _T("true") : _T("false");

    // 20
    szSetting[20] = m_Config.m_Options.szLogFileName;

    // 21
    szSetting[21].Format(_T("%d"), m_Config.m_Options.nLogEncoding);

    // 22
    szSetting[22] = this->GetMenuItemCheck(ID_OPTIONS_FORCECONSOLEWINDOW);

    // store all settings from szSetting buffer
    for (int i = 0; i < NUM_PROGRAM_SETTINGS; i++)
    {
        CUtf8String szBuffUtf8;
        tinyxml2::XMLElement *stg = doc.NewElement(g_szSettingsTags[i]);
        stg->LinkEndChild(doc.NewText(szBuffUtf8.Create(szSetting[i])));
        pSettingsElem->LinkEndChild(stg);
        szBuffUtf8.Clear();
    }

    // root: Presets
    tinyxml2::XMLElement *pPresetsElem = doc.NewElement("Presets");
    pRootElem->LinkEndChild(pPresetsElem);

    for (int i = 0; i < NUM_PRESET_FILES; i++)
    {
        CUtf8String szBuffUtf8;

        tinyxml2::XMLElement *pPresetElem = doc.NewElement(g_szPresetTags[i]);
        pPresetElem->LinkEndChild(doc.NewText(szBuffUtf8.Create(m_Config.szPresetsFile[i])));
        pPresetsElem->LinkEndChild(pPresetElem);
        szBuffUtf8.Clear();
    }

    // root: Formats
    tinyxml2::XMLElement *pFormatsElem = doc.NewElement("Formats");
    pRootElem->LinkEndChild(pFormatsElem);

    // NOTE:
    // same code as in CFormatsDlg::OnBnClickedButtonSaveConfig()
    // only root->LinkEndChild(formats) is different

    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        CUtf8String szBuffUtf8;

        tinyxml2::XMLElement *pFormatElem = doc.NewElement("Format");

        pFormatElem->LinkEndChild(doc.NewText(szBuffUtf8.Create(m_Config.m_Formats[i].szPath)));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("name", szBuffUtf8.Create(m_Config.m_Formats[i].szName));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("template", szBuffUtf8.Create(m_Config.m_Formats[i].szTemplate));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("input", (m_Config.m_Formats[i].bInput) ? "true" : "false");
        pFormatElem->SetAttribute("output", (m_Config.m_Formats[i].bOutput) ? "true" : "false");

        pFormatElem->SetAttribute("function", szBuffUtf8.Create(m_Config.m_Formats[i].szFunction));
        szBuffUtf8.Clear();

        pFormatsElem->LinkEndChild(pFormatElem);
    }

    /*
    // TODO: Save m_EdtOutPath as part of CSettings
    // get last browse for outpath this is special case 
    // because user can change this value 
    // without changing m_Config.m_Browse.szBrowsePath[4] variable
    m_EdtOutPath.GetWindowText(m_Config.m_Browse.szBrowsePath[4]);
    */

    // root: Items
    tinyxml2::XMLElement *pItemsElem = doc.NewElement("Items");
    pRootElem->LinkEndChild(pItemsElem);
    this->SaveItems(doc, pItemsElem);

    // save file
    ::UpdatePath();
    return doc.SaveFileW(szMainConfigFile);
}

void CBatchEncoderDlg::LoadUserSettings()
{
    if (bRunning == true)
        return;

    // NOTE: not recommended to use this function at normal usage
    // shortcut: Ctrl+Shift+L
    // load main settings from file

    CFileDialog fd(TRUE, _T("config"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Config Files (*.config)|*.config|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    fd.m_ofn.lpstrInitialDir = ::GetExeFilePath();

    if (fd.DoModal() == IDOK)
    {
        CString szPath;
        szPath = fd.GetPathName();

        // store config filename to temp buffer
        CString szTmp = szMainConfigFile;

        // load settings from user file
        szMainConfigFile = szPath;
        if (this->LoadConfigFile() == false)
        {
            MessageBox(_T("Failed to load settings!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
        }

        // restore config filename from temp buffer
        szMainConfigFile = szTmp;
    }
}

void CBatchEncoderDlg::SaveUserSettings()
{
    if (bRunning == true)
        return;

    // NOTE: not recommended to use this function at normal usage
    // shortcut: Ctrl+Shift+S
    // save main settings to file

    CFileDialog fd(FALSE, _T("config"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Config Files (*.config)|*.config|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    fd.m_ofn.lpstrInitialDir = ::GetExeFilePath();

    if (fd.DoModal() == IDOK)
    {
        CString szPath;
        szPath = fd.GetPathName();

        // store config filename to temp buffer
        CString szTmp = szMainConfigFile;

        // load settings from user file
        szMainConfigFile = szPath;
        if (this->SaveConfigFile() == false)
        {
            MessageBox(_T("Failed to save settings!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
        }

        // restore config filename from temp buffer
        szMainConfigFile = szTmp;
    }
}

void CBatchEncoderDlg::LoadDefaultSettings()
{
    if (bRunning == true)
        return;

    // NOTE: not recommended to use this function at normal usage
    // shortcut: Ctrl+Shift+D
    // load default settings from program resources

    ::UpdatePath();

    szMainConfigFile = ::GetExeFilePath() + MAIN_APP_CONFIG;

    // delete default main config file
    ::DeleteFile(MAIN_APP_CONFIG);

    // delete default presets files
    for (int i = 0; i < NUM_PRESET_FILES; i++)
        ::DeleteFile(g_szPresetFiles[i]);

    // load settings from resources
    this->LoadConfigFile();
}

void CBatchEncoderDlg::OnBnClickedButtonConvert()
{
    /*
    HANDLE hProcess = ::GetCurrentProcess();
    DWORD_PTR dwProcessAffinityMask;
    DWORD_PTR dwSystemAffinityMask;
    int nProcessorsAvail = 1;

    // get number of processors available for our enc/dec processes
    // to run simultaneously on different processors for greater performance

    if(::GetProcessAffinityMask(hProcess,
        &dwProcessAffinityMask,
        &dwSystemAffinityMask) == TRUE)
    {
        switch(dwProcessAffinityMask)
        {
        case 2-1: nProcessorsAvail = 1; break;
        case 4-1: nProcessorsAvail = 2; break;
        case 8-1: nProcessorsAvail = 3; break;
        case 16-1: nProcessorsAvail = 4; break;
        case 32-1: nProcessorsAvail = 5; break;
        case 64-1: nProcessorsAvail = 6; break;
        case 128-1: nProcessorsAvail = 7; break;
        case 256-1: nProcessorsAvail = 8; break;
        // (2^n)-1, n=1..32
        default: nProcessorsAvail = 1; break;
        };
    }


    // select processor #1 for 1st process
    if(::SetProcessAffinityMask(hProcess, 2-1) == TRUE)
    {
        MessageBox(_T("#1 OK"));
    }

    // select processor #2 for 2nd process
    if(::SetProcessAffinityMask(hProcess, 4-1) == TRUE)
    {
        MessageBox(_T("#2 OK"));
    }
    */

    static volatile bool bSafeCheck = false;
    if (bSafeCheck == true)
        return;

    if (bRunning == false)
    {
        bSafeCheck = true;

        // get number of files to encode
        int nFiles = m_LstInputItems.GetItemCount();
        if (nFiles <= 0)
        {
            // MessageBox(_T("Add files to Input Files List"), _T("WARNING"), MB_OK | MB_ICONWARNING);
            bSafeCheck = false;
            return;
        }

        // create full output path
        if (this->m_ChkOutPath.GetCheck() == BST_CHECKED)
        {
            CString szPath;

            this->m_EdtOutPath.GetWindowText(szPath);
            if (szPath.GetLength() > 0)
            {
                if (::MakeFullPath(szPath) == FALSE)
                {
                    MessageBox(_T("Unable to Create Output Path"), _T("ERROR"), MB_OK | MB_ICONERROR);
                    bSafeCheck = false;
                    return;
                }
            }
        }

        // check if forced console mode is enabled
        if (this->GetMenu()->GetMenuState(ID_OPTIONS_FORCECONSOLEWINDOW, MF_BYCOMMAND) == MF_CHECKED)
            this->bForceConsoleWindow = true;
        else
            this->bForceConsoleWindow = false;

        // create worker thread in background for processing
        // the argument for thread function is pointer to dialog
        dwThreadID = 0;
        hThread = ::CreateThread(NULL, 0, WorkThread, this, CREATE_SUSPENDED, &dwThreadID);
        if (hThread == NULL)
        {
            // ERROR
            MessageBox(_T("Fatal Error when Creating Thread"), _T("ERROR"), MB_OK | MB_ICONERROR);
            bSafeCheck = false;
            return;
        }

        this->EnableUserInterface(FALSE);

        VERIFY(m_StatusBar.SetText(_T(""), 1, 0));

        m_BtnConvert.SetWindowText(_T("S&top"));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("S&top\tF9"));

        nProgressCurrent = 0;
        bRunning = true;

        // wakeup worker thread
        ::ResumeThread(hThread);

        bSafeCheck = false;
    }
    else
    {
        bSafeCheck = true;

        // note that TerminateThread is not used
        // if you want do this please uncomment
        // the line below but its not recommended
        // ::TerminateThread(hThread, 0);

        m_BtnConvert.SetWindowText(_T("Conve&rt"));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("Conve&rt\tF9"));

        this->EnableUserInterface(TRUE);

        bRunning = false;
        bSafeCheck = false;
    }
}

bool CBatchEncoderDlg::WorkerCallback(int nProgress, bool bFinished, bool bError, double fTime, int nIndex)
{
    if (bError == true)
    {
        // handle errors here
        m_LstInputItems.SetItemText(nIndex, 5, _T("--:--")); // Time
        m_LstInputItems.SetItemText(nIndex, 6, _T("Error")); // Status
        m_FileProgress.SetPos(0);

        bRunning = false;
        return bRunning;
    }

    if (bFinished == false)
    {
        if (nProgress != nProgressCurrent)
        {
            nProgressCurrent = nProgress;
            m_FileProgress.SetPos(nProgress);
            this->ShowProgressTrayIcon(nProgress);
        }
    }

    if (bFinished == true)
    {
        // bRunning = false - this is set in the end of worker thread proc
        if (nProgress != 100)
        {
            m_LstInputItems.SetItemText(nIndex, 5, _T("--:--")); // Time
            m_LstInputItems.SetItemText(nIndex, 6, _T("Error")); // Status
            m_FileProgress.SetPos(0);
        }
        else
        {
            CString szTime = ::FormatTime(fTime, 1);

            m_LstInputItems.SetItemText(nIndex, 5, szTime); // Time
            m_LstInputItems.SetItemText(nIndex, 6, _T("Done")); // Status
        }
    }

    // on false the worker thread will stop
    return bRunning;
}

BOOL CBatchEncoderDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    if (bRunning == true)
        return FALSE;

    // note that this is not used
    // return CResizeDialog::OnHelpInfo(pHelpInfo);

    return FALSE;
}

void CBatchEncoderDlg::OnOK()
{
    // CResizeDialog::OnOK();

    if (bRunning == true)
        return;
}

void CBatchEncoderDlg::OnCancel()
{
    // CResizeDialog::OnCancel();

    if (bRunning == true)
        return;
}

void CBatchEncoderDlg::OnClose()
{
    CResizeDialog::OnClose();

    if (bRunning == true)
        return;

    // TODO: 
    // - kill worker thread and any running command-line tool
    // - don't save settings on read-only media
    //   check if the path to exe is on read only media
    //   if true then do not save settings to disk

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) != MF_CHECKED)
        this->SaveConfigFile();

    this->EnableTrayIcon(false);

    CResizeDialog::OnOK();
}

void CBatchEncoderDlg::OnDestroy()
{
    CResizeDialog::OnDestroy();

    if (bRunning == true)
        return;

    m_Config.m_Items.RemoveAllNodes();
}

void CBatchEncoderDlg::UpdateFormatAndPreset()
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        // get number of selected files
        int nSelected = 0;
        for (int i = 0; i < nCount; i++)
        {
            // if selected then change output format and preset #
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                // output extension
                CString szOutExt = m_Config.m_Items.GetOutFormatExt(nFormat);
                m_Config.m_Items.SetItemOutExt(szOutExt, i);
                m_Config.m_Items.SetItemOutFormat(nFormat, i);
                this->m_LstInputItems.SetItemText(i, 3, szOutExt);

                // output preset
                CString szPreset;
                szPreset.Format(_T("%d"), nPreset);
                m_Config.m_Items.SetItemOutPreset(nPreset, i);
                this->m_LstInputItems.SetItemText(i, 4, szPreset);

                // update selected items counter
                nSelected++;
            }
        }

        // if none of the items was selected
        // then change output format and preset # for all items
        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                // output extension
                CString szOutExt = m_Config.m_Items.GetOutFormatExt(nFormat);
                m_Config.m_Items.SetItemOutExt(szOutExt, i);
                m_Config.m_Items.SetItemOutFormat(nFormat, i);
                this->m_LstInputItems.SetItemText(i, 3, szOutExt);

                // output preset
                CString szPreset;
                szPreset.Format(_T("%d"), nPreset);
                m_Config.m_Items.SetItemOutPreset(nPreset, i);
                this->m_LstInputItems.SetItemText(i, 4, szPreset);
            }
        }
    }
}

void CBatchEncoderDlg::OnCbnSelchangeComboPresets()
{
    int nSelPresetIndex = this->m_CmbPresets.GetCurSel();
    int nSelIndex = this->m_CmbFormat.GetCurSel();

    nCurSel[nSelIndex] = nSelPresetIndex;

    this->UpdateFormatAndPreset();
}

void CBatchEncoderDlg::OnCbnSelchangeComboFormat()
{
    int nSelIndex = this->m_CmbFormat.GetCurSel();
    if (nSelIndex != -1)
    {
        // update presets combo-box
        int nSelPresetIndex = this->m_CmbPresets.GetCurSel();
        int nSelFormatIndex = this->m_CmbFormat.GetCurSel();

        this->UpdateOutputComboBoxes(nSelFormatIndex, 0);

        // set current preset position per format
        this->m_CmbPresets.SetCurSel(nCurSel[nSelFormatIndex]);
    }

    this->UpdateFormatAndPreset();
}

void CBatchEncoderDlg::OnBnClickedButtonBrowsePath()
{
    if (bRunning == true)
        return;

    // browse for output directory for converted files
    LPMALLOC pMalloc;
    BROWSEINFO bi;
    LPITEMIDLIST pidlDesktop;
    LPITEMIDLIST pidlBrowse;
    TCHAR *lpBuffer;

    CString szTmp;
    this->m_EdtOutPath.GetWindowText(szTmp);

    /* TODO: 
    if (szTmp == m_Config.m_Browse.szBrowsePath[4])
        szLastBrowse = m_Config.m_Browse.szBrowsePath[4];
    else
    */
    szLastBrowse = szTmp;

    if (SHGetMalloc(&pMalloc) == E_FAIL)
        return;

    if ((lpBuffer = (TCHAR *)pMalloc->Alloc(MAX_PATH * 2)) == NULL)
    {
        pMalloc->Release();
        return;
    }

    if (!SUCCEEDED(::SHGetSpecialFolderLocation(this->GetSafeHwnd(), CSIDL_DESKTOP, &pidlDesktop)))
    {
        pMalloc->Free(lpBuffer);
        pMalloc->Release();
        return;
    }

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

    bi.hwndOwner = this->GetSafeHwnd();
    bi.pidlRoot = pidlDesktop;
    bi.pszDisplayName = lpBuffer;
    bi.lpszTitle = _T("Output path for converted files:");
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.iImage = 0;
    bi.lpfn = ::BrowseCallbackOutPath;

    pidlBrowse = ::SHBrowseForFolder(&bi);
    if (pidlBrowse != NULL)
    {
        if (::SHGetPathFromIDList(pidlBrowse, lpBuffer))
        {
            // TODO: m_Config.m_Browse.szBrowsePath[4] = szLastBrowse;

            szLastBrowse.Format(_T("%s\0"), lpBuffer);
            m_EdtOutPath.SetWindowText(lpBuffer);
        }
        pMalloc->Free(pidlBrowse);
    }

    pMalloc->Free(pidlDesktop);
    pMalloc->Free(lpBuffer);
    pMalloc->Release();
}

void CBatchEncoderDlg::OnBnClickedCheckOutPath()
{
    if (bRunning == true)
        return;

    bool bIsChecked = false;

    if (m_ChkOutPath.GetCheck() == BST_CHECKED)
        bIsChecked = true;

    if (bIsChecked == false)
    {
        if (bSameAsSourceEdit == true)
            this->OnEnSetFocusEditOutPath();

        m_BtnBrowse.EnableWindow(FALSE);
        m_EdtOutPath.EnableWindow(FALSE);
    }
    else
    {
        if (bSameAsSourceEdit == true)
            this->OnEnKillFocusEditOutPath();

        m_BtnBrowse.EnableWindow(TRUE);
        m_EdtOutPath.EnableWindow(TRUE);
    }
}

bool CBatchEncoderDlg::InsertToMemoryList(NewItemData &nid)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    ULARGE_INTEGER ulSize;
    ULONGLONG nFileSize;
    int nCurFormat;
    int nCurPreset;

    // check the file extensions
    if (CItemsList::IsValidInFileExtension(nid.szPath) == false)
        return false;

    // check user out extension
    if (nid.szOutExt.Compare(_T("")) != 0)
    {
        if (CItemsList::IsValidOutExtension(nid.szOutExt) == false)
            return false;

        nCurFormat = m_Config.m_Items.GetOutFormatIndex(nid.szOutExt);
    }
    else
    {
        nCurFormat = this->m_CmbFormat.GetCurSel();
    }

    // get selected preset if there is no user preset
    if (nid.nOutPreset != -1)
        nCurPreset = nid.nOutPreset;
    else
        nCurPreset = this->m_CmbPresets.GetCurSel();

    // get file size (this also checks if file exists)
    hFind = ::FindFirstFile(nid.szPath, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;

    ::FindClose(hFind);

    ulSize.HighPart = FindFileData.nFileSizeHigh;
    ulSize.LowPart = FindFileData.nFileSizeLow;
    nFileSize = ulSize.QuadPart;

    // add new node to file-list
    nid.nItem = m_Config.m_Items.InsertNode(nid.szPath,
        nid.szName,
        nFileSize,
        nCurFormat,
        nCurPreset);

    return true;
}

bool CBatchEncoderDlg::InsertToControlList(NewItemData &nid)
{
    CString tmpBuf;
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.iItem = nid.nItem;

    // [Name] : file name
    lvi.pszText = (LPTSTR)(LPCTSTR)(m_Config.m_Items.GetItemFileName(nid.nItem));
    m_LstInputItems.InsertItem(&lvi);
    m_LstInputItems.SetItemData(nid.nItem, nid.nItem);

    // [Type] : input extension 
    tmpBuf.Format(_T("%s"), m_Config.m_Items.GetItemInExt(nid.nItem));
    lvi.iSubItem = 1;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 1, lvi.pszText);

    // [Size (bytes)] : file size
    tmpBuf.Format(_T("%I64d"), m_Config.m_Items.GetItemFileSize(nid.nItem));
    lvi.iSubItem = 2;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 2, lvi.pszText);

    // [Output] : output extension
    tmpBuf.Format(_T("%s"), m_Config.m_Items.GetItemOutExt(nid.nItem));
    lvi.iSubItem = 3;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 3, lvi.pszText);

    // [Preset] : selected preset index
    tmpBuf.Format(_T("%d"), m_Config.m_Items.GetItemOutPreset(nid.nItem));
    lvi.iSubItem = 4;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 4, lvi.pszText);

    // [Time] : enc/dec conversion time
    tmpBuf.Format(_T("%s"), (nid.szTime.Compare(_T("")) == 0) ? _T("--:--") : nid.szTime);
    lvi.iSubItem = 5;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 5, lvi.pszText);

    // [Status] : enc/dec progress status
    tmpBuf.Format(_T("%s"), (nid.szStatus.Compare(_T("")) == 0) ? _T("Not Done") : nid.szStatus);
    lvi.iSubItem = 6;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 6, lvi.pszText);

    // set item CheckBox state
    m_LstInputItems.SetCheck(nid.nItem, nid.bChecked);

    return true;
}

bool CBatchEncoderDlg::InsertToList(NewItemData &nid)
{
    if (((nid.nAction == ADD_ITEM_MEMORY) || (nid.nAction == ADD_ITEM_MEMORY_AND_CONTROL)) && (nid.nItem == -1))
    {
        if (this->InsertToMemoryList(nid) == false)
            return false;
    }

    if (((nid.nAction == ADD_ITEM_CONTROL) || (nid.nAction == ADD_ITEM_MEMORY_AND_CONTROL)) && (nid.nItem >= 0))
    {
        if (this->InsertToControlList(nid) == false)
            return false;
    }

    return true;
}

void CBatchEncoderDlg::OnLvnKeydownListInputFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

    switch (pLVKeyDow->wVKey)
    {
    case VK_INSERT:
        this->OnEditCrop();
        break;
    case VK_DELETE:
        this->OnEditRemove();
        break;
    default: break;
    };

    *pResult = 0;
}

void CBatchEncoderDlg::SearchFolderForFiles(CString szFile,
    const bool bRecurse,
    const TCHAR *szOutExt,
    const int nPreset)
{
    try
    {
        WIN32_FIND_DATA w32FileData;
        HANDLE hSearch = NULL;
        BOOL fFinished = FALSE;
        TCHAR cTempBuf[(MAX_PATH * 2) + 1];

        ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
        ZeroMemory(cTempBuf, MAX_PATH * 2);

        // remove '\' or '/' from end of search path
        szFile.TrimRight(_T("\\"));
        szFile.TrimRight(_T("/"));

        wsprintf(cTempBuf, _T("%s\\*.*\0"), szFile);

        hSearch = FindFirstFile(cTempBuf, &w32FileData);
        if (hSearch == INVALID_HANDLE_VALUE)
            return;

        while (fFinished == FALSE)
        {
            if (w32FileData.cFileName[0] != '.' &&
                !(w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                CString szTempBuf;
                szTempBuf.Format(_T("%s\\%s\0"), szFile, w32FileData.cFileName);

                NewItemData nid;
                this->InitNewItemData(nid);

                if ((szOutExt != NULL) && (nPreset != -1))
                {
                    nid.nAction = ADD_ITEM_MEMORY;
                    nid.szPath = szTempBuf;
                    nid.nItem = -1;
                    nid.szName = _T("");
                    nid.szOutExt = szOutExt;
                    nid.nOutPreset = nPreset;

                    this->InsertToList(nid);
                }
                else
                {
                    nid.nAction = ADD_ITEM_MEMORY_AND_CONTROL;
                    nid.szPath = szTempBuf;
                    nid.nItem = -1;

                    this->InsertToList(nid);
                }
            }

            if (w32FileData.cFileName[0] != '.' &&
                w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                wsprintf(cTempBuf, _T("%s\\%s\0"), szFile, w32FileData.cFileName);

                // recurse subdirs
                if (bRecurse == true)
                    this->SearchFolderForFiles(cTempBuf, true, szOutExt, nPreset);
            }

            if (FindNextFile(hSearch, &w32FileData) == FALSE)
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                    fFinished = TRUE;
                else
                    return;
            }
        }

        if (FindClose(hSearch) == FALSE)
            return;
    }
    catch (...)
    {
        MessageBox(_T("Error while searching for files!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CBatchEncoderDlg::HandleDropFiles(HDROP hDropInfo)
{
    int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, NULL, 0);
    if (nCount > 0)
    {
        NewItemData nid;
        this->InitNewItemData(nid);

        for (int i = 0; i < nCount; i++)
        {
            int nReqChars = ::DragQueryFile(hDropInfo, i, NULL, 0);

            CString szFile;
            ::DragQueryFile(hDropInfo,
                i,
                szFile.GetBuffer(nReqChars * 2 + 8),
                nReqChars * 2 + 8);
            if (::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
            {
                // insert dropped files in directory and subdirs
                this->SearchFolderForFiles(szFile, true);
            }
            else
            {
                // insert dropped files
                nid.nAction = ADD_ITEM_MEMORY_AND_CONTROL;
                nid.szPath = szFile;
                nid.nItem = -1;

                this->InsertToList(nid);
            }

            szFile.ReleaseBuffer();
        }

        this->UpdateStatusBar();
    }

    ::DragFinish(hDropInfo);
}

void CBatchEncoderDlg::OnDropFiles(HDROP hDropInfo)
{
    if (bRunning == true)
        return;

    // TODO: add wait dialog here (wait for hDDThread object)
    if (bHandleDrop == true)
    {
        bHandleDrop = false;

        m_DDParam.pDlg = this;
        m_DDParam.hDropInfo = hDropInfo;

        hDDThread = ::CreateThread(NULL, 0, DragAndDropThread, (LPVOID)&m_DDParam, 0, &dwDDThreadID);
        if (hDDThread == NULL)
            bHandleDrop = true;
    }

    // NOTE: under Win9x this did not work, we use separate thread to handle drop
    // this->HandleDropFiles(hDropInfo);

    CResizeDialog::OnDropFiles(hDropInfo);
}

void CBatchEncoderDlg::OnNMRclickListInputFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
    // right click contextmenu
    POINT point;
    GetCursorPos(&point);

    CMenu *subMenu = this->GetMenu()->GetSubMenu(1);

    subMenu->TrackPopupMenu(0, point.x, point.y, this, NULL);

    *pResult = 0;
}

void CBatchEncoderDlg::OnViewShowGridLines()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_VIEW_SHOWGRIDLINES, MF_BYCOMMAND) == MF_CHECKED)
        ShowGridlines(false);
    else
        ShowGridlines(true);
}

void CBatchEncoderDlg::OnNMDblclkListInputFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
    POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        /*
        int nItem = m_LstInputFiles.GetNextSelectedItem(pos);
        CString szPath = m_Config.m_Files.m_FileList.GetItemFilePath(nItem);

        ::LaunchAndWait(szPath, _T(""), FALSE);
        */
    }

    *pResult = 0;
}

void CBatchEncoderDlg::OnLvnItemchangedListInputFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // get item format and preset and setup ComboBoxes
    int nSelCount = this->m_LstInputItems.GetSelectedCount();
    if (nSelCount == 1)
    {
        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int nItem = this->m_LstInputItems.GetNextSelectedItem(pos);

            // check if we have such item in our file-list
            if (nItem < m_Config.m_Items.GetSize())
            {
                int nSelFormatIndex = m_Config.m_Items.GetItemOutFormat(nItem);
                int nSelPresetIndex = m_Config.m_Items.GetItemOutPreset(nItem);

                // load presets only if format is changing
                if (this->m_CmbFormat.GetCurSel() == nSelFormatIndex)
                {
                    this->m_CmbPresets.SetCurSel(nSelPresetIndex);
                }
                else
                {
                    this->m_CmbFormat.SetCurSel(nSelFormatIndex);
                    this->m_CmbPresets.SetCurSel(nSelPresetIndex);

                    this->UpdateOutputComboBoxes(nSelFormatIndex, nSelPresetIndex);
                }
            }
        }
    }

    /*
    if(pNMLV->uChanged == LVIF_STATE)
        this->UpdateStatusBar();
    */

    *pResult = 0;
}

void CBatchEncoderDlg::ResetOutput()
{
    this->UpdateFormatAndPreset();
}

void CBatchEncoderDlg::ResetConvertionTime()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        int nSelected = 0;
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                this->m_LstInputItems.SetItemText(i, 5, _T("--:--"));
                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
                this->m_LstInputItems.SetItemText(i, 5, _T("--:--"));
        }
    }
}

void CBatchEncoderDlg::ResetConvertionStatus()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        int nSelected = 0;
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                this->m_LstInputItems.SetItemText(i, 6, _T("Not Done"));
                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
                this->m_LstInputItems.SetItemText(i, 6, _T("Not Done"));
        }
    }
}

void CBatchEncoderDlg::EnableUserInterface(BOOL bEnable)
{
    if (bEnable == FALSE)
    {
        if (this->bForceConsoleWindow == false)
        {
            this->m_ChkOutPath.ShowWindow(SW_HIDE);
            this->m_EdtOutPath.ShowWindow(SW_HIDE);
            this->m_BtnBrowse.ShowWindow(SW_HIDE);
            this->m_FileProgress.ShowWindow(SW_SHOW);
        }
    }
    else
    {
        if (this->bForceConsoleWindow == false)
        {
            this->m_FileProgress.ShowWindow(SW_HIDE);
            this->m_ChkOutPath.ShowWindow(SW_SHOW);
            this->m_EdtOutPath.ShowWindow(SW_SHOW);
            this->m_BtnBrowse.ShowWindow(SW_SHOW);
            this->m_LstInputItems.ShowWindow(SW_SHOW);
        }
    }

    CMenu* pSysMenu = GetSystemMenu(FALSE);

    // enable/disable close button while conversion process
    if (bEnable == FALSE)
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
    else
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

    // enable/disable all main menu items
    UINT nEnable = (bEnable == TRUE) ? MF_ENABLED : MF_GRAYED;
    CMenu *pMainMenu = this->GetMenu();
    UINT nItems = pMainMenu->GetMenuItemCount();
    for (UINT i = 0; i < nItems; i++)
    {
        // skip only Action menu items
        if (i == 3)
            continue;

        CMenu *pSubMenu = pMainMenu->GetSubMenu(i);
        UINT nSubItems = pSubMenu->GetMenuItemCount();
        for (UINT j = 0; j < nSubItems; j++)
        {
            UINT nID = pSubMenu->GetMenuItemID(j);
            pSubMenu->EnableMenuItem(nID, nEnable);
        }
    }

    // enable/disable main dialog items
    this->m_CmbPresets.EnableWindow(bEnable);
    this->m_CmbFormat.EnableWindow(bEnable);
    this->m_LstInputItems.EnableWindow(bEnable);

    if (this->m_ChkOutPath.GetCheck() == BST_CHECKED)
        this->m_EdtOutPath.EnableWindow(bEnable);

    if (this->m_ChkOutPath.GetCheck() == BST_CHECKED)
        this->m_BtnBrowse.EnableWindow(bEnable);

    this->m_ChkOutPath.EnableWindow(bEnable);
}

void CBatchEncoderDlg::OnEnChangeEditOutPath()
{
    // CString szPath;
    // m_EdtOutPath.GetWindowText(szPath);
}

void CBatchEncoderDlg::OnEnSetFocusEditOutPath()
{
    // TODO: add option in Adv dialog to disable this type of behavior
    if (bSameAsSourceEdit == true)
    {
        CString szPath;
        m_EdtOutPath.GetWindowText(szPath);
        if (szPath.CompareNoCase(_T("<< same as source file >>")) == 0)
            m_EdtOutPath.SetWindowText(_T(""));
    }
}

void CBatchEncoderDlg::OnEnKillFocusEditOutPath()
{
    if (bSameAsSourceEdit == true)
    {
        CString szPath;
        m_EdtOutPath.GetWindowText(szPath);
        if (szPath.CompareNoCase(_T("")) == 0)
            m_EdtOutPath.SetWindowText(_T("<< same as source file >>"));
    }
}

bool CBatchEncoderDlg::LoadList(CString szFileXml)
{
    CXMLDocumentW doc;
    if (doc.LoadFileW(szFileXml) == true)
    {
        // root: Items
        tinyxml2::XMLElement *pItemsElem = doc.FirstChildElement();
        if (!pItemsElem)
        {
            // MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return false;
        }

        // check for "Items"
        const char *pszRoot = pItemsElem->Value();
        const char *pszRootName = "Items";
        if (strcmp(pszRootName, pszRoot) != 0)
        {
            // MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return false;
        }

        // clear the list
        this->OnEditClear();

        this->LoadItems(pItemsElem);

        return true;
    }

    return false;
}

bool CBatchEncoderDlg::SaveList(CString szFileXml)
{
    CXMLDocumentW doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    doc.LinkEndChild(decl);

    CString szBuff;
    CUtf8String szBuffUtf8;

    // root: Items
    tinyxml2::XMLElement *pItemsElem = doc.NewElement("Items");
    doc.LinkEndChild(pItemsElem);
    this->SaveItems(doc, pItemsElem);

    if (doc.SaveFileW(szFileXml) != true)
        return false;
    else
        return true;
}

void CBatchEncoderDlg::OnFileLoadList()
{
    if (bRunning == true)
        return;

    CFileDialog fd(TRUE, _T("list"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("List Items (*.list)|*.list|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();

        if (this->LoadList(szFileXml) == false)
        {
            MessageBox(_T("Failed to load file!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
        }
        else
        {
            this->UpdateStatusBar();
        }
    }
}

void CBatchEncoderDlg::OnFileSaveList()
{
    if (bRunning == true)
        return;

    CFileDialog fd(FALSE, _T("list"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("List Items (*.list)|*.list|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();

        if (this->SaveList(szFileXml) == false)
        {
            MessageBox(_T("Failed to save file!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
        }
    }
}

void CBatchEncoderDlg::OnFileClearList()
{
    if (bRunning == true)
        return;

    this->OnEditClear();
}

void CBatchEncoderDlg::OnFileCreateBatchFile()
{
    if (bRunning == true)
        return;

    CString szInitName;
    SYSTEMTIME st;

    ::GetLocalTime(&st);
    szInitName.Format(_T("convert_%04d%02d%02d_%02d%02d%02d%03d"),
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    CFileDialog fd(FALSE, _T("bat"), szInitName,
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Batch Files (*.bat)|*.bat|Script Files (*.cmd)|*.cmd|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileBatch = fd.GetPathName();

        if (this->CreateBatchFile(szFileBatch) == false)
        {
            MessageBox(_T("Failed to create batch-file!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
        }
    }
}

void CBatchEncoderDlg::OnFileExit()
{
    if (bRunning == true)
        return;

    if (this->bRunning == false)
        this->OnClose();
}

void CBatchEncoderDlg::OnEditAddFiles()
{
    if (bRunning == true)
        return;

    // buffer for selected files
    TCHAR *pFiles = NULL;
    const DWORD dwMaxSize = (4096 * MAX_PATH);
    try
    {
        // allocate memory for file buffer
        pFiles = (TCHAR *)malloc(dwMaxSize);
        if (pFiles == NULL)
        {
            MessageBox(_T("Failed to allocate memory for filenames buffer!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
            return;
        }

        // zero memory
        ZeroMemory(pFiles, dwMaxSize);

        // initialize File Open dialog
        CFileDialog fd(TRUE,
            _T(""),
            0,
            OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_ENABLESIZING,
            _T("Supported Audio Files|")
            _T("*.wav;*.mp1;*.mp2;*.mp3;*.mpc;*.mpp;*.mp+;*.mp4;*.m4a;*.aac;*.ogg;*.ac3;*.flac;*.wv;*.mac;*.ape;*.ofr;*.tta;*.tak|")
            _T("WAV (*.wav)|*.wav|")
            _T("MP1 (*.mp1)|*.mp1|")
            _T("MP2 (*.mp2)|*.mp2|")
            _T("MP3 (*.mp3)|*.mp3|")
            _T("MPC (*.mpc)|*.mpc|")
            _T("MPP (*.mpp)|*.mpp|")
            _T("MP+ (*.mp+)|*.mp+|")
            _T("MP4 (*.mp4)|*.mp4|")
            _T("M4A (*.m4a)|*.m4a|")
            _T("AAC (*.aac)|*.aac|")
            _T("OGG (*.ogg)|*.ogg|")
            _T("AC3 (*.ac3)|*.ac3|")
            _T("FLAC (*.flac)|*.flac|")
            _T("WV (*.wv)|*.wv|")
            _T("MAC (*.mac)|*.mac|")
            _T("APE (*.ape)|*.ape|")
            _T("OFR (*.ofr)|*.ofr|")
            _T("TTA (*.tta)|*.tta|")
            _T("TAK (*.tak)|*.tak||"),
            this);

        // use big buffer 
        fd.m_ofn.lpstrFile = pFiles;
        fd.m_ofn.nMaxFile = (dwMaxSize) / 2;

        // show File Open dialog
        if (fd.DoModal() != IDCANCEL)
        {
            CString sFilePath;
            POSITION pos = fd.GetStartPosition();

            NewItemData nid;
            this->InitNewItemData(nid);

            // insert all files to list
            do
            {
                sFilePath = fd.GetNextPathName(pos);
                if (!sFilePath.IsEmpty())
                {
                    nid.nAction = ADD_ITEM_MEMORY_AND_CONTROL;
                    nid.szPath = sFilePath;
                    nid.nItem = -1;

                    this->InsertToList(nid);
                }
            } while (pos != NULL);

            this->UpdateStatusBar();
        }
    }
    catch (...)
    {
        // free memory buffer on error
        if (pFiles != NULL)
        {
            free(pFiles);
            pFiles = NULL;
        }
    }

    // free memory buffer
    if (pFiles != NULL)
    {
        free(pFiles);
        pFiles = NULL;
    }
}

void CBatchEncoderDlg::OnEditAddDir()
{
    if (bRunning == true)
        return;

    LPMALLOC pMalloc;
    BROWSEINFO bi;
    LPITEMIDLIST pidlDesktop;
    LPITEMIDLIST pidlBrowse;
    TCHAR *lpBuffer;

    if (SHGetMalloc(&pMalloc) == E_FAIL)
        return;

    if ((lpBuffer = (TCHAR *)pMalloc->Alloc(MAX_PATH * 2)) == NULL)
    {
        pMalloc->Release();
        return;
    }

    if (!SUCCEEDED(::SHGetSpecialFolderLocation(this->GetSafeHwnd(), CSIDL_DESKTOP, &pidlDesktop)))
    {
        pMalloc->Free(lpBuffer);
        pMalloc->Release();
        return;
    }

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

    bi.hwndOwner = this->GetSafeHwnd();
    bi.pidlRoot = pidlDesktop;
    bi.pszDisplayName = lpBuffer;
    bi.lpszTitle = _T("Select folder with Audio Files:");
    bi.lpfn = NULL;
    bi.lParam = 0;
    bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.iImage = 0;
    bi.lpfn = ::BrowseCallbackAddDir;

    pidlBrowse = ::SHBrowseForFolder(&bi);
    if (pidlBrowse != NULL)
    {
        if (::SHGetPathFromIDList(pidlBrowse, lpBuffer))
        {
            CString szPath = lpBuffer;
            this->SearchFolderForFiles(szPath, bRecurseChecked);
            this->UpdateStatusBar();
        }
        pMalloc->Free(pidlBrowse);
    }

    pMalloc->Free(pidlDesktop);
    pMalloc->Free(lpBuffer);
    pMalloc->Release();
}

void CBatchEncoderDlg::OnEditClear()
{
    if (bRunning == true)
        return;

    // clear node list
    m_Config.m_Items.RemoveAllNodes();

    // clear list view
    m_LstInputItems.DeleteAllItems();

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditRemoveChecked()
{
    if (bRunning == true)
        return;

    // remove all checked items
    int nItems = m_LstInputItems.GetItemCount();
    if (nItems <= 0)
        return;

    for (int i = (nItems - 1); i >= 0; i--)
    {
        if (m_LstInputItems.GetCheck(i) == TRUE)
        {
            m_Config.m_Items.RemoveNode(i);
            m_LstInputItems.DeleteItem(i);
        }
    }

    if (m_LstInputItems.GetItemCount() == 0)
    {
        m_Config.m_Items.RemoveAllNodes();
        m_LstInputItems.DeleteAllItems();
    }

    // NOTE: check all items
    /*
    int nCount = m_LstInputFiles.GetItemCount();
    if(nCount > 0)
    {
        for(int i = 0; i < nCount; i++)
            m_LstInputFiles.SetCheck(i, TRUE);
    }
    */

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditRemoveUnchecked()
{
    if (bRunning == true)
        return;

    // remove all unchecked items
    int nItems = m_LstInputItems.GetItemCount();
    if (nItems <= 0)
        return;

    for (int i = (nItems - 1); i >= 0; i--)
    {
        if (m_LstInputItems.GetCheck(i) == FALSE)
        {
            m_Config.m_Items.RemoveNode(i);
            m_LstInputItems.DeleteItem(i);
        }
    }

    if (m_LstInputItems.GetItemCount() == 0)
    {
        m_Config.m_Items.RemoveAllNodes();
        m_LstInputItems.DeleteAllItems();
    }

    // NOTE: un-check all items
    /*
    int nCount = m_LstInputFiles.GetItemCount();
    if(nCount > 0)
    {
        for(int i = 0; i < nCount; i++)
            m_LstInputFiles.SetCheck(i, FALSE);
    }
    */

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditCheckSelected()
{
    if (bRunning == true)
        return;

    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                m_LstInputItems.SetCheck(i, TRUE);
        }
    }
}

void CBatchEncoderDlg::OnEditUncheckSelected()
{
    if (bRunning == true)
        return;

    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                m_LstInputItems.SetCheck(i, FALSE);
        }
    }

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditRename()
{
    if (bRunning == true)
        return;

    // check if control has keyboard focus
    if (m_LstInputItems.GetFocus()->GetSafeHwnd() != m_LstInputItems.GetSafeHwnd())
        return;

    POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        this->m_LstInputItems.SetFocus();
        int nItem = m_LstInputItems.GetNextSelectedItem(pos);
        this->m_LstInputItems.EditLabel(nItem);
    }
}

void CBatchEncoderDlg::OnEditOpen()
{
    if (bRunning == true)
        return;

    POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstInputItems.GetNextSelectedItem(pos);
        CString szPath = m_Config.m_Items.GetItemFilePath(nItem);

        ::LaunchAndWait(szPath, _T(""), FALSE);
    }
}

void CBatchEncoderDlg::OnEditExplore()
{
    if (bRunning == true)
        return;

    POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstInputItems.GetNextSelectedItem(pos);
        CString szPath = m_Config.m_Items.GetItemFilePath(nItem);

        CString szName = m_Config.m_Items.GetFileName(szPath);
        szPath.TrimRight(szName);

        ::LaunchAndWait(szPath, _T(""), FALSE);
    }
}

void CBatchEncoderDlg::OnEditCrop()
{
    if (bRunning == true)
        return;

    // invert selection
    int nFiles = m_LstInputItems.GetItemCount();
    for (int i = 0; i < nFiles; i++)
    {
        if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            m_LstInputItems.SetItemState(i, 0, LVIS_SELECTED);
        else
            m_LstInputItems.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }

    // now delete selected
    int nItem = -1;
    do
    {
        nItem = m_LstInputItems.GetNextItem(-1, LVIS_SELECTED);
        if (nItem != -1)
        {
            m_Config.m_Items.RemoveNode(nItem);
            m_LstInputItems.DeleteItem(nItem);
        }
    } while (nItem != -1);

    if (m_LstInputItems.GetItemCount() == 0)
    {
        m_Config.m_Items.RemoveAllNodes();
        m_LstInputItems.DeleteAllItems();
    }

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditSelectNone()
{
    if (bRunning == true)
        return;

    m_LstInputItems.SetItemState(-1, 0, LVIS_SELECTED);
}

void CBatchEncoderDlg::OnEditInvertSelection()
{
    if (bRunning == true)
        return;

    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                m_LstInputItems.SetItemState(i, 0, LVIS_SELECTED);
            else
                m_LstInputItems.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
}

void CBatchEncoderDlg::OnEditRemove()
{
    if (bRunning == true)
        return;

    int nItem = -1;
    int nItemLastRemoved = -1;
    do
    {
        nItem = m_LstInputItems.GetNextItem(-1, LVIS_SELECTED);
        if (nItem != -1)
        {
            m_Config.m_Items.RemoveNode(nItem);
            m_LstInputItems.DeleteItem(nItem);

            nItemLastRemoved = nItem;
        }
    } while (nItem != -1);

    // select other item in list
    int nItems = m_LstInputItems.GetItemCount();
    if (nItemLastRemoved != -1)
    {
        if (nItemLastRemoved < nItems && nItems >= 0)
            m_LstInputItems.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
        else if (nItemLastRemoved >= nItems && nItems >= 0)
            m_LstInputItems.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
    }

    if (m_LstInputItems.GetItemCount() == 0)
    {
        m_Config.m_Items.RemoveAllNodes();
        m_LstInputItems.DeleteAllItems();
    }

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditSelectAll()
{
    if (bRunning == true)
        return;

    m_LstInputItems.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
}

void CBatchEncoderDlg::OnEditResetOutput()
{
    if (bRunning == true)
        return;

    this->ResetOutput();
}

void CBatchEncoderDlg::OnEditResetTime()
{
    if (bRunning == true)
        return;

    // NOTE: when user pressed F3 reset StatusBar
    this->m_StatusBar.SetText(_T(""), 1, 0);

    this->ResetConvertionTime();
    this->ResetConvertionStatus();
}

void CBatchEncoderDlg::OnActionConvert()
{
    this->OnBnClickedButtonConvert();
}

void CBatchEncoderDlg::OnOptionsStayOnTop()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_STAYONTOP, MF_BYCOMMAND) == MF_CHECKED)
    {
        this->SetWindowPos(CWnd::FromHandle(HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STAYONTOP, MF_UNCHECKED);
    }
    else
    {
        this->SetWindowPos(CWnd::FromHandle(HWND_TOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STAYONTOP, MF_CHECKED);
    }
}

void CBatchEncoderDlg::OnOptionsShowTrayIcon()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_SHOWTRAYICON, MF_BYCOMMAND) == MF_CHECKED)
        this->EnableTrayIcon(false);
    else
        this->EnableTrayIcon(true);
}

void CBatchEncoderDlg::OnOptionsLogConsoleOutput()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsShowLog()
{
    if (bRunning == true)
        return;

    CFileStatus rStatus;
    if (CFile::GetStatus(m_Config.m_Options.szLogFileName, rStatus) == TRUE)
    {
        // load logfile in default system editor
        ::LaunchAndWait(m_Config.m_Options.szLogFileName, _T(""), FALSE);
    }
}

void CBatchEncoderDlg::OnOptionsDeleteLog()
{
    CFileStatus rStatus;
    if (CFile::GetStatus(m_Config.m_Options.szLogFileName, rStatus) == TRUE)
    {
        if (::DeleteFile(m_Config.m_Options.szLogFileName) == FALSE)
            this->MessageBox(_T("Failed to delete logfile!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CBatchEncoderDlg::OnOptionsDeleteSourceFileWhenDone()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsShutdownWhenFinished()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsDoNotSave()
{
    if (bRunning == true)
        return;

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsForceConsoleWindow()
{
    if (bRunning == true)
        return;

    // HELP:
    // To stop forced console mode conversion process you must do the following:
    // Step 1. Press Stop button in main window
    // Step 2. In current console use Ctrl+C shortcut.
    // Step 3. Delete any invalid result files.

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_FORCECONSOLEWINDOW, MF_BYCOMMAND) == MF_CHECKED)
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_UNCHECKED);
        this->bForceConsoleWindow = false;
    }
    else
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_CHECKED);
        this->bForceConsoleWindow = true;
    }
}

void CBatchEncoderDlg::OnOptionsAdvanced()
{
    if (bRunning == true)
        return;

    CAdvancedDlg dlg;

    m_Config.m_Options.Copy(dlg.m_Options);

    if (dlg.DoModal() == IDOK)
    {
        dlg.m_Options.Copy(m_Config.m_Options);
    }
}

void CBatchEncoderDlg::OnOptionsConfigurePresets()
{
    if (bRunning == true)
        return;

    CPresetsDlg dlg;

    if (this->GridlinesVisible() == true)
        dlg.bShowGridLines = true;
    else
        dlg.bShowGridLines = false;

    int nSelFormat = this->m_CmbFormat.GetCurSel();
    int nSelPreset = this->m_CmbPresets.GetCurSel();

    dlg.nSelFormat = nSelFormat;
    dlg.nSelPreset = nSelPreset;

    // configuration files
    for (int i = 0; i < NUM_PRESET_FILES; i++)
        dlg.szPresetsFile[i] = m_Config.szPresetsFile[i];

    dlg.szPresetsWndResize = szPresetsWndResize;
    dlg.szPresetsListColumns = this->szPresetsListColumns;

    INT_PTR nRet = dlg.DoModal();
    if (nRet == IDOK)
    {
        for (int i = 0; i < NUM_PRESET_FILES; i++)
        {
            // update executable file path
            m_Config.szPresetsFile[i] = dlg.szPresetsFile[i];

            // reload presets from files
            this->LoadPresets(m_Config.szPresetsFile[i], &m_Config.m_Presets[i]);
        }

        // update combo-box depending on selected format
        this->UpdateOutputComboBoxes(nSelFormat, nSelPreset);
    }
    else
    {
        // NOTE:
        // canceled all changes but some configuration files could changed
        // they will be loaded next time when app will start
    }

    this->szPresetsWndResize = dlg.szPresetsWndResize;
    this->szPresetsListColumns = dlg.szPresetsListColumns;
}

void CBatchEncoderDlg::OnOptionsConfigureFormat()
{
    if (bRunning == true)
        return;

    CFormatsDlg dlg;

    if (this->GridlinesVisible() == true)
        dlg.bShowGridLines = true;
    else
        dlg.bShowGridLines = false;

    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        m_Config.m_Formats[i].Copy(dlg.m_Formats[i]);
    }

    dlg.szFormatsWndResize = this->szFormatsWndResize;
    dlg.szFormatsListColumns = this->szFormatsListColumns;

    INT_PTR nRet = dlg.DoModal();
    if (nRet == IDOK)
    {
        for (int i = 0; i < NUM_FORMAT_NAMES; i++)
        {
            dlg.m_Formats[i].Copy(m_Config.m_Formats[i]);
        }
    }

    this->szFormatsWndResize = dlg.szFormatsWndResize;
    this->szFormatsListColumns = dlg.szFormatsListColumns;
}

void CBatchEncoderDlg::OnHelpWebsite()
{
    if (bRunning == true)
        return;

    ::LaunchAndWait(MAIN_APP_WEBSITE, _T(""), FALSE);
}

void CBatchEncoderDlg::OnHelpAbout()
{
    if (bRunning == true)
        return;

    CAboutDlg dlg;

    dlg.DoModal();
}
