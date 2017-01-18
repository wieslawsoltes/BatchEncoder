// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "BatchEncoderDlg.h"
#include "PresetsDlg.h"
#include "AboutDlg.h"
#include "FormatsDlg.h"
#include "AdvancedDlg.h"
#include "..\worker\WorkThread.h"

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

typedef struct tagDragAndDrop
{
    CBatchEncoderDlg *pDlg;
    HDROP hDropInfo;
} DragAndDrop;

static volatile bool bHandleDrop = true;
static HANDLE hDDThread;
static DWORD dwDDThreadID;
static DragAndDrop m_DDParam;

DWORD WINAPI DragAndDropThread(LPVOID lpParam)
{
    DragAndDrop* m_ThreadParam = (DragAndDrop*)lpParam;

    m_ThreadParam->pDlg->HandleDropFiles(m_ThreadParam->hDropInfo);
    bHandleDrop = true;

    return ::CloseHandle(hDDThread);
}

IMPLEMENT_DYNAMIC(CBatchEncoderDlg, CDialog)
CBatchEncoderDlg::CBatchEncoderDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CBatchEncoderDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    szMainConfigFile = ::GetExeFilePath() + _T("BatchEncoder.config");

    m_Config.m_Options.Defaults();
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
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_L, LoadConfig)
    ON_COMMAND(ID_ACCELERATOR_CTRL_SHIFT_S, SaveConfig)
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

    // handle the OnNotifyFormat message (WM_NOTIFYFORMAT)
#ifdef _UNICODE
    m_LstInputItems.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

    // enable or disable '<< same as source file >>' option
    bSameAsSourceEdit = true;

    // main dialog title with version number
    this->SetWindowText(MAIN_APP_NAME);

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

    // TODO: set ListCtrl columns order
    /*
    INT nOrder[7] = { 0, 1, 2, 3, 4, 5, 6 };
    this->m_LstInputItems.GetHeaderCtrl()->SetOrderArray(7, nOrder);
    this->m_LstInputItems.Invalidate();
    */

    // set to bold Convert/Stop button font style
    m_BtnConvert.SetBold(true);

    // hide ProgressBar when not running conversion process
    this->m_FileProgress.ShowWindow(SW_HIDE);

    // enable files/directories drag & drop for dialog
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

    try
    {
        this->LoadConfigFile(this->szMainConfigFile);
    }
    catch (...)
    {
        MessageBox(_T("Error loading configuration file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }

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
        m_Config.m_Options.bShowTrayIcon = true;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHOWTRAYICON, MF_CHECKED);
    }
    else if ((bEnable == true) && (bModify == true))
    {
        Shell_NotifyIcon(NIM_MODIFY, &tnd);
    }
    else
    {
        if (m_Config.m_Options.bShowTrayIcon == true)
        {
            Shell_NotifyIcon(NIM_DELETE, &tnd);
            m_Config.m_Options.bShowTrayIcon = false;
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHOWTRAYICON, MF_UNCHECKED);
        }
    }
}

void CBatchEncoderDlg::ShowProgressTrayIcon(int nProgress)
{
    if (m_Config.m_Options.bShowTrayIcon == false)
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

    if ((m_Config.m_Options.bShowTrayIcon == true) && (nType == SIZE_MINIMIZED))
    {
        ShowWindow(SW_HIDE);
        InvalidateRect(NULL, FALSE);
    }
}

LRESULT CBatchEncoderDlg::OnTrayIconMsg(WPARAM wParam, LPARAM lParam)
{
    UINT uID = (UINT)wParam;
    UINT uMouseMsg = (UINT)lParam;

    if (m_Config.m_Options.bShowTrayIcon == false)
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

    return(0);
}

LRESULT CBatchEncoderDlg::OnListItemChaged(WPARAM wParam, LPARAM lParam)
{
    INT nIndex = (INT)wParam;
    LPTSTR szText = (LPTSTR)lParam;
    if ((nIndex >= 0) && szText != NULL)
    {
        CItem& item = m_Config.m_Items.GetData(nIndex);
        item.szName = szText;
    }
    return(0);
}

void CBatchEncoderDlg::OnTrayMenuExit()
{
    if (m_Config.m_Options.bShowTrayIcon == true)
        this->OnClose();
}

LRESULT CBatchEncoderDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
{
    // required for list view control to receive notifications messages
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

void CBatchEncoderDlg::UpdateFormatComboBox()
{
    this->m_CmbFormat.ResetContent();

    int nFormats = m_Config.m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Config.m_Formats.GetData(i);

        // NOTE: insert only encoder, decoders are only for internal use
        if (format.nType == 0)
        {
            this->m_CmbFormat.InsertString(i, format.szName);
        }
    }

    static bool bResizeFormatsComboBox = false;
    if (bResizeFormatsComboBox == false)
    {
        ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_FORMAT);
        bResizeFormatsComboBox = true;
    }

    this->m_CmbFormat.SetCurSel(m_Config.m_Options.nSelectedFormat);
}

void CBatchEncoderDlg::UpdatePresetComboBox()
{
    this->m_CmbPresets.ResetContent();

    CFormat& format = m_Config.m_Formats.GetData(m_Config.m_Options.nSelectedFormat);

    int nPresets = format.m_Presets.GetSize();
    for (int i = 0; i < nPresets; i++)
    {
        CPreset& preset = format.m_Presets.GetData(i);
        this->m_CmbPresets.InsertString(i, preset.szName);
    }

    static bool bResizePresetsComboBox = false;
    if (bResizePresetsComboBox == false)
    {
        ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRESETS);
        bResizePresetsComboBox = true;
    }

    this->m_CmbPresets.SetCurSel(format.nDefaultPreset);
}

LPTSTR CBatchEncoderDlg::GetMenuItemCheck(int nID)
{
    return (this->GetMenu()->GetMenuState(nID, MF_BYCOMMAND) == MF_CHECKED) ? _T("true") : _T("false");
}

void CBatchEncoderDlg::SetMenuItemCheck(int nID, LPTSTR bChecked)
{
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
        if (dwExStyle & LVS_EX_GRIDLINES)
        {
            dwExStyle = dwExStyle ^ LVS_EX_GRIDLINES;
            m_LstInputItems.SetExtendedStyle(dwExStyle);
            this->GetMenu()->CheckMenuItem(ID_VIEW_SHOWGRIDLINES, MF_UNCHECKED);
        }
    }
}

void CBatchEncoderDlg::GetItems()
{
    int nItems = this->m_LstInputItems.GetItemCount();
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = m_Config.m_Items.GetData(i);
        item.nId = i;
        item.bChecked = this->m_LstInputItems.GetCheck(i) == TRUE;
        item.szTime = this->m_LstInputItems.GetItemText(i, 5);
        item.szStatus = this->m_LstInputItems.GetItemText(i, 6);
    }
}

void CBatchEncoderDlg::SetItems()
{
    int nItems = m_Config.m_Items.GetSize();
    for (int i = 0; i < nItems; i++)
    {
        this->InsertToControlList(i);
    }
}

void CBatchEncoderDlg::GetOptions()
{
    // option: SelectedFormat
    m_Config.m_Options.nSelectedFormat = this->m_CmbFormat.GetCurSel();

    // option: OutputPath
    m_EdtOutPath.GetWindowText(m_Config.m_Options.szOutputPath);

    // option: OutputPathChecked
    m_Config.m_Options.bOutputPathChecked = this->m_ChkOutPath.GetCheck() == BST_CHECKED;

    // option: LogConsoleOutput
    m_Config.m_Options.bLogConsoleOutput = this->GetMenu()->GetMenuState(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_BYCOMMAND) == MF_CHECKED;

    // option: DeleteSourceFiles
    m_Config.m_Options.bDeleteSourceFiles = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_BYCOMMAND) == MF_CHECKED;

    // option: StayOnTop
    m_Config.m_Options.bStayOnTop = this->GetMenu()->GetMenuState(ID_OPTIONS_STAYONTOP, MF_BYCOMMAND) == MF_CHECKED;

    // option: RecurseChecked
    m_Config.m_Options.bRecurseChecked = ::bRecurseChecked;

    // option: MainWindowResize
    m_Config.m_Options.szMainWindowResize = this->GetWindowRectStr();

    // option: FileListColumns
    int nColWidth[7];
    for (int i = 0; i < 7; i++)
        nColWidth[i] = m_LstInputItems.GetColumnWidth(i);
    m_Config.m_Options.szFileListColumns.Format(_T("%d %d %d %d %d %d %d"),
        nColWidth[0],
        nColWidth[1],
        nColWidth[2],
        nColWidth[3],
        nColWidth[4],
        nColWidth[5],
        nColWidth[6]);

    // option: ShowGridLines
    m_Config.m_Options.bShowGridLines = this->GetMenu()->GetMenuState(ID_VIEW_SHOWGRIDLINES, MF_BYCOMMAND) == MF_CHECKED;

    // option: ShowTrayIcon
    m_Config.m_Options.bShowTrayIcon = this->GetMenu()->GetMenuState(ID_OPTIONS_SHOWTRAYICON, MF_BYCOMMAND) == MF_CHECKED;

    // option: ShutdownWhenFinished
    m_Config.m_Options.bShutdownWhenFinished = this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_BYCOMMAND) == MF_CHECKED;

    // option: DoNotSaveConfiguration
    m_Config.m_Options.bDoNotSaveConfiguration = this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) == MF_CHECKED;

    // option: ForceConsoleWindow
    m_Config.m_Options.bForceConsoleWindow = this->GetMenu()->GetMenuState(ID_OPTIONS_FORCECONSOLEWINDOW, MF_BYCOMMAND) == MF_CHECKED;
}

void CBatchEncoderDlg::SetOptions()
{
    // option: SelectedFormat

    // option: OutputPath
    if (m_Config.m_Options.szOutputPath.Compare(_T("")) != 0)
    {
        this->m_EdtOutPath.SetWindowText(m_Config.m_Options.szOutputPath);
        szLastBrowse = m_Config.m_Options.szOutputPath;
    }
    else
    {
        m_Config.m_Options.szOutputPath = ::GetExeFilePath();
        szLastBrowse = m_Config.m_Options.szOutputPath;
        this->m_EdtOutPath.SetWindowText(m_Config.m_Options.szOutputPath);
    }

    // option: OutputPathChecked
    if (m_Config.m_Options.bOutputPathChecked)
    {
        m_ChkOutPath.SetCheck(BST_CHECKED);
        m_BtnBrowse.EnableWindow(TRUE);
        m_EdtOutPath.EnableWindow(TRUE);
    }
    else
    {
        m_ChkOutPath.SetCheck(BST_UNCHECKED);
        m_BtnBrowse.EnableWindow(FALSE);
        m_EdtOutPath.EnableWindow(FALSE);
    }

    // option: LogConsoleOutput
    if (m_Config.m_Options.bLogConsoleOutput)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_UNCHECKED);

    // option: DeleteSourceFiles
    if (m_Config.m_Options.bDeleteSourceFiles)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_UNCHECKED);

    // option: StayOnTop
    if (m_Config.m_Options.bStayOnTop)
    {
        this->SetWindowPos(CWnd::FromHandle(HWND_TOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STAYONTOP, MF_CHECKED);
    }
    else
    {
        this->SetWindowPos(CWnd::FromHandle(HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STAYONTOP, MF_UNCHECKED);
    }

    // option: RecurseChecked
    if (m_Config.m_Options.bRecurseChecked)
        ::bRecurseChecked = true;
    else
        ::bRecurseChecked = false;

    // option: MainWindowResize
    if (m_Config.m_Options.szMainWindowResize.Compare(_T("")) != 0)
    {
        this->SetWindowRectStr(m_Config.m_Options.szMainWindowResize);
    }

    // option: FileListColumns
    if (m_Config.m_Options.szFileListColumns.Compare(_T("")) != 0)
    {
        int nColWidth[7];
        if (_stscanf(m_Config.m_Options.szFileListColumns, _T("%d %d %d %d %d %d %d"),
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

    // option: ShowGridLines
    ShowGridlines(m_Config.m_Options.bShowGridLines);

    // option: ShowTrayIcon
    EnableTrayIcon(m_Config.m_Options.bShowTrayIcon);

    // option: ShutdownWhenFinished
    if (m_Config.m_Options.bShutdownWhenFinished)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_UNCHECKED);

    // option: DoNotSaveConfiguration
    if (m_Config.m_Options.bDoNotSaveConfiguration)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);

    // option: PresetsDialogResize

    // option: PresetsListColumns

    // option: FormatsDialogResize

    // option: FormatsListColumns

    // option: DeleteOnError

    // option: StopOnErrors

    // option: LogFileName

    // option: LogFileEncoding

    // option: ForceConsoleWindow
    if (m_Config.m_Options.bForceConsoleWindow)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_UNCHECKED);
}

bool CBatchEncoderDlg::LoadListFile(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.LoadFileUtf8(szFileXml) == true)
    {
        tinyxml2::XMLElement *pItemsElem = doc.FirstChildElement();
        if (!pItemsElem)
        {
            return false;
        }

        const char *pszRoot = pItemsElem->Value();
        const char *pszRootName = "Items";
        if (strcmp(pszRootName, pszRoot) != 0)
        {
            return false;
        }

        this->OnEditClear();

        doc.LoadItems(pItemsElem, m_Config.m_Items);

        this->SetItems();

        return true;
    }

    return false;
}

bool CBatchEncoderDlg::SaveListFile(CString szFileXml)
{
    XmlConfiguration doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    doc.LinkEndChild(decl);

    tinyxml2::XMLElement *pItemsElem = doc.NewElement("Items");
    doc.LinkEndChild(pItemsElem);

    this->GetItems();
    doc.SaveItems(pItemsElem, m_Config.m_Items);

    return doc.SaveFileUtf8(szFileXml);
}

bool CBatchEncoderDlg::LoadConfigFile(CString szFileXml)
{
    ::UpdatePath();

    XmlConfiguration doc;
    if (doc.LoadFileUtf8(szFileXml) == false)
        return false;

    tinyxml2::XMLElement *pRootElem = doc.FirstChildElement();
    if (!pRootElem)
        return false;

    // root: "Configuration"
    const char *szRoot = pRootElem->Value();
    const char *szRootName = "Configuration";
    if (strcmp(szRootName, szRoot) != 0)
        return false;

    // root: Options
    tinyxml2::XMLElement *pOptionsElem = pRootElem->FirstChildElement("Options");
    doc.LoadOptions(pOptionsElem, m_Config.m_Options);

    this->SetOptions();

    // root: Formats
    tinyxml2::XMLElement *pFormatsElem = pRootElem->FirstChildElement("Formats");
    doc.LoadFormats(pFormatsElem, m_Config.m_Formats);

    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();

    // root: Items
    this->OnFileClearList();

    tinyxml2::XMLElement *pItemsElem = pRootElem->FirstChildElement("Items");
    doc.LoadItems(pItemsElem, m_Config.m_Items);

    this->SetItems();
    this->UpdateStatusBar();

    return true;
}

bool CBatchEncoderDlg::SaveConfigFile(CString szFileXml)
{
    XmlConfiguration doc;
    CUtf8String szBuffUtf8;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    doc.LinkEndChild(decl);

    // root: Configuration
    tinyxml2::XMLElement *pRootElem = doc.NewElement("Configuration");
    doc.LinkEndChild(pRootElem);

    // root: Options
    tinyxml2::XMLElement *pOptionsElem = doc.NewElement("Options");
    pRootElem->LinkEndChild(pOptionsElem);

    this->GetOptions();
    doc.SaveOptions(pOptionsElem, m_Config.m_Options);

    // root: Formats
    tinyxml2::XMLElement *pFormatsElem = doc.NewElement("Formats");
    pRootElem->LinkEndChild(pFormatsElem);
    doc.SaveFormats(pFormatsElem, m_Config.m_Formats);

    // root: Items
    tinyxml2::XMLElement *pItemsElem = doc.NewElement("Items");
    pRootElem->LinkEndChild(pItemsElem);

    this->GetItems();
    doc.SaveItems(pItemsElem, m_Config.m_Items);

    ::UpdatePath();

    return doc.SaveFileUtf8(szFileXml);
}

void CBatchEncoderDlg::LoadConfig()
{
    CFileDialog fd(TRUE, _T("config"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Config Files (*.config)|*.config|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    fd.m_ofn.lpstrInitialDir = ::GetExeFilePath();

    if (fd.DoModal() == IDOK)
    {
        CString szPath = fd.GetPathName();
        try
        {
            if (this->LoadConfigFile(szPath) == false)
            {
                MessageBox(_T("Failed to load configuration!"),
                    _T("ERROR"),
                    MB_OK | MB_ICONERROR);
            }
        }
        catch (...)
        {
            MessageBox(_T("Error loading configuration file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
        }
    }
}

void CBatchEncoderDlg::SaveConfig()
{
    CFileDialog fd(FALSE, _T("config"), _T("config"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Config Files (*.config)|*.config|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    fd.m_ofn.lpstrInitialDir = ::GetExeFilePath();

    if (fd.DoModal() == IDOK)
    {
        CString szPath = fd.GetPathName();
        try
        {
            if (this->SaveConfigFile(szPath) == false)
            {
                MessageBox(_T("Failed to save configuration!"),
                    _T("ERROR"),
                    MB_OK | MB_ICONERROR);
            }
        }
        catch (...)
        {
            MessageBox(_T("Error saving configuration file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
        }
    }
}

void CBatchEncoderDlg::StartConvert()
{
    static volatile bool bSafeCheck = false;
    if (bSafeCheck == true)
        return;

    if (bRunning == false)
    {
        bSafeCheck = true;

        this->GetOptions();
        this->GetItems();

        int nItems = this->m_Config.m_Items.GetSize();
        if (nItems <= 0)
        {
            bSafeCheck = false;
            return;
        }

        if (this->m_Config.m_Options.bOutputPathChecked)
        {
            CString szPath = this->m_Config.m_Options.szOutputPath;
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

        dwThreadID = 0;
        hThread = ::CreateThread(NULL, 0, WorkThread, this, CREATE_SUSPENDED, &dwThreadID);
        if (hThread == NULL)
        {
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

        ::ResumeThread(hThread);

        bSafeCheck = false;
    }
    else
    {
        bSafeCheck = true;

        m_BtnConvert.SetWindowText(_T("Conve&rt"));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("Conve&rt\tF9"));
        this->EnableUserInterface(TRUE);

        bRunning = false;
        bSafeCheck = false;
    }
}

void CBatchEncoderDlg::FinishConvert()
{
    // restore user interface to default state
    if (this->m_Config.m_Options.bShowTrayIcon == true)
        this->EnableTrayIcon(true, true);

    this->m_BtnConvert.SetWindowText(_T("Conve&rt"));
    this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("Conve&rt\tF9"));
    this->EnableUserInterface(TRUE);

    this->m_FileProgress.SetPos(0);
    this->bRunning = false;

    // now we shutdown the system
    if (this->m_Config.m_Options.bShutdownWhenFinished == true)
    {
        // save current configuration to file
        if (this->m_Config.m_Options.bDoNotSaveConfiguration == false)
            this->SaveConfigFile(this->szMainConfigFile);

        // disable tray icon
        this->EnableTrayIcon(false);

        ::DoTheShutdown();
    }
}

void CBatchEncoderDlg::OnBnClickedButtonConvert()
{
    this->StartConvert();
}

bool CBatchEncoderDlg::WorkerCallback(int nProgress, bool bFinished, bool bError, double fTime, int nIndex)
{
    if (bError == true)
    {
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

    return bRunning;
}

BOOL CBatchEncoderDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    return FALSE;
}

void CBatchEncoderDlg::OnOK()
{

}

void CBatchEncoderDlg::OnCancel()
{

}

void CBatchEncoderDlg::OnClose()
{
    CResizeDialog::OnClose();

    // TODO: Kill worker thread and running command-line tool.

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) != MF_CHECKED)
        this->SaveConfigFile(this->szMainConfigFile);

    this->EnableTrayIcon(false);

    CResizeDialog::OnOK();
}

void CBatchEncoderDlg::OnDestroy()
{
    CResizeDialog::OnDestroy();

    m_Config.m_Items.RemoveAllNodes();
}

void CBatchEncoderDlg::UpdateFormatAndPreset()
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();

    CFormat& format = m_Config.m_Formats.GetData(nFormat);
    CPreset& preset = format.m_Presets.GetData(nPreset);

    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        int nSelected = 0;
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                CItem& item = m_Config.m_Items.GetData(i);
                item.szFormatId = format.szId;
                item.nPreset = nPreset;

                this->m_LstInputItems.SetItemText(i, 3, format.szId);

                CString szPreset;
                szPreset.Format(_T("%d"), nPreset);
                this->m_LstInputItems.SetItemText(i, 4, szPreset);

                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                CItem& item = m_Config.m_Items.GetData(i);
                item.szFormatId = format.szId;
                item.nPreset = nPreset;

                this->m_LstInputItems.SetItemText(i, 3, format.szId);

                CString szPreset;
                szPreset.Format(_T("%d"), nPreset);
                this->m_LstInputItems.SetItemText(i, 4, szPreset);
            }
        }
    }
}

void CBatchEncoderDlg::OnCbnSelchangeComboPresets()
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();

    CFormat& format = m_Config.m_Formats.GetData(nFormat);
    format.nDefaultPreset = nPreset;

    this->UpdateFormatAndPreset();
}

void CBatchEncoderDlg::OnCbnSelchangeComboFormat()
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        m_Config.m_Options.nSelectedFormat = nFormat;
        this->UpdatePresetComboBox();
    }

    this->UpdateFormatAndPreset();
}

void CBatchEncoderDlg::OnBnClickedButtonBrowsePath()
{
    LPMALLOC pMalloc;
    BROWSEINFO bi;
    LPITEMIDLIST pidlDesktop;
    LPITEMIDLIST pidlBrowse;
    TCHAR *lpBuffer;

    CString szTmp;
    this->m_EdtOutPath.GetWindowText(szTmp);

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

int CBatchEncoderDlg::InsertToMemoryList(CString szPath)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();
    CString szFormatId = m_Config.m_Formats.GetData(nFormat).szId;

    CString szExt = ::GetFileExtension(szPath);
    szExt.MakeUpper();
    if (m_Config.m_Formats.IsValidInExtension(szExt) == false)
        return -1;

    return m_Config.m_Items.InsertNode(szPath, szFormatId, nPreset);
}

void CBatchEncoderDlg::InsertToControlList(int nItem)
{
    CItem& item = m_Config.m_Items.GetData(nItem);

    CString tmpBuf;
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.iItem = nItem;

    // [Name] : file name
    lvi.pszText = (LPTSTR)(LPCTSTR)(item.szName);
    m_LstInputItems.InsertItem(&lvi);
    m_LstInputItems.SetItemData(nItem, nItem);

    // [Type] : input extension 
    tmpBuf.Format(_T("%s"), item.szExtension);
    lvi.iSubItem = 1;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 1, lvi.pszText);

    // [Size (bytes)] : file size
    tmpBuf.Format(_T("%s"), item.szSize);
    lvi.iSubItem = 2;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 2, lvi.pszText);

    // [Output] : output format
    tmpBuf.Format(_T("%s"), item.szFormatId);
    lvi.iSubItem = 3;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 3, lvi.pszText);

    // [Preset] : selected preset index
    tmpBuf.Format(_T("%d"), item.nPreset);
    lvi.iSubItem = 4;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 4, lvi.pszText);

    // [Time] : encoder/decoder conversion time
    tmpBuf.Format(_T("%s"), (item.szTime.Compare(_T("")) == 0) ? _T("--:--") : item.szTime);
    lvi.iSubItem = 5;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 5, lvi.pszText);

    // [Status] : encoder/decoder progress status
    tmpBuf.Format(_T("%s"), (item.szStatus.Compare(_T("")) == 0) ? _T("Not Done") : item.szStatus);
    lvi.iSubItem = 6;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, 6, lvi.pszText);

    // set item CheckBox state
    m_LstInputItems.SetCheck(nItem, item.bChecked);
}

bool CBatchEncoderDlg::InsertToList(CString szPath)
{
    int nItem = this->InsertToMemoryList(szPath);
    if (nItem == -1)
        return false;

    this->InsertToControlList(nItem);

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

void CBatchEncoderDlg::SearchFolderForFiles(CString szFile, const bool bRecurse)
{
    try
    {
        WIN32_FIND_DATA w32FileData;
        HANDLE hSearch = NULL;
        BOOL fFinished = FALSE;
        TCHAR cTempBuf[(MAX_PATH * 2) + 1];

        ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
        ZeroMemory(cTempBuf, MAX_PATH * 2);

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
                CString szPath;
                szPath.Format(_T("%s\\%s\0"), szFile, w32FileData.cFileName);
                this->InsertToList(szPath);
            }

            if (w32FileData.cFileName[0] != '.' &&
                w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                wsprintf(cTempBuf, _T("%s\\%s\0"), szFile, w32FileData.cFileName);
                if (bRecurse == true)
                    this->SearchFolderForFiles(cTempBuf, true);
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
                this->SearchFolderForFiles(szFile, true);
            }
            else
            {
                CString szPath = szFile;
                this->InsertToList(szPath);
            }

            szFile.ReleaseBuffer();
        }

        this->UpdateStatusBar();
    }

    ::DragFinish(hDropInfo);
}

void CBatchEncoderDlg::OnDropFiles(HDROP hDropInfo)
{
    if (bHandleDrop == true)
    {
        bHandleDrop = false;
        m_DDParam.pDlg = this;
        m_DDParam.hDropInfo = hDropInfo;
        hDDThread = ::CreateThread(NULL, 0, DragAndDropThread, (LPVOID)&m_DDParam, 0, &dwDDThreadID);
        if (hDDThread == NULL)
            bHandleDrop = true;
    }

    CResizeDialog::OnDropFiles(hDropInfo);
}

void CBatchEncoderDlg::OnNMRclickListInputFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
    POINT point;
    GetCursorPos(&point);
    CMenu *subMenu = this->GetMenu()->GetSubMenu(1);
    subMenu->TrackPopupMenu(0, point.x, point.y, this, NULL);
    *pResult = 0;
}

void CBatchEncoderDlg::OnViewShowGridLines()
{
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
        // TODO:
    }

    *pResult = 0;
}

void CBatchEncoderDlg::OnLvnItemchangedListInputFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    int nSelCount = this->m_LstInputItems.GetSelectedCount();
    if (nSelCount == 1)
    {
        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int nItem = this->m_LstInputItems.GetNextSelectedItem(pos);
            if (nItem < m_Config.m_Items.GetSize())
            {
                CItem& item = m_Config.m_Items.GetData(nItem);
                CFormat& format = m_Config.m_Formats.GetData(this->m_CmbFormat.GetCurSel());

                if (item.szFormatId.Compare(format.szId) == 0)
                {
                    format.nDefaultPreset = item.nPreset;
                    this->m_CmbPresets.SetCurSel(item.nPreset);
                }
                else
                {
                    int nFormat = m_Config.m_Formats.GetFormatById(item.szFormatId);
                    CFormat& format = m_Config.m_Formats.GetData(nFormat);

                    m_Config.m_Options.nSelectedFormat = nFormat;
                    format.nDefaultPreset = item.nPreset;

                    this->UpdateFormatComboBox();
                    this->UpdatePresetComboBox();
                }
            }
        }
    }

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
        if (m_Config.m_Options.bForceConsoleWindow == false)
        {
            this->m_ChkOutPath.ShowWindow(SW_HIDE);
            this->m_EdtOutPath.ShowWindow(SW_HIDE);
            this->m_BtnBrowse.ShowWindow(SW_HIDE);
            this->m_FileProgress.ShowWindow(SW_SHOW);
        }
    }
    else
    {
        if (m_Config.m_Options.bForceConsoleWindow == false)
        {
            this->m_FileProgress.ShowWindow(SW_HIDE);
            this->m_ChkOutPath.ShowWindow(SW_SHOW);
            this->m_EdtOutPath.ShowWindow(SW_SHOW);
            this->m_BtnBrowse.ShowWindow(SW_SHOW);
            this->m_LstInputItems.ShowWindow(SW_SHOW);
        }
    }

    CMenu* pSysMenu = GetSystemMenu(FALSE);

    if (bEnable == FALSE)
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
    else
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

    UINT nEnable = (bEnable == TRUE) ? MF_ENABLED : MF_GRAYED;
    CMenu *pMainMenu = this->GetMenu();
    UINT nItems = pMainMenu->GetMenuItemCount();
    for (UINT i = 0; i < nItems; i++)
    {
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
    // TODO:
    // CString szPath;
    // m_EdtOutPath.GetWindowText(szPath);
}

void CBatchEncoderDlg::OnEnSetFocusEditOutPath()
{
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

void CBatchEncoderDlg::OnFileLoadList()
{
    CFileDialog fd(TRUE, _T("list"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("List Items (*.list)|*.list|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();

        if (this->LoadListFile(szFileXml) == false)
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
    CFileDialog fd(FALSE, _T("list"), _T("items"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("List Items (*.list)|*.list|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();

        if (this->SaveListFile(szFileXml) == false)
        {
            MessageBox(_T("Failed to save file!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
        }
    }
}

void CBatchEncoderDlg::OnFileClearList()
{
    this->OnEditClear();
}

void CBatchEncoderDlg::OnFileExit()
{
    if (this->bRunning == false)
        this->OnClose();
}

void CBatchEncoderDlg::OnEditAddFiles()
{
    TCHAR *pFiles = NULL;
    const DWORD dwMaxSize = (4096 * MAX_PATH);
    try
    {
        pFiles = (TCHAR *)malloc(dwMaxSize);
        if (pFiles == NULL)
        {
            MessageBox(_T("Failed to allocate memory for filenames buffer!"),
                _T("ERROR"),
                MB_OK | MB_ICONERROR);
            return;
        }

        ZeroMemory(pFiles, dwMaxSize);

        // TODO: Get extensions from input formats.
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

        fd.m_ofn.lpstrFile = pFiles;
        fd.m_ofn.nMaxFile = (dwMaxSize) / 2;

        if (fd.DoModal() != IDCANCEL)
        {
            CString sFilePath;
            POSITION pos = fd.GetStartPosition();

            do
            {
                sFilePath = fd.GetNextPathName(pos);
                if (!sFilePath.IsEmpty())
                {
                    CString szPath = sFilePath;
                    this->InsertToList(szPath);
                }
            } while (pos != NULL);

            this->UpdateStatusBar();
        }
    }
    catch (...)
    {
        if (pFiles != NULL)
        {
            free(pFiles);
            pFiles = NULL;
        }
    }

    if (pFiles != NULL)
    {
        free(pFiles);
        pFiles = NULL;
    }
}

void CBatchEncoderDlg::OnEditAddDir()
{
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
    m_Config.m_Items.RemoveAllNodes();
    m_LstInputItems.DeleteAllItems();
    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditRemoveChecked()
{
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

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditRemoveUnchecked()
{
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

    this->UpdateStatusBar();
}

void CBatchEncoderDlg::OnEditCheckSelected()
{
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
    POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstInputItems.GetNextSelectedItem(pos);
        CItem& item = m_Config.m_Items.GetData(nItem);
        ::LaunchAndWait(item.szPath, _T(""), FALSE);
    }
}

void CBatchEncoderDlg::OnEditExplore()
{
    POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstInputItems.GetNextSelectedItem(pos);
        CItem& item = m_Config.m_Items.GetData(nItem);
        CString szPath = item.szPath;
        CString szName = item.szName;
        szPath.TrimRight(szName);
        ::LaunchAndWait(szPath, _T(""), FALSE);
    }
}

void CBatchEncoderDlg::OnEditCrop()
{
    // invert selection
    int nFiles = m_LstInputItems.GetItemCount();
    for (int i = 0; i < nFiles; i++)
    {
        if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            m_LstInputItems.SetItemState(i, 0, LVIS_SELECTED);
        else
            m_LstInputItems.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
    }

    // delete selected
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
    m_LstInputItems.SetItemState(-1, 0, LVIS_SELECTED);
}

void CBatchEncoderDlg::OnEditInvertSelection()
{
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
    m_LstInputItems.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
}

void CBatchEncoderDlg::OnEditResetOutput()
{
    this->ResetOutput();
}

void CBatchEncoderDlg::OnEditResetTime()
{
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
    if (this->GetMenu()->GetMenuState(ID_OPTIONS_SHOWTRAYICON, MF_BYCOMMAND) == MF_CHECKED)
        this->EnableTrayIcon(false);
    else
        this->EnableTrayIcon(true);
}

void CBatchEncoderDlg::OnOptionsLogConsoleOutput()
{
    if (this->GetMenu()->GetMenuState(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsShowLog()
{
    CFileStatus rStatus;
    if (CFile::GetStatus(m_Config.m_Options.szLogFileName, rStatus) == TRUE)
    {
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
    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsShutdownWhenFinished()
{
    if (this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsDoNotSave()
{
    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) == MF_CHECKED)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_CHECKED);
}

void CBatchEncoderDlg::OnOptionsForceConsoleWindow()
{
    if (this->GetMenu()->GetMenuState(ID_OPTIONS_FORCECONSOLEWINDOW, MF_BYCOMMAND) == MF_CHECKED)
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_UNCHECKED);
        m_Config.m_Options.bForceConsoleWindow = false;
    }
    else
    {
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FORCECONSOLEWINDOW, MF_CHECKED);
        m_Config.m_Options.bForceConsoleWindow = true;
    }
}

void CBatchEncoderDlg::OnOptionsAdvanced()
{
    CAdvancedDlg dlg;

    dlg.m_Options = m_Config.m_Options;

    if (dlg.DoModal() == IDOK)
    {
        m_Config.m_Options = dlg.m_Options;
        this->SetOptions();
    }
}

void CBatchEncoderDlg::OnOptionsConfigurePresets()
{
    CPresetsDlg dlg;
    dlg.bShowGridLines = this->GridlinesVisible();

    dlg.nSelectedFormat = this->m_CmbFormat.GetCurSel();
    dlg.m_Formats = m_Config.m_Formats;

    dlg.szPresetsDialogResize = m_Config.m_Options.szPresetsDialogResize;
    dlg.szPresetsListColumns = m_Config.m_Options.szPresetsListColumns;

    INT_PTR nRet = dlg.DoModal();
    if (nRet == IDOK)
    {
        m_Config.m_Formats.RemoveAllNodes();
        m_Config.m_Formats = dlg.m_Formats;
        this->UpdateFormatComboBox();
        this->UpdatePresetComboBox();
    }

    m_Config.m_Options.szPresetsDialogResize = dlg.szPresetsDialogResize;
    m_Config.m_Options.szPresetsListColumns = dlg.szPresetsListColumns;
}

void CBatchEncoderDlg::OnOptionsConfigureFormat()
{
    CFormatsDlg dlg;
    dlg.bShowGridLines = this->GridlinesVisible();

    dlg.m_Formats = m_Config.m_Formats;

    dlg.szFormatsDialogResize = m_Config.m_Options.szFormatsDialogResize;
    dlg.szFormatsListColumns = m_Config.m_Options.szFormatsListColumns;

    INT_PTR nRet = dlg.DoModal();
    if (nRet == IDOK)
    {
        m_Config.m_Formats.RemoveAllNodes();
        m_Config.m_Formats = dlg.m_Formats;
        this->UpdateFormatComboBox();
        this->UpdatePresetComboBox();
    }

    m_Config.m_Options.szFormatsDialogResize = dlg.szFormatsDialogResize;
    m_Config.m_Options.szFormatsListColumns = dlg.szFormatsListColumns;
}

void CBatchEncoderDlg::OnHelpWebsite()
{
    ::LaunchAndWait(MAIN_APP_WEBSITE, _T(""), FALSE);
}

void CBatchEncoderDlg::OnHelpAbout()
{
    CAboutDlg dlg;
    dlg.DoModal();
}
