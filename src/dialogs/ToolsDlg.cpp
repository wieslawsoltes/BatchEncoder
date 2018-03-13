// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "ToolsDlg.h"

namespace dialogs
{
    int CALLBACK BrowseCallbackExportToolsPath(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
    {
        if (uMsg == BFFM_INITIALIZED)
        {
            CToolsDlg* pDlg = (CToolsDlg*)pData;
            std::wstring szPath = pDlg->szLastToolsBrowse;
            ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath.c_str());
        }
        return(0);
    }

    IMPLEMENT_DYNAMIC(CToolsDlg, CDialog)
    CToolsDlg::CToolsDlg(CWnd* pParent /*=nullptr*/)
        : CMyDialogEx(CToolsDlg::IDD, pParent)
    {
        this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
        this->szToolsDialogResize = L"";
        this->szToolsListColumns = L"";
        this->bUpdate = false;
        this->nSelectedTool = 0;
        m_Downloader.bDownload = false;
    }

    CToolsDlg::~CToolsDlg()
    {
    }

    void CToolsDlg::DoDataExchange(CDataExchange* pDX)
    {
        CMyDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_STATIC_TOOL_NAME, m_StcName);
        DDX_Control(pDX, IDC_STATIC_TOOL_PLATFORM, m_StcPlatform);
        DDX_Control(pDX, IDC_STATIC_TOOL_PRIORITY, m_StcPriority);
        DDX_Control(pDX, IDC_STATIC_TOOL_FORMATS, m_StcFormats);
        DDX_Control(pDX, IDC_STATIC_TOOL_URL, m_StcUrl);
        DDX_Control(pDX, IDC_STATIC_TOOL_FILE, m_StcFile);
        DDX_Control(pDX, IDC_STATIC_TOOL_EXTRACT, m_StcExtract);
        DDX_Control(pDX, IDC_STATIC_TOOL_PATH, m_StcPath);
        DDX_Control(pDX, IDC_LIST_TOOLS, m_LstTools);
        DDX_Control(pDX, IDC_EDIT_TOOL_NAME, m_EdtName);
        DDX_Control(pDX, IDC_EDIT_TOOL_PLATFORM, m_EdtPlatform);
        DDX_Control(pDX, IDC_EDIT_TOOL_PRIORITY, m_EdtPriority);
        DDX_Control(pDX, IDC_SPIN_TOOL_PRIORITY, m_SpinPriority);
        DDX_Control(pDX, IDC_EDIT_TOOL_FORMATS, m_EdtFormats);
        DDX_Control(pDX, IDC_EDIT_TOOL_URL, m_EdtUrl);
        DDX_Control(pDX, IDC_EDIT_TOOL_FILE, m_EdtFile);
        DDX_Control(pDX, IDC_EDIT_TOOL_EXTRACT, m_EdtExtract);
        DDX_Control(pDX, IDC_EDIT_TOOL_PATH, m_EdtPath);
        DDX_Control(pDX, IDOK, m_BtnOK);
        DDX_Control(pDX, IDCANCEL, m_BtnCancel);
        DDX_Control(pDX, IDC_BUTTON_TOOL_IMPORT, m_BtnImport);
        DDX_Control(pDX, IDC_BUTTON_TOOL_EXPORT, m_BtnExport);
        DDX_Control(pDX, IDC_BUTTON_TOOL_DUPLICATE, m_BtnDuplicate);
        DDX_Control(pDX, IDC_BUTTON_TOOL_REMOVE_ALL, m_BtnRemoveAll);
        DDX_Control(pDX, IDC_BUTTON_TOOL_REMOVE, m_BtnRemove);
        DDX_Control(pDX, IDC_BUTTON_TOOL_ADD, m_BtnAdd);
        DDX_Control(pDX, IDC_BUTTON_TOOL_UPDATE, m_BtnUpdate);
        DDX_Control(pDX, IDC_BUTTON_TOOL_DOWNLOAD, m_BtnDownload);
        DDX_Control(pDX, IDC_BUTTON_TOOL_SETFORMAT, m_BtnSetFormat);
        DDX_Control(pDX, IDC_BUTTON_TOOL_SETFORMATX86, m_BtnSetFormatX86);
        DDX_Control(pDX, IDC_BUTTON_TOOL_SETFORMATX64, m_BtnSetFormatX64);
    }

    BEGIN_MESSAGE_MAP(CToolsDlg, CMyDialogEx)
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        ON_WM_DROPFILES()
        ON_BN_CLICKED(IDOK, OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
        ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
        ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TOOLS, OnLvnGetdispinfoListTools)
        ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST_TOOLS, OnOdfindListTools)
        ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOOLS, OnLvnItemchangedListTools)
        ON_EN_CHANGE(IDC_EDIT_TOOL_NAME, OnEnChangeEditToolName)
        ON_EN_CHANGE(IDC_EDIT_TOOL_PLATFORM, OnEnChangeEditToolPlatform)
        ON_EN_CHANGE(IDC_EDIT_TOOL_PRIORITY, OnEnChangeEditToolPriority)
        ON_EN_CHANGE(IDC_EDIT_TOOL_FORMATS, OnEnChangeEditToolFormats)
        ON_EN_CHANGE(IDC_EDIT_TOOL_URL, OnEnChangeEditToolUrl)
        ON_EN_CHANGE(IDC_EDIT_TOOL_FILE, OnEnChangeEditToolFile)
        ON_EN_CHANGE(IDC_EDIT_TOOL_EXTRACT, OnEnChangeEditToolExtract)
        ON_EN_CHANGE(IDC_EDIT_TOOL_PATH, OnEnChangeEditToolPath)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_IMPORT, OnBnClickedButtonImport)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_EXPORT, OnBnClickedButtonExport)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_DUPLICATE, OnBnClickedButtonDuplicate)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_REMOVE_ALL, OnBnClickedButtonRemoveAllTools)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_REMOVE, OnBnClickedButtonRemoveTool)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_ADD, OnBnClickedButtonAddTool)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_UPDATE, OnBnClickedButtonUpdateTool)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_DOWNLOAD, OnBnClickedButtonDownloadSelected)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_SETFORMAT, OnBnClickedButtonToolSetFormat)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_SETFORMATX86, OnBnClickedButtonToolSetFormatX86)
        ON_BN_CLICKED(IDC_BUTTON_TOOL_SETFORMATX64, OnBnClickedButtonToolSetFormatX64)
        ON_WM_CLOSE()
        ON_WM_DESTROY()
    END_MESSAGE_MAP()

    BOOL CToolsDlg::OnInitDialog()
    {
        CMyDialogEx::OnInitDialog();

        InitCommonControls();

        SetIcon(m_hIcon, TRUE);
        SetIcon(m_hIcon, FALSE);

        m_SpinPriority.SetRange32(INT_MIN, INT_MAX);

#ifdef _UNICODE
        m_LstTools.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

        DWORD dwExStyle = m_LstTools.GetExtendedStyle();
        dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
        m_LstTools.SetExtendedStyle(dwExStyle);

        m_LstTools.InsertColumn(TOOL_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 155);
        m_LstTools.InsertColumn(TOOL_COLUMN_PRIORITY, _T("Priority"), LVCFMT_LEFT, 50);
        m_LstTools.InsertColumn(TOOL_COLUMN_URL, _T("Url"), LVCFMT_LEFT, 185);
        m_LstTools.InsertColumn(TOOL_COLUMN_STATUS, _T("Status"), LVCFMT_LEFT, 95);

        this->DragAcceptFiles(TRUE);
        this->LoadWindowSettings();
        this->SetLanguage();
        this->RedrawTools();

        m_LstTools.SetItemState(nSelectedTool, LVIS_SELECTED, LVIS_SELECTED);
        m_LstTools.EnsureVisible(nSelectedTool, FALSE);

        return TRUE;
    }

    void CToolsDlg::OnPaint()
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

    HCURSOR CToolsDlg::OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CToolsDlg::OnDropFiles(HDROP hDropInfo)
    {
        if (m_Downloader.bDownload == true)
            return;

        std::thread m_DropThread = std::thread([&]()
        {
            int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, nullptr, 0);
            if (nCount > 0)
            {
                bool bNewTools = false;
                for (int i = 0; i < nCount; i++)
                {
                    int nReqChars = ::DragQueryFile(hDropInfo, i, nullptr, 0);
                    CString szFile;
                    ::DragQueryFile(hDropInfo, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
                    if (!(::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        std::wstring szPath = szFile;
                        std::wstring szExt = util::GetFileExtension(szPath);
                        if (util::string::CompareNoCase(szExt, L"xml"))
                        {
                            config::xml::XmlDocumnent doc;
                            std::string szName = config::xml::XmlConfig::GetRootName(szPath, doc);
                            if (!szName.empty())
                            {
                                if (util::string::CompareNoCase(szName, "Tool"))
                                {
                                    this->LoadTool(doc);
                                    bNewTools = true;
                                }
                            }
                        }
                    }
                    szFile.ReleaseBuffer();
                }
                if (bNewTools)
                {
                    config::CTool::Sort(m_Tools);
                    this->RedrawTools();
                }
            }
            ::DragFinish(hDropInfo);
        });
        m_DropThread.detach();

        CMyDialogEx::OnDropFiles(hDropInfo);
    }

    void CToolsDlg::OnBnClickedOk()
    {
        if (m_Downloader.bDownload == true)
            return;

        POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
        if (pos != nullptr)
            nSelectedTool = m_LstTools.GetNextSelectedItem(pos);
        else
            nSelectedTool = -1;

        this->SaveWindowSettings();

        OnOK();
    }

    void CToolsDlg::OnBnClickedCancel()
    {
        if (m_Downloader.bDownload == true)
            return;

        this->SaveWindowSettings();

        OnCancel();
    }

    void CToolsDlg::OnLvnItemchangedListTools(NMHDR *pNMHDR, LRESULT *pResult)
    {
        LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

        if (m_Downloader.bDownload == false)
        {
            this->ListSelectionChange();
        }

        *pResult = 0;
    }

    LRESULT CToolsDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
    {
#ifdef _UNICODE
        return NFR_UNICODE;
#else
        return NFR_ANSI;
#endif
    }

    void CToolsDlg::OnLvnGetdispinfoListTools(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
        LV_ITEM* pItem = &(pDispInfo)->item;
        int nItem = pItem->iItem;

        if (pItem->mask & LVIF_TEXT)
        {
            config::CTool& tool = m_Tools[nItem];
            std::wstring szText;

            switch (pItem->iSubItem)
            {
            case TOOL_COLUMN_NAME:
                szText = tool.szName;
                break;
            case TOOL_COLUMN_PRIORITY:
                szText = std::to_wstring(tool.nPriority);
                break;
            case TOOL_COLUMN_URL:
                szText = tool.szUrl;
                break;
            case TOOL_COLUMN_STATUS:
                szText = tool.szStatus;
                break;
            }

            _tcscpy_s(pItem->pszText, pItem->cchTextMax, szText.c_str());
        }

        *pResult = 0;
    }

    void CToolsDlg::OnOdfindListTools(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;
        *pResult = -1;
        if ((pFindInfo->lvfi.flags & LVFI_STRING) == 0)
        {
            return;
        }

        if (m_Downloader.bDownload == true)
            return;

        CString szSearchStr = pFindInfo->lvfi.psz;
        int startPos = pFindInfo->iStart;
        if (startPos >= m_LstTools.GetItemCount())
            startPos = 0;

        int currentPos = startPos;
        do
        {
            config::CTool& tool = m_Tools[currentPos];
            if (_tcsnicmp(tool.szName.c_str(), szSearchStr, szSearchStr.GetLength()) == 0)
            {
                *pResult = currentPos;
                break;
            }
            currentPos++;
            if (currentPos >= m_LstTools.GetItemCount())
                currentPos = 0;
        } while (currentPos != startPos);
    }

    void CToolsDlg::OnBnClickedButtonImport()
    {
        if (m_Downloader.bDownload == true)
            return;

        int nSize = (768 * (MAX_PATH + 1)) + 1;
        auto buffer = std::make_unique<TCHAR[]>(nSize);

        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310010).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("xml"), _T(""),
            OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        fd.m_ofn.lpstrFile = buffer.get();
        fd.m_ofn.nMaxFile = nSize;

        if (fd.DoModal() == IDOK)
        {
            bool bNewTools = false;
            POSITION pos = fd.GetStartPosition();
            do
            {
                std::wstring szFilePath = fd.GetNextPathName(pos);
                if (!szFilePath.empty())
                {
                    this->LoadTool(szFilePath);
                    bNewTools = true;
                }
            } while (pos != nullptr);

            if (bNewTools)
            {
                config::CTool::Sort(m_Tools);
                this->RedrawTools();
            }
        }
    }

    void CToolsDlg::OnBnClickedButtonExport()
    {
        if (m_Downloader.bDownload == true)
            return;

        int nCount = m_LstTools.GetItemCount();
        int nSelectedItems = 0;
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                    nSelectedItems++;
            }
        }

        if (nSelectedItems == 1)
        {
            POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
            if (pos != nullptr)
            {
                int nSelected = m_LstTools.GetNextSelectedItem(pos);
                if (nSelected >= 0)
                {
                    config::CTool& tool = m_Tools[nSelected];

                    CString szFilter;
                    szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
                        pConfig->GetString(0x00310010).c_str(),
                        pConfig->GetString(0x00310001).c_str());

                    CFileDialog fd(FALSE, _T("xml"), tool.szName.c_str(),
                        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
                        szFilter, this);

                    if (fd.DoModal() == IDOK)
                    {
                        std::wstring szFileXml = fd.GetPathName();
                        this->SaveTool(szFileXml, tool);
                    }
                }
            }
        }
        else if (nSelectedItems > 1)
        {
            LPMALLOC pMalloc;
            BROWSEINFO bi;
            LPITEMIDLIST pidlDesktop;
            LPITEMIDLIST pidlBrowse;
            TCHAR *lpBuffer;

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

            static std::wstring szTitle;
            szTitle = pConfig->GetString(0x0021000A);

            bi.hwndOwner = this->GetSafeHwnd();
            bi.pidlRoot = pidlDesktop;
            bi.pszDisplayName = lpBuffer;
            bi.lpszTitle = szTitle.c_str();
            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
            bi.iImage = 0;
            bi.lpfn = BrowseCallbackExportToolsPath;
            bi.lParam = reinterpret_cast<LPARAM>(this);

            pidlBrowse = ::SHBrowseForFolder(&bi);
            if (pidlBrowse != nullptr)
            {
                if (::SHGetPathFromIDList(pidlBrowse, lpBuffer))
                {
                    this->szLastToolsBrowse.Format(_T("%s\0"), lpBuffer);
                    std::wstring szPath = std::wstring(lpBuffer);
                    this->SaveTools(szPath);
                }
                pMalloc->Free(pidlBrowse);
            }

            pMalloc->Free(pidlDesktop);
            pMalloc->Free(lpBuffer);
            pMalloc->Release();
        }
    }

    void CToolsDlg::OnBnClickedButtonDuplicate()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nSelected = m_LstTools.GetNextSelectedItem(pos);
            if (nSelected >= 0)
            {
                config::CTool& tool = m_Tools[nSelected];
                config::CTool copy = tool;

                while (config::CTool::IsUniqueName(m_Tools, copy.szName) == false)
                {
                    copy.szName += pConfig->GetString(0x00240005);
                }

                m_Tools.insert(m_Tools.begin() + nSelected + 1, copy);

                m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstTools.SetItemState(nSelected + 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstTools.EnsureVisible(nSelected + 1, FALSE);
            }
        }

        bUpdate = false;

        this->RedrawTools();
        this->ListSelectionChange();
    }

    void CToolsDlg::OnBnClickedButtonRemoveAllTools()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (m_Tools.size() > 0)
        {
            m_Tools = std::vector<config::CTool>();
            nSelectedTool = -1;
            this->RedrawTools();
            this->ListSelectionChange();
        }
    }

    void CToolsDlg::OnBnClickedButtonRemoveTool()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        bUpdate = true;

        int nItemLastRemoved = -1;
        int nItems = m_LstTools.GetItemCount();
        if (nItems <= 0)
        {
            bUpdate = false;
            return;
        }

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                m_Tools.erase(m_Tools.begin() + i);
                nItemLastRemoved = i;
            }
        }

        this->RedrawTools();

        m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);

        nItems = m_LstTools.GetItemCount();
        if (nItemLastRemoved != -1)
        {
            if (nItemLastRemoved < nItems && nItems >= 0)
            {
                m_LstTools.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
                m_LstTools.EnsureVisible(nItemLastRemoved, FALSE);
            }
            else if (nItemLastRemoved >= nItems && nItems >= 0)
            {
                m_LstTools.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstTools.EnsureVisible(nItemLastRemoved, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CToolsDlg::OnBnClickedButtonAddTool()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        bUpdate = true;

        config::CTool tool;
        tool.szName = pConfig->GetString(0x00240004);
        tool.szPlatform = L"";
        tool.nPriority = -1;
        tool.szFormats = L"";
        tool.szUrl = L"";
        tool.szFile = L"";
        tool.szExtract = L"";
        tool.szPath = L"";
        tool.szStatus = L"";

        while (config::CTool::IsUniqueName(m_Tools, tool.szName) == false)
        {
            tool.szName += pConfig->GetString(0x00240005);
        }
        std::wstring szNewName = tool.szName;

        m_Tools.emplace_back(tool);

        config::CTool::Sort(m_Tools);
        this->RedrawTools();

        size_t nSelectedItem = config::CTool::GetToolByName(m_Tools, szNewName);
        m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
        m_LstTools.SetItemState(nSelectedItem, LVIS_SELECTED, LVIS_SELECTED);
        m_LstTools.EnsureVisible(nSelectedItem, FALSE);

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CToolsDlg::OnBnClickedButtonUpdateTool()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        if (m_LstTools.m_hWnd == nullptr)
            return;

        bUpdate = true;

        POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstTools.GetNextSelectedItem(pos);
            CString szName = _T("");
            CString szPlatform = _T("");
            CString szPriority = _T("");
            CString szFormats = _T("");
            CString szUrl = _T("");
            CString szFile = _T("");
            CString szExtract = _T("");
            CString szPath = _T("");

            this->m_EdtName.GetWindowText(szName);
            this->m_EdtPlatform.GetWindowText(szPlatform);
            this->m_EdtPriority.GetWindowText(szPriority);
            this->m_EdtFormats.GetWindowText(szFormats);
            this->m_EdtUrl.GetWindowText(szUrl);
            this->m_EdtFile.GetWindowText(szFile);
            this->m_EdtExtract.GetWindowText(szExtract);
            this->m_EdtPath.GetWindowText(szPath);

            config::CTool& tool = m_Tools[nItem];
            std::wstring szNewName = szName;

            tool.szName = szName;
            tool.szPlatform = szPlatform;
            tool.nPriority = _tstoi(szPriority);
            tool.szFormats = szFormats;
            tool.szUrl = szUrl;
            tool.szFile = szFile;
            tool.szExtract = szExtract;
            tool.szPath = szPath;

            config::CTool::Sort(m_Tools);
            this->RedrawTools();

            size_t nSelectedItem = config::CTool::GetToolByName(m_Tools, szNewName);
            m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstTools.SetItemState(nSelectedItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nSelectedItem, FALSE);
        }

        bUpdate = false;
    }

    void CToolsDlg::OnEnChangeEditToolName()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolPlatform()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolPriority()
    {
        if (m_Downloader.bDownload == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolFormats()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolUrl()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolFile()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolExtract()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnEnChangeEditToolPath()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateTool();
    }

    void CToolsDlg::OnBnClickedButtonDownloadSelected()
    {
        if (bUpdate == true)
            return;

        if (m_Downloader.bDownload == true)
        {
            bAbort = true;
            return;
        }

        bAbort = false;
        m_Downloader.bDownload = true;

        int nCount = m_LstTools.GetItemCount();
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                    m_Tools[i].szStatus = L"";
            }
            this->RedrawTools();

            m_Thread = std::thread([this]() 
            {
                this->DownloadTools();
            });
            m_Thread.detach();
        }
    }

    void CToolsDlg::OnBnClickedButtonToolSetFormat()
    {
        if (bUpdate == true)
            return;

        auto filter = [this](int nIndex, config::CTool& tool) -> bool
        {
            return (this->m_LstTools.GetItemState(nIndex, LVIS_SELECTED) == LVIS_SELECTED);
        };
        config::CConfig::SetFormatPaths(m_Formats, m_Tools, filter);
    }

    void CToolsDlg::OnBnClickedButtonToolSetFormatX86()
    {
        if (bUpdate == true)
            return;

        config::CConfig::SetFormatPaths(m_Formats, m_Tools, L"x86");
    }

    void CToolsDlg::OnBnClickedButtonToolSetFormatX64()
    {
        if (bUpdate == true)
            return;

        config::CConfig::SetFormatPaths(m_Formats, m_Tools, L"x64");
    }

    void CToolsDlg::OnClose()
    {
        this->SaveWindowSettings();

        CMyDialogEx::OnClose();
    }

    void CToolsDlg::OnDestroy()
    {
        CMyDialogEx::OnDestroy();

        if (m_Downloader.bDownload == true)
        {
            bAbort = true;
            m_Downloader.bDownload = false;
        }
    }

    void CToolsDlg::LoadWindowSettings()
    {
        if (!szToolsDialogResize.empty())
            this->SetWindowRectStr(szToolsDialogResize.c_str());

        if (!szToolsListColumns.empty())
        {
            auto widths = util::string::Split(szToolsListColumns.c_str(), ' ');
            if (widths.size() == 4)
            {
                for (int i = 0; i < 4; i++)
                {
                    std::wstring szWidth = widths[i];
                    int nWidth = util::string::ToInt(szWidth);
                    m_LstTools.SetColumnWidth(i, nWidth);
                }
            }
        }
    }

    void CToolsDlg::SaveWindowSettings()
    {
        this->szToolsDialogResize = this->GetWindowRectStr();

        int nColWidth[4];
        for (int i = 0; i < 4; i++)
            nColWidth[i] = m_LstTools.GetColumnWidth(i);

        szToolsListColumns =
            std::to_wstring(nColWidth[0]) + L" " +
            std::to_wstring(nColWidth[1]) + L" " +
            std::to_wstring(nColWidth[2]) + L" " +
            std::to_wstring(nColWidth[3]);
    }

    void CToolsDlg::SetLanguage()
    {
        app::CLanguageHelper helper(pConfig);

        helper.SetColumnText(m_LstTools, TOOL_COLUMN_NAME, 0x000E0001);
        helper.SetColumnText(m_LstTools, TOOL_COLUMN_PRIORITY, 0x000E0002);
        helper.SetColumnText(m_LstTools, TOOL_COLUMN_URL, 0x000E0003);
        helper.SetColumnText(m_LstTools, TOOL_COLUMN_STATUS, 0x000E0004);

        helper.SetWndText(this, 0x000E0010);
        helper.SetWndText(&m_BtnCancel, 0x000E0011);
        helper.SetWndText(&m_StcName, 0x000E0012);
        helper.SetWndText(&m_StcPlatform, 0x000E0013);
        helper.SetWndText(&m_StcPriority, 0x000E002A);
        helper.SetWndText(&m_StcFormats, 0x000E0014);
        helper.SetWndText(&m_StcUrl, 0x000E0015);
        helper.SetWndText(&m_StcFile, 0x000E0016);
        helper.SetWndText(&m_StcExtract, 0x000E0017);
        helper.SetWndText(&m_StcPath, 0x000E0018);
        helper.SetWndText(&m_BtnImport, 0x000E001B);
        helper.SetWndText(&m_BtnExport, 0x000E001C);
        helper.SetWndText(&m_BtnDuplicate, 0x000E001D);
        helper.SetWndText(&m_BtnRemoveAll, 0x000E001E);
        helper.SetWndText(&m_BtnRemove, 0x000E001F);
        helper.SetWndText(&m_BtnAdd, 0x000E0020);
        helper.SetWndText(&m_BtnDownload, 0x000E0023);
        helper.SetWndText(&m_BtnSetFormat, 0x000E0025);
        helper.SetWndText(&m_BtnSetFormatX86, 0x000E0026);
        helper.SetWndText(&m_BtnSetFormatX64, 0x000E0027);
        helper.SetWndText(&m_BtnUpdate, 0x000E0028);
        helper.SetWndText(&m_BtnOK, 0x000E0029);
    }

    void CToolsDlg::RedrawTool(int nId)
    {
        this->m_LstTools.RedrawItems(nId, nId);
    }

    void CToolsDlg::RedrawTools()
    {
        this->m_LstTools.RedrawItems(0, m_Tools.size() - 1);
        this->m_LstTools.SetItemCount(m_Tools.size());
    }

    void CToolsDlg::UpdateFields(config::CTool &tool)
    {
        this->m_EdtName.SetWindowText(tool.szName.c_str());
        this->m_EdtPlatform.SetWindowText(tool.szPlatform.c_str());
        this->m_EdtPriority.SetWindowText(std::to_wstring(tool.nPriority).c_str());
        this->m_EdtFormats.SetWindowText(tool.szFormats.c_str());
        this->m_EdtUrl.SetWindowText(tool.szUrl.c_str());
        this->m_EdtFile.SetWindowText(tool.szFile.c_str());
        this->m_EdtExtract.SetWindowText(tool.szExtract.c_str());
        this->m_EdtPath.SetWindowText(tool.szPath.c_str());
    }

    void CToolsDlg::ListSelectionChange()
    {
        if (m_Downloader.bDownload == true)
            return;

        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstTools.GetNextSelectedItem(pos);
            if (nItem >= 0)
            {
                config::CTool& tool = this->m_Tools[nItem];
                this->UpdateFields(tool);
            }
        }
        else
        {
            this->m_EdtName.SetWindowText(_T(""));
            this->m_EdtPlatform.SetWindowText(_T(""));
            this->m_EdtPriority.SetWindowText(_T(""));
            this->m_EdtFormats.SetWindowText(_T(""));
            this->m_EdtUrl.SetWindowText(_T(""));
            this->m_EdtFile.SetWindowText(_T(""));
            this->m_EdtExtract.SetWindowText(_T(""));
            this->m_EdtPath.SetWindowText(_T(""));
        }

        bUpdate = false;
    }

    void CToolsDlg::EnableUserInterface(BOOL bEnable)
    {
        CMenu* pSysMenu = GetSystemMenu(FALSE);
        if (bEnable == FALSE)
            pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
        else
            pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

        this->m_EdtName.EnableWindow(bEnable);
        this->m_EdtPlatform.EnableWindow(bEnable);
        this->m_EdtFormats.EnableWindow(bEnable);
        this->m_EdtPriority.EnableWindow(bEnable);
        this->m_SpinPriority.EnableWindow(bEnable);
        this->m_EdtUrl.EnableWindow(bEnable);
        this->m_EdtFile.EnableWindow(bEnable);
        this->m_EdtExtract.EnableWindow(bEnable);
        this->m_EdtPath.EnableWindow(bEnable);
        this->m_BtnOK.EnableWindow(bEnable);
        this->m_BtnCancel.EnableWindow(bEnable);
        this->m_BtnImport.EnableWindow(bEnable);
        this->m_BtnExport.EnableWindow(bEnable);
        this->m_BtnDuplicate.EnableWindow(bEnable);
        this->m_BtnRemoveAll.EnableWindow(bEnable);
        this->m_BtnRemove.EnableWindow(bEnable);
        this->m_BtnAdd.EnableWindow(bEnable);
        this->m_BtnUpdate.EnableWindow(bEnable);
        this->m_BtnSetFormat.EnableWindow(bEnable);
        this->m_BtnSetFormatX86.EnableWindow(bEnable);
        this->m_BtnSetFormatX64.EnableWindow(bEnable);
    }

    void CToolsDlg::DownloadTools()
    {
        m_Downloader.bDownload = true;
        EnableUserInterface(FALSE);

        util::SetCurrentDirectory_(pConfig->m_Settings.szToolsPath);

        app::CLanguageHelper helper(pConfig);
        helper.SetWndText(&m_BtnDownload, 0x000E0024);

        int nCount = m_LstTools.GetItemCount();
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                {
                    m_LstTools.EnsureVisible(i, FALSE);
                    config::CTool& tool = m_Tools[i];
                    auto callback = [this](int nIndex, std::wstring szStatus) -> bool
                    {
                        this->m_Tools[nIndex].szStatus = szStatus;
                        this->RedrawTool(nIndex);
                        return this->bAbort;
                    };
                    m_Downloader.Download(pConfig, tool, true, true, i, callback);
                }
            }
        }

        helper.SetWndText(&m_BtnDownload, 0x000E0023);
        EnableUserInterface(TRUE);

        m_Downloader.bDownload = false;
    }

    bool CToolsDlg::LoadTool(const std::wstring& szFileXml)
    {
        config::xml::XmlDocumnent doc;
        std::string szName = config::xml::XmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::string::CompareNoCase(szName, "Tool"))
        {
            return this->LoadTool(doc);
        }
        return false;
    }

    bool CToolsDlg::LoadTool(config::xml::XmlDocumnent &doc)
    {
        config::CTool tool;
        if (config::xml::XmlConfig::LoadTool(doc, tool))
        {
            m_Tools.emplace_back(tool);
            return true;
        }
        return false;
    }

    bool CToolsDlg::SaveTool(const std::wstring& szFileXml, config::CTool &tool)
    {
        return config::xml::XmlConfig::SaveTool(szFileXml, tool);
    }

    bool CToolsDlg::SaveTools(const std::wstring& szPath)
    {
        int nCount = m_LstTools.GetItemCount();
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                {
                    config::CTool& tool = m_Tools[i];
                    std::wstring path = util::CombinePath(szPath, tool.szName + L".xml");
                    if (this->SaveTool(path, tool) == false)
                        return false;
                }
            }
        }
        return true;
    }
}
