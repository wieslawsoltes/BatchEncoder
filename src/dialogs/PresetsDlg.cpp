// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "PresetsDlg.h"

namespace dialogs
{
    IMPLEMENT_DYNAMIC(CPresetsDlg, CDialog)
    CPresetsDlg::CPresetsDlg(CWnd* pParent /*=nullptr*/)
        : CMyDialogEx(CPresetsDlg::IDD, pParent)
    {
        this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
        this->szPresetsDialogResize = L"";
        this->szPresetsListColumns = L"";
        this->bUpdate = false;
        this->nSelectedFormat = 0;
    }

    CPresetsDlg::~CPresetsDlg()
    {
    }

    void CPresetsDlg::DoDataExchange(CDataExchange* pDX)
    {
        CMyDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_STATIC_PRESET_NAME, m_StcName);
        DDX_Control(pDX, IDC_STATIC_PRESET_OPTIONS, m_StcOptions);
        DDX_Control(pDX, IDC_LIST_PRESETS, m_LstPresets);
        DDX_Control(pDX, IDC_COMBO_PRESET_FORMAT, m_CmbFormat);
        DDX_Control(pDX, IDC_EDIT_PRESET_NAME, m_EdtName);
        DDX_Control(pDX, IDC_EDIT_PRESET_OPTIONS, m_EdtOptions);
        DDX_Control(pDX, IDOK, m_BtnOK);
        DDX_Control(pDX, IDCANCEL, m_BtnCancel);
        DDX_Control(pDX, IDC_BUTTON_PRESET_DUPLICATE, m_BtnDuplicate);
        DDX_Control(pDX, IDC_BUTTON_PRESET_REMOVE_ALL, m_BtnRemoveAll);
        DDX_Control(pDX, IDC_BUTTON_PRESET_REMOVE, m_BtnRemove);
        DDX_Control(pDX, IDC_BUTTON_PRESET_ADD, m_BtnAdd);
        DDX_Control(pDX, IDC_BUTTON_PRESET_UP, m_BtnMoveUp);
        DDX_Control(pDX, IDC_BUTTON_PRESET_DOWN, m_BtnMoveDown);
        DDX_Control(pDX, IDC_BUTTON_PRESET_UPDATE, m_BtnUpdate);
        DDX_Control(pDX, IDC_BUTTON_PRESET_IMPORT, m_BtnImport);
        DDX_Control(pDX, IDC_BUTTON_PRESET_EXPORT, m_BtnExport);
    }

    BEGIN_MESSAGE_MAP(CPresetsDlg, CMyDialogEx)
        ON_WM_PAINT()
        ON_WM_QUERYDRAGICON()
        ON_WM_DROPFILES()
        ON_BN_CLICKED(IDOK, OnBnClickedOk)
        ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
        ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRESETS, OnLvnItemchangedListPresets)
        ON_CBN_SELCHANGE(IDC_COMBO_PRESET_FORMAT, OnCbnSelchangeComboPresetFormat)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_DUPLICATE, OnBnClickedButtonDuplicate)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_REMOVE_ALL, OnBnClickedButtonRemoveAllPresets)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_REMOVE, OnBnClickedButtonRemovePreset)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_ADD, OnBnClickedButtonAddPreset)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_UP, OnBnClickedButtonPresetUp)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_DOWN, OnBnClickedButtonPresetDown)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_UPDATE, OnBnClickedButtonUpdatePreset)
        ON_EN_CHANGE(IDC_EDIT_PRESET_NAME, OnEnChangeEditPresetName)
        ON_EN_CHANGE(IDC_EDIT_PRESET_OPTIONS, OnEnChangeEditPresetOptions)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_IMPORT, OnBnClickedButtonImportPresets)
        ON_BN_CLICKED(IDC_BUTTON_PRESET_EXPORT, OnBnClickedButtonExportPresets)
        ON_WM_CLOSE()
    END_MESSAGE_MAP()

    BOOL CPresetsDlg::OnInitDialog()
    {
        CMyDialogEx::OnInitDialog();

        InitCommonControls();

        SetIcon(m_hIcon, TRUE);
        SetIcon(m_hIcon, FALSE);

        // update list style
        DWORD dwExStyle = m_LstPresets.GetExtendedStyle();
        dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
        m_LstPresets.SetExtendedStyle(dwExStyle);

        // insert listView columns
        m_LstPresets.InsertColumn(PRESET_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 200);
        m_LstPresets.InsertColumn(PRESET_COLUMN_OPTIONS, _T("Options"), LVCFMT_LEFT, 290);

        // file format combo-box
        size_t nFormats = m_Formats.size();
        for (size_t i = 0; i < nFormats; i++)
        {
            config::CFormat& format = m_Formats[i];
            m_CmbFormat.InsertString(i, format.szName.c_str());
        }

        util::Utilities::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRESET_FORMAT, 15);

        m_CmbFormat.SetCurSel(nSelectedFormat);

        this->OnCbnSelchangeComboPresetFormat();

        // enable drag & drop
        this->DragAcceptFiles(TRUE);

        this->LoadWindowSettings();
        this->SetLanguage();

        return TRUE;
    }

    void CPresetsDlg::OnPaint()
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

    HCURSOR CPresetsDlg::OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CPresetsDlg::OnDropFiles(HDROP hDropInfo)
    {
        std::thread m_DropThread = std::thread([this, hDropInfo]() { this->HandleDropFiles(hDropInfo); });
        m_DropThread.detach();

        CMyDialogEx::OnDropFiles(hDropInfo);
    }

    void CPresetsDlg::OnBnClickedOk()
    {
        this->SaveWindowSettings();

        OnOK();
    }

    void CPresetsDlg::OnBnClickedCancel()
    {
        this->SaveWindowSettings();

        OnCancel();
    }

    void CPresetsDlg::OnLvnItemchangedListPresets(NMHDR *pNMHDR, LRESULT *pResult)
    {
        LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

        this->ListSelectionChange();

        *pResult = 0;
    }

    void CPresetsDlg::OnBnClickedButtonDuplicate()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nSelected = m_LstPresets.GetNextSelectedItem(pos);
            if (nSelected >= 0)
            {
                config::CFormat& format = m_Formats[nSelectedFormat];
                config::CPreset& preset = format.m_Presets[nSelected];
                config::CPreset copy = preset;

                format.m_Presets.insert(format.m_Presets.begin() + nSelected + 1, copy);

                this->m_LstPresets.DeleteAllItems();
                this->InsertPresetsToListCtrl();

                m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstPresets.SetItemState(nSelected + 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPresets.EnsureVisible(nSelected + 1, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CPresetsDlg::OnBnClickedButtonRemoveAllPresets()
    {
        if (m_Formats.size() > 0)
        {
            config::CFormat& format = m_Formats[nSelectedFormat];
            format.m_Presets = std::vector<config::CPreset>();

            m_LstPresets.DeleteAllItems();

            this->ListSelectionChange();
        }
    }

    void CPresetsDlg::OnBnClickedButtonRemovePreset()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        int nItemLastRemoved = -1;
        int nItems = m_LstPresets.GetItemCount();
        if (nItems <= 0)
            return;

        config::CFormat& format = m_Formats[nSelectedFormat];

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstPresets.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                format.m_Presets.erase(format.m_Presets.begin() + i);
                m_LstPresets.DeleteItem(i);
                nItemLastRemoved = i;
            }
        }

        m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);

        nItems = m_LstPresets.GetItemCount();
        if (nItemLastRemoved != -1)
        {
            if (nItemLastRemoved < nItems && nItems >= 0)
            {
                m_LstPresets.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPresets.EnsureVisible(nItemLastRemoved, FALSE);
            }
            else if (nItemLastRemoved >= nItems && nItems >= 0)
            {
                m_LstPresets.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPresets.EnsureVisible(nItemLastRemoved, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CPresetsDlg::OnBnClickedButtonAddPreset()
    {
        if (m_Formats.size() > 0)
        {
            if (bUpdate == true)
                return;

            bUpdate = true;

            int nItem = m_LstPresets.GetItemCount();

            config::CFormat& format = m_Formats[nSelectedFormat];
            config::CPreset preset;
            preset.szName = pConfig->GetString(0x00220004);
            preset.szOptions = L"";
            format.m_Presets.emplace_back(preset);

            AddToList(preset, nItem);

            m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstPresets.EnsureVisible(nItem, FALSE);

            bUpdate = false;

            this->ListSelectionChange();
        }
    }

    void CPresetsDlg::OnBnClickedButtonPresetUp()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPresets.GetNextSelectedItem(pos);
            if (nItem > 0)
            {
                config::CFormat& format = m_Formats[nSelectedFormat];
                config::CPreset& preset1 = format.m_Presets[nItem];
                config::CPreset& preset2 = format.m_Presets[nItem - 1];

                std::swap(preset1, preset2);

                m_LstPresets.SetItemText(nItem, PRESET_COLUMN_NAME, preset1.szName.c_str());
                m_LstPresets.SetItemText(nItem, PRESET_COLUMN_OPTIONS, preset1.szOptions.c_str());
                m_LstPresets.SetItemText(nItem - 1, PRESET_COLUMN_NAME, preset2.szName.c_str());
                m_LstPresets.SetItemText(nItem - 1, PRESET_COLUMN_OPTIONS, preset2.szOptions.c_str());

                m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstPresets.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPresets.EnsureVisible(nItem - 1, FALSE);
            }
        }

        bUpdate = false;
    }

    void CPresetsDlg::OnBnClickedButtonPresetDown()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPresets.GetNextSelectedItem(pos);
            int nItems = m_LstPresets.GetItemCount();
            if (nItem != (nItems - 1) && nItem >= 0)
            {
                config::CFormat& format = m_Formats[nSelectedFormat];
                config::CPreset& preset1 = format.m_Presets[nItem];
                config::CPreset& preset2 = format.m_Presets[nItem + 1];

                std::swap(preset1, preset2);

                m_LstPresets.SetItemText(nItem, PRESET_COLUMN_NAME, preset1.szName.c_str());
                m_LstPresets.SetItemText(nItem, PRESET_COLUMN_OPTIONS, preset1.szOptions.c_str());
                m_LstPresets.SetItemText(nItem + 1, PRESET_COLUMN_NAME, preset2.szName.c_str());
                m_LstPresets.SetItemText(nItem + 1, PRESET_COLUMN_OPTIONS, preset2.szOptions.c_str());

                m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstPresets.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
                m_LstPresets.EnsureVisible(nItem + 1, FALSE);
            }
        }

        bUpdate = false;
    }

    void CPresetsDlg::OnBnClickedButtonUpdatePreset()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPresets.GetNextSelectedItem(pos);
            CString szName = _T("");
            CString szOptions = _T("");
            this->m_EdtName.GetWindowText(szName);
            this->m_EdtOptions.GetWindowText(szOptions);

            config::CFormat& format = m_Formats[nSelectedFormat];
            config::CPreset& preset = format.m_Presets[nItem];
            preset.szName = szName;
            preset.szOptions = szOptions;

            m_LstPresets.SetItemText(nItem, PRESET_COLUMN_NAME, szName);
            m_LstPresets.SetItemText(nItem, PRESET_COLUMN_OPTIONS, szOptions);

            m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstPresets.EnsureVisible(nItem, FALSE);
        }

        bUpdate = false;
    }

    void CPresetsDlg::OnCbnSelchangeComboPresetFormat()
    {
        m_LstPresets.DeleteAllItems();

        nSelectedFormat = this->m_CmbFormat.GetCurSel();

        this->InsertPresetsToListCtrl();

        if (this->m_Formats.size() > 0)
        {
            config::CFormat& format = this->m_Formats[nSelectedFormat];

            m_LstPresets.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstPresets.SetItemState(format.nDefaultPreset, LVIS_SELECTED, LVIS_SELECTED);
            m_LstPresets.EnsureVisible(format.nDefaultPreset, FALSE);

            CString szName = this->m_LstPresets.GetItemText(format.nDefaultPreset, PRESET_COLUMN_NAME);
            CString szOptions = this->m_LstPresets.GetItemText(format.nDefaultPreset, PRESET_COLUMN_OPTIONS);

            this->m_EdtName.SetWindowText(szName);
            this->m_EdtOptions.SetWindowText(szOptions);
        }
    }

    void CPresetsDlg::OnEnChangeEditPresetName()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdatePreset();
    }

    void CPresetsDlg::OnEnChangeEditPresetOptions()
    {
        if (bUpdate == true)
            return;

        OnBnClickedButtonUpdatePreset();
    }

    void CPresetsDlg::OnBnClickedButtonImportPresets()
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310004).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("xml"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->LoadPresets(szFileXml);
        }
    }

    void CPresetsDlg::OnBnClickedButtonExportPresets()
    {
        config::CFormat& format = this->m_Formats[this->nSelectedFormat];

        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310004).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(FALSE, _T("xml"), format.szId.c_str(),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->SavePresets(szFileXml, format);
        }
    }

    void CPresetsDlg::OnClose()
    {
        this->SaveWindowSettings();

        CMyDialogEx::OnClose();
    }

    void CPresetsDlg::LoadWindowSettings()
    {
        // set window rectangle and position
        if (!szPresetsDialogResize.empty())
            this->SetWindowRectStr(szPresetsDialogResize.c_str());

        // load columns width for PresetsList
        if (!szPresetsListColumns.empty())
        {
            auto widths = util::string::Split(szPresetsListColumns.c_str(), ' ');
            if (widths.size() == 2)
            {
                for (int i = 0; i < 2; i++)
                {
                    std::wstring szWidth = widths[i];
                    int nWidth = util::string::ToInt(szWidth);
                    m_LstPresets.SetColumnWidth(i, nWidth);
                }
            }
        }
    }

    void CPresetsDlg::SaveWindowSettings()
    {
        // save window rectangle and position
        this->szPresetsDialogResize = this->GetWindowRectStr();

        // save columns width from PresetsList
        int nColWidth[2];
        for (int i = 0; i < 2; i++)
            nColWidth[i] = m_LstPresets.GetColumnWidth(i);

        szPresetsListColumns =
            std::to_wstring(nColWidth[0]) + L" " +
            std::to_wstring(nColWidth[1]);
    }

    void CPresetsDlg::SetLanguage()
    {
        app::CLanguageHelper helper(pConfig);

        helper.SetColumnText(m_LstPresets, PRESET_COLUMN_NAME, 0x000B0001);
        helper.SetColumnText(m_LstPresets, PRESET_COLUMN_OPTIONS, 0x000B0002);

        helper.SetWndText(this, 0x000B0010);
        helper.SetWndText(&m_BtnCancel, 0x000B0011);
        helper.SetWndText(&m_StcName, 0x000B0012);
        helper.SetWndText(&m_StcOptions, 0x000B0013);
        helper.SetWndText(&m_BtnMoveUp, 0x000B0014);
        helper.SetWndText(&m_BtnMoveDown, 0x000B0015);
        helper.SetWndText(&m_BtnDuplicate, 0x000B001D);
        helper.SetWndText(&m_BtnRemoveAll, 0x000B0016);
        helper.SetWndText(&m_BtnRemove, 0x000B0017);
        helper.SetWndText(&m_BtnAdd, 0x000B0018);
        helper.SetWndText(&m_BtnImport, 0x000B0019);
        helper.SetWndText(&m_BtnExport, 0x000B001A);
        helper.SetWndText(&m_BtnUpdate, 0x000B001B);
        helper.SetWndText(&m_BtnOK, 0x000B001C);
    }

    void CPresetsDlg::AddToList(config::CPreset &preset, int nItem)
    {
        LVITEM lvi;

        ZeroMemory(&lvi, sizeof(LVITEM));

        lvi.mask = LVIF_TEXT | LVIF_STATE;
        lvi.state = 0;
        lvi.stateMask = 0;
        lvi.iItem = nItem;

        lvi.iSubItem = PRESET_COLUMN_NAME;
        lvi.pszText = (LPTSTR)(LPCTSTR)(preset.szName.c_str());
        m_LstPresets.InsertItem(&lvi);

        lvi.iSubItem = PRESET_COLUMN_OPTIONS;
        lvi.pszText = (LPTSTR)(LPCTSTR)(preset.szOptions.c_str());
        m_LstPresets.SetItemText(lvi.iItem, PRESET_COLUMN_OPTIONS, lvi.pszText);
    }

    void CPresetsDlg::InsertPresetsToListCtrl()
    {
        if (this->m_Formats.size() > 0)
        {
            config::CFormat& format = this->m_Formats[this->nSelectedFormat];
            size_t nPresets = format.m_Presets.size();
            for (size_t i = 0; i < nPresets; i++)
            {
                config::CPreset& preset = format.m_Presets[i];
                this->AddToList(preset, i);
            }
        }
    }

    void CPresetsDlg::HandleDropFiles(HDROP hDropInfo)
    {
        int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, nullptr, 0);
        if (nCount > 0)
        {
            for (int i = 0; i < nCount; i++)
            {
                int nReqChars = ::DragQueryFile(hDropInfo, i, nullptr, 0);

                CString szFile;
                ::DragQueryFile(hDropInfo, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
                if (!(::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY))
                {
                    std::wstring szPath = szFile;
                    std::wstring szExt = util::Utilities::GetFileExtension(szPath);

                    if (util::string::CompareNoCase(szExt, L"xml"))
                    {
                        xml::XmlDocumnent doc;
                        std::string szName = xml::XmlConfig::GetRootName(szPath, doc);
                        if (!szName.empty())
                        {
                            if (util::string::CompareNoCase(szName, "Presets"))
                            {
                                this->LoadPresets(doc);
                            }
                        }
                    }
                }
                szFile.ReleaseBuffer();
            }
        }

        ::DragFinish(hDropInfo);
    }

    void CPresetsDlg::UpdateFields(config::CPreset& preset)
    {
        this->m_EdtName.SetWindowText(preset.szName.c_str());
        this->m_EdtOptions.SetWindowText(preset.szOptions.c_str());
    }

    void CPresetsDlg::ListSelectionChange()
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nItem = m_LstPresets.GetNextSelectedItem(pos);

            config::CFormat& format = m_Formats[nSelectedFormat];
            config::CPreset& preset = format.m_Presets[nItem];
            format.nDefaultPreset = nItem;

            this->UpdateFields(preset);
        }
        else
        {
            this->m_EdtName.SetWindowText(_T(""));
            this->m_EdtOptions.SetWindowText(_T(""));
        }

        bUpdate = false;
    }

    bool CPresetsDlg::LoadPresets(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::string::CompareNoCase(szName, "Presets"))
        {
            return this->LoadPresets(doc);
        }
        return false;
    }

    bool CPresetsDlg::LoadPresets(xml::XmlDocumnent &doc)
    {
        std::vector<config::CPreset> presets;
        if (xml::XmlConfig::LoadPresets(doc, presets))
        {
            this->m_LstPresets.DeleteAllItems();
            config::CFormat& format = this->m_Formats[this->nSelectedFormat];
            format.m_Presets = std::move(presets);
            this->InsertPresetsToListCtrl();
            return true;
        }
        return false;
    }

    bool CPresetsDlg::SavePresets(const std::wstring& szFileXml, config::CFormat &format)
    {
        return xml::XmlConfig::SavePresets(szFileXml, format.m_Presets);
    }
}
