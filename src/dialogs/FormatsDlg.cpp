// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "FormatsDlg.h"
#include "PresetsDlg.h"

namespace dialogs
{
    int CALLBACK BrowseCallbackExportFormatsPath(HWND hWnd, UINT uMsg, LPARAM lp, LPARAM pData)
    {
        if (uMsg == BFFM_INITIALIZED)
        {
            CFormatsDlg* pDlg = (CFormatsDlg*)pData;
            TCHAR szPath[MAX_PATH + 1] = _T("");
            wsprintf(szPath, _T("%s\0"), pDlg->szLastFormatsBrowse);
            ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)szPath);
        }
        return(0);
    }

    IMPLEMENT_DYNAMIC(CFormatsDlg, CDialog)
    CFormatsDlg::CFormatsDlg(CWnd* pParent /*=nullptr*/)
        : CMyDialogEx(CFormatsDlg::IDD, pParent)
    {
        this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
        this->szFormatsDialogResize = L"";
        this->szFormatsListColumns = L"";
        this->bUpdate = false;
        this->nSelectedFormat = 0;
    }

    CFormatsDlg::~CFormatsDlg()
    {
    }

    void CFormatsDlg::DoDataExchange(CDataExchange* pDX)
    {
        CMyDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_STATIC_GROUP_FORMAT_PIPES, m_GrpPipes);
        DDX_Control(pDX, IDC_STATIC_GROUP_FORMAT_TYPE, m_GrpTypes);
        DDX_Control(pDX, IDC_STATIC_FORMAT_ID, m_StcId);
        DDX_Control(pDX, IDC_STATIC_FORMAT_NAME, m_StcName);
        DDX_Control(pDX, IDC_STATIC_FORMAT_PRIORITY, m_StcPriority);
        DDX_Control(pDX, IDC_STATIC_FORMAT_EXTENSION, m_StcExtension);
        DDX_Control(pDX, IDC_STATIC_FORMAT_FORMATS, m_StcFormats);
        DDX_Control(pDX, IDC_STATIC_FORMAT_CODE, m_StcCode);
        DDX_Control(pDX, IDC_STATIC_FORMAT_DEFAULT, m_StcDefault);
        DDX_Control(pDX, IDC_STATIC_FORMAT_PATH, m_StcPath);
        DDX_Control(pDX, IDC_STATIC_FORMAT_TEMPLATE, m_StcTemplate);
        DDX_Control(pDX, IDC_STATIC_FORMAT_FUNCTION, m_StcProgress);
        DDX_Control(pDX, IDC_LIST_FORMATS, m_LstFormats);
        DDX_Control(pDX, IDC_EDIT_FORMAT_ID, m_EdtId);
        DDX_Control(pDX, IDC_EDIT_FORMAT_NAME, m_EdtName);
        DDX_Control(pDX, IDC_EDIT_FORMAT_PRIORITY, m_EdtPriority);
        DDX_Control(pDX, IDC_SPIN_FORMAT_PRIORITY, m_SpinPriority);
        DDX_Control(pDX, IDC_EDIT_FORMAT_EXTENSION, m_EdtExtension);
        DDX_Control(pDX, IDC_EDIT_FORMAT_FORMATS, m_EdtFormats);
        DDX_Control(pDX, IDC_EDIT_FORMAT_CODE, m_EdtCode);
        DDX_Control(pDX, IDC_COMBO_FORMAT_DEFAULT, m_CmbDefault);
        DDX_Control(pDX, IDC_EDIT_FORMAT_PATH, m_EdtPath);
        DDX_Control(pDX, IDC_EDIT_FORMAT_TEMPLATE, m_EdtTemplate);
        DDX_Control(pDX, IDC_EDIT_FORMAT_FUNCTION, m_EdtFunction);
        DDX_Control(pDX, IDOK, m_BtnOK);
        DDX_Control(pDX, IDCANCEL, m_BtnCancel);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_IMPORT, m_BtnImport);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_EXPORT, m_BtnExport);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_DUPLICATE, m_BtnDuplicate);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_REMOVE_ALL, m_BtnRemoveAll);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_REMOVE, m_BtnRemove);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_ADD, m_BtnAdd);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_UPDATE, m_BtnUpdate);
        DDX_Control(pDX, IDC_BUTTON_EDIT_PRESETS, m_BtnEditPresets);
        DDX_Control(pDX, IDC_BUTTON_BROWSE_PATH, m_BtnBrowsePath);
        DDX_Control(pDX, IDC_BUTTON_BROWSE_FUNCTION, m_BtnBrowseFunction);
    }

    BEGIN_MESSAGE_MAP(CFormatsDlg, CMyDialogEx)
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        ON_WM_DROPFILES()
        ON_BN_CLICKED(IDOK, OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
        ON_MESSAGE(WM_NOTIFYFORMAT, OnNotifyFormat)
        ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FORMATS, OnLvnGetdispinfoListFormats)
        ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST_FORMATS, OnOdfindListFormats)
        ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FORMATS, OnLvnItemchangedListFormats)
        ON_BN_CLICKED(IDC_RADIO_TYPE_ENCODER, OnBnClickedRadioTypeEncoder)
        ON_BN_CLICKED(IDC_RADIO_TYPE_DECODER, OnBnClickedRadioTypeDecoder)
        ON_BN_CLICKED(IDC_CHECK_FORMAT_PIPES_INPUT, OnBnClickedCheckPipesInput)
        ON_BN_CLICKED(IDC_CHECK_FORMAT_PIPES_OUTPUT, OnBnClickedCheckPipesOutput)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_ID, OnEnChangeEditFormatId)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_NAME, OnEnChangeEditFormatName)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_PRIORITY, OnEnChangeEditFormatPriority)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_EXTENSION, OnEnChangeEditFormatExtension)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_FORMATS, OnEnChangeEditFormatFormats)
        ON_CBN_SELCHANGE(IDC_COMBO_FORMAT_DEFAULT, OnCbnSelchangeComboDefault)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_PATH, OnEnChangeEditFormatPath)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_TEMPLATE, OnEnChangeEditFormatTemplate)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_FUNCTION, OnEnChangeEditFormatFunction)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_IMPORT, OnBnClickedButtonImport)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_EXPORT, OnBnClickedButtonExport)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_DUPLICATE, OnBnClickedButtonDuplicate)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_REMOVE_ALL, OnBnClickedButtonRemoveAllFormats)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_REMOVE, OnBnClickedButtonRemoveFormat)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_ADD, OnBnClickedButtonAddFormat)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_UPDATE, OnBnClickedButtonUpdateFormat)
        ON_BN_CLICKED(IDC_BUTTON_EDIT_PRESETS, OnBnClickedButtonEditPresets)
        ON_BN_CLICKED(IDC_BUTTON_BROWSE_PATH, OnBnClickedButtonBrowsePath)
        ON_BN_CLICKED(IDC_BUTTON_BROWSE_FUNCTION, OnBnClickedButtonBrowseProgress)
        ON_WM_CLOSE()
    END_MESSAGE_MAP()

    BOOL CFormatsDlg::OnInitDialog()
    {
        CMyDialogEx::OnInitDialog();

        InitCommonControls();

        SetIcon(m_hIcon, TRUE);
        SetIcon(m_hIcon, FALSE);

        // priority spin
        m_SpinPriority.SetRange32(INT_MIN, INT_MAX);

        // OnNotifyFormat WM_NOTIFYFORMAT
#ifdef _UNICODE
        m_LstFormats.SendMessage(CCM_SETUNICODEFORMAT, (WPARAM)(BOOL)TRUE, 0);
#endif

        // update list style
        DWORD dwExStyle = m_LstFormats.GetExtendedStyle();
        dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
        m_LstFormats.SetExtendedStyle(dwExStyle);

        // insert all ListCtrl columns
        m_LstFormats.InsertColumn(FORMAT_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 195);
        m_LstFormats.InsertColumn(FORMAT_COLUMN_TEMPLATE, _T("Template"), LVCFMT_LEFT, 295);

        // enable drag & drop
        this->DragAcceptFiles(TRUE);

        this->LoadWindowSettings();
        this->SetLanguage();

        this->RedrawFormats();

        // select format
        m_LstFormats.SetItemState(nSelectedFormat, LVIS_SELECTED, LVIS_SELECTED);
        m_LstFormats.EnsureVisible(nSelectedFormat, FALSE);

        return TRUE;
    }

    void CFormatsDlg::OnPaint()
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

    HCURSOR CFormatsDlg::OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CFormatsDlg::OnDropFiles(HDROP hDropInfo)
    {
        std::thread m_DropThread = std::thread([this, hDropInfo]() { this->HandleDropFiles(hDropInfo); });
        m_DropThread.detach();

        CMyDialogEx::OnDropFiles(hDropInfo);
    }

    void CFormatsDlg::OnBnClickedOk()
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
            nSelectedFormat = m_LstFormats.GetNextSelectedItem(pos);
        else
            nSelectedFormat = -1;

        this->SaveWindowSettings();

        OnOK();
    }

    void CFormatsDlg::OnBnClickedCancel()
    {
        this->SaveWindowSettings();

        OnCancel();
    }

    void CFormatsDlg::OnLvnItemchangedListFormats(NMHDR *pNMHDR, LRESULT *pResult)
    {
        LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

        this->ListSelectionChange();

        *pResult = 0;
    }

    LRESULT CFormatsDlg::OnNotifyFormat(WPARAM wParam, LPARAM lParam)
    {
#ifdef _UNICODE
        return NFR_UNICODE;
#else
        return NFR_ANSI;
#endif
    }

    void CFormatsDlg::OnLvnGetdispinfoListFormats(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
        LV_ITEM* pItem = &(pDispInfo)->item;
        int nItem = pItem->iItem;

        if (pItem->mask & LVIF_TEXT)
        {
            config::CFormat& format = m_Formats[nItem];
            std::wstring szText;

            switch (pItem->iSubItem)
            {
            case FORMAT_COLUMN_NAME:
                szText = format.szName;
                break;
            case FORMAT_COLUMN_TEMPLATE:
                szText = format.szTemplate;
                break;
            }

            _tcscpy_s(pItem->pszText, pItem->cchTextMax, szText.c_str());
        }

        *pResult = 0;
    }

    void CFormatsDlg::OnOdfindListFormats(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NMLVFINDITEM* pFindInfo = (NMLVFINDITEM*)pNMHDR;
        *pResult = -1;
        if ((pFindInfo->lvfi.flags & LVFI_STRING) == 0)
        {
            return;
        }

        CString szSearchStr = pFindInfo->lvfi.psz;
        int startPos = pFindInfo->iStart;
        if (startPos >= m_LstFormats.GetItemCount())
            startPos = 0;

        int currentPos = startPos;
        do
        {
            config::CFormat& format = m_Formats[currentPos];
            if (_tcsnicmp(format.szName.c_str(), szSearchStr, szSearchStr.GetLength()) == 0)
            {
                *pResult = currentPos;
                break;
            }
            currentPos++;
            if (currentPos >= m_LstFormats.GetItemCount())
                currentPos = 0;
        } while (currentPos != startPos);
    }

    void CFormatsDlg::OnBnClickedButtonImport()
    {
        std::array<TCHAR, (768*(MAX_PATH+1))+1> buffer { 0 };
        
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310008).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("xml"), _T(""),
            OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        fd.m_ofn.lpstrFile = buffer.data();
        fd.m_ofn.nMaxFile = buffer.size();

        if (fd.DoModal() == IDOK)
        {
            bool bNewFormats = false;
            POSITION pos = fd.GetStartPosition();
            do
            {
                std::wstring szFilePath = fd.GetNextPathName(pos);
                if (!szFilePath.empty())
                {
                    this->LoadFormat(szFilePath);
                    bNewFormats = true;
                }
            } while (pos != nullptr);

            if (bNewFormats)
            {
                config::CFormat::Sort(m_Formats);
                this->RedrawFormats();
            }
        }
    }

    void CFormatsDlg::OnBnClickedButtonExport()
    {
        int nCount = m_LstFormats.GetItemCount();
        int nSelectedItems = 0;
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstFormats.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                    nSelectedItems++;
            }
        }

        if (nSelectedItems == 1)
        {
            POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
            if (pos != nullptr)
            {
                int nSelected = m_LstFormats.GetNextSelectedItem(pos);
                if (nSelected >= 0)
                {
                    config::CFormat& format = m_Formats[nSelected];

                    CString szFilter;
                    szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
                        pConfig->GetString(0x00310008).c_str(),
                        pConfig->GetString(0x00310001).c_str());

                    CFileDialog fd(FALSE, _T("xml"), format.szId.c_str(),
                        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
                        szFilter, this);

                    if (fd.DoModal() == IDOK)
                    {
                        std::wstring szFileXml = fd.GetPathName();
                        this->SaveFormat(szFileXml, format);
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
            bi.lpfn = BrowseCallbackExportFormatsPath;
            bi.lParam = reinterpret_cast<LPARAM>(this);

            pidlBrowse = ::SHBrowseForFolder(&bi);
            if (pidlBrowse != nullptr)
            {
                if (::SHGetPathFromIDList(pidlBrowse, lpBuffer))
                {
                    this->szLastFormatsBrowse.Format(_T("%s\0"), lpBuffer);
                    std::wstring szPath = std::wstring(lpBuffer);
                    this->SaveFormats(szPath);
                }
                pMalloc->Free(pidlBrowse);
            }

            pMalloc->Free(pidlDesktop);
            pMalloc->Free(lpBuffer);
            pMalloc->Release();
        }
    }

    void CFormatsDlg::OnBnClickedButtonDuplicate()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nSelected = m_LstFormats.GetNextSelectedItem(pos);
            if (nSelected >= 0)
            {
                config::CFormat& format = m_Formats[nSelected];
                config::CFormat copy = format;

                m_Formats.insert(m_Formats.begin() + nSelected + 1, copy);

                this->RedrawFormats();

                m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstFormats.SetItemState(nSelected + 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstFormats.EnsureVisible(nSelected + 1, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CFormatsDlg::OnBnClickedButtonRemoveAllFormats()
    {
        if (m_Formats.size() > 0)
        {
            m_Formats = std::vector<config::CFormat>();
            nSelectedFormat = -1;
            this->RedrawFormats();
            this->ListSelectionChange();
        }
    }

    void CFormatsDlg::OnBnClickedButtonRemoveFormat()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        int nItemLastRemoved = -1;
        int nItems = m_LstFormats.GetItemCount();
        if (nItems <= 0)
        {
            bUpdate = false;
            return;
        }

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstFormats.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                m_Formats.erase(m_Formats.begin() + i);
                nItemLastRemoved = i;
            }
        }

        this->RedrawFormats();

        m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);

        nItems = m_LstFormats.GetItemCount();
        if (nItemLastRemoved != -1)
        {
            if (nItemLastRemoved < nItems && nItems >= 0)
            {
                m_LstFormats.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
                m_LstFormats.EnsureVisible(nItemLastRemoved, FALSE);
            }
            else if (nItemLastRemoved >= nItems && nItems >= 0)
            {
                m_LstFormats.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstFormats.EnsureVisible(nItemLastRemoved, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CFormatsDlg::OnBnClickedButtonAddFormat()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        config::CFormat format;
        format.szId = L"ID";
        format.szName = pConfig->GetString(0x00230004);
        format.szTemplate = L"$EXE $OPTIONS $INFILE $OUTFILE";
        format.bPipeInput = true;
        format.bPipeOutput = false;
        format.szFunction = L"- none -";
        format.szPath = L"program.exe";
        format.nExitCodeSuccess = 0;
        format.nType = config::FormatType::Encoder;
        format.nPriority = -1;
        format.szInputExtensions = L"WAV";
        format.szOutputExtension = L"EXT";
        format.nDefaultPreset = 0;

        config::CPreset preset;
        preset.szName = pConfig->GetString(0x00230005);
        preset.szOptions = L"";
        format.m_Presets.emplace_back(preset);

        m_Formats.emplace_back(format);

        config::CFormat::Sort(m_Formats);
        this->RedrawFormats();

        size_t nSelectedItem = config::CFormat::GetFormatById(m_Formats, format.szId);
        m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
        m_LstFormats.SetItemState(nSelectedItem, LVIS_SELECTED, LVIS_SELECTED);
        m_LstFormats.EnsureVisible(nSelectedItem, FALSE);

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CFormatsDlg::OnBnClickedButtonUpdateFormat()
    {
        if (bUpdate == true)
            return;

        if (m_LstFormats.m_hWnd == nullptr)
            return;

        bUpdate = true;

        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);
            CString szId = _T("");
            CString szName = _T("");
            CString szExtension = _T("");
            CString szFormats = _T("");
            int nDefaultPreset = 0;
            CString szTemplate = _T("");
            CString szPath = _T("");
            CString szExitCodeSuccess = _T("");
            config::FormatType nType = config::FormatType::Encoder;
            CString szPriority = _T("");
            bool bInput = false;
            bool bOutput = false;
            CString szFunction = _T("");

            this->m_EdtId.GetWindowText(szId);
            this->m_EdtName.GetWindowText(szName);
            this->m_EdtExtension.GetWindowText(szExtension);
            this->m_EdtFormats.GetWindowText(szFormats);
            this->m_EdtCode.GetWindowText(szExitCodeSuccess);

            nDefaultPreset = this->m_CmbDefault.GetCurSel();

            this->m_EdtName.GetWindowText(szName);
            this->m_EdtTemplate.GetWindowText(szTemplate);

            int nCheckID = this->GetCheckedRadioButton(IDC_RADIO_TYPE_ENCODER, IDC_RADIO_TYPE_DECODER);
            if (nCheckID == IDC_RADIO_TYPE_ENCODER)
                nType = config::FormatType::Encoder;
            else if (nCheckID == IDC_RADIO_TYPE_DECODER)
                nType = config::FormatType::Decoder;

            this->m_EdtPriority.GetWindowText(szPriority);

            if (IsDlgButtonChecked(IDC_CHECK_FORMAT_PIPES_INPUT) == BST_CHECKED)
                bInput = true;

            if (IsDlgButtonChecked(IDC_CHECK_FORMAT_PIPES_OUTPUT) == BST_CHECKED)
                bOutput = true;

            this->m_EdtPath.GetWindowText(szPath);
            this->m_EdtFunction.GetWindowText(szFunction);

            config::CFormat& format = m_Formats[nItem];

            format.szId = szId;
            format.szName = szName;
            format.szOutputExtension = szExtension;
            format.szInputExtensions = szFormats;
            format.nDefaultPreset = nDefaultPreset;
            format.szTemplate = szTemplate;
            format.nExitCodeSuccess = _tstoi(szExitCodeSuccess);
            format.nType = nType;
            format.nPriority = _tstoi(szPriority);
            format.bPipeInput = bInput;
            format.bPipeOutput = bOutput;
            format.szPath = szPath;
            format.szFunction = szFunction;

            this->RedrawFormats();

            //m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
            //m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            //m_LstFormats.EnsureVisible(nItem, FALSE);
        }

        bUpdate = false;
    }

    void CFormatsDlg::OnBnClickedRadioTypeEncoder()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnBnClickedRadioTypeDecoder()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnBnClickedCheckPipesInput()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnBnClickedCheckPipesOutput()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatId()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatName()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatPriority()
    {
        if (bUpdate == true)
            return;

        if (m_LstFormats.m_hWnd == nullptr)
            return;

        bUpdate = true;

        //bool bChangedSelection = false;
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);

            CString szPriority = _T("");
            this->m_EdtPriority.GetWindowText(szPriority);

            int nNewPriority = _tstoi(szPriority);

            config::CFormat& format = m_Formats[nItem];

            bool bSortFormats = nNewPriority != format.nPriority;
            if (bSortFormats)
            {
                format.nPriority = nNewPriority;
                config::CFormat::Sort(m_Formats);
                this->RedrawFormats();

                //size_t nSelectedItem = config::CFormat::GetFormatById(m_Formats, format.szId);
                //m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
                //m_LstFormats.SetItemState(nSelectedItem, LVIS_SELECTED, LVIS_SELECTED);
                //m_LstFormats.EnsureVisible(nSelectedItem, FALSE);
                //bChangedSelection = true;
            }
        }

        bUpdate = false;

        //if (bChangedSelection)
        //{
        //    this->ListSelectionChange();
        //}

        //OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatExtension()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatFormats()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnCbnSelchangeComboDefault()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatPath()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatTemplate()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnEnChangeEditFormatFunction()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdateFormat();
    }

    void CFormatsDlg::OnBnClickedButtonEditPresets()
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);

            CPresetsDlg dlg;
            dlg.pConfig = pConfig;
            dlg.nSelectedFormat = nItem;
            dlg.m_Formats = m_Formats;
            dlg.szPresetsDialogResize = pConfig->m_Options.szPresetsDialogResize;
            dlg.szPresetsListColumns = pConfig->m_Options.szPresetsListColumns;

            INT_PTR nRet = dlg.DoModal();
            if (nRet == IDOK)
            {
                this->m_Formats = std::move(dlg.m_Formats);

                //m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
                //m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
                //m_LstFormats.EnsureVisible(nItem, FALSE);

                this->RedrawFormats();
                this->ListSelectionChange();
            }

            pConfig->m_Options.szPresetsDialogResize = dlg.szPresetsDialogResize;
            pConfig->m_Options.szPresetsListColumns = dlg.szPresetsListColumns;
        }
    }

    void CFormatsDlg::OnBnClickedButtonBrowsePath()
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);
            CString szPath;
            this->m_EdtPath.GetWindowText(szPath);
            BrowseForPath(szPath, &m_EdtPath, nItem);
        }
    }

    void CFormatsDlg::OnBnClickedButtonBrowseProgress()
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);
            CString szFunction;
            this->m_EdtFunction.GetWindowText(szFunction);
            BrowseForFunction(szFunction, &m_EdtFunction, nItem);
        }
    }

    void CFormatsDlg::OnClose()
    {
        this->SaveWindowSettings();

        CMyDialogEx::OnClose();
    }

    void CFormatsDlg::LoadWindowSettings()
    {
        // set window rectangle and position
        if (!szFormatsDialogResize.empty())
            this->SetWindowRectStr(szFormatsDialogResize.c_str());

        // load columns width for FormatsList
        if (!szFormatsListColumns.empty())
        {
            auto widths = util::string::Split(szFormatsListColumns.c_str(), ' ');
            if (widths.size() == 2)
            {
                for (int i = 0; i < 2; i++)
                {
                    std::wstring szWidth = widths[i];
                    int nWidth = util::string::ToInt(szWidth);
                    m_LstFormats.SetColumnWidth(i, nWidth);
                }
            }
        }
    }

    void CFormatsDlg::SaveWindowSettings()
    {
        // save window rectangle and position
        this->szFormatsDialogResize = this->GetWindowRectStr();

        // save columns width from FormatsList
        int nColWidth[2];
        for (int i = 0; i < 2; i++)
            nColWidth[i] = m_LstFormats.GetColumnWidth(i);

        szFormatsListColumns = 
            std::to_wstring(nColWidth[0]) + L" " + 
            std::to_wstring(nColWidth[1]);
    }

    void CFormatsDlg::SetLanguage()
    {
        app::CLanguageHelper helper(pConfig);

        helper.SetColumnText(m_LstFormats, FORMAT_COLUMN_NAME, 0x000C0001);
        helper.SetColumnText(m_LstFormats, FORMAT_COLUMN_TEMPLATE, 0x000C0002);

        helper.SetWndText(this, 0x000C0010);
        helper.SetWndText(&m_BtnCancel, 0x000C0011);
        helper.SetItemText(this, IDC_RADIO_TYPE_ENCODER, 0x000C0012);
        helper.SetItemText(this, IDC_RADIO_TYPE_DECODER, 0x000C0013);
        helper.SetWndText(&m_GrpTypes, 0x000C0014);
        helper.SetItemText(this, IDC_CHECK_FORMAT_PIPES_INPUT, 0x000C0015);
        helper.SetItemText(this, IDC_CHECK_FORMAT_PIPES_OUTPUT, 0x000C0016);
        helper.SetWndText(&m_GrpPipes, 0x000C0017);
        helper.SetWndText(&m_StcId, 0x000C0018);
        helper.SetWndText(&m_StcName, 0x000C0019);
        helper.SetWndText(&m_StcPriority, 0x000C0030);
        helper.SetWndText(&m_StcExtension, 0x000C001A);
        helper.SetWndText(&m_StcFormats, 0x000C001B);
        helper.SetWndText(&m_StcCode, 0x000C002B);
        helper.SetWndText(&m_StcDefault, 0x000C001C);
        helper.SetWndText(&m_StcPath, 0x000C001D);
        helper.SetWndText(&m_BtnEditPresets, 0x000C002C);
        helper.SetWndText(&m_BtnBrowsePath, 0x000C001E);
        helper.SetWndText(&m_StcTemplate, 0x000C001F);
        helper.SetWndText(&m_StcProgress, 0x000C0020);
        helper.SetWndText(&m_BtnBrowseFunction, 0x000C0021);
        helper.SetWndText(&m_BtnImport, 0x000C002D);
        helper.SetWndText(&m_BtnExport, 0x000C002E);
        helper.SetWndText(&m_BtnDuplicate, 0x000C002F);
        helper.SetWndText(&m_BtnRemoveAll, 0x000C0024);
        helper.SetWndText(&m_BtnRemove, 0x000C0025);
        helper.SetWndText(&m_BtnAdd, 0x000C0026);
        helper.SetWndText(&m_BtnUpdate, 0x000C0029);
        helper.SetWndText(&m_BtnOK, 0x000C002A);
    }

    void CFormatsDlg::RedrawFormat(int nId)
    {
        this->m_LstFormats.RedrawItems(nId, nId);
    }

    void CFormatsDlg::RedrawFormats()
    {
        this->m_LstFormats.RedrawItems(0, m_Formats.size() - 1);
        this->m_LstFormats.SetItemCount(m_Formats.size());
    }

    void CFormatsDlg::HandleDropFiles(HDROP hDropInfo)
    {
        int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, nullptr, 0);
        if (nCount > 0)
        {
            bool bNewFormats = false;
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
                            if (util::string::CompareNoCase(szName, "Format"))
                            {
                                this->LoadFormat(doc);
                                bNewFormats = true;
                            }
                            else if (util::string::CompareNoCase(szName, "Presets"))
                            {
                                this->LoadPresets(doc);
                            }
                        }
                    }
                    else if (util::string::CompareNoCase(szExt, L"exe"))
                    {
                        // Set current format exe path.
                        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
                        if (pos != nullptr)
                        {
                            int nItem = m_LstFormats.GetNextSelectedItem(pos);
                            config::CFormat& format = this->m_Formats[nItem];
                            format.szPath = szPath;
                            this->m_EdtPath.SetWindowText(format.szPath.c_str());
                        }
                    }
                    else if (util::string::CompareNoCase(szExt, L"lua"))
                    {
                        // Set current format progress path.
                        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
                        if (pos != nullptr)
                        {
                            int nItem = m_LstFormats.GetNextSelectedItem(pos);
                            config::CFormat& format = this->m_Formats[nItem];
                            format.szFunction = szPath;
                            this->m_EdtFunction.SetWindowText(format.szFunction.c_str());
                        }
                    }
                }

                szFile.ReleaseBuffer();
            }

            if (bNewFormats)
            {
                config::CFormat::Sort(m_Formats);
                this->RedrawFormats();
            }
        }

        ::DragFinish(hDropInfo);
    }

    void CFormatsDlg::UpdateFields(config::CFormat &format)
    {
        this->m_EdtId.SetWindowText(format.szId.c_str());
        this->m_EdtName.SetWindowText(format.szName.c_str());
        this->m_EdtExtension.SetWindowText(format.szOutputExtension.c_str());
        this->m_EdtFormats.SetWindowText(format.szInputExtensions.c_str());
        this->m_EdtCode.SetWindowText(std::to_wstring(format.nExitCodeSuccess).c_str());
        this->m_EdtTemplate.SetWindowText(format.szTemplate.c_str());
        this->m_EdtPath.SetWindowText(format.szPath.c_str());

        switch (format.nType)
        {
        default:
        case config::FormatType::Encoder:
            this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER, IDC_RADIO_TYPE_DECODER, IDC_RADIO_TYPE_ENCODER);
            break;
        case config::FormatType::Decoder:
            this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER, IDC_RADIO_TYPE_DECODER, IDC_RADIO_TYPE_DECODER);
            break;
        };

        this->m_EdtPriority.SetWindowText(std::to_wstring(format.nPriority).c_str());
        this->CheckDlgButton(IDC_CHECK_FORMAT_PIPES_INPUT, format.bPipeInput ? BST_CHECKED : BST_UNCHECKED);
        this->CheckDlgButton(IDC_CHECK_FORMAT_PIPES_OUTPUT, format.bPipeOutput ? BST_CHECKED : BST_UNCHECKED);
        this->m_EdtFunction.SetWindowText(format.szFunction.c_str());
    }

    void CFormatsDlg::UpdateDefaultComboBox(config::CFormat &format)
    {
        this->m_CmbDefault.ResetContent();

        int nPreset = -1;

        size_t nPresets = format.m_Presets.size();
        for (size_t i = 0; i < nPresets; i++)
        {
            config::CPreset& preset = format.m_Presets[i];
            this->m_CmbDefault.InsertString(i, preset.szName.c_str());
        }

        nPreset = format.nDefaultPreset;

        static bool bResizeDefaultComboBox = false;
        if (bResizeDefaultComboBox == false)
        {
            util::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_FORMAT_DEFAULT, 15);
            bResizeDefaultComboBox = true;
        }

        this->m_CmbDefault.SetCurSel(nPreset);
    }

    void CFormatsDlg::ListSelectionChange()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);
            if (nItem >= 0)
            {
                config::CFormat& format = this->m_Formats[nItem];
                this->UpdateFields(format);
                this->UpdateDefaultComboBox(format);
            }
        }
        else
        {
            this->m_EdtId.SetWindowText(_T(""));
            this->m_EdtName.SetWindowText(_T(""));
            this->m_EdtExtension.SetWindowText(_T(""));
            this->m_EdtFormats.SetWindowText(_T(""));
            this->m_EdtCode.SetWindowText(_T(""));

            this->m_CmbDefault.ResetContent();

            this->m_EdtTemplate.SetWindowText(_T(""));
            this->m_EdtPath.SetWindowText(_T(""));
            this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER,
                IDC_RADIO_TYPE_DECODER,
                IDC_RADIO_TYPE_ENCODER);

            this->m_EdtPriority.SetWindowText(_T(""));

            CheckDlgButton(IDC_CHECK_FORMAT_PIPES_INPUT, BST_UNCHECKED);
            CheckDlgButton(IDC_CHECK_FORMAT_PIPES_OUTPUT, BST_UNCHECKED);
            this->m_EdtFunction.SetWindowText(_T(""));
        }

        bUpdate = false;
    }

    bool CFormatsDlg::BrowseForPath(CString szDefaultFName, CEdit *pEdit, int nID)
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.exe)|*.exe|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310006).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("exe"), szDefaultFName,
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            CString szPath = fd.GetPathName();
            pEdit->SetWindowText(szPath);
            return true;
        }
        return false;
    }

    bool CFormatsDlg::BrowseForFunction(CString szDefaultFName, CEdit *pEdit, int nID)
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.lua)|*.lua|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310007).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("lua"), szDefaultFName,
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            CString szPath;
            szPath = fd.GetPathName();
            pEdit->SetWindowText(szPath);
            return true;
        }
        return false;
    }

    bool CFormatsDlg::LoadPresets(config::xml::XmlDocumnent &doc)
    {
        std::vector<config::CPreset> presets;
        if (config::xml::XmlConfig::LoadPresets(doc, presets))
        {
            POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
            if (pos != nullptr)
            {
                int nItem = m_LstFormats.GetNextSelectedItem(pos);
                config::CFormat& format = this->m_Formats[nItem];
                format.m_Presets = std::move(presets);
                this->UpdateDefaultComboBox(format);
                return true;
            }
        }
        return false;
    }

    bool CFormatsDlg::LoadFormat(const std::wstring& szFileXml)
    {
        config::xml::XmlDocumnent doc;
        std::string szName = config::xml::XmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::string::CompareNoCase(szName, "Format"))
        {
            return this->LoadFormat(doc);
        }
        return false;
    }

    bool CFormatsDlg::LoadFormat(config::xml::XmlDocumnent &doc)
    {
        config::CFormat format;
        if (config::xml::XmlConfig::LoadFormat(doc, format))
        {
            m_Formats.emplace_back(format);
            return true;
        }
        return false;
    }

    bool CFormatsDlg::SaveFormat(const std::wstring& szFileXml, config::CFormat &format)
    {
        return config::xml::XmlConfig::SaveFormat(szFileXml, format);
    }

    bool CFormatsDlg::SaveFormats(const std::wstring& szPath)
    {
        int nCount = m_LstFormats.GetItemCount();
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                if (m_LstFormats.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
                {
                    config::CFormat& format = m_Formats[i];
                    std::wstring path = util::CombinePath(szPath, format.szId + L".xml");
                    if (this->SaveFormat(path, format) == false)
                        return false;
                }
            }
        }
        return true;
    }
}
