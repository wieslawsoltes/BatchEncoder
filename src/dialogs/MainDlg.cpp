// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "language\LanguageHelper.h"
#include "utilities\Utilities.h"
#include "xml\XmlConfig.h"
#include "worker\OutputPath.h"
#include "MainDlg.h"
#include "PresetsDlg.h"
#include "AboutDlg.h"
#include "FormatsDlg.h"
#include "ToolsDlg.h"

#define WM_TRAY (WM_USER + 0x10)
#define IDC_STATUSBAR 1500

#define IDC_FOLDERTREE          0x3741
#define IDC_TITLE               0x3742
#define IDC_STATUSTEXT          0x3743
#define IDC_CHECK_RECURSE       0x3744
#define IDC_BROWSE_NEW_FOLDER   0x3746

#define ID_LANGUAGE_MIN 2000
#define ID_LANGUAGE_MAX 2999

namespace app
{
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
                pDlg->m_Config.GetString(0x00210005).c_str(),
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

    class CMainDlgWorkerContext : public worker::IWorkerContext
    {
    private:
        util::CTimeCount timer;
        CMainDlg *pDlg;
    public:
        CMainDlgWorkerContext(config::CConfiguration* pConfig, CMainDlg* pDlg)
            : IWorkerContext(pConfig)
        {
            this->bDone = true;
            this->pDlg = pDlg;
        }
        virtual ~CMainDlgWorkerContext()
        {
        }
    public:
        void Init()
        {
            this->timer.Start();

            pDlg->m_Progress.SetPos(0);
        }
        void Next(int nItemId)
        {
            this->nProcessedFiles++;
            this->nErrors = (this->nProcessedFiles - 1) - this->nDoneWithoutError;

            if (this->nThreadCount == 1)
            {
                CString szFormat = this->pConfig->GetString(0x00190003).c_str();
                CString szText;
                szText.Format(szFormat,
                    this->nProcessedFiles,
                    this->nTotalFiles,
                    this->nDoneWithoutError,
                    this->nErrors,
                    ((this->nErrors == 0) || (this->nErrors > 1)) ?
                    this->pConfig->GetString(0x00190002).c_str() : this->pConfig->GetString(0x00190001).c_str());
                pDlg->m_StatusBar.SetText(szText, 1, 0);

                this->nLastItemId = nItemId;

                if (pDlg->m_Config.m_Options.bEnsureItemIsVisible == true)
                    pDlg->MakeItemVisible(nItemId);
            }
        }
        void Done()
        {
            this->timer.Stop();
            this->nErrors = this->nProcessedFiles - this->nDoneWithoutError;

            CString szFormat = this->pConfig->GetString(0x00190004).c_str();
            CString szText;
            szText.Format(szFormat,
                this->nProcessedFiles,
                this->nTotalFiles,
                this->nDoneWithoutError,
                this->nErrors,
                ((this->nErrors == 0) || (this->nErrors > 1)) ?
                this->pConfig->GetString(0x00190002).c_str() : this->pConfig->GetString(0x00190001).c_str(),
                util::CTimeCount::Format(this->timer.ElapsedTime()).c_str());
            pDlg->m_StatusBar.SetText(szText, 1, 0);

            pDlg->FinishConvert();
        }
        bool IsRunning()
        {
            return this->bRunning;
        }
        bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false)
        {
            if (bError == true)
            {
                config::CItem &item = pConfig->m_Items.Get(nItemId);
                item.bFinished = true;

                if (pConfig->m_Options.bStopOnErrors == true)
                {
                    pDlg->m_Progress.SetPos(0);
                    this->bRunning = false;
                }

                return this->bRunning;
            }

            if (bFinished == true)
            {
                config::CItem &item = pConfig->m_Items.Get(nItemId);
                item.bFinished = true;
            }

            if ((bFinished == false) && (this->bRunning == true))
            {
                config::CItem &current = pConfig->m_Items.Get(nItemId);
                current.nProgress = nProgress;
                if (current.nPreviousProgress > nProgress)
                    current.nPreviousProgress = nProgress;

                static volatile bool bSafeCheck = false;
                if (bSafeCheck == false)
                {
                    bSafeCheck = true;

                    if (nItemId > this->nLastItemId)
                    {
                        this->nLastItemId = nItemId;
                        if (pDlg->m_Config.m_Options.bEnsureItemIsVisible == true)
                            pDlg->MakeItemVisible(nItemId);
                    }

                    int nTotalProgress = 0;
                    int nItems = pConfig->m_Items.Count();
                    for (int i = 0; i < nItems; i++)
                    {
                        config::CItem &item = pConfig->m_Items.Get(i);
                        if (item.bChecked == true)
                        {
                            int nItemProgress = item.nProgress;
                            int nItemPreviousProgress = item.nPreviousProgress;

                            nTotalProgress += nItemProgress;

                            if (item.bFinished == false)
                            {
                                if (nItemProgress > 0 && nItemProgress < 100 && nItemProgress > nItemPreviousProgress)
                                {
                                    item.szStatus = std::to_wstring(nItemProgress) + L"%";
                                    item.nPreviousProgress = nItemProgress;
                                    pDlg->RedrawItem(i);
                                }
                                else if (nItemProgress == 100 && nItemProgress > nItemPreviousProgress)
                                {
                                    item.nPreviousProgress = nItemProgress;
                                }
                            }
                        }
                    }

                    int nPos = nTotalProgress / nTotalFiles;
                    if (pDlg->m_Progress.GetPos() != nPos)
                        pDlg->m_Progress.SetPos(nPos);

                    bSafeCheck = false;
                }
            }

            return this->bRunning;
        }
        void Status(int nItemId, const std::wstring& szTime, const std::wstring& szStatus)
        {
            config::CItem &item = pConfig->m_Items.Get(nItemId);
            item.szTime = szTime;
            item.szStatus = szStatus;
            pDlg->RedrawItem(nItemId);
        }
    };

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
        ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ITEMS, OnLvnGetdispinfoListItems)
        ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST_ITEMS, OnOdfindListItems)
        ON_NOTIFY(NM_CLICK, IDC_LIST_ITEMS, OnNMClickListItems)
        ON_EN_KILLFOCUS(IDC_EDIT_ITEM, OnEnKillfocusEditItem)
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
        ON_COMMAND(ID_OPTIONS_DOWNLOAD_TOOLS, OnOptionsDownloadTools)
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
        dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
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

        // edit item
        m_EdtItem.Create(ES_AUTOHSCROLL | WS_CHILD, CRect(0, 0, 1, 1), &m_LstInputItems, IDC_EDIT_ITEM);
        m_EdtItem.SetFont(this->GetFont());

        // bold style
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

        util::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_OUTPUT, 15);
        this->m_CmbOutPath.SetCurSel(1);
        this->m_CmbOutPath.SetFocus();

        try
        {
            this->LoadTools(app::m_App.szToolsFile);
            this->LoadFormats(app::m_App.szFormatsFile);

            if (this->LoadOptions(app::m_App.szOptionsFile) == false)
            {
                this->m_Config.m_Options.Defaults();
                this->SetOptions();
                this->UpdateFormatComboBox();
                this->UpdatePresetComboBox();
            }

            this->SearchFolderForLanguages(app::m_App.szSettingsPath.c_str());
            this->SearchFolderForLanguages(app::m_App.szLanguagesPath.c_str());
            this->InitLanguageMenu();
            this->SetLanguage();
            this->LoadItems(app::m_App.szItemsFile);
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
        if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_EdtItem.m_hWnd)
        {
            if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            {
                UpdateEdtItem(pMsg->wParam == VK_RETURN);
                return TRUE;
            }
        }

        if (pMsg->message == WM_MOUSEWHEEL && pMsg->hwnd == m_LstInputItems.m_hWnd)
        {
            UpdateEdtItem(FALSE);
        }

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
                this->SaveTools(app::m_App.szToolsFile);
                this->SaveFormats(app::m_App.szFormatsFile);
                this->SaveOptions(app::m_App.szOptionsFile);
                this->SaveItems(app::m_App.szItemsFile);
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
                config::CItem& item = m_Config.m_Items.Get(nIndex);
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

    void CMainDlg::OnLvnGetdispinfoListItems(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
        LV_ITEM* pItem = &(pDispInfo)->item;
        int nItem = pItem->iItem;

        if (pItem->mask & LVIF_TEXT)
        {
            config::CItem& item = m_Config.m_Items.Get(nItem);
            std::wstring szText;

            switch(pItem->iSubItem)
            {
            case ITEM_COLUMN_NAME:
                    // [Name] : item name
                    szText = item.szName;
                    break;
            case ITEM_COLUMN_INPUT:
                    // [Type] : input extension 
                    szText = item.szExtension;
                    break;
            case ITEM_COLUMN_SIZE:
                    // [Size (bytes)] : file size
                    szText = item.szSize;
                    break;
            case ITEM_COLUMN_OUTPUT:
                    // [Output] : output format
                    szText = item.szFormatId;
                    break;
            case ITEM_COLUMN_PRESET:
                    // [Preset] : selected preset index
                    szText = std::to_wstring(item.nPreset);
                    break;
            case ITEM_COLUMN_OPTIONS:
                    // [Options] : additional options
                    szText = item.szOptions;
                    break;
            case ITEM_COLUMN_TIME:
                    // [Time] : encoder/decoder conversion time
                    szText = item.szTime.empty() ? m_Config.GetString(0x00150001) : item.szTime;
                    break;
            case ITEM_COLUMN_STATUS:
                    // [Status] : encoder/decoder progress status
                    szText = item.szStatus.empty() ? m_Config.GetString(0x00210001) : item.szStatus;
                    break;
            }

            _tcscpy_s(pItem->pszText, pItem->cchTextMax, szText.c_str());
        }

        if(pItem->mask & LVIF_IMAGE) 
        {
            config::CItem& item = m_Config.m_Items.Get(nItem);

            pItem->mask |= LVIF_STATE;
            pItem->stateMask = LVIS_STATEIMAGEMASK;
            pItem->state = item.bChecked ? INDEXTOSTATEIMAGEMASK(2) : INDEXTOSTATEIMAGEMASK(1);
        }

        *pResult = 0;
    }

    void CMainDlg::OnOdfindListItems(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        if (this->pWorkerContext->bRunning == false)
        {
            NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;
            *pResult = -1;
            if((pFindInfo->lvfi.flags & LVFI_STRING) == 0)
            {
                return;
            }

            CString szSearchStr = pFindInfo->lvfi.psz;
            int startPos = pFindInfo->iStart;
            if (startPos >= m_LstInputItems.GetItemCount())
                startPos = 0;

            int currentPos = startPos;
            do
            {
                config::CItem& item = m_Config.m_Items.Get(currentPos);
                if( _tcsnicmp(item.szName.c_str(), szSearchStr, szSearchStr.GetLength()) == 0)
                {
                    *pResult = currentPos;
                    break;
                }
                currentPos++;
                if(currentPos >= m_LstInputItems.GetItemCount())
                    currentPos = 0;
            } while(currentPos != startPos);
        }
    }

    void CMainDlg::OnNMClickListItems(NMHDR *pNMHDR, LRESULT *pResult)
    {
        if (this->pWorkerContext->bRunning == false)
        {
            NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
            LVHITTESTINFO hitInfo;
            hitInfo.pt = pNMListView->ptAction;
            int nItem = m_LstInputItems.HitTest(&hitInfo); 
            if (nItem != -1)
            {
                if (hitInfo.flags == LVHT_ONITEMSTATEICON)
                {
                    ToggleItem(nItem);
                }
            }

            LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
            if (pNMItemActivate->iSubItem == -1 || pNMItemActivate->iItem == -1)
            {
                UpdateEdtItem(TRUE);
                *pResult = 0;
                return;
            }

            if (pNMItemActivate->iSubItem == 0 || pNMItemActivate->iSubItem != ITEM_COLUMN_OPTIONS)
            {
                UpdateEdtItem(TRUE);
                *pResult = 0;
                return;
            }

            UpdateEdtItem(TRUE);

            nEdtItem = pNMItemActivate->iItem;
            nEdtSubItem = pNMItemActivate->iSubItem;

            config::CItem& item = m_Config.m_Items.Get(nEdtItem);
            szEdtText = item.szOptions.c_str();

            ShowEdtItem();
        }

        *pResult = 0;
    }

    void CMainDlg::OnEnKillfocusEditItem()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            UpdateEdtItem(TRUE);
        }
    }

    void CMainDlg::OnLvnKeydownListInputItems(NMHDR *pNMHDR, LRESULT *pResult)
    {
        if (this->pWorkerContext->bRunning == false)
        {
            LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
            switch (pLVKeyDow->wVKey)
            {
            case VK_SPACE:
                this->OnEditToggleSelected();
                break;
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
                        config::CItem& item = m_Config.m_Items.Get(nItem);
                        if (m_Config.m_Formats.Count() > 0)
                        {
                            config::CFormat& format = m_Config.m_Formats.Get(this->m_CmbFormat.GetCurSel());
                            if (util::StringHelper::CompareNoCase(item.szFormatId, format.szId))
                            {
                                format.nDefaultPreset = item.nPreset;
                                this->m_CmbPresets.SetCurSel(item.nPreset);
                            }
                            else
                            {
                                int nFormat = m_Config.m_Formats.GetFormatById(item.szFormatId);
                                if (nFormat >= 0)
                                {
                                    config::CFormat& format = m_Config.m_Formats.Get(nFormat);

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

            config::CFormat& format = m_Config.m_Formats.Get(nFormat);
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

            std::wstring szTitle = m_Config.GetString(0x00210006);

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
            bi.lpszTitle = szTitle.c_str();
            bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
            bi.iImage = 0;
            bi.lpfn = app::BrowseCallbackOutPath;
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
            szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
                m_Config.GetString(0x00310003),
                m_Config.GetString(0x00310001));

            CFileDialog fd(TRUE, _T("xml"), _T(""),
                OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
                szFilter, this);

            if (fd.DoModal() == IDOK)
            {
                std::wstring szFileXml = fd.GetPathName();
                if (this->LoadItems(szFileXml) == false)
                    m_StatusBar.SetText(m_Config.GetString(0x00210007).c_str(), 1, 0);
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
            szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
                m_Config.GetString(0x00310003),
                m_Config.GetString(0x00310001));

            CFileDialog fd(FALSE, _T("xml"), _T("Items"),
                OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
                szFilter, this);

            if (fd.DoModal() == IDOK)
            {
                std::wstring szFileXml = fd.GetPathName();
                if (this->SaveItems(szFileXml) == false)
                    m_StatusBar.SetText(m_Config.GetString(0x00210008).c_str(), 1, 0);
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
                m_LstInputItems.SetItemCount(0);
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
                    m_StatusBar.SetText(m_Config.GetString(0x00210009).c_str(), 1, 0);
                    return;
                }

                ZeroMemory(pFiles, dwMaxSize);

                CString szFilter;
                szFilter.Format(_T("%s (*.*)|*.*||"), m_Config.GetString(0x00310001));

                CFileDialog fd(TRUE, _T(""), _T(""),
                    OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_ENABLESIZING,
                    szFilter,
                    this);

                fd.m_ofn.lpstrFile = pFiles;
                fd.m_ofn.nMaxFile = (dwMaxSize) / 2;

                if (fd.DoModal() != IDCANCEL)
                {
                    POSITION pos = fd.GetStartPosition();

                    do
                    {
                        std::wstring sFilePath = fd.GetNextPathName(pos);
                        if (!sFilePath.empty())
                        {
                            this->AddToList(sFilePath);
                        }
                    } while (pos != nullptr);

                    this->SetItems();
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

            CString szTitle = m_Config.GetString(0x0021000A).c_str();

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
            bi.lpfn = app::BrowseCallbackAddDir;
            bi.lParam = reinterpret_cast<LPARAM>(this);

            pidlBrowse = ::SHBrowseForFolder(&bi);
            if (pidlBrowse != nullptr)
            {
                if (::SHGetPathFromIDList(pidlBrowse, lpBuffer))
                {
                    CString szPath = lpBuffer;
                    this->SearchFolderForFiles(szPath, bRecurseChecked);
                    this->SetItems();
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
            int nItemLastRemoved = -1;
            int nItems = m_Config.m_Items.Count();
            if (nItems <= 0)
                return;

            for (int i = (nItems - 1); i >= 0; i--)
            {
                if (this->IsItemSelected(i))
                {
                    m_Config.m_Items.Remove(i);
                    nItemLastRemoved = i;
                }
            }

            nItems = m_Config.m_Items.Count();
            m_LstInputItems.SetItemCount(nItems);

            if (nItemLastRemoved != -1)
            {
                if (nItemLastRemoved < nItems && nItems >= 0)
                {
                    this->SelectItem(nItemLastRemoved);
                    this->MakeItemVisible(nItemLastRemoved);
                }
                else if (nItemLastRemoved >= nItems && nItems >= 0)
                {
                    this->SelectItem(nItemLastRemoved - 1);
                    this->MakeItemVisible(nItemLastRemoved - 1);
                }
            }

            if (nItems == 0)
            {
                m_Config.m_Items.RemoveAll();
                m_LstInputItems.SetItemCount(0);
            }

            this->UpdateStatusBar();
        }
    }

    void CMainDlg::OnEditCrop()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            int nItems = m_Config.m_Items.Count();
            for (int i = 0; i < nItems; i++)
            {
                if (this->IsItemSelected(i))
                    this->DeselectItem(i);
                else
                    this->SelectItem(i);
            }

            OnEditRemove();
        }
    }

    void CMainDlg::OnEditRemoveChecked()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            int nItems = m_Config.m_Items.Count();
            if (nItems <= 0)
                return;

            for (int i = (nItems - 1); i >= 0; i--)
            {
                config::CItem& item = m_Config.m_Items.Get(i);
                if (item.bChecked == true)
                    m_Config.m_Items.Remove(i);
            }

            nItems = m_Config.m_Items.Count();
            m_LstInputItems.SetItemCount(nItems);
            this->RedrawItem(0, nItems - 1);

            if (nItems == 0)
            {
                m_Config.m_Items.RemoveAll();
                m_LstInputItems.SetItemCount(0);
            }

            this->UpdateStatusBar();
        }
    }

    void CMainDlg::OnEditRemoveUnchecked()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            int nItems = m_Config.m_Items.Count();
            if (nItems <= 0)
                return;

            for (int i = (nItems - 1); i >= 0; i--)
            {
                config::CItem& item = m_Config.m_Items.Get(i);
                if (item.bChecked == false)
                    m_Config.m_Items.Remove(i);
            }

            nItems = m_Config.m_Items.Count();
            m_LstInputItems.SetItemCount(nItems);
            this->RedrawItem(0, nItems - 1);

            if (nItems == 0)
            {
                m_Config.m_Items.RemoveAll();
                m_LstInputItems.SetItemCount(0);
            }

            this->UpdateStatusBar();
        }
    }

    void CMainDlg::OnEditCheckSelected()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            int nItems = m_Config.m_Items.Count();
            if (nItems > 0)
            {
                for (int i = 0; i < nItems; i++)
                {
                    if (this->IsItemSelected(i))
                    {
                        config::CItem& item = m_Config.m_Items.Get(i);
                        if (item.bChecked == false)
                        {
                            item.bChecked = true;
                            this->RedrawItem(i);
                        }
                    }
                }

                this->UpdateStatusBar();
            }
        }
    }

    void CMainDlg::OnEditUncheckSelected()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            int nItems = m_Config.m_Items.Count();
            if (nItems > 0)
            {
                for (int i = 0; i < nItems; i++)
                {
                    if (this->IsItemSelected(i))
                    {
                        config::CItem& item = m_Config.m_Items.Get(i);
                        if (item.bChecked == true)
                        {
                            item.bChecked = false;
                            this->RedrawItem(i);
                        }
                    }
                }
                this->UpdateStatusBar();
            }
        }
    }

    void CMainDlg::OnEditToggleSelected()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            int nItems = m_Config.m_Items.Count();
            if (nItems > 0)
            {
                for (int i = 0; i < nItems; i++)
                {
                    if (this->IsItemSelected(i))
                    {
                        config::CItem& item = m_Config.m_Items.Get(i);
                        item.bChecked = !item.bChecked;
                        this->RedrawItem(i);
                    }
                }
                this->UpdateStatusBar();
            }
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
            int nItems = m_Config.m_Items.Count();
            if (nItems > 0)
            {
                for (int i = 0; i < nItems; i++)
                {
                    if (this->IsItemSelected(i))
                        this->DeselectItem(i);
                    else
                        this->SelectItem(i);
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
                config::CItem& item = m_Config.m_Items.Get(nItem);
                config::CPath& path = item.m_Paths.Get(0);
                util::LaunchAndWait(path.szPath, L"", FALSE);
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
                config::CItem& item = m_Config.m_Items.Get(nItem);
                config::CPath& path = item.m_Paths.Get(0);
                util::LaunchAndWait(util::GetFilePath(path.szPath), L"", FALSE);
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
            int nSelectedTool = 0;
            int nFormat = this->m_CmbFormat.GetCurSel();
            if (nFormat >= 0)
            {
    #if defined(_WIN32) & !defined(_WIN64)
                const std::wstring szPlatform = L"x86";
    #else
                const std::wstring szPlatform = L"x64";
    #endif
                config::CFormat& format = m_Config.m_Formats.Get(nFormat);
                int nTool = m_Config.m_Tools.GetToolByFormatAndPlatform(format.szId, szPlatform);
                if (nTool >= 0)
                    nSelectedTool = nTool;
            }

            CToolsDlg dlg;
            dlg.pConfig = &m_Config;
            dlg.nSelectedTool = nSelectedTool;
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

    void CMainDlg::OnOptionsDownloadTools()
    {
        if (this->pWorkerContext->bRunning == false)
        {
            BOOL bChecked = this->GetMenu()->GetMenuState(ID_OPTIONS_DOWNLOAD_TOOLS, MF_BYCOMMAND) == MF_CHECKED;
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DOWNLOAD_TOOLS, (bChecked == TRUE) ? MF_UNCHECKED : MF_CHECKED);
            this->m_Config.m_Options.bTryToDownloadTools = !bChecked;
        }
    }

    void CMainDlg::OnLanguageDefault()
    {
    }

    void CMainDlg::OnLanguageChange(UINT nID)
    {
        int nSelectedLanguage = nID - ID_LANGUAGE_MIN;
        lang::CLanguage& language = m_Config.m_Language.m_Languages.Get(nSelectedLanguage);
        m_Config.m_Options.szSelectedLanguage = language.szId;
        m_Config.m_Language.pLanguage = &language;

        CMenu *m_hMenu = this->GetMenu();
        CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);

        int nLanguages = m_Config.m_Language.m_Languages.Count();
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
            util::LaunchAndWait(_T("https://github.com/wieslawsoltes/BatchEncoder"), _T(""), FALSE);
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
            wsprintf(cTempBuf, _T("%s\\*.xml\0"), szFile);

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

                    xml::XmlDocumnent doc;
                    if (xml::XmlDoc::Open(std::wstring(CT2CW(szFileXml)), doc) == true)
                    {
                        std::string szName = xml::XmlDoc::GetRootName(doc);
                        if (util::StringHelper::CompareNoCase(szName, "Language"))
                        {
                            lang::CLanguage language;

                            xml::CXmlConfig::LoadLanguage(doc, language);

                            this->m_Config.m_Language.m_Languages.Insert(language);
                        }
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
            m_StatusBar.SetText(m_Config.GetString(0x0021000B).c_str(), 1, 0);
        }
        return true;
    }

    void CMainDlg::InitLanguageMenu()
    {
        CMenu *m_hMenu = this->GetMenu();
        CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);

        int nLanguages = m_Config.m_Language.m_Languages.Count();
        if (nLanguages > 0 && nLanguages <= (ID_LANGUAGE_MAX - ID_LANGUAGE_MIN))
        {
            m_hLangMenu->DeleteMenu(ID_LANGUAGE_DEFAULT, 0);

            for (int i = 0; i < nLanguages; i++)
            {
                lang::CLanguage language = m_Config.m_Language.m_Languages.Get(i);

                CString szBuff;
                szBuff.Format(_T("%s (%s)"), language.szOriginalName, language.szTranslatedName);

                UINT nLangID = ID_LANGUAGE_MIN + i;
                m_hLangMenu->AppendMenu(MF_STRING, nLangID, szBuff);
                m_hLangMenu->CheckMenuItem(nLangID, MF_UNCHECKED);
            }

            int nSelectedLanguage = m_Config.m_Language.m_Languages.GetLanguageById(m_Config.m_Options.szSelectedLanguage);
            if (nSelectedLanguage >= 0)
            {
                m_hLangMenu->CheckMenuItem(ID_LANGUAGE_MIN + nSelectedLanguage, MF_CHECKED);

                lang::CLanguage& language = m_Config.m_Language.m_Languages.Get(nSelectedLanguage);
                m_Config.m_Language.pLanguage = &language;
            }
            else
            {
                m_hLangMenu->CheckMenuItem(ID_LANGUAGE_MIN, MF_CHECKED);

                lang::CLanguage& language = m_Config.m_Language.m_Languages.Get(0);
                m_Config.m_Options.szSelectedLanguage = language.szId;
                m_Config.m_Language.pLanguage = &language;
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
        lang::CLanguageHelper helper(&m_Config.m_Language);
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
        helper.SetMenuItemText(m_hMenu, ID_OPTIONS_DOWNLOAD_TOOLS, 0x0004000F);
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

    void CMainDlg::GetItems()
    {
        int nItems = this->m_LstInputItems.GetItemCount();
        for (int i = 0; i < nItems; i++)
        {
            config::CItem& item = m_Config.m_Items.Get(i);
            item.nId = i;
        }
    }

    void CMainDlg::SetItems()
    {
        int nItems = m_Config.m_Items.Count();
        m_LstInputItems.SetItemCount(nItems);
    }

    void CMainDlg::GetOptions()
    {
        // option: SelectedFormat
        m_Config.m_Options.nSelectedFormat = this->m_CmbFormat.GetCurSel();

        // option: OutputPath
        CString szOutputPath;
        m_CmbOutPath.GetWindowText(szOutputPath);
        m_Config.m_Options.szOutputPath = szOutputPath;

        // option: DeleteSourceFiles
        m_Config.m_Options.bDeleteSourceFiles = this->GetMenu()->GetMenuState(ID_OPTIONS_DELETE_SOURCE, MF_BYCOMMAND) == MF_CHECKED;

        // option: RecurseChecked
        m_Config.m_Options.bRecurseChecked = app::bRecurseChecked;

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

        // option: TryToDownloadTools
        m_Config.m_Options.bTryToDownloadTools = this->GetMenu()->GetMenuState(ID_OPTIONS_DOWNLOAD_TOOLS, MF_BYCOMMAND) == MF_CHECKED;

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

        m_Config.m_Options.szFileListColumns =
            std::to_wstring(nColWidth[0]) + L" " +
            std::to_wstring(nColWidth[1]) + L" " +
            std::to_wstring(nColWidth[2]) + L" " +
            std::to_wstring(nColWidth[3]) + L" " +
            std::to_wstring(nColWidth[4]) + L" " +
            std::to_wstring(nColWidth[5]) + L" " +
            std::to_wstring(nColWidth[6]) + L" " +
            std::to_wstring(nColWidth[7]);
    }

    void CMainDlg::SetOptions()
    {
        // option: SelectedFormat

        // option: OutputPath
        if (!m_Config.m_Options.szOutputPath.empty())
        {
            this->m_CmbOutPath.SetWindowText(m_Config.m_Options.szOutputPath.c_str());
            szLastBrowse = m_Config.m_Options.szOutputPath.c_str();
        }
        else
        {
            m_Config.m_Options.szOutputPath = app::m_App.szSettingsPath;
            szLastBrowse = m_Config.m_Options.szOutputPath.c_str();
            this->m_CmbOutPath.SetWindowText(m_Config.m_Options.szOutputPath.c_str());
        }

        // option: DeleteSourceFiles
        if (m_Config.m_Options.bDeleteSourceFiles)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, MF_CHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DELETE_SOURCE, MF_UNCHECKED);

        // option: RecurseChecked
        if (m_Config.m_Options.bRecurseChecked)
            app::bRecurseChecked = true;
        else
            app::bRecurseChecked = false;

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

        // option: TryToDownloadTools
        if (m_Config.m_Options.bTryToDownloadTools)
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DOWNLOAD_TOOLS, MF_CHECKED);
        else
            this->GetMenu()->CheckMenuItem(ID_OPTIONS_DOWNLOAD_TOOLS, MF_UNCHECKED);

        // option: ThreadCount
        CString szThreadCount;
        szThreadCount.Format(_T("%d\0"), m_Config.m_Options.nThreadCount);
        m_EdtThreads.SetWindowText(szThreadCount);

        // option: MainWindowResize
        if (!m_Config.m_Options.szMainWindowResize.empty())
        {
            this->SetWindowRectStr(m_Config.m_Options.szMainWindowResize.c_str());
        }

        // option: FileListColumns
        if (!m_Config.m_Options.szFileListColumns.empty())
        {
            auto widths = util::StringHelper::Split(m_Config.m_Options.szFileListColumns.c_str(), ' ');
            if (widths.size() == 8)
            {
                for (int i = 0; i < 8; i++)
                {
                    std::wstring szWidth = widths[i];
                    int nWidth = util::StringHelper::ToInt(szWidth);
                    m_LstInputItems.SetColumnWidth(i, nWidth);
                }
            }
        }

        // option: PresetsDialogResize

        // option: PresetsListColumns

        // option: FormatsDialogResize

        // option: FormatsListColumns

        // option: ToolsDialogResize

        // option: ToolsListColumns
    }

    bool CMainDlg::IsItemSelected(int nItem)
    {
        return m_LstInputItems.GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED;
    }

    void CMainDlg::SelectItem(int nItem)
    {
        m_LstInputItems.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    }

    void CMainDlg::DeselectItem(int nItem)
    {
        m_LstInputItems.SetItemState(nItem, 0, LVIS_SELECTED);
    }

    void CMainDlg::MakeItemVisible(int nItem)
    {
        m_LstInputItems.EnsureVisible(nItem, FALSE);
    }

    void CMainDlg::ToggleItem(int nItem)
    {
        config::CItem& item = m_Config.m_Items.Get(nItem);
        item.bChecked = !item.bChecked;
        this->RedrawItem(nItem);
    }

    int CMainDlg::AddToItems(const std::wstring& szPath)
    {
        int nFormat = this->m_CmbFormat.GetCurSel();
        int nPreset = this->m_CmbPresets.GetCurSel();

        std::wstring szFormatId = L"";
        if (m_Config.m_Formats.Count() > 0)
        {
            if (m_Config.m_Options.bTryToFindDecoder == true)
            {
                int nDecoder = m_Config.m_Formats.GetDecoderByExtension(util::GetFileExtension(szPath));
                if (nDecoder == -1)
                {
                    config::CFormat &format = m_Config.m_Formats.Get(nFormat);
                    szFormatId = format.szId;
                }
                else
                {
                    config::CFormat &format = m_Config.m_Formats.Get(nDecoder);
                    szFormatId = format.szId;
                    nPreset = format.nDefaultPreset;
                }
            }
            else
            {
                config::CFormat &format = m_Config.m_Formats.Get(nFormat);
                szFormatId = format.szId;
            }
        }

        ULONGLONG nFileSize = util::GetFileSize64(szPath);
        CString szFileSize;
        szFileSize.Format(_T("%I64d"), nFileSize);

        config::CItem item;
        config::CPath path;
        path.szPath = szPath;
        path.szSize = szFileSize;
        item.m_Paths.Insert(path);
        item.szSize = szFileSize;
        item.szName = util::GetOnlyFileName(szPath);
        item.szExtension = util::StringHelper::ToUpper(util::GetFileExtension(szPath));
        item.szFormatId = szFormatId;
        item.nPreset = nPreset;
        item.bChecked = true;

        m_Config.m_Items.Insert(item);

        return (int)m_Config.m_Items.Count() - 1;
    }

    void CMainDlg::RedrawItem(int nItem)
    {
        m_LstInputItems.RedrawItems(nItem, nItem);
    }

    void CMainDlg::RedrawItem(int nStart, int nEnd)
    {
        m_LstInputItems.RedrawItems(nStart, nEnd);
    }

    bool CMainDlg::AddToList(const std::wstring& szPath)
    {
        if (m_Config.m_Options.bValidateInputFiles == true)
        {
            std::wstring szExt = util::GetFileExtension(szPath);
            if (m_Config.m_Formats.IsValidInputExtension(szExt) == false)
                return false;
        }

        int nItem = this->AddToItems(szPath);
        if (nItem == -1)
            return false;

        this->RedrawItem(nItem);

        return true;
    }

    void CMainDlg::ShowEdtItem()
    {
        CRect rect;
        m_LstInputItems.GetSubItemRect(nEdtItem, nEdtSubItem, LVIR_BOUNDS, rect);
        rect.InflateRect(-6, -2, -2, -2);

        m_EdtItem.SetWindowText(szEdtText);
        m_EdtItem.MoveWindow(rect, TRUE);
        m_EdtItem.SetSel(0, szEdtText.GetLength(), TRUE);
        m_EdtItem.ShowWindow(SW_SHOW);
        m_EdtItem.ShowCaret();
        m_EdtItem.SetFocus();
    }

    void CMainDlg::UpdateEdtItem(BOOL bUpdateText)
    {
        if (m_EdtItem.IsWindowVisible())
        {
            if (bUpdateText == TRUE)
            {
                m_EdtItem.GetWindowText(szEdtText);

                if (nEdtSubItem == ITEM_COLUMN_OPTIONS)
                {
                    config::CItem& item = m_Config.m_Items.Get(nEdtItem);
                    item.szOptions = szEdtText;
                    this->RedrawItem(nEdtItem);
                }
            }

            m_EdtItem.HideCaret();
            m_EdtItem.ShowWindow(SW_HIDE);
            m_LstInputItems.SetFocus();
        }
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
                    this->SetItems();
                }
                else
                {
                    std::wstring szPath = szFile;
                    std::wstring szExt = util::GetFileExtension(szPath);

                    if (util::StringHelper::CompareNoCase(szExt, L"xml"))
                    {
                        xml::XmlDocumnent doc;
                        std::string szName = xml::CXmlConfig::GetRootName(szPath, doc);
                        if (!szName.empty())
                        {
                            if (util::StringHelper::CompareNoCase(szName, "Items"))
                            {
                                this->LoadItems(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Formats"))
                            {
                                this->LoadFormats(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Format"))
                            {
                                this->LoadFormat(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Presets"))
                            {
                                this->LoadPresets(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Options"))
                            {
                                this->LoadOptions(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Tools"))
                            {
                                this->LoadTools(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Tool"))
                            {
                                this->LoadTool(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Language"))
                            {
                                this->LoadLanguage(doc);
                            }
                        }
                    }
                    else if (util::StringHelper::CompareNoCase(szExt, L"exe"))
                    {
                        // Set current format exe path.
                        int nFormat = this->m_CmbFormat.GetCurSel();
                        if (nFormat != -1)
                        {
                            config::CFormat& format = m_Config.m_Formats.Get(nFormat);
                            format.szPath = szPath;
                        }
                    }
                    else if (util::StringHelper::CompareNoCase(szExt, L"lua"))
                    {
                        // Set current format progress path.
                        int nFormat = this->m_CmbFormat.GetCurSel();
                        if (nFormat != -1)
                        {
                            config::CFormat& format = m_Config.m_Formats.Get(nFormat);
                            format.szFunction = szPath;
                        }
                    }
                    else
                    {
                        this->AddToList(szPath);
                        this->SetItems();
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
                    this->AddToList(std::wstring(CT2CW(szPath)));
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
            m_StatusBar.SetText(m_Config.GetString(0x0021000C).c_str(), 1, 0);
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
                config::CFormat& format = m_Config.m_Formats.Get(i);
                this->m_CmbFormat.InsertString(i, format.szName.c_str());
            }

            static bool bResizeFormatsComboBox = false;
            if (bResizeFormatsComboBox == false)
            {
                util::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_FORMAT, 15);
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
                config::CFormat& format = m_Config.m_Formats.Get(m_Config.m_Options.nSelectedFormat);
                int nPresets = format.m_Presets.Count();
                for (int i = 0; i < nPresets; i++)
                {
                    config::CPreset& preset = format.m_Presets.Get(i);
                    this->m_CmbPresets.InsertString(i, preset.szName.c_str());
                }

                nPreset = format.nDefaultPreset;
            }

            static bool bResizePresetsComboBox = false;
            if (bResizePresetsComboBox == false)
            {
                util::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRESETS, 15);
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
            config::CFormat& format = m_Config.m_Formats.Get(nFormat);
            config::CPreset& preset = format.m_Presets.Get(nPreset);
            int nItems = m_Config.m_Items.Count();

            if (nItems > 0)
            {
                int nSelected = 0;
                for (int i = 0; i < nItems; i++)
                {
                    if (this->IsItemSelected(i))
                    {
                        config::CItem& item = m_Config.m_Items.Get(i);
                        item.szFormatId = format.szId;
                        item.nPreset = nPreset;
                        this->RedrawItem(i);
                        nSelected++;
                    }
                }

                if (nSelected == 0)
                {
                    for (int i = 0; i < nItems; i++)
                    {
                        config::CItem& item = m_Config.m_Items.Get(i);
                        item.szFormatId = format.szId;
                        item.nPreset = nPreset;
                    }

                    this->RedrawItem(0, nItems - 1);
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
        int nItems = m_Config.m_Items.Count();
        if (nItems > 0)
        {
            std::wstring szDefaultTime = m_Config.GetString(0x00150001);
            int nSelected = 0;

            for (int i = 0; i < nItems; i++)
            {
                if (this->IsItemSelected(i))
                {
                    config::CItem& item = m_Config.m_Items.Get(i);
                    item.szTime = szDefaultTime;
                    this->RedrawItem(i);
                    nSelected++;
                }
            }

            if (nSelected == 0)
            {
                for (int i = 0; i < nItems; i++)
                {
                    config::CItem& item = m_Config.m_Items.Get(i);
                    item.szTime = szDefaultTime;
                }
                this->RedrawItem(0, nItems - 1);
            }
        }
    }

    void CMainDlg::ResetConvertionStatus()
    {
        int nItems = m_Config.m_Items.Count();
        if (nItems > 0)
        {
            std::wstring szDefaultStatus = m_Config.GetString(0x00210001);
            int nSelected = 0;

            for (int i = 0; i < nItems; i++)
            {
                if (this->IsItemSelected(i))
                {
                    config::CItem& item = m_Config.m_Items.Get(i);
                    item.szStatus = szDefaultStatus;
                    this->RedrawItem(i);
                    nSelected++;
                }
            }

            if (nSelected == 0)
            {
                for (int i = 0; i < nItems; i++)
                {
                    config::CItem& item = m_Config.m_Items.Get(i);
                    item.szStatus = szDefaultStatus;
                }
                this->RedrawItem(0, nItems - 1);
            }
        }
    }

    void CMainDlg::UpdateStatusBar()
    {
        int nItems = m_Config.m_Items.Count();
        if (nItems > 0)
        {
            CString szText;
            szText.Format(_T("%d %s"), nItems, (nItems > 1) ? m_Config.GetString(0x00210003) : m_Config.GetString(0x00210002));
            m_StatusBar.SetText(szText, 0, 0);
        }
        else
        {
            m_StatusBar.SetText(m_Config.GetString(0x00210004).c_str(), 0, 0);
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

            ::SetCurrentDirectory(app::m_App.szSettingsPath.c_str());

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

            std::wstring szDefaultTime = m_Config.GetString(0x00150001);
            std::wstring szDefaultStatus = m_Config.GetString(0x00210001);

            for (int i = 0; i < nItems; i++)
            {
                config::CItem& item = this->m_Config.m_Items.Get(i);
                if (item.bChecked == true)
                {
                    item.szTime = szDefaultTime;
                    item.szStatus = szDefaultStatus;
                    this->RedrawItem(i);
                    nChecked++;
                }
            }

            if (nChecked <= 0)
            {
                bSafeCheck = false;
                this->pWorkerContext->bDone = true;
                return;
            }

            worker::COutputPath m_Output;
            std::wstring szOutput = this->m_Config.m_Options.szOutputPath;
            if (m_Output.Validate(szOutput) == false)
            {
                m_StatusBar.SetText(m_Config.GetString(0x0021000F).c_str(), 1, 0);
                bSafeCheck = false;
                this->pWorkerContext->bDone = true;
                return;
            }

            if (this->m_WorkerThread.Start(
                [this]() 
                { 
                    this->m_Worker.Convert(this->pWorkerContext);
                    this->m_WorkerThread.Close();
                },
                true) == false)
            {
                m_StatusBar.SetText(m_Config.GetString(0x0021000E).c_str(), 1, 0);
                this->pWorkerContext->bDone = true;
                bSafeCheck = false;
                return;
            }

            this->EnableUserInterface(FALSE);

            m_StatusBar.SetText(_T(""), 1, 0);
            m_BtnConvert.SetWindowText(m_Config.GetString(0x000A0018).c_str());
            this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, m_Config.GetString(0x00030003).c_str());

            this->pWorkerContext->bRunning = true;
            this->m_WorkerThread.Resume();

            bSafeCheck = false;
        }
        else
        {
            bSafeCheck = true;

            m_BtnConvert.SetWindowText(m_Config.GetString(0x000A0017).c_str());
            this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, m_Config.GetString(0x00030002).c_str());
            this->EnableUserInterface(TRUE);

            this->pWorkerContext->bRunning = false;
            bSafeCheck = false;
        }
    }

    void CMainDlg::FinishConvert()
    {
        this->m_BtnConvert.SetWindowText(m_Config.GetString(0x000A0017).c_str());
        this->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, m_Config.GetString(0x00030002).c_str());
        this->EnableUserInterface(TRUE);

        this->m_Progress.SetPos(0);
        this->pWorkerContext->bRunning = false;

        if (this->m_Config.m_Options.bShutdownWhenFinished == true)
        {
            if (this->m_Config.m_Options.bDoNotSaveConfiguration == false)
            {
                try
                {
                    this->SaveFormats(app::m_App.szFormatsFile.c_str());
                    this->SaveOptions(app::m_App.szOptionsFile.c_str());
                    this->SaveItems(app::m_App.szItemsFile.c_str());
                }
                catch (...) {}
            }

            util::ShutdownWindows();
        }
    }

    bool CMainDlg::LoadOptions(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Options"))
        {
            return this->LoadOptions(doc);
        }
        return false;
    }

    bool CMainDlg::LoadOptions(xml::XmlDocumnent &doc)
    {
        config::COptions options;
        if (xml::CXmlConfig::LoadOptions(doc, options))
        {
            this->m_Config.m_Options = options;
            this->SetOptions();
            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
            return true;
        }
        return false;
    }

    bool CMainDlg::SaveOptions(const std::wstring& szFileXml)
    {
        this->GetOptions();
        return xml::CXmlConfig::SaveOptions(szFileXml, this->m_Config.m_Options);
    }

    bool CMainDlg::LoadFormats(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Formats"))
        {
            return this->LoadFormats(doc);
        }
        return false;
    }

    bool CMainDlg::LoadFormats(xml::XmlDocumnent &doc)
    {
        config::CFormatsList formats;
        if (xml::CXmlConfig::LoadFormats(doc, formats))
        {
            this->m_Config.m_Formats = formats;
            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
            return true;
        }
        return false;
    }

    bool CMainDlg::SaveFormats(const std::wstring& szFileXml)
    {
        return xml::CXmlConfig::SaveFormats(szFileXml, this->m_Config.m_Formats);
    }

    bool CMainDlg::LoadFormat(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Format"))
        {
            return this->LoadFormat(doc);
        }
        return false;
    }

    bool CMainDlg::LoadFormat(xml::XmlDocumnent &doc)
    {
        config::CFormat format;
        if (xml::CXmlConfig::LoadFormat(doc, format))
        {
            m_Config.m_Formats.Insert(format);
            this->UpdateFormatComboBox();
            this->UpdatePresetComboBox();
            return true;
        }
        return false;
    }

    bool CMainDlg::SaveFormat(const std::wstring& szFileXml)
    {
        int nFormat = this->m_CmbFormat.GetCurSel();
        if (nFormat != -1)
        {
            config::CFormat& format = m_Config.m_Formats.Get(nFormat);
            return xml::CXmlConfig::SaveFormat(szFileXml, format);
        }
        return false;
    }

    bool CMainDlg::LoadPresets(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Presets"))
        {
            return this->LoadPresets(doc);
        }
        return false;
    }

    bool CMainDlg::LoadPresets(xml::XmlDocumnent &doc)
    {
        config::CPresetsList presets;
        if (xml::CXmlConfig::LoadPresets(doc, presets))
        {
            int nFormat = this->m_CmbFormat.GetCurSel();
            if (nFormat != -1)
            {
                config::CFormat& format = m_Config.m_Formats.Get(nFormat);
                format.m_Presets = presets;
                this->UpdatePresetComboBox();
                return true;
            }
        }
        return false;
    }

    bool CMainDlg::SavePresets(const std::wstring& szFileXml)
    {
        int nFormat = this->m_CmbFormat.GetCurSel();
        if (nFormat != -1)
        {
            config::CFormat& format = m_Config.m_Formats.Get(nFormat);
            return xml::CXmlConfig::SavePresets(szFileXml, format.m_Presets);
        }
        return false;
    }

    bool CMainDlg::LoadTools(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Tools"))
        {
            return this->LoadTools(doc);
        }
        return false;
    }

    bool CMainDlg::LoadTools(xml::XmlDocumnent &doc)
    {
        config::CToolsList tools;
        if (xml::CXmlConfig::LoadTools(doc, tools))
        {
            this->m_Config.m_Tools = tools;
            return true;
        }
        return false;
    }

    bool CMainDlg::SaveTools(const std::wstring& szFileXml)
    {
        return xml::CXmlConfig::SaveTools(szFileXml, this->m_Config.m_Tools);
    }

    bool CMainDlg::LoadTool(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Tool"))
        {
            return this->LoadTool(doc);
        }
        return false;
    }

    bool CMainDlg::LoadTool(xml::XmlDocumnent &doc)
    {
        config::CTool tool;
        if (xml::CXmlConfig::LoadTool(doc, tool))
        {
            m_Config.m_Tools.Insert(tool);
            return true;
        }
        return false;
    }

    bool CMainDlg::LoadItems(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Items"))
        {
            return this->LoadItems(doc);
        }
        return false;
    }

    bool CMainDlg::LoadItems(xml::XmlDocumnent &doc)
    {
        config::CItemsList items;
        if (xml::CXmlConfig::LoadItems(doc, items))
        {
            m_LstInputItems.SetItemCount(0);
            this->m_Config.m_Items = items;
            this->SetItems();
            this->UpdateStatusBar();
            return true;
        }
        return false;
    }

    bool CMainDlg::SaveItems(const std::wstring& szFileXml)
    {
        this->GetItems();
        return xml::CXmlConfig::SaveItems(szFileXml, this->m_Config.m_Items);
    }

    bool CMainDlg::LoadLanguage(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Language"))
        {
            return this->LoadLanguage(doc);
        }
        return false;
    }

    bool CMainDlg::LoadLanguage(xml::XmlDocumnent &doc)
    {
        lang::CLanguage language;
        if (xml::CXmlConfig::LoadLanguage(doc, language))
        {
            this->m_Config.m_Language.m_Languages.Insert(language);

            CMenu *m_hMenu = this->GetMenu();
            CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);
            int nLanguages = m_Config.m_Language.m_Languages.Count();

            std::wstring szBuff = language.szOriginalName + L" (" + language.szTranslatedName + L")";

            UINT nLangID = ID_LANGUAGE_MIN + nLanguages - 1;
            m_hLangMenu->AppendMenu(MF_STRING, nLangID, szBuff.c_str());
            m_hLangMenu->CheckMenuItem(nLangID, MF_UNCHECKED);

            return true;
        }
        return false;
    }
}
