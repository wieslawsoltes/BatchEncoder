// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\XmlConfiguration.h"
#include "BatchEncoderDlg.h"
#include "PresetsDlg.h"
#include "AboutDlg.h"
#include "FormatsDlg.h"

#define ITEM_COLUMN_NAME    0
#define ITEM_COLUMN_INPUT   1
#define ITEM_COLUMN_SIZE    2
#define ITEM_COLUMN_OUTPUT  3
#define ITEM_COLUMN_PRESET  4
#define ITEM_COLUMN_TIME    5
#define ITEM_COLUMN_STATUS  6

#define WM_TRAY (WM_USER + 0x10)
#define IDC_STATUSBAR 1500

#define IDC_FOLDERTREE          0x3741
#define IDC_TITLE               0x3742
#define IDC_STATUSTEXT          0x3743
#define IDC_CHECK_RECURSE       0x3744
#define IDC_BROWSE_NEW_FOLDER   0x3746

static CString szLastBrowse;
static WNDPROC lpOldWindowProc;
static bool bRecurseChecked = true;
static HWND hWndBtnRecurse = NULL;
static HWND hWndStaticText = NULL;

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
        HWND hWndTitle = NULL;
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
        TCHAR szText[256] = _T("");
        HWND hWndTitle = NULL;
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
            NULL, NULL, NULL);
        if (hWndStaticText != NULL)
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
    this->m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAYICON);
    this->szOptionsFile = ::GetExeFilePath() + _T("BatchEncoder.options");
    this->szFormatsFile = ::GetExeFilePath() + _T("BatchEncoder.formats");
    this->szItemsFile = ::GetExeFilePath() + _T("BatchEncoder.items");
    this->m_Config.m_Options.Defaults();
    this->pWorkerContext = new CBatchEncoderWorkerContext(&this->m_Config, this);
    this->pWorkerContext->bRunning = false;
}

CBatchEncoderDlg::~CBatchEncoderDlg()
{
    if (this->pWorkerContext != NULL)
        delete this->pWorkerContext;
}

void CBatchEncoderDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_CONVERT, m_Progress);
    DDX_Control(pDX, IDC_COMBO_PRESETS, m_CmbPresets);
    DDX_Control(pDX, IDC_COMBO_FORMAT, m_CmbFormat);
    DDX_Control(pDX, IDC_LIST_ITEMS, m_LstInputItems);
    DDX_Control(pDX, IDC_CHECK_OUTPUT, m_ChkOutPath);
    DDX_Control(pDX, IDC_EDIT_OUTPUT, m_EdtOutPath);
    DDX_Control(pDX, IDC_EDIT_THREADCOUNT, m_EdtThreads);
    DDX_Control(pDX, IDC_SPIN_THREADCOUNT, m_SpinThreads);
    DDX_Control(pDX, IDC_STATIC_TEXT_PRESET, m_StcPreset);
    DDX_Control(pDX, IDC_STATIC_TEXT_FORMAT, m_StcFormat);
    DDX_Control(pDX, IDC_STATIC_THREAD_COUNT, m_StcThreads);
    DDX_Control(pDX, IDC_BUTTON_CONVERT, m_BtnConvert);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_OUTPUT, m_BtnBrowse);
}

BEGIN_MESSAGE_MAP(CBatchEncoderDlg, CResizeDialog)
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
    ON_BN_CLICKED(IDC_CHECK_OUTPUT, OnBnClickedCheckOutPath)
    ON_EN_SETFOCUS(IDC_EDIT_OUTPUT, OnEnSetFocusEditOutPath)
    ON_EN_KILLFOCUS(IDC_EDIT_OUTPUT, OnEnKillFocusEditOutPath)
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
    ON_COMMAND(ID_OPTIONS_DELETE_SOURCE, OnOptionsDeleteSource)
    ON_COMMAND(ID_OPTIONS_SHUTDOWN_WINDOWS, OnOptionsShutdownWindows)
    ON_COMMAND(ID_OPTIONS_DO_NOT_SAVE, OnOptionsDoNotSave)
    ON_COMMAND(ID_OPTIONS_DELETE_ON_ERRORS, OnOptionsDeleteOnErrors)
    ON_COMMAND(ID_OPTIONS_STOP_ON_ERRORS, OnOptionsStopOnErrors)
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
END_MESSAGE_MAP()

BOOL CBatchEncoderDlg::OnInitDialog()
{
    CResizeDialog::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // status-bar
    m_StatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, CRect(0, 0, 0, 0), this, IDC_STATUSBAR);

    int nStatusBarParts[2] = { 100, -1 };
    m_StatusBar.SetParts(2, nStatusBarParts);

    // accelerators
    m_hAccel = ::LoadAccelerators(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_ACCELERATOR_BATCHENCODER));

    // OnNotifyFormat WM_NOTIFYFORMAT
#ifdef _UNICODE
    m_LstInputItems.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

    // enable '<< same as source file >>'
    bSameAsSourceEdit = true;

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
    dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
    m_LstInputItems.SetExtendedStyle(dwExStyle);

    // list columns
    m_LstInputItems.InsertColumn(ITEM_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 225);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_INPUT, _T("Input"), LVCFMT_LEFT, 50);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_SIZE, _T("Size (bytes)"), LVCFMT_LEFT, 80);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_OUTPUT, _T("Output"), LVCFMT_LEFT, 100);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_PRESET, _T("Preset#"), LVCFMT_LEFT, 55);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_TIME, _T("Time"), LVCFMT_LEFT, 95);
    m_LstInputItems.InsertColumn(ITEM_COLUMN_STATUS, _T("Status"), LVCFMT_LEFT, 85);

    m_StcFormat.SetBold(true);
    m_StcPreset.SetBold(true);
    m_BtnConvert.SetBold(true);

    // enable drag & drop
    this->DragAcceptFiles(TRUE);

    // resize anchors
    AddAnchor(IDC_STATIC_GROUP_OUTPUT, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_STATIC_TEXT_FORMAT, TOP_LEFT);
    AddAnchor(IDC_COMBO_FORMAT, TOP_LEFT);
    AddAnchor(IDC_STATIC_TEXT_PRESET, TOP_LEFT);
    AddAnchor(IDC_COMBO_PRESETS, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_LIST_ITEMS, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_CHECK_OUTPUT, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_OUTPUT, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_BROWSE_OUTPUT, BOTTOM_RIGHT);
    AddAnchor(IDC_PROGRESS_CONVERT, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_THREAD_COUNT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_THREADCOUNT, BOTTOM_RIGHT);
    AddAnchor(IDC_SPIN_THREADCOUNT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_CONVERT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATUSBAR, BOTTOM_LEFT, BOTTOM_RIGHT);

    try
    {
        this->LoadFormats(this->szFormatsFile);
        this->LoadOptions(this->szOptionsFile);
        this->LoadItems(this->szItemsFile);
    }
    catch (...) {}

    return TRUE;
}

void CBatchEncoderDlg::OnOK()
{

}

void CBatchEncoderDlg::OnCancel()
{

}

BOOL CBatchEncoderDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_hAccel != NULL)
    {
        if (::TranslateAccelerator(this->GetSafeHwnd(), m_hAccel, pMsg))
            return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
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

void CBatchEncoderDlg::OnClose()
{
    CResizeDialog::OnClose();

    // TODO: Kill worker thread and running command-line tool.

    if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) != MF_CHECKED)
    {
        try
        {
            this->SaveFormats(this->szFormatsFile);
            this->SaveOptions(this->szOptionsFile);
            this->SaveItems(this->szItemsFile);
        }
        catch (...) {}
    }

    CResizeDialog::OnOK();
}

void CBatchEncoderDlg::OnDestroy()
{
    CResizeDialog::OnDestroy();

    m_Config.m_Items.RemoveAllNodes();
    m_Config.m_Formats.RemoveAllNodes();
}

void CBatchEncoderDlg::OnDropFiles(HDROP hDropInfo)
{
    if (this->pWorkerContext->bRunning == false && bHandleDrop == true)
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

BOOL CBatchEncoderDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    return FALSE;
}

LRESULT CBatchEncoderDlg::OnListItemChaged(WPARAM wParam, LPARAM lParam)
{
    if (this->pWorkerContext->bRunning == false)
    {
        INT nIndex = (INT)wParam;
        LPTSTR szText = (LPTSTR)lParam;
        if ((nIndex >= 0) && szText != NULL)
        {
            CItem& item = m_Config.m_Items.GetData(nIndex);
            item.szName = szText;
        }
    }
    return(0);
}

LRESULT CBatchEncoderDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
{
#ifdef _UNICODE
    return NFR_UNICODE;
#else
    return NFR_ANSI;
#endif
}

void CBatchEncoderDlg::OnLvnKeydownListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
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

void CBatchEncoderDlg::OnNMRclickListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (this->pWorkerContext->bRunning == false)
    {
        POINT point;
        GetCursorPos(&point);
        CMenu *subMenu = this->GetMenu()->GetSubMenu(1);
        subMenu->TrackPopupMenu(0, point.x, point.y, this, NULL);
    }
    *pResult = 0;
}

void CBatchEncoderDlg::OnLvnItemchangingListInputItems(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (this->pWorkerContext->bRunning == true)
    {
        *pResult = true;
        return;
    }
    *pResult = false;
}

void CBatchEncoderDlg::OnLvnItemchangedListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (this->pWorkerContext->bRunning == false)
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
                    if (m_Config.m_Formats.GetSize() > 0)
                    {
                        CFormat& format = m_Config.m_Formats.GetData(this->m_CmbFormat.GetCurSel());
                        if (item.szFormatId.Compare(format.szId) == 0)
                        {
                            format.nDefaultPreset = item.nPreset;
                            this->m_CmbPresets.SetCurSel(item.nPreset);
                        }
                        else
                        {
                            int nFormat = m_Config.m_Formats.GetFormatById(item.szFormatId);
                            if (nFormat >= 0)
                            {
                                CFormat& format = m_Config.m_Formats.GetData(nFormat);

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

void CBatchEncoderDlg::OnCbnSelchangeComboPresets()
{
    if (this->pWorkerContext->bRunning == false)
    {
        int nFormat = this->m_CmbFormat.GetCurSel();
        int nPreset = this->m_CmbPresets.GetCurSel();

        CFormat& format = m_Config.m_Formats.GetData(nFormat);
        format.nDefaultPreset = nPreset;

        this->UpdateFormatAndPreset();
    }
}

void CBatchEncoderDlg::OnCbnSelchangeComboFormat()
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

void CBatchEncoderDlg::OnBnClickedCheckOutPath()
{
    if (this->pWorkerContext->bRunning == false)
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
}

void CBatchEncoderDlg::OnEnSetFocusEditOutPath()
{
    if (this->pWorkerContext->bRunning == false && bSameAsSourceEdit == true)
    {
        CString szPath;
        m_EdtOutPath.GetWindowText(szPath);
        if (szPath.CompareNoCase(_T("<< same as source file >>")) == 0)
            m_EdtOutPath.SetWindowText(_T(""));
    }
}

void CBatchEncoderDlg::OnEnKillFocusEditOutPath()
{
    if (this->pWorkerContext->bRunning == false && bSameAsSourceEdit == true)
    {
        CString szPath;
        m_EdtOutPath.GetWindowText(szPath);
        if (szPath.CompareNoCase(_T("")) == 0)
            m_EdtOutPath.SetWindowText(_T("<< same as source file >>"));
    }
}

void CBatchEncoderDlg::OnBnClickedButtonBrowsePath()
{
    if (this->pWorkerContext->bRunning == false)
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
        bi.lpszTitle = _T("Output path:");
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
}

void CBatchEncoderDlg::OnBnClickedButtonConvert()
{
    this->StartConvert();
}

void CBatchEncoderDlg::OnFileLoadList()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CFileDialog fd(TRUE, _T("items"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            _T("Items Files (*.items)|*.items|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

        if (fd.DoModal() == IDOK)
        {
            CString szFileXml = fd.GetPathName();
            if (this->LoadItems(szFileXml) == false)
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
}

void CBatchEncoderDlg::OnFileSaveList()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CFileDialog fd(FALSE, _T("items"), _T("items"),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
            _T("Items Files (*.items)|*.items|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

        if (fd.DoModal() == IDOK)
        {
            CString szFileXml = fd.GetPathName();
            if (this->SaveItems(szFileXml) == false)
            {
                MessageBox(_T("Failed to save file!"),
                    _T("ERROR"),
                    MB_OK | MB_ICONERROR);
            }
        }
    }
}

void CBatchEncoderDlg::OnFileClearList()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->pWorkerContext->bRunning == false)
        {
            m_Config.m_Items.RemoveAllNodes();
            m_LstInputItems.DeleteAllItems();
            this->UpdateStatusBar();
        }
    }
}

void CBatchEncoderDlg::OnFileExit()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->OnClose();
    }
}

void CBatchEncoderDlg::OnEditAddFiles()
{
    if (this->pWorkerContext->bRunning == false)
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
                _T("All Files (*.*)|*.*||"),
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
}

void CBatchEncoderDlg::OnEditAddDir()
{
    if (this->pWorkerContext->bRunning == false)
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
        bi.lpszTitle = _T("Select folder:");
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
}

void CBatchEncoderDlg::OnEditRename()
{
    if (this->pWorkerContext->bRunning == false)
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
}

void CBatchEncoderDlg::OnEditResetTime()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->m_StatusBar.SetText(_T(""), 1, 0);
        this->ResetConvertionTime();
        this->ResetConvertionStatus();
    }
}

void CBatchEncoderDlg::OnEditResetOutput()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->ResetOutput();
    }
}

void CBatchEncoderDlg::OnEditRemove()
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
}

void CBatchEncoderDlg::OnEditCrop()
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
}

void CBatchEncoderDlg::OnEditRemoveChecked()
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
}

void CBatchEncoderDlg::OnEditRemoveUnchecked()
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
}

void CBatchEncoderDlg::OnEditCheckSelected()
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

void CBatchEncoderDlg::OnEditUncheckSelected()
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

void CBatchEncoderDlg::OnEditSelectAll()
{
    if (this->pWorkerContext->bRunning == false)
    {
        m_LstInputItems.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
    }
}

void CBatchEncoderDlg::OnEditSelectNone()
{
    if (this->pWorkerContext->bRunning == false)
    {
        m_LstInputItems.SetItemState(-1, 0, LVIS_SELECTED);
    }
}

void CBatchEncoderDlg::OnEditInvertSelection()
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

void CBatchEncoderDlg::OnEditOpen()
{
    if (this->pWorkerContext->bRunning == false)
    {
        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int nItem = m_LstInputItems.GetNextSelectedItem(pos);
            CItem& item = m_Config.m_Items.GetData(nItem);
            ::LaunchAndWait(item.szPath, _T(""), FALSE);
        }
    }
}

void CBatchEncoderDlg::OnEditExplore()
{
    if (this->pWorkerContext->bRunning == false)
    {
        POSITION pos = m_LstInputItems.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int nItem = m_LstInputItems.GetNextSelectedItem(pos);
            CItem& item = m_Config.m_Items.GetData(nItem);
            CString szPath = item.szPath;
            szPath.TrimRight(::GetFileName(item.szPath));
            ::LaunchAndWait(szPath, _T(""), FALSE);
        }
    }
}

void CBatchEncoderDlg::OnActionConvert()
{
    this->StartConvert();
}

void CBatchEncoderDlg::OnOptionsConfigurePresets()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CPresetsDlg dlg;
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
}

void CBatchEncoderDlg::OnOptionsConfigureFormat()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CFormatsDlg dlg;
        dlg.nSelectedFormat = this->m_CmbFormat.GetCurSel();
        dlg.m_Formats = m_Config.m_Formats;
        dlg.szFormatsDialogResize = m_Config.m_Options.szFormatsDialogResize;
        dlg.szFormatsListColumns = m_Config.m_Options.szFormatsListColumns;

        INT_PTR nRet = dlg.DoModal();
        if (nRet == IDOK)
        {
            m_Config.m_Formats.RemoveAllNodes();
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

void CBatchEncoderDlg::OnOptionsDeleteSource()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_SOURCE, MF_BYCOMMAND) == MF_CHECKED)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, MF_UNCHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, MF_CHECKED);
    }
}

void CBatchEncoderDlg::OnOptionsShutdownWindows()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_BYCOMMAND) == MF_CHECKED)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_UNCHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_SHUTDOWN_WINDOWS, MF_CHECKED);
    }
}

void CBatchEncoderDlg::OnOptionsDoNotSave()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) == MF_CHECKED)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_UNCHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DO_NOT_SAVE, MF_CHECKED);
    }
}

void CBatchEncoderDlg::OnOptionsDeleteOnErrors()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_ON_ERRORS, MF_UNCHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_ON_ERRORS, MF_CHECKED);
    }
}

void CBatchEncoderDlg::OnOptionsStopOnErrors()
{
    if (this->pWorkerContext->bRunning == false)
    {
        if (this->GetMenu()->GetMenuState(ID_OPTIONS_STOP_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_STOP_ON_ERRORS, MF_UNCHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_STOP_ON_ERRORS, MF_CHECKED);
    }
}

void CBatchEncoderDlg::OnHelpWebsite()
{
    if (this->pWorkerContext->bRunning == false)
    {
        ::LaunchAndWait(_T("https://github.com/wieslawsoltes/BatchEncoder"), _T(""), FALSE);
    }
}

void CBatchEncoderDlg::OnHelpAbout()
{
    if (this->pWorkerContext->bRunning == false)
    {
        CAboutDlg dlg;
        dlg.DoModal();
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
        item.szTime = this->m_LstInputItems.GetItemText(i, ITEM_COLUMN_TIME);
        item.szStatus = this->m_LstInputItems.GetItemText(i, ITEM_COLUMN_STATUS);
    }
}

void CBatchEncoderDlg::SetItems()
{
    int nItems = m_Config.m_Items.GetSize();
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = m_Config.m_Items.GetData(i);
        this->AddToList(item, i);
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
    m_Config.m_Options.bDeleteOnErrors = this->GetMenu()->GetMenuState(ID_OPTIONS_STOP_ON_ERRORS, MF_BYCOMMAND) == MF_CHECKED;

    // option: ThreadCount
    CString szThreadCount;
    m_EdtThreads.GetWindowText(szThreadCount);
    m_Config.m_Options.nThreadCount = _tstoi(szThreadCount);

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

    // option: ThreadCount
    CString szThreadCount;
    szThreadCount.Format(_T("%d\0"), m_Config.m_Options.nThreadCount);
    m_EdtThreads.SetWindowText(szThreadCount);

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

    // option: PresetsDialogResize

    // option: PresetsListColumns

    // option: FormatsDialogResize

    // option: FormatsListColumns
}

bool CBatchEncoderDlg::LoadOptions(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.Open(szFileXml) == false)
        return false;

    doc.GetOptions(this->m_Config.m_Options);

    this->SetOptions();
    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();

    return true;
}

bool CBatchEncoderDlg::SaveOptions(CString szFileXml)
{
    this->GetOptions();

    XmlConfiguration doc;
    doc.SetOptions(this->m_Config.m_Options);
    return doc.Save(szFileXml);
}

bool CBatchEncoderDlg::LoadFormats(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.Open(szFileXml) == false)
        return false;

    this->m_Config.m_Formats.RemoveAllNodes();

    doc.GetFormats(this->m_Config.m_Formats);

    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();

    return true;
}

bool CBatchEncoderDlg::SaveFormats(CString szFileXml)
{
    XmlConfiguration doc;
    doc.SetFormats(this->m_Config.m_Formats);
    return doc.Save(szFileXml);
}

bool CBatchEncoderDlg::LoadItems(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.Open(szFileXml) == false)
        return false;

    this->m_LstInputItems.DeleteAllItems();
    this->m_Config.m_Items.RemoveAllNodes();

    doc.GetItems(this->m_Config.m_Items);

    this->SetItems();
    this->UpdateStatusBar();

    return true;
}

bool CBatchEncoderDlg::SaveItems(CString szFileXml)
{
    this->GetItems();

    XmlConfiguration doc;
    doc.SetItems(this->m_Config.m_Items);
    return doc.Save(szFileXml);
}

int CBatchEncoderDlg::AddToItems(CString szPath)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    int nPreset = this->m_CmbPresets.GetCurSel();

    CString szFormatId = _T("");
    if (m_Config.m_Formats.GetSize() > 0)
        szFormatId = m_Config.m_Formats.GetData(nFormat).szId;

    WIN32_FIND_DATA lpFindFileData;
    HANDLE hFind;
    ULARGE_INTEGER ulSize;
    ULONGLONG nFileSize;

    hFind = ::FindFirstFile(szPath, &lpFindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return -1;

    ::FindClose(hFind);

    ulSize.HighPart = lpFindFileData.nFileSizeHigh;
    ulSize.LowPart = lpFindFileData.nFileSizeLow;
    nFileSize = ulSize.QuadPart;

    CString szFileSize;
    szFileSize.Format(_T("%I64d"), nFileSize);

    CItem item;
    item.szPath = szPath;
    item.szSize = szFileSize;
    item.szName = ::GetOnlyFileName(szPath);
    item.szExtension = ::GetFileExtension(szPath).MakeUpper();
    item.szFormatId = szFormatId;
    item.nPreset = nPreset;
    item.bChecked = true;

    m_Config.m_Items.InsertNode(item);

    return (int)m_Config.m_Items.GetSize() - 1;
}

void CBatchEncoderDlg::AddToList(CItem &item, int nItem)
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

    // [Time] : encoder/decoder conversion time
    tmpBuf.Format(_T("%s"), (item.szTime.Compare(_T("")) == 0) ? _T("--:--") : item.szTime);
    lvi.iSubItem = ITEM_COLUMN_TIME;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_TIME, lvi.pszText);

    // [Status] : encoder/decoder progress status
    tmpBuf.Format(_T("%s"), (item.szStatus.Compare(_T("")) == 0) ? _T("Not Done") : item.szStatus);
    lvi.iSubItem = ITEM_COLUMN_STATUS;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tmpBuf);
    m_LstInputItems.SetItemText(lvi.iItem, ITEM_COLUMN_STATUS, lvi.pszText);

    // item CheckBox state
    m_LstInputItems.SetCheck(nItem, item.bChecked);
}

bool CBatchEncoderDlg::AddToList(CString szPath)
{
    int nItem = this->AddToItems(szPath);
    if (nItem == -1)
        return false;

    CItem& item = m_Config.m_Items.GetData(nItem);
    this->AddToList(item, nItem);

    return true;
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
            ::DragQueryFile(hDropInfo, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
            if (::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
            {
                this->SearchFolderForFiles(szFile, true);
            }
            else
            {
                CString szPath = szFile;
                this->AddToList(szPath);
            }

            szFile.ReleaseBuffer();
        }

        this->UpdateStatusBar();
    }

    ::DragFinish(hDropInfo);
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
        MessageBox(_T("Error while searching for files!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CBatchEncoderDlg::UpdateFormatComboBox()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->m_CmbFormat.ResetContent();

        int nFormats = m_Config.m_Formats.GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = m_Config.m_Formats.GetData(i);
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

void CBatchEncoderDlg::UpdatePresetComboBox()
{
    if (this->pWorkerContext->bRunning == false)
    {
        this->m_CmbPresets.ResetContent();

        int nPreset = -1;

        if (m_Config.m_Options.nSelectedFormat > m_Config.m_Formats.GetSize() - 1)
            m_Config.m_Options.nSelectedFormat = 0;

        if (m_Config.m_Options.nSelectedFormat >= 0 && m_Config.m_Formats.GetSize() > 0)
        {
            CFormat& format = m_Config.m_Formats.GetData(m_Config.m_Options.nSelectedFormat);
            int nPresets = format.m_Presets.GetSize();
            for (int i = 0; i < nPresets; i++)
            {
                CPreset& preset = format.m_Presets.GetData(i);
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
                CItem& item = m_Config.m_Items.GetData(i);
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
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_TIME, _T("--:--"));
                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_TIME, _T("--:--"));
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
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_STATUS, _T("Not Done"));
                nSelected++;
            }
        }

        if (nSelected == 0)
        {
            for (int i = 0; i < nCount; i++)
                this->m_LstInputItems.SetItemText(i, ITEM_COLUMN_STATUS, _T("Not Done"));
        }
    }
}

void CBatchEncoderDlg::UpdateStatusBar()
{
    int nCount = m_LstInputItems.GetItemCount();
    if (nCount > 0)
    {
        CString szText;
        szText.Format(_T("%d %s"), nCount, (nCount > 1) ? _T("Items") : _T("Item"));
        m_StatusBar.SetText(szText, 0, 0);
    }
    else
    {
        m_StatusBar.SetText(_T("No Items"), 0, 0);
        m_StatusBar.SetText(_T(""), 1, 0);
    }
}

void CBatchEncoderDlg::EnableUserInterface(BOOL bEnable)
{
    if (bEnable == FALSE)
    {
        this->m_ChkOutPath.ShowWindow(SW_HIDE);
        this->m_EdtOutPath.ShowWindow(SW_HIDE);
        this->m_BtnBrowse.ShowWindow(SW_HIDE);
        this->m_StcThreads.ShowWindow(SW_HIDE);
        this->m_EdtThreads.ShowWindow(SW_HIDE);
        this->m_SpinThreads.ShowWindow(SW_HIDE);
        this->m_Progress.ShowWindow(SW_SHOW);
    }
    else
    {
        this->m_Progress.ShowWindow(SW_HIDE);
        this->m_ChkOutPath.ShowWindow(SW_SHOW);
        this->m_EdtOutPath.ShowWindow(SW_SHOW);
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

    if (this->m_ChkOutPath.GetCheck() == BST_CHECKED)
        this->m_EdtOutPath.EnableWindow(bEnable);

    if (this->m_ChkOutPath.GetCheck() == BST_CHECKED)
        this->m_BtnBrowse.EnableWindow(bEnable);

    this->m_ChkOutPath.EnableWindow(bEnable);
}

void CBatchEncoderDlg::StartConvert()
{
    static volatile bool bSafeCheck = false;
    if (bSafeCheck == true)
        return;

    if (this->pWorkerContext->bRunning == false)
    {
        bSafeCheck = true;

        this->GetOptions();
        this->GetItems();

        this->pWorkerContext->pConfig = &this->m_Config;

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

        this->pWorkerContext->dwThreadID = 0;
        this->pWorkerContext->hThread = ::CreateThread(NULL, 0, WorkThread, this->pWorkerContext, CREATE_SUSPENDED, &this->pWorkerContext->dwThreadID);
        if (this->pWorkerContext->hThread == NULL)
        {
            MessageBox(_T("Fatal Error when Creating Thread"), _T("ERROR"), MB_OK | MB_ICONERROR);
            bSafeCheck = false;
            return;
        }

        this->EnableUserInterface(FALSE);

        m_StatusBar.SetText(_T(""), 1, 0);
        m_BtnConvert.SetWindowText(_T("S&top"));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("S&top\tF9"));

        this->ResetConvertionTime();
        this->ResetConvertionStatus();

        this->pWorkerContext->bRunning = true;

        ::ResumeThread(this->pWorkerContext->hThread);

        bSafeCheck = false;
    }
    else
    {
        bSafeCheck = true;

        m_BtnConvert.SetWindowText(_T("Conve&rt"));
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("Conve&rt\tF9"));
        this->EnableUserInterface(TRUE);

        this->pWorkerContext->bRunning = false;
        bSafeCheck = false;
    }
}

void CBatchEncoderDlg::FinishConvert()
{
    this->m_BtnConvert.SetWindowText(_T("Conve&rt"));
    this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("Conve&rt\tF9"));
    this->EnableUserInterface(TRUE);

    this->m_Progress.SetPos(0);
    this->pWorkerContext->bRunning = false;

    if (this->m_Config.m_Options.bShutdownWhenFinished == true)
    {
        if (this->m_Config.m_Options.bDoNotSaveConfiguration == false)
        {
            try
            {
                this->SaveFormats(this->szFormatsFile);
                this->SaveOptions(this->szOptionsFile);
                this->SaveItems(this->szItemsFile);
            }
            catch (...) {}
        }

        ::ShutdownWindows();
    }
}

void CBatchEncoderWorkerContext::Init()
{
    this->timer.Start();

    pDlg->m_Progress.SetPos(0);
}

void CBatchEncoderWorkerContext::Next(int nItemId)
{
    this->nProcessedFiles++;
    this->nErrors = (this->nProcessedFiles - 1) - this->nDoneWithoutError;

    if (this->nThreadCount == 1)
    {
        CString szText;
        szText.Format(_T("Processing item %d of %d (%d Done, %d %s)"),
            this->nProcessedFiles,
            this->nTotalFiles,
            this->nDoneWithoutError,
            this->nErrors,
            ((this->nErrors == 0) || (this->nErrors > 1)) ? _T("Errors") : _T("Error"));
        pDlg->m_StatusBar.SetText(szText, 1, 0);

        this->nLastItemId = nItemId;
        pDlg->m_LstInputItems.EnsureVisible(nItemId, FALSE);
    }
}

void CBatchEncoderWorkerContext::Done()
{
    this->timer.Stop();
    this->nErrors = this->nProcessedFiles - this->nDoneWithoutError;

    CString szText;
    szText.Format(_T("Converted %d of %d (%d Done, %d %s) in %s"),
        this->nProcessedFiles,
        this->nTotalFiles,
        this->nDoneWithoutError,
        this->nErrors,
        ((this->nErrors == 0) || (this->nErrors > 1)) ? _T("Errors") : _T("Error"),
        ::FormatTime(this->timer.ElapsedTime(), 3));
    pDlg->m_StatusBar.SetText(szText, 1, 0);

    pDlg->FinishConvert();
}

bool CBatchEncoderWorkerContext::Callback(int nItemId, int nProgress, bool bFinished, bool bError)
{
    if (bError == true)
    {
        if (pDlg->pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
        {
            pDlg->m_Progress.SetPos(0);
            this->bRunning = false;
        }

        return this->bRunning;
    }

    if (bFinished == false)
    {
        CString szProgress;
        szProgress.Format(_T("%d%%\0"), nProgress);
        pDlg->m_LstInputItems.SetItemText(nItemId, ITEM_COLUMN_STATUS, szProgress); // Status

        pDlg->pWorkerContext->nProgess[nItemId] = nProgress;

        static volatile bool bSafeCheck = false;
        if (bSafeCheck == false)
        {
            bSafeCheck = true;

            if (nItemId > this->nLastItemId)
            {
                this->nLastItemId = nItemId;
                pDlg->m_LstInputItems.EnsureVisible(nItemId, FALSE);
            }

            int nTotalProgress = 0;
            int nItems = pDlg->pWorkerContext->pConfig->m_Items.GetSize();
            for (int i = 0; i < nItems; i++)
            {
                if (pDlg->pWorkerContext->pConfig->m_Items.GetData(i).bChecked == TRUE)
                    nTotalProgress += pDlg->pWorkerContext->nProgess[i];
            }

            int nPos = nTotalProgress / pDlg->pWorkerContext->nTotalFiles;
            if (pDlg->m_Progress.GetPos() != nPos)
            {
                pDlg->m_Progress.SetPos(nPos);
            }

            bSafeCheck = false;
        }
    }

    return this->bRunning;
}

void CBatchEncoderWorkerContext::Status(int nItemId, CString szTime, CString szStatus)
{
    pDlg->m_LstInputItems.SetItemText(nItemId, ITEM_COLUMN_TIME, szTime); // Time
    pDlg->m_LstInputItems.SetItemText(nItemId, ITEM_COLUMN_STATUS, szStatus); // Status
};
