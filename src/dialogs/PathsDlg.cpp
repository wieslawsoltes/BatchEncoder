// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "PathsDlg.h"

namespace dialogs
{
    IMPLEMENT_DYNAMIC(CPathsDlg, CDialog)
    CPathsDlg::CPathsDlg(CWnd* pParent /*=nullptr*/)
        : CMyDialogEx(CPathsDlg::IDD, pParent)
    {
        this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
        this->szPathsDialogResize = L"";
        this->szPathsListColumns = L"";
        this->bUpdate = false;
    }

    CPathsDlg::~CPathsDlg()
    {
    }

    void CPathsDlg::DoDataExchange(CDataExchange* pDX)
    {
        CMyDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_STATIC_PATH_PATH, m_StcPath);
        DDX_Control(pDX, IDC_STATIC_PATH_SIZE, m_StcSize);
        DDX_Control(pDX, IDC_LIST_PATHS, m_LstPaths);
        DDX_Control(pDX, IDC_EDIT_PATH_PATH, m_EdtPath);
        DDX_Control(pDX, IDC_EDIT_PATH_SIZE, m_EdtSize);
        DDX_Control(pDX, IDOK, m_BtnOK);
        DDX_Control(pDX, IDCANCEL, m_BtnCancel);
        DDX_Control(pDX, IDC_BUTTON_PATH_DUPLICATE, m_BtnDuplicate);
        DDX_Control(pDX, IDC_BUTTON_PATH_REMOVE_ALL, m_BtnRemoveAll);
        DDX_Control(pDX, IDC_BUTTON_PATH_REMOVE, m_BtnRemove);
        DDX_Control(pDX, IDC_BUTTON_PATH_ADD, m_BtnAdd);
        DDX_Control(pDX, IDC_BUTTON_PATH_UP, m_BtnMoveUp);
        DDX_Control(pDX, IDC_BUTTON_PATH_DOWN, m_BtnMoveDown);
        DDX_Control(pDX, IDC_BUTTON_PATH_UPDATE, m_BtnUpdate);
        DDX_Control(pDX, IDC_BUTTON_PATH_IMPORT, m_BtnImport);
        DDX_Control(pDX, IDC_BUTTON_PATH_EXPORT, m_BtnExport);
    }

    BEGIN_MESSAGE_MAP(CPathsDlg, CMyDialogEx)
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        ON_WM_DROPFILES()
        ON_BN_CLICKED(IDOK, OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
        ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
        ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_PATHS, OnLvnGetdispinfoListPaths)
        ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST_PATHS, OnOdfindListPaths)
        ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PATHS, OnLvnItemchangedListPaths)
        ON_BN_CLICKED(IDC_BUTTON_PATH_DUPLICATE, OnBnClickedButtonDuplicate)
        ON_BN_CLICKED(IDC_BUTTON_PATH_REMOVE_ALL, OnBnClickedButtonRemoveAllPaths)
        ON_BN_CLICKED(IDC_BUTTON_PATH_REMOVE, OnBnClickedButtonRemovePath)
        ON_BN_CLICKED(IDC_BUTTON_PATH_ADD, OnBnClickedButtonAddPath)
        ON_BN_CLICKED(IDC_BUTTON_PATH_UP, OnBnClickedButtonPathUp)
        ON_BN_CLICKED(IDC_BUTTON_PATH_DOWN, OnBnClickedButtonPathDown)
        ON_BN_CLICKED(IDC_BUTTON_PATH_UPDATE, OnBnClickedButtonUpdatePath)
        ON_EN_CHANGE(IDC_EDIT_PATH_PATH, OnEnChangeEditPathPath)
        ON_EN_CHANGE(IDC_EDIT_PATH_SIZE, OnEnChangeEditPathSize)
        ON_BN_CLICKED(IDC_BUTTON_PATH_IMPORT, OnBnClickedButtonImportPaths)
        ON_BN_CLICKED(IDC_BUTTON_PATH_EXPORT, OnBnClickedButtonExportPaths)
        ON_WM_CLOSE()
    END_MESSAGE_MAP()

    BOOL CPathsDlg::OnInitDialog()
    {
        CMyDialogEx::OnInitDialog();

        InitCommonControls();

        SetIcon(m_hIcon, TRUE);
        SetIcon(m_hIcon, FALSE);

#ifdef _UNICODE
        m_LstPaths.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

        DWORD dwExStyle = m_LstPaths.GetExtendedStyle();
        dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
        m_LstPaths.SetExtendedStyle(dwExStyle);

        m_LstPaths.InsertColumn(PATH_COLUMN_PATH, _T("Path"), LVCFMT_LEFT, 390);
        m_LstPaths.InsertColumn(PATH_COLUMN_SIZE, _T("Size (bytes)"), LVCFMT_LEFT, 100);

        this->DragAcceptFiles(TRUE);
        this->LoadWindowSettings();
        this->SetLanguage();
        this->RedrawPaths();

        return TRUE;
    }

    void CPathsDlg::OnPaint()
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

    HCURSOR CPathsDlg::OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CPathsDlg::OnDropFiles(HDROP hDropInfo)
    {
        static volatile bool bSafeCheck = false;
        static HDROP hDropInfoLocal;
        static bool bNewPaths;

        if (bSafeCheck == true)
            return;

        hDropInfoLocal = hDropInfo;
        bNewPaths = false;

        static auto HandleFile = [&](const std::wstring& file)
        {
            std::wstring szExt = util::GetFileExtension(file);
            if (util::string::CompareNoCase(szExt, L"xml"))
            {
                config::xml::XmlDocumnent doc;
                std::string szName = config::xml::XmlConfig::GetRootName(file, doc);
                if (!szName.empty())
                {
                    if (util::string::CompareNoCase(szName, "Paths"))
                    {
                        this->LoadPaths(doc);
                        bNewPaths = true;
                    }
                }
            }
            else
            {
                if (this->AddToList(file) == true)
                    bNewPaths = true;
            }
        };

        static auto HandleFinish = [&]()
        {
            if (bNewPaths)
            {
                this->RedrawPaths();
            }
        };

        auto m_DropThread = std::thread([&]()
        {
            if (bSafeCheck == true)
                return;

            bSafeCheck = true;

            int nCount = ::DragQueryFile(hDropInfoLocal, (UINT)0xFFFFFFFF, nullptr, 0);
            if (nCount > 0)
            {
                for (int i = 0; i < nCount; i++)
                {
                    int nReqChars = ::DragQueryFile(hDropInfoLocal, i, nullptr, 0);
                    CString szFile;
                    ::DragQueryFile(hDropInfoLocal, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
                    if (::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        std::wstring szPath = szFile;
                        std::vector<std::wstring> files;
                        if (util::FindFiles(szPath, files, true) == true)
                        {
                            for (auto& file : files)
                                HandleFile(file);
                        }
                    }
                    else
                    {
                        std::wstring file = szFile;
                        HandleFile(file);
                    }
                    szFile.ReleaseBuffer();
                }
                HandleFinish();
            }
            ::DragFinish(hDropInfoLocal);

            bSafeCheck = false;
        });
        m_DropThread.detach();

        CMyDialogEx::OnDropFiles(hDropInfoLocal);
    }

    void CPathsDlg::OnBnClickedOk()
    {
        this->SaveWindowSettings();

        OnOK();
    }

    void CPathsDlg::OnBnClickedCancel()
    {
        this->SaveWindowSettings();

        OnCancel();
    }

    void CPathsDlg::OnLvnItemchangedListPaths(NMHDR *pNMHDR, LRESULT *pResult)
    {
        LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

        this->ListSelectionChange();

        *pResult = 0;
    }

    LRESULT CPathsDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
    {
#ifdef _UNICODE
        return NFR_UNICODE;
#else
        return NFR_ANSI;
#endif
    }

    void CPathsDlg::OnLvnGetdispinfoListPaths(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
        LV_ITEM* pItem = &(pDispInfo)->item;
        int nItem = pItem->iItem;

        if (pItem->mask & LVIF_TEXT)
        {
            if (this->m_Paths.size() > 0)
            {
                config::CPath& path = this->m_Paths[nItem];
                std::wstring szText;

                switch (pItem->iSubItem)
                {
                case PATH_COLUMN_PATH:
                    szText = path.szPath;
                    break;
                case PATH_COLUMN_SIZE:
                    szText = std::to_wstring(path.nSize);
                    break;
                }

                _tcscpy_s(pItem->pszText, pItem->cchTextMax, szText.c_str());
            }
        }

        *pResult = 0;
    }

    void CPathsDlg::OnOdfindListPaths(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;
        *pResult = -1;
        if ((pFindInfo->lvfi.flags & LVFI_STRING) == 0)
        {
            return;
        }

        if (this->m_Paths.size() <= 0)
            return;

        CString szSearchStr = pFindInfo->lvfi.psz;
        int startPos = pFindInfo->iStart;
        if (startPos >= m_LstPaths.GetItemCount())
            startPos = 0;

        int currentPos = startPos;
        do
        {
            config::CPath& path = this->m_Paths[currentPos];
            if (_tcsnicmp(path.szPath.c_str(), szSearchStr, szSearchStr.GetLength()) == 0)
            {
                *pResult = currentPos;
                break;
            }
            currentPos++;
            if (currentPos >= m_LstPaths.GetItemCount())
                currentPos = 0;
        } while (currentPos != startPos);
    }

    void CPathsDlg::OnBnClickedButtonDuplicate()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPaths.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nSelected = m_LstPaths.GetNextSelectedItem(pos);
            if (nSelected >= 0)
            {
                config::CPath& path = m_Paths[nSelected];
                config::CPath copy = path;

                m_Paths.insert(m_Paths.begin() + nSelected + 1, copy);

                m_LstPaths.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstPaths.SetItemState(nSelected + 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPaths.EnsureVisible(nSelected + 1, FALSE);
            }
        }

        bUpdate = false;

        this->RedrawPaths();
        this->ListSelectionChange();
    }

    void CPathsDlg::OnBnClickedButtonRemoveAllPaths()
    {
        if (m_Paths.size() > 0)
        {
            this->m_Paths = std::vector<config::CPath>();
            this->RedrawPaths();
            this->ListSelectionChange();
        }
    }

    void CPathsDlg::OnBnClickedButtonRemovePath()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        int nItemLastRemoved = -1;
        int nItems = m_LstPaths.GetItemCount();
        if (nItems <= 0)
            return;

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstPaths.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                m_Paths.erase(m_Paths.begin() + i);
                nItemLastRemoved = i;
            }
        }

        this->RedrawPaths();

        m_LstPaths.SetItemState(-1, 0, LVIS_SELECTED);

        nItems = m_LstPaths.GetItemCount();
        if (nItemLastRemoved != -1)
        {
            if (nItemLastRemoved < nItems && nItems >= 0)
            {
                m_LstPaths.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPaths.EnsureVisible(nItemLastRemoved, FALSE);
            }
            else if (nItemLastRemoved >= nItems && nItems >= 0)
            {
                m_LstPaths.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPaths.EnsureVisible(nItemLastRemoved, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CPathsDlg::OnBnClickedButtonAddPath()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        config::CPath path { L"", 0 };

        m_Paths.emplace_back(path);

        this->RedrawPaths();

        int nItem = m_LstPaths.GetItemCount();
        m_LstPaths.SetItemState(-1, 0, LVIS_SELECTED);
        m_LstPaths.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
        m_LstPaths.EnsureVisible(nItem - 1, FALSE);

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CPathsDlg::OnBnClickedButtonPathUp()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPaths.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPaths.GetNextSelectedItem(pos);
            if (nItem > 0)
            {
                config::CPath& path1 = m_Paths[nItem];
                config::CPath& path2 = m_Paths[nItem - 1];

                std::swap(path1, path2);

                this->RedrawPaths();

                m_LstPaths.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstPaths.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPaths.EnsureVisible(nItem - 1, FALSE);
            }
        }

        bUpdate = false;
    }

    void CPathsDlg::OnBnClickedButtonPathDown()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPaths.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPaths.GetNextSelectedItem(pos);
            int nItems = m_LstPaths.GetItemCount();
            if (nItem != (nItems - 1) && nItem >= 0)
            {
                config::CPath& path1 = m_Paths[nItem];
                config::CPath& path2 = m_Paths[nItem + 1];

                std::swap(path1, path2);

                this->RedrawPaths();

                m_LstPaths.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstPaths.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPaths.EnsureVisible(nItem + 1, FALSE);
            }
        }

        bUpdate = false;
    }

    void CPathsDlg::OnBnClickedButtonUpdatePath()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPaths.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPaths.GetNextSelectedItem(pos);
            CString szPath = _T("");
            CString szSize = _T("");
            this->m_EdtPath.GetWindowText(szPath);
            this->m_EdtSize.GetWindowText(szSize);

            config::CPath& path = m_Paths[nItem];
            path.szPath = szPath;
            path.nSize = std::wcstoull(szSize, nullptr, 10);

            this->RedrawPaths();

            m_LstPaths.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstPaths.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstPaths.EnsureVisible(nItem, FALSE);
        }

        bUpdate = false;
    }

    void CPathsDlg::OnEnChangeEditPathPath()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdatePath();
    }

    void CPathsDlg::OnEnChangeEditPathSize()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdatePath();
    }

    void CPathsDlg::OnBnClickedButtonImportPaths()
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x0031000B).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("xml"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->LoadPaths(szFileXml);
            this->RedrawPaths();
        }
    }

    void CPathsDlg::OnBnClickedButtonExportPaths()
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x0031000B).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(FALSE, _T("xml"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->SavePaths(szFileXml);
        }
    }

    void CPathsDlg::OnClose()
    {
        this->SaveWindowSettings();

        CMyDialogEx::OnClose();
    }

    void CPathsDlg::LoadWindowSettings()
    {
        if (!szPathsDialogResize.empty())
            this->SetWindowRectStr(szPathsDialogResize.c_str());

        if (!szPathsListColumns.empty())
        {
            auto widths = util::string::Split(szPathsListColumns.c_str(), ' ');
            if (widths.size() == 2)
            {
                for (int i = 0; i < 2; i++)
                {
                    std::wstring szWidth = widths[i];
                    int nWidth = util::string::ToInt(szWidth);
                    m_LstPaths.SetColumnWidth(i, nWidth);
                }
            }
        }
    }

    void CPathsDlg::SaveWindowSettings()
    {
        this->szPathsDialogResize = this->GetWindowRectStr();

        int nColWidth[2];
        for (int i = 0; i < 2; i++)
            nColWidth[i] = m_LstPaths.GetColumnWidth(i);

        szPathsListColumns =
            std::to_wstring(nColWidth[0]) + L" " +
            std::to_wstring(nColWidth[1]);
    }

    void CPathsDlg::SetLanguage()
    {
        app::CLanguageHelper helper(pConfig);

        helper.SetColumnText(m_LstPaths, PATH_COLUMN_PATH, 0x000F0001);
        helper.SetColumnText(m_LstPaths, PATH_COLUMN_SIZE, 0x000F0002);

        helper.SetWndText(this, 0x000F0010);
        helper.SetWndText(&m_BtnCancel, 0x000F0011);
        helper.SetWndText(&m_StcPath, 0x000F0012);
        helper.SetWndText(&m_StcSize, 0x000F0013);
        helper.SetWndText(&m_BtnMoveUp, 0x000F0014);
        helper.SetWndText(&m_BtnMoveDown, 0x000F0015);
        helper.SetWndText(&m_BtnDuplicate, 0x000F001D);
        helper.SetWndText(&m_BtnRemoveAll, 0x000F0016);
        helper.SetWndText(&m_BtnRemove, 0x000F0017);
        helper.SetWndText(&m_BtnAdd, 0x000F0018);
        helper.SetWndText(&m_BtnImport, 0x000F0019);
        helper.SetWndText(&m_BtnExport, 0x000F001A);
        helper.SetWndText(&m_BtnUpdate, 0x000F001B);
        helper.SetWndText(&m_BtnOK, 0x000F001C);
    }

    bool CPathsDlg::AddToList(const std::wstring& szPath)
    {
        if (pConfig->m_Options.bValidateInputFiles == true)
        {
            std::wstring szExt = util::GetFileExtension(szPath);
            if (config::CFormat::IsValidInputExtension(pConfig->m_Formats, szExt) == false)
                return false;
        }

        unsigned __int64 nFileSize = pConfig->FileSystem->GetFileSize64(szPath);
        config::CPath path;
        path.szPath = szPath;
        path.nSize = nFileSize;
        m_Paths.emplace_back(std::move(path));

        return true;
    }

    void CPathsDlg::RedrawPath(int nId)
    {
        this->m_LstPaths.RedrawItems(nId, nId);
    }

    void CPathsDlg::RedrawPaths()
    {
        if (this->m_Paths.size() > 0)
        {
            this->m_LstPaths.RedrawItems(0, m_Paths.size() - 1);
            this->m_LstPaths.SetItemCount(m_Paths.size());
        }
        else
        {
            this->m_LstPaths.RedrawItems(0, -1);
            this->m_LstPaths.SetItemCount(0);
        }
    }

    void CPathsDlg::UpdateFields(config::CPath& path)
    {
        this->m_EdtPath.SetWindowText(path.szPath.c_str());
        this->m_EdtSize.SetWindowText(std::to_wstring(path.nSize).c_str());
    }

    void CPathsDlg::ListSelectionChange()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPaths.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPaths.GetNextSelectedItem(pos);
            if (nItem >= 0)
            {
                config::CPath& path = m_Paths[nItem];
                this->UpdateFields(path);
            }
        }
        else
        {
            this->m_EdtPath.SetWindowText(_T(""));
            this->m_EdtSize.SetWindowText(_T(""));
        }

        bUpdate = false;
    }

    bool CPathsDlg::LoadPaths(const std::wstring& szFileXml)
    {
        config::xml::XmlDocumnent doc;
        std::string szName = config::xml::XmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::string::CompareNoCase(szName, "Paths"))
        {
            return this->LoadPaths(doc);
        }
        return false;
    }

    bool CPathsDlg::LoadPaths(config::xml::XmlDocumnent &doc)
    {
        std::vector<config::CPath> paths;
        if (config::xml::XmlConfig::LoadPaths(doc, paths))
        {
            m_Paths = std::move(paths);
            return true;
        }
        return false;
    }

    bool CPathsDlg::SavePaths(const std::wstring& szFileXml)
    {
        return config::xml::XmlConfig::SavePaths(szFileXml, m_Paths);
    }
}
