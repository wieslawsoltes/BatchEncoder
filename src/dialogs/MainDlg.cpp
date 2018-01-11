// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "Strings.h"
#include "language\LanguageHelper.h"
#include "utilities\Utilities.h"
#include "xml\XmlFormats.h"
#include "xml\XmlItems.h"
#include "xml\XmlLanguages.h"
#include "xml\XmlOptions.h"
#include "xml\XmlPresets.h"
#include "xml\XmlTools.h"
#include "MainDlg.h"
#include "PresetsDlg.h"
#include "AboutDlg.h"
#include "FormatsDlg.h"
#include "ToolsDlg.h"
#include "contexts\MainDlgWorkerContext.h"

#define WM_TRAY (WM_USER + 0x10)
#define IDC_STATUSBAR 1500

#define IDC_FOLDERTREE          0x3741
#define IDC_TITLE               0x3742
#define IDC_STATUSTEXT          0x3743
#define IDC_CHECK_RECURSE       0x3744
#define IDC_BROWSE_NEW_FOLDER   0x3746

#define ID_LANGUAGE_MIN 2000
#define ID_LANGUAGE_MAX 2999

static CString szLastBrowse;
static WNDPROC lpOldWindowProc;
static bool bRecurseChecked = true;
static HWND hWndBtnRecurse = nullptr;
static HWND hWndStaticText = nullptr;

int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    if (uMsg == BFFM_INITIALIZED)
    {
        TCHAR szPath[MAX_PATH + 1] = _T("");
        wsprintf(szPath, _T("%s\0"), szLastBrowse);
        ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath);
    }
    return(0);
}

LRESULT CALLBACK BrowseDlgWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_COMMAND)
    {
        if ((HIWORD(wParam) == BN_CLICKED) && ((HWND)lParam == hWndBtnRecurse))
            bRecurseChecked = ::SendMessage(hWndBtnRecurse, BM_GETCHECK, (WPARAM)0, (LPARAM)0) == BST_CHECKED;
    }
    return ::CallWindowProc(lpOldWindowProc, hWnd, uMsg, wParam, lParam);
}

int CALLBACK BrowseCallbackAddDir(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    if (uMsg == BFFM_INITIALIZED)
    {
        CMainDlg* pDlg = (CMainDlg*)pData;

        HWND hWndTitle = nullptr;
        HFONT hFont;
        RECT rc, rcTitle, rcTree, rcWnd;

        TCHAR szPath[MAX_PATH + 1] = _T("");

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
            pDlg->m_Config.GetString(0x00210005, pszMainDialog[4]),
            BS_CHECKBOX | BS_AUTOCHECKBOX | WS_CHILD | WS_TABSTOP | WS_VISIBLE,
            rc.left, rc.top,
            rc.right - rc.left, rc.bottom - rc.top,
            hWnd,
            nullptr, nullptr, nullptr);
        if (hWndBtnRecurse != nullptr)
        {
            ::ShowWindow(hWndTitle, SW_HIDE);
            ::ShowWindow(::GetDlgItem(hWnd, IDC_STATUSTEXT), SW_HIDE);

            if (bRecurseChecked == true)
                ::SendMessage(hWndBtnRecurse, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
            else
                ::SendMessage(hWndBtnRecurse, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
            lpOldWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)BrowseDlgWindowProc);
            ::ShowWindow(hWndBtnRecurse, SW_SHOW);
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
    if (uMsg == BFFM_INITIALIZED)
    {
        CMainDlg* pDlg = (CMainDlg*)pData;

        TCHAR szText[256] = _T("");
        HWND hWndTitle = nullptr;
        HFONT hFont;
        RECT rc, rcTitle, rcTree, rcWnd;

        TCHAR szPath[MAX_PATH + 1] = _T("");
        wsprintf(szPath, _T("%s\0"), szLastBrowse);

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
            nullptr, nullptr, nullptr);
        if (hWndStaticText != nullptr)
        {
            ::ShowWindow(hWndTitle, SW_HIDE);
            ::ShowWindow(::GetDlgItem(hWnd, IDC_STATUSTEXT), SW_HIDE);

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
            lpOldWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)BrowseDlgWindowProc);
            ::ShowWindow(hWndStaticText, SW_SHOW);
#pragma warning(pop)

            hFont = (HFONT) ::SendMessage(hWnd, WM_GETFONT, 0, 0);
            ::SendMessage(hWndStaticText, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
        }

        ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath);
    }
    return(0);
}

DWORD WINAPI MainDlgDropThread(LPVOID lpParam)
{
    MainDlgDropContext* pDD = (MainDlgDropContext*)lpParam;
    pDD->pDlg->HandleDropFiles(pDD->hDrop);
    pDD->bHandled = true;
    return ::CloseHandle(pDD->hThread);
}

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
    : CMyDialogEx(CMainDlg::IDD, pParent)
{
    this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
    this->m_Config.m_Options.Defaults();
    this->pWorkerContext = new CMainDlgWorkerContext(&this->m_Config, this);
    this->pWorkerContext->bRunning = false;
}

CMainDlg::~CMainDlg()
{
    if (this->pWorkerContext != nullptr)
        delete this->pWorkerContext;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CMyDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_CONVERT, m_Progress);
    DDX_Control(pDX, IDC_STATIC_GROUP_OUTPUT, m_GrpOutput);
    DDX_Control(pDX, IDC_COMBO_PRESETS, m_CmbPresets);
    DDX_Control(pDX, IDC_COMBO_FORMAT, m_CmbFormat);
    DDX_Control(pDX, IDC_LIST_ITEMS, m_LstInputItems);
    DDX_Control(pDX, IDC_STATIC_TEXT_OUTPUT, m_StcOutPath);
    DDX_Control(pDX, IDC_COMBO_OUTPUT, m_CmbOutPath);
    DDX_Control(pDX, IDC_EDIT_THREADCOUNT, m_EdtThreads);
    DDX_Control(pDX, IDC_SPIN_THREADCOUNT, m_SpinThreads);
    DDX_Control(pDX, IDC_STATIC_TEXT_PRESET, m_StcPreset);
    DDX_Control(pDX, IDC_STATIC_TEXT_FORMAT, m_StcFormat);
    DDX_Control(pDX, IDC_STATIC_THREAD_COUNT, m_StcThreads);
    DDX_Control(pDX, IDC_BUTTON_CONVERT, m_BtnConvert);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_OUTPUT, m_BtnBrowse);
}

BEGIN_MESSAGE_MAP(CMainDlg, CMyDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_DROPFILES()
    ON_WM_HELPINFO()
    ON_MESSAGE(WM_ITEMCHANGED, OnListItemChaged)
    ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_ITEMS, OnLvnKeydownListInputItems)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_ITEMS, OnNMRclickListInputItems)
    ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_ITEMS, OnLvnItemchangingListInputItems)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ITEMS, OnLvnItemchangedListInputItems)
    ON_CBN_SELCHANGE(IDC_COMBO_PRESETS, OnCbnSelchangeComboPresets)
    ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, OnCbnSelchangeComboFormat)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_OUTPUT, OnBnClickedButtonBrowsePath)
    ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnBnClickedButtonConvert)
    ON_COMMAND(ID_FILE_LOADLIST, OnFileLoadList)
    ON_COMMAND(ID_FILE_SAVELIST, OnFileSaveList)
    ON_COMMAND(ID_FILE_CLEARLIST, OnFileClearList)
    ON_COMMAND(ID_FILE_EXIT, OnFileExit)
    ON_COMMAND(ID_EDIT_ADDFILES, OnEditAddFiles)
    ON_COMMAND(ID_EDIT_ADDDIR, OnEditAddDir)
    ON_COMMAND(ID_EDIT_RENAME, OnEditRename)
    ON_COMMAND(ID_EDIT_RESETTIME, OnEditResetTime)
    ON_COMMAND(ID_EDIT_RESETOUTPUT, OnEditResetOutput)
    ON_COMMAND(ID_EDIT_REMOVE, OnEditRemove)
    ON_COMMAND(ID_EDIT_CROP, OnEditCrop)
    ON_COMMAND(ID_EDIT_REMOVECHECKED, OnEditRemoveChecked)
    ON_COMMAND(ID_EDIT_REMOVEUNCHECKED, OnEditRemoveUnchecked)
    ON_COMMAND(ID_EDIT_CHECKSELECTED, OnEditCheckSelected)
    ON_COMMAND(ID_EDIT_UNCHECKSELECTED, OnEditUncheckSelected)
    ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
    ON_COMMAND(ID_EDIT_SELECTNONE, OnEditSelectNone)
    ON_COMMAND(ID_EDIT_INVERTSELECTION, OnEditInvertSelection)
    ON_COMMAND(ID_EDIT_OPEN, OnEditOpen)
    ON_COMMAND(ID_EDIT_EXPLORE, OnEditExplore)
    ON_COMMAND(ID_ACTION_CONVERT, OnActionConvert)
    ON_COMMAND(ID_OPTIONS_CONFIGUREPRESETS, OnOptionsConfigurePresets)
    ON_COMMAND(ID_OPTIONS_CONFIGUREFORMAT, OnOptionsConfigureFormat)
    ON_COMMAND(ID_OPTIONS_CONFIGURETOOLS, OnOptionsConfigureTools)
    ON_COMMAND(ID_OPTIONS_DELETE_SOURCE, OnOptionsDeleteSource)
    ON_COMMAND(ID_OPTIONS_SHUTDOWN_WINDOWS, OnOptionsShutdownWindows)
    ON_COMMAND(ID_OPTIONS_DO_NOT_SAVE, OnOptionsDoNotSave)
    ON_COMMAND(ID_OPTIONS_DELETE_ON_ERRORS, OnOptionsDeleteOnErrors)
    ON_COMMAND(ID_OPTIONS_STOP_ON_ERRORS, OnOptionsStopOnErrors)
    ON_COMMAND(ID_OPTIONS_HIDE_CONSOLE, OnOptionsHideConsole)
    ON_COMMAND(ID_OPTIONS_ENSURE_VISIBLE, OnOptionsEnsureVisible)
    ON_COMMAND(ID_OPTIONS_FIND_DECODER, OnOptionsFindDecoder)
    ON_COMMAND(ID_OPTIONS_VALIDATE_FILES, OnOptionsValidateFiles)
    ON_COMMAND(ID_OPTIONS_OVERWRITE_FILES, OnOptionsOverwriteFiles)
    ON_COMMAND(ID_LANGUAGE_DEFAULT, OnLanguageDefault)
    ON_COMMAND_RANGE(ID_LANGUAGE_MIN, ID_LANGUAGE_MAX, OnLanguageChange)
    ON_COMMAND(ID_HELP_WEBSITE, OnHelpWebsite)
    ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
    ON_COMMAND(ID_ACCELERATOR_F5, OnEditAddFiles)
    ON_COMMAND(ID_ACCELERATOR_F6, OnEditAddDir)
    ON_COMMAND(ID_ACCELERATOR_CTRL_L, OnFileLoadList)
    ON_COMMAND(ID_ACCELERATOR_CTRL_S, OnFileSaveList)
    ON_COMMAND(ID_ACCELERATOR_CTRL_E, OnFileClearList)
    ON_COMMAND(ID_ACCELERATOR_ALT_F4, OnFileExit)
    ON_COMMAND(ID_ACCELERATOR_F2, OnEditRename)
    ON_COMMAND(ID_ACCELERATOR_F3, OnEditResetTime)
    ON_COMMAND(ID_ACCELERATOR_F4, OnEditResetOutput)
    ON_COMMAND(ID_ACCELERATOR_CTRL_A, OnEditSelectAll)
    ON_COMMAND(ID_ACCELERATOR_CTRL_N, OnEditSelectNone)
    ON_COMMAND(ID_ACCELERATOR_CTRL_I, OnEditInvertSelection)
    ON_COMMAND(ID_ACCELERATOR_SHIFT_PLUS, OnEditCheckSelected)
    ON_COMMAND(ID_ACCELERATOR_SHIFT_MINUS, OnEditUncheckSelected)
    ON_COMMAND(ID_ACCELERATOR_CTRL_PLUS, OnEditRemoveChecked)
    ON_COMMAND(ID_ACCELERATOR_CTRL_MINUS, OnEditRemoveUnchecked)
    ON_COMMAND(ID_ACCELERATOR_F9, OnBnClickedButtonConvert)
    ON_COMMAND(ID_ACCELERATOR_F7, OnOptionsConfigurePresets)
    ON_COMMAND(ID_ACCELERATOR_F8, OnOptionsConfigureFormat)
    ON_COMMAND(ID_ACCELERATOR_F10, OnOptionsConfigureTools)
END_MESSAGE_MAP()

BOOL CMainDlg::OnInitDialog()
{
    CMyDialogEx::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // status-bar
    m_StatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, CRect(0, 0, 0, 0), this, IDC_STATUSBAR);

    int nStatusBarParts[2] = { 100, -1 };
    m_StatusBar.SetParts(2, nStatusBarParts);

    CMFCDynamicLayout* layout = this->GetDynamicLayout();
    layout->AddItem(IDC_STATUSBAR, CMFCDynamicLayout::MoveVertical(100), CMFCDynamicLayout::SizeHorizontal(100));

    // accelerators
    m_hAccel = ::LoadAccelerators(::GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_ACCELERATOR_MAIN));

    // OnNotifyFormat WM_NOTIFYFORMAT
#ifdef _UNICODE
    m_LstInputItems.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

    // dialog title
    this->SetWindowText(_T(VER_PRODUCTNAME_STR));

    // threads count spin
    m_SpinThreads.SetRange32(0, 1024);

    // progress
    m_Progress.SetRange(0, 100);
    m_Progress.SetPos(0);
    m_Progress.ShowWindow(SW_HIDE);

    // list style
    DWORD dwExStyle = m_LstInputItems.GetExtendedStyle();
    dwExStyle |= LVS_EDITLABELS | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
    m_LstInputItems.SetExtendedStyle(dwExStyle);

    // list columns
    m_LstInputItems.InsertColumn(ITEM_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 200);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_INPUT, _T("Input"), LVCFMT_LEFT, 50);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_SIZE, _T("Size (bytes)"), LVCFMT_LEFT, 80);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_OUTPUT, _T("Output"), LVCFMT_LEFT, 70);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_PRESET, _T("Preset#"), LVCFMT_LEFT, 55);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_OPTIONS, _T("Options"), LVCFMT_LEFT, 65);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_TIME, _T("Time"), LVCFMT_LEFT, 90);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_STATUS, _T("Status"), LVCFMT_LEFT, 80);

    m_StcFormat.SetBold(true);
    m_StcPreset.SetBold(true);
    m_BtnConvert.SetBold(true);

    // enable drag & drop
    this->DragAcceptFiles(TRUE);

    // output path presets
    this->m_CmbOutPath.InsertString(0, _T("$SourceDirectory$"));
    this->m_CmbOutPath.InsertString(1, _T("$SourceDirectory$\\$Name$.$Ext$"));
    this->m_CmbOutPath.InsertString(2, _T("$SourceDirectory$\\$Name$_converted.$Ext$"));
    this->m_CmbOutPath.InsertString(3, _T("$SourceDirectory$\\Converted\\$Name$.$Ext$"));
    this->m_CmbOutPath.InsertString(4, _T("C:\\Output"));
    this->m_CmbOutPath.InsertString(5, _T("C:\\Output\\$Name$.$Ext$"));
    this->m_CmbOutPath.InsertString(6, _T("C:\\Output\\$Name$_converted.$Ext$"));

    ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_OUTPUT, 15);
    this->m_CmbOutPath.SetCurSel(1);
    this->m_CmbOutPath.SetFocus();

    try
    {
        this->LoadTools(m_App.szToolsFile);
        this->LoadFormats(m_App.szFormatsFile);

        if (this->LoadOptions(m_App.szOptionsFile) == false)
        {
            this->m_Config.m_Options.Defaults();
            this->SetOptions();
            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
        }

        this->SearchFolderForLanguages(m_App.szSettingsPath);
        this->SearchFolderForLanguages(m_App.szLanguagesPath);
        this->InitLanguageMenu();
        this->SetLanguage();
        this->LoadItems(m_App.szItemsFile);
    }
    catch (...) {}

    return TRUE;
}

void CMainDlg::OnOK()
{

}

void CMainDlg::OnCancel()
{

}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_hAccel != nullptr)
    {
        if (::TranslateAccelerator(this->GetSafeHwnd(), m_hAccel, pMsg))
            return TRUE;
    }
    return CMyDialogEx::PreTranslateMessage(pMsg);
}

void CMainDlg::OnPaint()
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
        CMyDialogEx::OnPaint();
    }
}

HCURSOR CMainDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnClose()
{
    CMyDialogEx::OnClose();

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) != MF_CHECKED)
    {
        try
        {
            this->SaveTools(m_App.szToolsFile);
            this->SaveFormats(m_App.szFormatsFile);
            this->SaveOptions(m_App.szOptionsFile);
            this->SaveItems(m_App.szItemsFile);
        }
        catch (...) {}
    }

    CMyDialogEx::OnOK();
}

void CMainDlg::OnDestroy()
{
    CMyDialogEx::OnDestroy();

    m_Config.m_Items.RemoveAll();
    m_Config.m_Formats.RemoveAll();
    m_Config.m_Tools.RemoveAll();
}

void CMainDlg::OnDropFiles(HDROP hDropInfo)
{
    if (this->pWorkerContext->bRunning == false && this->m_DD.bHandled == true)
    {
        this->m_DD.bHandled = false;
        this->m_DD.pDlg = this;
        this->m_DD.hDrop = hDropInfo;
        this->m_DD.hThread = ::CreateThread(nullptr, 0, MainDlgDropThread, (LPVOID)&this->m_DD, 0, &this->m_DD.dwThreadID);
        if (this->m_DD.hThread == nullptr)
            this->m_DD.bHandled = true;
    }
    CMyDialogEx::OnDropFiles(hDropInfo);
}

BOOL CMainDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    return FALSE;
}

LRESULT CMainDlg::OnListItemChaged(WPARAM wParam, LPARAM lParam)
{
    if (this->pWorkerContext->bRunning == false)
    {
        INT nIndex = (INT)wParam;
        LPTSTR szText = (LPTSTR)lParam;
        if ((nIndex >= 0) && szText != nullptr)
        {
            CItem& item = m_Config.m_Items.Get(nIndex);
            item.szName = szText;
        }
    }
    return(0);
}

LRESULT CMainDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
{
#ifdef _UNICODE
    return NFR_UNICODE;
#else
    return NFR_ANSI;
#endif
}

void CMainDlg::OnLvnKeydownListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (this->pWorkerContext->bRunning == false)
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
    }

    *pResult = 0;
}

void CMainDlg::OnNMRclickListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (this->pWorkerContext->bRunning == false)
    {
        POINT point;
        GetCursorPos(&point);
        CMenu *subMenu = this->GetMenu()->GetSubMenu(1);
        subMenu->TrackPopupMenu(0, point.x, point.y, this, nullptr);
    }
    *pResult = 0;
}

void CMainDlg::OnLvnItemchangingListInputItems(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (this->pWorkerContext->bRunning == true)
    {
        *pResult = true;
        return;
    }
    *pResult = false;
}

void CMainDlg::OnLvnItemchangedListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (this->pWorkerContext->bRunning == false)
    {
        LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

        int nSelCount = this->m_LstInputItems.GetSelectedCount();
        if (nSelCount == 1)
        {
            POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
            if (pos != nullptr)
            {
                int nItem = this->m_LstInputItems.GetNextSelectedItem(pos);
                if (nItem < m_Config.m_Items.Count())
                {
                    CItem& item = m_Config.m_Items.Get(nItem);
                    if (m_Config.m_Formats.Count() > 0)
                    {
                        CFormat& format = m_Config.m_Formats.Get(this->m_CmbFormat.GetCurSel());
                        if (item.szFormatId.CompareNoCase(format.szId) == 0)
                        {
                            format.nDefaultPreset = item.nPreset;
                            this->m_CmbPresets.SetCurSel(item.nPreset);
                        }
                        else
                        {
                            int nFormat = m_Config.m_Formats.GetFormatById(item.szFormatId);
                            if (nFormat >= 0)
                            {
                                CFormat& format = m_Config.m_Formats.Get(nFormat);

                                m_Config.m_Options.nSelectedFormat = nFormat;
                                format.nDefaultPreset = item.nPreset;

                                this->UpdateFormatComboBox();
                                this->UpdatePresetComboBox();
                            }
                        }
                    }
                }
            }
        }
    }

    *pResult = 0;
}

void CMainDlg::OnCbnSelchangeComboPresets()
{
    if (this->pWorkerContext->bRunning == false)
    {
        int nFormat = this->m_CmbFormat.GetCurSel();
        int nPreset = this->m_CmbPresets.GetCurSel();

        CFormat& format = m_Config.m_Formats.Get(nFormat);
        format.nDefaultPreset = nPreset;

        this->UpdateFormatAndPreset();
    }
}

void CMainDlg::OnCbnSelchangeComboFormat()
{
    if (this->pWorkerContext->bRunning == false)
    {
        int nFormat = this->m_CmbFormat.GetCurSel();
        if (nFormat != -1)
        {
            m_Config.m_Options.nSelectedFormat = nFormat;
            this->UpdatePresetComboBox();
        }

        this->UpdateFormatAndPreset();
    }
}

void CMainDlg::OnBnClickedButtonBrowsePath()
{
    if (this->pWorkerContext->bRunning == false)
    {
        LPMALLOC pMalloc;
        BROWSEINFO bi;
        LPITEMIDLIST pidlDesktop;
        LPITEMIDLIST pidlBrowse;
        TCHAR *lpBuffer;

        CString szTitle = m_Config.GetString(0x00210006, pszMainDialog[5]);

        CString szTmp;
        this->m_CmbOutPath.GetWindowText(szTmp);

        szLastBrowse = szTmp;

        if (SHGetMalloc(&pMalloc) == E_FAIL)
            return;

        if ((lpBuffer = (TCHAR *)pMalloc->Alloc(MAX_PATH * 2)) == nullptr)
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
        bi.lpszTitle = szTitle;
        bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.iImage = 0;
        bi.lpfn = ::BrowseCallbackOutPath;
        bi.lParam = reinterpret_cast<LPARAM>(this);

        pidlBrowse = ::SHBrowseForFolder(&bi);
        if (pidlBrowse != nullptr)
        {
            if (::SHGetPathFromIDList(pidlBrowse, lpBuffer))
            {
                szLastBrowse.Format(_T("%s\0"), lpBuffer);

                CString szOutPath;
                szOutPath.Format(_T("%s\\%s.%s\0"), lpBuffer, VAR_OUTPUT_NAME, VAR_OUTPUT_EXTENSION);
                this->m_CmbOutPath.SetWindowText(szOutPath);
            }
            pMalloc->Free(pidlBrowse);
        }

        pMalloc->Free(pidlDesktop);
        pMalloc->Free(lpBuffer);
        pMalloc->Release();
    }
}

void CMainDlg::OnBnClickedButtonConvert()
{
    this->StartConvert();
}

void CMainDlg::OnFileLoadList()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.items)|*.items|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            m_Config.GetString(0x00310003, pszFileDialogs[2]),
            m_Config.GetString(0x00310002, pszFileDialogs[1]),
            m_Config.GetString(0x00310001, pszFileDialogs[0]));

        CFileDialog fd(TRUE, _T("items"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            CString szFileXml = fd.GetPathName();
            if (this->LoadItems(szFileXml) == false)
                m_StatusBar.SetText(m_Config.GetString(0x00210007, pszMainDialog[6]), 1, 0);
            else
                this->UpdateStatusBar();
        }
    }
}

void CMainDlg::OnFileSaveList()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.items)|*.items|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            m_Config.GetString(0x00310003, pszFileDialogs[2]),
            m_Config.GetString(0x00310002, pszFileDialogs[1]),
            m_Config.GetString(0x00310001, pszFileDialogs[0]));

        CFileDialog fd(FALSE, _T("items"), _T("items"),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            CString szFileXml = fd.GetPathName();
            if (this->SaveItems(szFileXml) == false)
                m_StatusBar.SetText(m_Config.GetString(0x00210008, pszMainDialog[7]), 1, 0);
        }
    }
}

void CMainDlg::OnFileClearList()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->pWorkerContext->bRunning == false)
        {
            m_Config.m_Items.RemoveAll();
            m_LstInputItems.DeleteAllItems();
            this->UpdateStatusBar();
        }
    }
}

void CMainDlg::OnFileExit()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->OnClose();
    }
}

void CMainDlg::OnEditAddFiles()
{
    if (this->pWorkerContext->bRunning == false)
    {
        TCHAR *pFiles = nullptr;
        const DWORD dwMaxSize = (4096 * MAX_PATH);
        try
        {
            pFiles = (TCHAR *)malloc(dwMaxSize);
            if (pFiles == nullptr)
            {
                m_StatusBar.SetText(m_Config.GetString(0x00210009, pszMainDialog[8]), 1, 0);
                return;
            }

            ZeroMemory(pFiles, dwMaxSize);

            CString szFilter;
            szFilter.Format(_T("%s (*.*)|*.*||"), m_Config.GetString(0x00310001, pszFileDialogs[0]));

            CFileDialog fd(TRUE, _T(""), _T(""),
                OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_ENABLESIZING,
                szFilter,
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
                        this->AddToList(szPath);
                    }
                } while (pos != nullptr);

                this->UpdateStatusBar();
            }
        }
        catch (...)
        {
            if (pFiles != nullptr)
            {
                free(pFiles);
                pFiles = nullptr;
            }
        }

        if (pFiles != nullptr)
        {
            free(pFiles);
            pFiles = nullptr;
        }
    }
}

void CMainDlg::OnEditAddDir()
{
    if (this->pWorkerContext->bRunning == false)
    {
        LPMALLOC pMalloc;
        BROWSEINFO bi;
        LPITEMIDLIST pidlDesktop;
        LPITEMIDLIST pidlBrowse;
        TCHAR *lpBuffer;

        CString szTitle = m_Config.GetString(0x0021000A, pszMainDialog[9]);

        if (SHGetMalloc(&pMalloc) == E_FAIL)
            return;

        if ((lpBuffer = (TCHAR *)pMalloc->Alloc(MAX_PATH * 2)) == nullptr)
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
        bi.lpszTitle = szTitle;
        bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.iImage = 0;
        bi.lpfn = ::BrowseCallbackAddDir;
        bi.lParam = reinterpret_cast<LPARAM>(this);

        pidlBrowse = ::SHBrowseForFolder(&bi);
        if (pidlBrowse != nullptr)
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
}

void CMainDlg::OnEditRename()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (m_LstInputItems.GetFocus()->GetSafeHwnd() != m_LstInputItems.GetSafeHwnd())
            return;

        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            this->m_LstInputItems.SetFocus();
            int nItem = m_LstInputItems.GetNextSelectedItem(pos);
            this->m_LstInputItems.EditLabel(nItem);
        }
    }
}

void CMainDlg::OnEditResetTime()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->m_StatusBar.SetText(_T(""), 1, 0);
        this->ResetConvertionTime();
        this->ResetConvertionStatus();
    }
}

void CMainDlg::OnEditResetOutput()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->ResetOutput();
    }
}

void CMainDlg::OnEditRemove()
{
    if (this->pWorkerContext->bRunning == false)
    {
        int nItem = -1;
        int nItemLastRemoved = -1;
        do
        {
            nItem = m_LstInputItems.GetNextItem(-1, LVIS_SELECTED);
            if (nItem != -1)
            {
                m_Config.m_Items.Remove(nItem);
                m_LstInputItems.DeleteItem(nItem);

                nItemLastRemoved = nItem;
            }
        } while (nItem != -1);

        int nItems = m_LstInputItems.GetItemCount();
        if (nItemLastRemoved != -1)
        {
            if (nItemLastRemoved < nItems && nItems >= 0)
            {
                m_LstInputItems.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
                m_LstInputItems.EnsureVisible(nItemLastRemoved, FALSE);
            }
            else if (nItemLastRemoved >= nItems && nItems >= 0)
            {
                m_LstInputItems.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstInputItems.EnsureVisible(nItemLastRemoved, FALSE);
            }
        }

        if (m_LstInputItems.GetItemCount() == 0)
        {
            m_Config.m_Items.RemoveAll();
            m_LstInputItems.DeleteAllItems();
        }

        this->UpdateStatusBar();
    }
}

void CMainDlg::OnEditCrop()
{
    if (this->pWorkerContext->bRunning == false)
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
                m_Config.m_Items.Remove(nItem);
                m_LstInputItems.DeleteItem(nItem);
            }
        } while (nItem != -1);

        if (m_LstInputItems.GetItemCount() == 0)
        {
            m_Config.m_Items.RemoveAll();
            m_LstInputItems.DeleteAllItems();
        }

        this->UpdateStatusBar();
    }
}

void CMainDlg::OnEditRemoveChecked()
{
    if (this->pWorkerContext->bRunning == false)
    {
        int nItems = m_LstInputItems.GetItemCount();
        if (nItems <= 0)
            return;

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstInputItems.GetCheck(i) == TRUE)
            {
                m_Config.m_Items.Remove(i);
                m_LstInputItems.DeleteItem(i);
            }
        }

        if (m_LstInputItems.GetItemCount() == 0)
        {
            m_Config.m_Items.RemoveAll();
            m_LstInputItems.DeleteAllItems();
        }

        this->UpdateStatusBar();
    }
}

void CMainDlg::OnEditRemoveUnchecked()
{
    if (this->pWorkerContext->bRunning == false)
    {
        int nItems = m_LstInputItems.GetItemCount();
        if (nItems <= 0)
            return;

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstInputItems.GetCheck(i) == FALSE)
            {
                m_Config.m_Items.Remove(i);
                m_LstInputItems.DeleteItem(i);
            }
        }

        if (m_LstInputItems.GetItemCount() == 0)
        {
            m_Config.m_Items.RemoveAll();
            m_LstInputItems.DeleteAllItems();
        }

        this->UpdateStatusBar();
    }
}

void CMainDlg::OnEditCheckSelected()
{
    if (this->pWorkerContext->bRunning == false)
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
}

void CMainDlg::OnEditUncheckSelected()
{
    if (this->pWorkerContext->bRunning == false)
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
}

void CMainDlg::OnEditSelectAll()
{
    if (this->pWorkerContext->bRunning == false)
    {
        m_LstInputItems.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
    }
}

void CMainDlg::OnEditSelectNone()
{
    if (this->pWorkerContext->bRunning == false)
    {
        m_LstInputItems.SetItemState(-1, 0, LVIS_SELECTED);
    }
}

void CMainDlg::OnEditInvertSelection()
{
    if (this->pWorkerContext->bRunning == false)
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
}

void CMainDlg::OnEditOpen()
{
    if (this->pWorkerContext->bRunning == false)
    {
        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstInputItems.GetNextSelectedItem(pos);
            CItem& item = m_Config.m_Items.Get(nItem);
            CPath& path = item.m_Paths.Get(0);
            ::LaunchAndWait(path.szPath, _T(""), FALSE);
        }
    }
}

void CMainDlg::OnEditExplore()
{
    if (this->pWorkerContext->bRunning == false)
    {
        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstInputItems.GetNextSelectedItem(pos);
            CItem& item = m_Config.m_Items.Get(nItem);
            CPath& path = item.m_Paths.Get(0);
            CString szPath = path.szPath;
            szPath.TrimRight(::GetFileName(path.szPath));
            ::LaunchAndWait(szPath, _T(""), FALSE);
        }
    }
}

void CMainDlg::OnActionConvert()
{
    this->StartConvert();
}

void CMainDlg::OnOptionsConfigurePresets()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CPresetsDlg dlg;
        dlg.pConfig = &m_Config;
        dlg.nSelectedFormat = this->m_CmbFormat.GetCurSel();
        dlg.m_Formats = m_Config.m_Formats;
        dlg.szPresetsDialogResize = m_Config.m_Options.szPresetsDialogResize;
        dlg.szPresetsListColumns = m_Config.m_Options.szPresetsListColumns;

        INT_PTR nRet = dlg.DoModal();
        if (nRet == IDOK)
        {
            m_Config.m_Formats.RemoveAll();
            m_Config.m_Formats = dlg.m_Formats;
            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
        }

        m_Config.m_Options.szPresetsDialogResize = dlg.szPresetsDialogResize;
        m_Config.m_Options.szPresetsListColumns = dlg.szPresetsListColumns;
    }
}

void CMainDlg::OnOptionsConfigureFormat()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CFormatsDlg dlg;
        dlg.pConfig = &m_Config;
        dlg.nSelectedFormat = this->m_CmbFormat.GetCurSel();
        dlg.m_Formats = m_Config.m_Formats;
        dlg.szFormatsDialogResize = m_Config.m_Options.szFormatsDialogResize;
        dlg.szFormatsListColumns = m_Config.m_Options.szFormatsListColumns;

        INT_PTR nRet = dlg.DoModal();
        if (nRet == IDOK)
        {
            m_Config.m_Formats.RemoveAll();
            m_Config.m_Formats = dlg.m_Formats;

            if (dlg.nSelectedFormat >= 0)
                m_Config.m_Options.nSelectedFormat = dlg.nSelectedFormat;

            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
        }

        m_Config.m_Options.szFormatsDialogResize = dlg.szFormatsDialogResize;
        m_Config.m_Options.szFormatsListColumns = dlg.szFormatsListColumns;
    }
}

void CMainDlg::OnOptionsConfigureTools()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CToolsDlg dlg;
        dlg.pConfig = &m_Config;
        dlg.nSelectedTool = 0;
        dlg.m_Tools = m_Config.m_Tools;
        dlg.m_Formats = m_Config.m_Formats;
        dlg.szToolsDialogResize = m_Config.m_Options.szToolsDialogResize;
        dlg.szToolsListColumns = m_Config.m_Options.szToolsListColumns;

        INT_PTR nRet = dlg.DoModal();
        if (nRet == IDOK)
        {
            m_Config.m_Tools.RemoveAll();
            m_Config.m_Tools = dlg.m_Tools;

            m_Config.m_Formats.RemoveAll();
            m_Config.m_Formats = dlg.m_Formats;

            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
        }

        m_Config.m_Options.szToolsDialogResize = dlg.szToolsDialogResize;
        m_Config.m_Options.szToolsListColumns = dlg.szToolsListColumns;
    }
}

void CMainDlg::OnOptionsDeleteSource()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_SOURCE, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bDeleteSourceFiles = !bChecked;
    }
}

void CMainDlg::OnOptionsShutdownWindows()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WINDOWS, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bShutdownWhenFinished = !bChecked;
    }
}

void CMainDlg::OnOptionsDoNotSave()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bDoNotSaveConfiguration = !bChecked;
    }
}

void CMainDlg::OnOptionsDeleteOnErrors()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_ON_ERRORS, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bDeleteOnErrors = !bChecked;
    }
}

void CMainDlg::OnOptionsStopOnErrors()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_STOP_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STOP_ON_ERRORS, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bStopOnErrors = !bChecked;
    }
}

void CMainDlg::OnOptionsHideConsole()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_HIDE_CONSOLE, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_HIDE_CONSOLE, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bHideConsoleWindow = !bChecked;
    }
}

void CMainDlg::OnOptionsEnsureVisible()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_ENSURE_VISIBLE, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_ENSURE_VISIBLE, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bEnsureItemIsVisible = !bChecked;
    }
}

void CMainDlg::OnOptionsFindDecoder()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_FIND_DECODER, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FIND_DECODER, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bTryToFindDecoder = !bChecked;
    }
}

void CMainDlg::OnOptionsValidateFiles()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_VALIDATE_FILES, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_VALIDATE_FILES, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bValidateInputFiles = !bChecked;
    }
}

void CMainDlg::OnOptionsOverwriteFiles()
{
    if (this->pWorkerContext->bRunning == false)
    {
        BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_OVERWRITE_FILES, MF_BYCOMMAND) == MF_CHECKED;
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_OVERWRITE_FILES, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
        this->m_Config.m_Options.bOverwriteExistingFiles = !bChecked;
    }
}

void CMainDlg::OnLanguageDefault()
{
}

void CMainDlg::OnLanguageChange(UINT nID)
{
    int nSelectedLanguage = nID - ID_LANGUAGE_MIN;
    CLanguage& language = m_Config.m_Languages.Get(nSelectedLanguage);
    m_Config.m_Options.szSelectedLanguage = language.szId;
    m_Config.pLanguage = &language;

    CMenu *m_hMenu = this->GetMenu();
    CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);

    int nLanguages = m_Config.m_Languages.Count();
    for (int i = 0; i < nLanguages; i++)
    {
        UINT nLangID = ID_LANGUAGE_MIN + i;
        if (nLangID != nID)
            m_hLangMenu->CheckMenuItem(nLangID, MF_UNCHECKED);
    }

    m_hLangMenu->CheckMenuItem(nID, MF_CHECKED);

    this->SetLanguage();
    this->UpdateStatusBar();
}

void CMainDlg::OnHelpWebsite()
{
    if (this->pWorkerContext->bRunning == false)
    {
        ::LaunchAndWait(_T("https://github.com/wieslawsoltes/BatchEncoder"), _T(""), FALSE);
    }
}

void CMainDlg::OnHelpAbout()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CAboutDlg dlg;
        dlg.pConfig = &m_Config;
        dlg.DoModal();
    }
}

bool CMainDlg::SearchFolderForLanguages(CString szFile)
{
    try
    {
        WIN32_FIND_DATA w32FileData;
        HANDLE hSearch = nullptr;
        BOOL fFinished = FALSE;
        TCHAR cTempBuf[(MAX_PATH * 2) + 1];

        ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
        ZeroMemory(cTempBuf, MAX_PATH * 2);

        szFile.TrimRight(_T("\\"));
        szFile.TrimRight(_T("/"));
        wsprintf(cTempBuf, _T("%s\\*.language\0"), szFile);

        hSearch = FindFirstFile(cTempBuf, &w32FileData);
        if (hSearch == INVALID_HANDLE_VALUE)
            return false;

        while (fFinished == FALSE)
        {
            if (w32FileData.cFileName[0] != '.' &&
                !(w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                CString szFileXml;
                szFileXml.Format(_T("%s\\%s\0"), szFile, w32FileData.cFileName);

                XmlDocumnent doc;
                XmlLanguages xmlLanguages(doc);
                if (xmlLanguages.Open(szFileXml) == true)
                {
                    CLanguage language;
                    xmlLanguages.GetLanguage(language);
                    this->m_Config.m_Languages.Insert(language);
                }
            }

            if (FindNextFile(hSearch, &w32FileData) == FALSE)
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                    fFinished = TRUE;
                else
                    return false;
            }
        }

        if (FindClose(hSearch) == FALSE)
            return false;
    }
    catch (...)
    {
        m_StatusBar.SetText(m_Config.GetString(0x0021000B, pszMainDialog[10]), 1, 0);
    }
    return true;
}

void CMainDlg::InitLanguageMenu()
{
    CMenu *m_hMenu = this->GetMenu();
    CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);

    int nLanguages = m_Config.m_Languages.Count();
    if (nLanguages > 0 && nLanguages <= (ID_LANGUAGE_MAX - ID_LANGUAGE_MIN))
    {
        m_hLangMenu->DeleteMenu(ID_LANGUAGE_DEFAULT, 0);

        for (int i = 0; i < nLanguages; i++)
        {
            CLanguage language = m_Config.m_Languages.Get(i);

            CString szBuff;
            szBuff.Format(_T("%s (%s)"), language.szOriginalName, language.szTranslatedName);

            UINT nLangID = ID_LANGUAGE_MIN + i;
            m_hLangMenu->AppendMenu(MF_STRING, nLangID, szBuff);
            m_hLangMenu->CheckMenuItem(nLangID, MF_UNCHECKED);
        }

        int nSelectedLanguage = m_Config.m_Languages.GetLanguageById(m_Config.m_Options.szSelectedLanguage);
        if (nSelectedLanguage >= 0)
        {
            m_hLangMenu->CheckMenuItem(ID_LANGUAGE_MIN + nSelectedLanguage, MF_CHECKED);

            CLanguage& language = m_Config.m_Languages.Get(nSelectedLanguage);
            m_Config.pLanguage = &language;
        }
        else
        {
            m_hLangMenu->CheckMenuItem(ID_LANGUAGE_MIN, MF_CHECKED);

            CLanguage& language = m_Config.m_Languages.Get(0);
            m_Config.m_Options.szSelectedLanguage = language.szId;
            m_Config.pLanguage = &language;
        }
    }
    else
    {
        m_hLangMenu->CheckMenuItem(ID_LANGUAGE_DEFAULT, MF_CHECKED);
        m_hLangMenu->EnableMenuItem(ID_LANGUAGE_DEFAULT, MF_DISABLED);
    }
}

void CMainDlg::SetLanguage()
{
    CLanguageHelper helper(&m_Config);
    CMenu *m_hMenu = this->GetMenu();

    // File Menu
    helper.SetMenuPopupText(m_hMenu, 0, 0x00010001);
    helper.SetMenuItemText(m_hMenu, ID_FILE_LOADLIST, 0x00010002);
    helper.SetMenuItemText(m_hMenu, ID_FILE_SAVELIST, 0x00010003);
    helper.SetMenuItemText(m_hMenu, ID_FILE_CLEARLIST, 0x00010004);
    helper.SetMenuItemText(m_hMenu, ID_FILE_EXIT, 0x00010005);

    // Edit Menu
    helper.SetMenuPopupText(m_hMenu, 1, 0x00020001);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_ADDFILES, 0x00020002);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_ADDDIR, 0x00020003);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_RENAME, 0x00020004);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_RESETTIME, 0x00020005);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_RESETOUTPUT, 0x00020006);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_REMOVE, 0x00020007);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_CROP, 0x00020008);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_REMOVECHECKED, 0x00020009);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_REMOVEUNCHECKED, 0x0002000A);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_CHECKSELECTED, 0x0002000B);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_UNCHECKSELECTED, 0x0002000C);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_SELECTALL, 0x0002000D);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_SELECTNONE, 0x0002000E);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_INVERTSELECTION, 0x0002000F);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_OPEN, 0x00020010);
    helper.SetMenuItemText(m_hMenu, ID_EDIT_EXPLORE, 0x00020011);

    // Action Menu
    helper.SetMenuPopupText(m_hMenu, 2, 0x00030001);
    helper.SetMenuItemText(m_hMenu, ID_ACTION_CONVERT, 0x00030002);

    // Options Menu
    helper.SetMenuPopupText(m_hMenu, 3, 0x00040001);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_CONFIGUREPRESETS, 0x00040002);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_CONFIGUREFORMAT, 0x00040003);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_DELETE_SOURCE, 0x00040004);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_SHUTDOWN_WINDOWS, 0x00040005);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_DO_NOT_SAVE, 0x00040006);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_DELETE_ON_ERRORS, 0x00040007);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_STOP_ON_ERRORS, 0x00040008);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_HIDE_CONSOLE, 0x00040009);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_ENSURE_VISIBLE, 0x0004000A);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_FIND_DECODER, 0x0004000B);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_VALIDATE_FILES, 0x0004000C);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_OVERWRITE_FILES, 0x0004000D);
    helper.SetMenuItemText(m_hMenu, ID_OPTIONS_CONFIGURETOOLS, 0x0004000E);

    // Language Menu
    helper.SetMenuPopupText(m_hMenu, 4, 0x00050001);
    helper.SetMenuItemText(m_hMenu, ID_LANGUAGE_DEFAULT, 0x00050002);

    // Help Menu
    helper.SetMenuPopupText(m_hMenu, 5, 0x00060001);
    helper.SetMenuItemText(m_hMenu, ID_HELP_WEBSITE, 0x00060002);
    helper.SetMenuItemText(m_hMenu, ID_HELP_ABOUT, 0x00060003);

    this->DrawMenuBar();

    // Main Dialog
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_NAME, 0x000A0001);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_INPUT, 0x000A0002);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_SIZE, 0x000A0003);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_OUTPUT, 0x000A0004);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_PRESET, 0x000A0005);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_OPTIONS, 0x000B0002);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_TIME, 0x000A0006);
    helper.SetColumnText(m_LstInputItems, ITEM_COLUMN_STATUS, 0x000A0007);

    helper.SetWndText(this, 0x000A0010);
    helper.SetWndText(&m_GrpOutput, 0x000A0011);
    helper.SetWndText(&m_StcFormat, 0x000A0012);
    helper.SetWndText(&m_StcPreset, 0x000A0013);
    helper.SetWndText(&m_StcOutPath, 0x000A0014);
    helper.SetWndText(&m_BtnBrowse, 0x000A0015);
    helper.SetWndText(&m_StcThreads, 0x000A0016);
    helper.SetWndText(&m_BtnConvert, 0x000A0017);
}

void CMainDlg::LoadLanguage(CString szFileXml)
{
    XmlDocumnent doc;
    XmlLanguages xmlLanguages(doc);
    if (xmlLanguages.Open(szFileXml) == true)
    {
        // insert to languages list
        CLanguage language;
        xmlLanguages.GetLanguage(language);

        this->m_Config.m_Languages.Insert(language);

        // insert to languages menu
        CMenu *m_hMenu = this->GetMenu();
        CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);
        int nLanguages = m_Config.m_Languages.Count();

        CString szBuff;
        szBuff.Format(_T("%s (%s)"), language.szOriginalName, language.szTranslatedName);

        UINT nLangID = ID_LANGUAGE_MIN + nLanguages - 1;
        m_hLangMenu->AppendMenu(MF_STRING, nLangID, szBuff);
        m_hLangMenu->CheckMenuItem(nLangID, MF_UNCHECKED);
    }
}

void CMainDlg::GetItems()
{
    int nItems = this->m_LstInputItems.GetItemCount();
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = m_Config.m_Items.Get(i);
        item.nId = i;
        item.bChecked = this->m_LstInputItems.GetCheck(i) == TRUE;
        item.szOptions = this->m_LstInputItems.GetItemText(i, ITEM_COLUMN_OPTIONS);
        item.szTime = this->m_LstInputItems.GetItemText(i, ITEM_COLUMN_TIME);
        item.szStatus = this->m_LstInputItems.GetItemText(i, ITEM_COLUMN_STATUS);
    }
}

void CMainDlg::SetItems()
{
    int nItems = m_Config.m_Items.Count();
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = m_Config.m_Items.Get(i);
        this->AddToList(item, i);
    }
}

void CMainDlg::GetOptions()
{
    // option: SelectedFormat
    m_Config.m_Options.nSelectedFormat = this->m_CmbFormat.GetCurSel();

    // option: OutputPath
    m_CmbOutPath.GetWindowText(m_Config.m_Options.szOutputPath);

    // option: DeleteSourceFiles
    m_Config.m_Options.bDeleteSourceFiles = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_SOURCE, MF_BYCOMMAND) == MF_CHECKED;

    // option: RecurseChecked
    m_Config.m_Options.bRecurseChecked = ::bRecurseChecked;

    // option: ShutdownWhenFinished
    m_Config.m_Options.bShutdownWhenFinished = this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_BYCOMMAND) == MF_CHECKED;

    // option: DoNotSaveConfiguration
    m_Config.m_Options.bDoNotSaveConfiguration = this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) == MF_CHECKED;

    // option: DeleteOnErrors
    m_Config.m_Options.bDeleteOnErrors = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED;

    // option: StopOnErrors
    m_Config.m_Options.bStopOnErrors = this->GetMenu()->GetMenuState(ID_OPTIONS_STOP_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED;

    // option: HideConsoleWindow
    m_Config.m_Options.bHideConsoleWindow = this->GetMenu()->GetMenuState(ID_OPTIONS_HIDE_CONSOLE, MF_BYCOMMAND) == MF_CHECKED;

    // option: EnsureItemIsVisible
    m_Config.m_Options.bEnsureItemIsVisible = this->GetMenu()->GetMenuState(ID_OPTIONS_ENSURE_VISIBLE, MF_BYCOMMAND) == MF_CHECKED;

    // option: TryToFindDecoder
    m_Config.m_Options.bTryToFindDecoder = this->GetMenu()->GetMenuState(ID_OPTIONS_FIND_DECODER, MF_BYCOMMAND) == MF_CHECKED;

    // option: ValidateInputFiles
    m_Config.m_Options.bValidateInputFiles = this->GetMenu()->GetMenuState(ID_OPTIONS_VALIDATE_FILES, MF_BYCOMMAND) == MF_CHECKED;

    // option: OverwriteExistingFiles
    m_Config.m_Options.bOverwriteExistingFiles = this->GetMenu()->GetMenuState(ID_OPTIONS_OVERWRITE_FILES, MF_BYCOMMAND) == MF_CHECKED;

    // option: ThreadCount
    CString szThreadCount;
    m_EdtThreads.GetWindowText(szThreadCount);
    m_Config.m_Options.nThreadCount = _tstoi(szThreadCount);

    // option: MainWindowResize
    m_Config.m_Options.szMainWindowResize = this->GetWindowRectStr();

    // option: FileListColumns
    int nColWidth[8];
    for (int i = 0; i < 8; i++)
        nColWidth[i] = m_LstInputItems.GetColumnWidth(i);
    m_Config.m_Options.szFileListColumns.Format(_T("%d %d %d %d %d %d %d %d"),
        nColWidth[0],
        nColWidth[1],
        nColWidth[2],
        nColWidth[3],
        nColWidth[4],
        nColWidth[5],
        nColWidth[6],
        nColWidth[7]);
}

void CMainDlg::SetOptions()
{
    // option: SelectedFormat

    // option: OutputPath
    if (m_Config.m_Options.szOutputPath.CompareNoCase(_T("")) != 0)
    {
        this->m_CmbOutPath.SetWindowText(m_Config.m_Options.szOutputPath);
        szLastBrowse = m_Config.m_Options.szOutputPath;
    }
    else
    {
        m_Config.m_Options.szOutputPath = m_App.szSettingsPath;
        szLastBrowse = m_Config.m_Options.szOutputPath;
        this->m_CmbOutPath.SetWindowText(m_Config.m_Options.szOutputPath);
    }

    // option: DeleteSourceFiles
    if (m_Config.m_Options.bDeleteSourceFiles)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, MF_UNCHECKED);

    // option: RecurseChecked
    if (m_Config.m_Options.bRecurseChecked)
        ::bRecurseChecked = true;
    else
        ::bRecurseChecked = false;

    // option: ShutdownWhenFinished
    if (m_Config.m_Options.bShutdownWhenFinished)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_UNCHECKED);

    // option: DoNotSaveConfiguration
    if (m_Config.m_Options.bDoNotSaveConfiguration)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);

    // option: DeleteOnErrors
    if (m_Config.m_Options.bDeleteOnErrors)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_ON_ERRORS, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_ON_ERRORS, MF_UNCHECKED);

    // option: StopOnErrors
    if (m_Config.m_Options.bStopOnErrors)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STOP_ON_ERRORS, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_STOP_ON_ERRORS, MF_UNCHECKED);

    // option: HideConsoleWindow
    if (m_Config.m_Options.bHideConsoleWindow)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_HIDE_CONSOLE, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_HIDE_CONSOLE, MF_UNCHECKED);

    // option: EnsureItemIsVisible
    if (m_Config.m_Options.bEnsureItemIsVisible)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_ENSURE_VISIBLE, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_ENSURE_VISIBLE, MF_UNCHECKED);

    // option: TryToFindDecoder
    if (m_Config.m_Options.bTryToFindDecoder)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FIND_DECODER, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_FIND_DECODER, MF_UNCHECKED);

    // option: ValidateInputFiles
    if (m_Config.m_Options.bValidateInputFiles)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_VALIDATE_FILES, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_VALIDATE_FILES, MF_UNCHECKED);

    // option: OverwriteExistingFiles
    if (m_Config.m_Options.bOverwriteExistingFiles)
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_OVERWRITE_FILES, MF_CHECKED);
    else
        this->GetMenu()->CheckMenuItem(ID_OPTIONS_OVERWRITE_FILES, MF_UNCHECKED);

    // option: ThreadCount
    CString szThreadCount;
    szThreadCount.Format(_T("%d\0"), m_Config.m_Options.nThreadCount);
    m_EdtThreads.SetWindowText(szThreadCount);

    // option: MainWindowResize
    if (m_Config.m_Options.szMainWindowResize.CompareNoCase(_T("")) != 0)
    {
        this->SetWindowRectStr(m_Config.m_Options.szMainWindowResize);
    }

    // option: FileListColumns
    if (m_Config.m_Options.szFileListColumns.CompareNoCase(_T("")) != 0)
    {
        int nColWidth[8];
        if (_stscanf(m_Config.m_Options.szFileListColumns, _T("%d %d %d %d %d %d %d %d"),
            &nColWidth[0],
            &nColWidth[1],
            &nColWidth[2],
            &nColWidth[3],
            &nColWidth[4],
            &nColWidth[5],
            &nColWidth[6],
            &nColWidth[7]) == 8)
        {
            for (int i = 0; i < 8; i++)
                m_LstInputItems.SetColumnWidth(i, nColWidth[i]);
        }
    }

    // option: PresetsDialogResize

    // option: PresetsListColumns

    // option: FormatsDialogResize

    // option: FormatsListColumns

    // option: ToolsDialogResize

    // option: ToolsListColumns
}

bool CMainDlg::LoadOptions(CString szFileXml)
{
    XmlDocumnent doc;
    XmlOptions xmlOptions(doc);
    if (xmlOptions.Open(szFileXml) == false)
        return false;

    xmlOptions.GetOptions(this->m_Config.m_Options);

    this->SetOptions();
    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();

    return true;
}

bool CMainDlg::SaveOptions(CString szFileXml)
{
    this->GetOptions();

    XmlDocumnent doc;
    XmlOptions xmlOptions(doc);
    xmlOptions.Create();
    xmlOptions.SetOptions(this->m_Config.m_Options);
    return xmlOptions.Save(szFileXml);
}

bool CMainDlg::LoadFormats(CString szFileXml)
{
    XmlDocumnent doc;
    XmlFormats xmlFormats(doc);
    if (xmlFormats.Open(szFileXml) == false)
        return false;

    this->m_Config.m_Formats.RemoveAll();

    xmlFormats.GetFormats(this->m_Config.m_Formats);

    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();

    return true;
}

bool CMainDlg::SaveFormats(CString szFileXml)
{
    XmlDocumnent doc;
    XmlFormats xmlFormats(doc);
    xmlFormats.Create();
    xmlFormats.SetFormats(this->m_Config.m_Formats);
    return xmlFormats.Save(szFileXml);
}

bool CMainDlg::LoadFormat(CString szFileXml)
{
    XmlDocumnent doc;
    XmlFormats xmlFormats(doc);
    if (xmlFormats.Open(szFileXml) == false)
        return false;

    CFormat format;
    xmlFormats.GetFormat(format);
    m_Config.m_Formats.Insert(format);

    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();
    return true;
}

bool CMainDlg::SaveFormat(CString szFileXml)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);

        XmlDocumnent doc;
        XmlFormats xmlFormats(doc);
        xmlFormats.Create();
        xmlFormats.SetFormat(format);
        return xmlFormats.Save(szFileXml);
    }
    return false;
}

bool CMainDlg::LoadPresets(CString szFileXml)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);

        XmlDocumnent doc;
        XmlPresets xmlPresets(doc);
        if (xmlPresets.Open(szFileXml) == false)
            return false;

        format.m_Presets.RemoveAll();
        xmlPresets.GetPresets(format.m_Presets);

        this->UpdatePresetComboBox();

        return true;
    }
    return false;
}

bool CMainDlg::SavePresets(CString szFileXml)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);

        XmlDocumnent doc;
        XmlPresets xmlPresets(doc);
        xmlPresets.Create();
        xmlPresets.SetPresets(format.m_Presets);
        return xmlPresets.Save(szFileXml);
    }
    return false;
}

bool CMainDlg::LoadTools(CString szFileXml)
{
    XmlDocumnent doc;
    XmlTools xmlTools(doc);
    if (xmlTools.Open(szFileXml) == false)
        return false;

    this->m_Config.m_Tools.RemoveAll();

    xmlTools.GetTools(this->m_Config.m_Tools);

    return true;
}

bool CMainDlg::SaveTools(CString szFileXml)
{
    XmlDocumnent doc;
    XmlTools xmlTools(doc);
    xmlTools.Create();
    xmlTools.SetTools(this->m_Config.m_Tools);
    return xmlTools.Save(szFileXml);
}

bool CMainDlg::LoadTool(CString szFileXml)
{
    XmlDocumnent doc;
    XmlTools xmlTools(doc);
    if (xmlTools.Open(szFileXml) == false)
        return false;

    CTool tool;
    xmlTools.GetTool(tool);
    m_Config.m_Tools.Insert(tool);

    return true;
}

bool CMainDlg::LoadItems(CString szFileXml)
{
    XmlDocumnent doc;
    XmlItems xmlItems(doc);
    if (xmlItems.Open(szFileXml) == false)
        return false;

    this->m_LstInputItems.DeleteAllItems();
    this->m_Config.m_Items.RemoveAll();

    xmlItems.GetItems(this->m_Config.m_Items);

    this->SetItems();
    this->UpdateStatusBar();

    return true;
}

bool CMainDlg::SaveItems(CString szFileXml)
{
    this->GetItems();

    XmlDocumnent doc;
    XmlItems xmlItems(doc);
    xmlItems.Create();
    xmlItems.SetItems(this->m_Config.m_Items);
    return xmlItems.Save(szFileXml);
}

int CMainDlg::AddToItems(CString szPath)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();

    CString szFormatId = _T("");
    if (m_Config.m_Formats.Count() > 0)
    {
        if (m_Config.m_Options.bTryToFindDecoder == true)
        {
            int nDecoder = m_Config.m_Formats.GetDecoderByExtension(::GetFileExtension(szPath));
            if (nDecoder == -1)
            {
                CFormat &format = m_Config.m_Formats.Get(nFormat);
                szFormatId = format.szId;
            }
            else
            {
                CFormat &format = m_Config.m_Formats.Get(nDecoder);
                szFormatId = format.szId;
                nPreset = format.nDefaultPreset;
            }
        }
        else
        {
            CFormat &format = m_Config.m_Formats.Get(nFormat);
            szFormatId = format.szId;
        }
    }

    WIN32_FIND_DATA w32FileData;
    HANDLE hFind;
    ULARGE_INTEGER ulSize;
    ULONGLONG nFileSize;

    ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
    hFind = ::FindFirstFile(szPath, &w32FileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return -1;

    ::FindClose(hFind);

    ulSize.HighPart = w32FileData.nFileSizeHigh;
    ulSize.LowPart = w32FileData.nFileSizeLow;
    nFileSize = ulSize.QuadPart;

    CString szFileSize;
    szFileSize.Format(_T("%I64d"), nFileSize);

    CItem item;
    CPath path;
    path.szPath = szPath;
    path.szSize = szFileSize;
    item.m_Paths.Insert(path);
    item.szSize = szFileSize;
    item.szName = ::GetOnlyFileName(szPath);
    item.szExtension = ::GetFileExtension(szPath).MakeUpper();
    item.szFormatId = szFormatId;
    item.nPreset = nPreset;
    item.bChecked = true;

    m_Config.m_Items.Insert(item);

    return (int)m_Config.m_Items.Count() - 1;
}

void CMainDlg::AddToList(CItem &item, int nItem)
{
    CString tmpBuf;
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.iItem = nItem;

    // [Name] : item name
    lvi.pszText = (LPTSTR)(LPCTSTR)(item.szName);
    m_LstInputItems.InsertItem(&lvi);
    m_LstInputItems.SetItemData(nItem, nItem);

    // [Type] : input extension 
    tmpBuf.Format(_T("%s"), item.szExtension);
    lvi.iSubItem = ITEM_COLUMN_INPUT;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_INPUT, lvi.pszText);

    // [Size (bytes)] : file size
    tmpBuf.Format(_T("%s"), item.szSize);
    lvi.iSubItem = ITEM_COLUMN_SIZE;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_SIZE, lvi.pszText);

    // [Output] : output format
    tmpBuf.Format(_T("%s"), item.szFormatId);
    lvi.iSubItem = ITEM_COLUMN_OUTPUT;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_OUTPUT, lvi.pszText);

    // [Preset] : selected preset index
    tmpBuf.Format(_T("%d"), item.nPreset);
    lvi.iSubItem = ITEM_COLUMN_PRESET;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_PRESET, lvi.pszText);

    // [Options] : additional options
    tmpBuf.Format(_T("%s"), item.szOptions);
    lvi.iSubItem = ITEM_COLUMN_OPTIONS;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_OPTIONS, lvi.pszText);

    // [Time] : encoder/decoder conversion time
    tmpBuf.Format(_T("%s"), (item.szTime.CompareNoCase(_T("")) == 0) ? pszDefaulTime : item.szTime);
    lvi.iSubItem = ITEM_COLUMN_TIME;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_TIME, lvi.pszText);

    // [Status] : encoder/decoder progress status
    tmpBuf.Format(_T("%s"), (item.szStatus.CompareNoCase(_T("")) == 0) ? m_Config.GetString(0x00210001, pszMainDialog[0]) : item.szStatus);
    lvi.iSubItem = ITEM_COLUMN_STATUS;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_STATUS, lvi.pszText);

    // item CheckBox state
    m_LstInputItems.SetCheck(nItem, item.bChecked);
}

bool CMainDlg::AddToList(CString szPath)
{
    if (m_Config.m_Options.bValidateInputFiles == true)
    {
        CString szExt = ::GetFileExtension(szPath);
        if (m_Config.m_Formats.IsValidInputExtension(szExt) == false)
            return false;
    }

    int nItem = this->AddToItems(szPath);
    if (nItem == -1)
        return false;

    CItem& item = m_Config.m_Items.Get(nItem);
    this->AddToList(item, nItem);

    return true;
}

void CMainDlg::HandleDropFiles(HDROP hDropInfo)
{
    int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, nullptr, 0);
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            int nReqChars = ::DragQueryFile(hDropInfo, i, nullptr, 0);

            CString szFile;
            ::DragQueryFile(hDropInfo, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
            if (::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
            {
                this->SearchFolderForFiles(szFile, true);
            }
            else
            {
                CString szPath = szFile;
                CString szExt = ::GetFileExtension(szPath);

                if (szExt.CompareNoCase(_T("items")) == 0)
                {
                    this->LoadItems(szPath);
                }
                else if (szExt.CompareNoCase(_T("formats")) == 0)
                {
                    this->LoadFormats(szPath);
                }
                else if (szExt.CompareNoCase(_T("format")) == 0)
                {
                    this->LoadFormat(szPath);
                }
                else if (szExt.CompareNoCase(_T("presets")) == 0)
                {
                    this->LoadPresets(szPath);
                }
                else if (szExt.CompareNoCase(_T("options")) == 0)
                {
                    this->LoadOptions(szPath);
                }
                else if (szExt.CompareNoCase(_T("tools")) == 0)
                {
                    this->LoadTools(szPath);
                }
                else if (szExt.CompareNoCase(_T("tool")) == 0)
                {
                    this->LoadTool(szPath);
                }
                else if (szExt.CompareNoCase(_T("exe")) == 0)
                {
                    // Set current format exe path.
                    int nFormat = this->m_CmbFormat.GetCurSel();
                    if (nFormat != -1)
                    {
                        CFormat& format = m_Config.m_Formats.Get(nFormat);
                        format.szPath = szPath;
                    }
                }
                else if (szExt.CompareNoCase(_T("progress")) == 0)
                {
                    // Set current format progress path.
                    int nFormat = this->m_CmbFormat.GetCurSel();
                    if (nFormat != -1)
                    {
                        CFormat& format = m_Config.m_Formats.Get(nFormat);
                        format.szFunction = szPath;
                    }
                }
                else if (szExt.CompareNoCase(_T("language")) == 0)
                {
                    LoadLanguage(szPath);
                }
                else
                {
                    this->AddToList(szPath);
                }
            }

            szFile.ReleaseBuffer();
        }

        this->UpdateStatusBar();
    }

    ::DragFinish(hDropInfo);
}

void CMainDlg::SearchFolderForFiles(CString szFile, const bool bRecurse)
{
    try
    {
        WIN32_FIND_DATA w32FileData;
        HANDLE hSearch = nullptr;
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
                this->AddToList(szPath);
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
        m_StatusBar.SetText(m_Config.GetString(0x0021000C, pszMainDialog[11]), 1, 0);
    }
}

void CMainDlg::UpdateFormatComboBox()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->m_CmbFormat.ResetContent();

        int nFormats = m_Config.m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = m_Config.m_Formats.Get(i);
            this->m_CmbFormat.InsertString(i, format.szName);
        }

        static bool bResizeFormatsComboBox = false;
        if (bResizeFormatsComboBox == false)
        {
            ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_FORMAT, 15);
            bResizeFormatsComboBox = true;
        }

        this->m_CmbFormat.SetCurSel(m_Config.m_Options.nSelectedFormat);
    }
}

void CMainDlg::UpdatePresetComboBox()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->m_CmbPresets.ResetContent();

        int nPreset = -1;

        if (m_Config.m_Options.nSelectedFormat > m_Config.m_Formats.Count() - 1)
            m_Config.m_Options.nSelectedFormat = 0;

        if (m_Config.m_Options.nSelectedFormat >= 0 && m_Config.m_Formats.Count() > 0)
        {
            CFormat& format = m_Config.m_Formats.Get(m_Config.m_Options.nSelectedFormat);
            int nPresets = format.m_Presets.Count();
            for (int i = 0; i < nPresets; i++)
            {
                CPreset& preset = format.m_Presets.Get(i);
                this->m_CmbPresets.InsertString(i, preset.szName);
            }

            nPreset = format.nDefaultPreset;
        }

        static bool bResizePresetsComboBox = false;
        if (bResizePresetsComboBox == false)
        {
            ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRESETS, 15);
            bResizePresetsComboBox = true;
        }

        this->m_CmbPresets.SetCurSel(nPreset);
    }
}

void CMainDlg::UpdateFormatAndPreset()
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();
    if ((nFormat >= 0) && (nPreset >= 0))
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);
        CPreset& preset = format.m_Presets.Get(nPreset);
        int nCount = m_LstInputItems.GetItemCount();
        if (nCount > 0)
        {
            int nSelected = 0;
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                {
                    CItem& item = m_Config.m_Items.Get(i);
                    item.szFormatId = format.szId;
                    item.nPreset = nPreset;

                    this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_OUTPUT, format.szId);

                    CString szPreset;
                    szPreset.Format(_T("%d"), nPreset);
                    this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_PRESET, szPreset);

                    nSelected++;
                }
            }

            if (nSelected == 0)
            {
                for (int i = 0; i < nCount; i++)
                {
                    CItem& item = m_Config.m_Items.Get(i);
                    item.szFormatId = format.szId;
                    item.nPreset = nPreset;

                    this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_OUTPUT, format.szId);

                    CString szPreset;
                    szPreset.Format(_T("%d"), nPreset);
                    this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_PRESET, szPreset);
                }
            }
        }
    }
}

void CMainDlg::ResetOutput()
{
    this->UpdateFormatAndPreset();
}

void CMainDlg::ResetConvertionTime()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        int nSelected = 0;
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_TIME, pszDefaulTime);
                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_TIME, pszDefaulTime);
        }
    }
}

void CMainDlg::ResetConvertionStatus()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        int nSelected = 0;
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstInputItems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_STATUS, m_Config.GetString(0x00210001, pszMainDialog[0]));
                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_STATUS, m_Config.GetString(0x00210001, pszMainDialog[0]));
        }
    }
}

void CMainDlg::UpdateStatusBar()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        CString szText;
        szText.Format(_T("%d %s"),
            nCount,
            (nCount > 1) ?
            m_Config.GetString(0x00210003, pszMainDialog[2]) :
            m_Config.GetString(0x00210002, pszMainDialog[1]));
        m_StatusBar.SetText(szText, 0, 0);
    }
    else
    {
        m_StatusBar.SetText(m_Config.GetString(0x00210004, pszMainDialog[3]), 0, 0);
        m_StatusBar.SetText(_T(""), 1, 0);
    }
}

void CMainDlg::EnableUserInterface(BOOL bEnable)
{
    if (bEnable == FALSE)
    {
        this->m_StcOutPath.ShowWindow(SW_HIDE);
        this->m_CmbOutPath.ShowWindow(SW_HIDE);
        this->m_BtnBrowse.ShowWindow(SW_HIDE);
        this->m_StcThreads.ShowWindow(SW_HIDE);
        this->m_EdtThreads.ShowWindow(SW_HIDE);
        this->m_SpinThreads.ShowWindow(SW_HIDE);
        this->m_Progress.ShowWindow(SW_SHOW);
    }
    else
    {
        this->m_Progress.ShowWindow(SW_HIDE);
        this->m_StcOutPath.ShowWindow(SW_SHOW);
        this->m_CmbOutPath.ShowWindow(SW_SHOW);
        this->m_BtnBrowse.ShowWindow(SW_SHOW);
        this->m_StcThreads.ShowWindow(SW_SHOW);
        this->m_EdtThreads.ShowWindow(SW_SHOW);
        this->m_SpinThreads.ShowWindow(SW_SHOW);
        this->m_LstInputItems.ShowWindow(SW_SHOW);
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

    this->m_CmbOutPath.EnableWindow(bEnable);
    this->m_BtnBrowse.EnableWindow(bEnable);
}

void CMainDlg::StartConvert()
{
    static volatile bool bSafeCheck = false;
    if (bSafeCheck == true)
        return;

    if (this->pWorkerContext->bRunning == false && this->pWorkerContext->bDone == true)
    {
        bSafeCheck = true;
        this->pWorkerContext->bDone = false;

        m_StatusBar.SetText(_T(""), 1, 0);

        ::SetCurrentDirectory(m_App.szSettingsPath);

        this->GetOptions();
        this->GetItems();

        this->pWorkerContext->pConfig = &this->m_Config;

        int nItems = this->m_Config.m_Items.Count();
        int nChecked = 0;

        if (nItems <= 0)
        {
            bSafeCheck = false;
            this->pWorkerContext->bDone = true;
            return;
        }

        for (int i = 0; i < nItems; i++)
        {
            CItem& item = this->m_Config.m_Items.Get(i);
            if (item.bChecked == true)
            {
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_TIME, pszDefaulTime);
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_STATUS, m_Config.GetString(0x00210001, pszMainDialog[0]));
                nChecked++;
            }
        }

        if (nChecked <= 0)
        {
            bSafeCheck = false;
            this->pWorkerContext->bDone = true;
            return;
        }

        CString szOutput = this->m_Config.m_Options.szOutputPath;
        if (this->pWorkerContext->m_Output.Validate(szOutput) == false)
        {
            m_StatusBar.SetText(m_Config.GetString(0x0021000F, pszMainDialog[14]), 1, 0);
            bSafeCheck = false;
            this->pWorkerContext->bDone = true;
            return;
        }

        if (this->pWorkerContext->m_Thread.Start([this]() { this->m_Worker.Convert(this->pWorkerContext); }, true) == false)
        {
            m_StatusBar.SetText(m_Config.GetString(0x0021000E, pszMainDialog[13]), 1, 0);
            this->pWorkerContext->bDone = true;
            bSafeCheck = false;
            return;
        }

        this->EnableUserInterface(FALSE);

        m_StatusBar.SetText(_T(""), 1, 0);
        m_BtnConvert.SetWindowText(m_Config.GetString(0x000A0018, _T("S&top")));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, m_Config.GetString(0x00030003, _T("S&top\tF9")));

        this->pWorkerContext->bRunning = true;
        this->pWorkerContext->m_Thread.Resume();

        bSafeCheck = false;
    }
    else
    {
        bSafeCheck = true;

        m_BtnConvert.SetWindowText(m_Config.GetString(0x000A0017, _T("Conve&rt")));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, m_Config.GetString(0x00030002, _T("Conve&rt\tF9")));
        this->EnableUserInterface(TRUE);

        this->pWorkerContext->bRunning = false;
        bSafeCheck = false;
    }
}

void CMainDlg::FinishConvert()
{
    this->m_BtnConvert.SetWindowText(m_Config.GetString(0x000A0017, _T("Conve&rt")));
    this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, m_Config.GetString(0x00030002, _T("Conve&rt\tF9")));
    this->EnableUserInterface(TRUE);

    this->m_Progress.SetPos(0);
    this->pWorkerContext->bRunning = false;

    if (this->m_Config.m_Options.bShutdownWhenFinished == true)
    {
        if (this->m_Config.m_Options.bDoNotSaveConfiguration == false)
        {
            try
            {
                this->SaveFormats(m_App.szFormatsFile);
                this->SaveOptions(m_App.szOptionsFile);
                this->SaveItems(m_App.szItemsFile);
            }
            catch (...) {}
        }

        ::ShutdownWindows();
    }
}
