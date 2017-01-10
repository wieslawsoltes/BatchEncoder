// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "UnicodeUtf8.h"
#include "Utf8String.h"
#include "PresetsDlg.h"
#include ".\presetsdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CPresetsDlg, CDialog)
CPresetsDlg::CPresetsDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CPresetsDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    nSelFormat = 1;

    // TODO: 
    // apply this flag to dialog differ between versions
    // one for Combo and 1 for only 1 file mode
    bStandAlone = false;
    szStandAloneFile = _T("");

    szPresetsWndResize = _T("");
    szPresetsListColumns = _T("");

    bUpdate = false;

    this->bShowGridLines = true;
}

CPresetsDlg::~CPresetsDlg()
{

}

void CPresetsDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PD_PRESETS, m_LstPresets);
    DDX_Control(pDX, IDC_EDIT_PD_NAME, m_EdtName);
    DDX_Control(pDX, IDC_EDIT_PD_OPTIONS, m_EdtOptions);
    DDX_Control(pDX, IDC_EDIT_PD_PATH_TO_CONFIG, m_EdtConfigFile);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_COMBO_PD_FORMAT, m_CmbFormat);
    DDX_Control(pDX, IDC_STATIC_PD_TEXT_NAME, m_StcName);
    DDX_Control(pDX, IDC_STATIC_PD_TEXT_OPTIONS, m_StcOptions);
    DDX_Control(pDX, IDC_BUTTON_PD_LOAD_PRESETS, m_BtnLoad);
    DDX_Control(pDX, IDC_BUTTON_PD_SAVE_PRESETS, m_BtnSave);
    DDX_Control(pDX, IDC_BUTTON_PD_UPDATE, m_BtnUpdateChanges);
    DDX_Control(pDX, IDC_BUTTON_PD_UP, m_BtnMoveUp);
    DDX_Control(pDX, IDC_BUTTON_PD_DOWN, m_BtnMoveDown);
    DDX_Control(pDX, IDC_BUTTON_PD_REMOVE_ALL_PRESETS, m_BtnRemoveAll);
    DDX_Control(pDX, IDC_BUTTON_PD_REMOVE_PRESETS, m_BtnRemove);
    DDX_Control(pDX, IDC_BUTTON_PD_UPDATE_PRESET, m_BtnUpdate);
    DDX_Control(pDX, IDC_BUTTON_PD_ADD_PRESET, m_BtnAdd);
}

BEGIN_MESSAGE_MAP(CPresetsDlg, CResizeDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_NOTIFY(LVN_KEYDOWN, IDC_EDIT_PD_PRESETS, OnLvnKeydownListPdPresets)
    ON_NOTIFY(NM_CLICK, IDC_EDIT_PD_PRESETS, OnNMClickListPdPresets)
    ON_NOTIFY(LVN_ITEMCHANGING, IDC_EDIT_PD_PRESETS, OnLvnItemchangingListPdPresets)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDIT_PD_PRESETS, OnLvnItemchangedListPdPresets)
    ON_BN_CLICKED(IDC_BUTTON_PD_REMOVE_ALL_PRESETS, OnBnClickedButtonPdRemoveAllPresets)
    ON_BN_CLICKED(IDC_BUTTON_PD_REMOVE_PRESETS, OnBnClickedButtonPdRemovePresets)
    ON_BN_CLICKED(IDC_BUTTON_PD_LOAD_PRESETS, OnBnClickedButtonPdLoadPresets)
    ON_BN_CLICKED(IDC_BUTTON_PD_SAVE_PRESETS, OnBnClickedButtonPdSavePresets)
    ON_BN_CLICKED(IDC_BUTTON_PD_ADD_PRESET, OnBnClickedButtonPdAddPreset)
    ON_BN_CLICKED(IDC_BUTTON_PD_UPDATE_PRESET, OnBnClickedButtonPdUpdatePreset)
    ON_BN_CLICKED(IDC_BUTTON_PD_UP, OnBnClickedButtonPdUp)
    ON_BN_CLICKED(IDC_BUTTON_PD_DOWN, OnBnClickedButtonPdDown)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_CBN_SELCHANGE(IDC_COMBO_PD_FORMAT, OnCbnSelchangeComboPdFormat)
    ON_BN_CLICKED(IDC_BUTTON_PD_UPDATE, OnBnClickedButtonPdUpdate)
    ON_WM_CLOSE()
    ON_EN_CHANGE(IDC_EDIT_PD_NAME, OnEnChangeEditPdName)
    ON_EN_CHANGE(IDC_EDIT_PD_OPTIONS, OnEnChangeEditPdOptions)
END_MESSAGE_MAP()

BOOL CPresetsDlg::OnInitDialog()
{
    CResizeDialog::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // update list style
    DWORD dwExStyle = m_LstPresets.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    m_LstPresets.SetExtendedStyle(dwExStyle);

    // insert listView columns
    m_LstPresets.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 200);
    m_LstPresets.InsertColumn(1, _T("Options"), LVCFMT_LEFT, 290);

    // in standalone editor only one-at-time file editing is allowed
    if (bStandAlone == false)
    {
        // file format combo-box
        for (int i = 0; i < NUM_PRESET_FILES; i++)
            m_CmbFormat.InsertString(i, g_szPresetNames[i]);

        ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PD_FORMAT);

        m_CmbFormat.SetCurSel(nSelFormat);

        // load config file for current format
        this->OnCbnSelchangeComboPdFormat();

        // select current preset
        int nItem = this->nSelPreset;

        CString szName = this->m_LstPresets.GetItemText(nItem, 0);
        CString szOptions = this->m_LstPresets.GetItemText(nItem, 1);

        this->m_EdtName.SetWindowText(szName);
        this->m_EdtOptions.SetWindowText(szOptions);

        m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        m_LstPresets.EnsureVisible(nItem, FALSE);
    }

    // behave like standalone editor
    if (bStandAlone == true)
    {
        // method 1: hide only ComboBox and resize EditBox

        // items to hide
        GetDlgItem(IDC_COMBO_PD_FORMAT)->ShowWindow(SW_HIDE);

        // items to move
        CRect rcFormatCombo;
        GetDlgItem(IDC_COMBO_PD_FORMAT)->GetWindowRect(rcFormatCombo);
        ScreenToClient(&rcFormatCombo);

        CRect rcPathEdit;
        GetDlgItem(IDC_EDIT_PD_PATH_TO_CONFIG)->GetWindowRect(rcPathEdit);
        ScreenToClient(&rcPathEdit);

        int nDiff = rcPathEdit.left - rcFormatCombo.left;

        rcPathEdit.left -= nDiff;
        GetDlgItem(IDC_EDIT_PD_PATH_TO_CONFIG)->MoveWindow(rcPathEdit);

        // method 2: hide ComboBox and EditBox

        /*
        // items to hide
        GetDlgItem(IDC_COMBO_PD_FORMAT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_PD_PATH_TO_CONFIG)->ShowWindow(SW_HIDE);

        // items to move
        CRect rcFormatCombo;
        GetDlgItem(IDC_COMBO_PD_FORMAT)->GetWindowRect(rcFormatCombo);
        ScreenToClient(&rcFormatCombo);

        CRect rcPresetsList;
        GetDlgItem(IDC_EDIT_PD_PRESETS)->GetWindowRect(rcPresetsList);
        ScreenToClient(&rcPresetsList);

        CRect rcButtonUp;
        GetDlgItem(IDC_BUTTON_PD_UP)->GetWindowRect(rcButtonUp);
        ScreenToClient(&rcButtonUp);

        CRect rcButtonDown;
        GetDlgItem(IDC_BUTTON_PD_DOWN)->GetWindowRect(rcButtonDown);
        ScreenToClient(&rcButtonDown);

        int nDiff = rcPresetsList.top - rcFormatCombo.top;

        rcPresetsList.top -= nDiff;
        rcPresetsList.bottom -= 0;
        GetDlgItem(IDC_EDIT_PD_PRESETS)->MoveWindow(rcPresetsList);

        rcButtonUp.top -= nDiff;
        rcButtonUp.bottom -= nDiff;
        GetDlgItem(IDC_BUTTON_PD_UP)->MoveWindow(rcButtonUp);

        rcButtonDown.top -= nDiff;
        rcButtonDown.bottom -= nDiff;
        GetDlgItem(IDC_BUTTON_PD_DOWN)->MoveWindow(rcButtonDown);
        */
    }

    // setup resize anchors
    AddAnchor(IDC_COMBO_PD_FORMAT, TOP_LEFT);
    AddAnchor(IDC_EDIT_PD_PRESETS, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_PD_PATH_TO_CONFIG, TOP_LEFT, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_PD_UP, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_PD_DOWN, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_PD_ADD_PRESET, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PD_REMOVE_ALL_PRESETS, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PD_REMOVE_PRESETS, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PD_LOAD_PRESETS, BOTTOM_LEFT);
    AddAnchor(IDC_BUTTON_PD_UPDATE, BOTTOM_LEFT);
    AddAnchor(IDC_BUTTON_PD_SAVE_PRESETS, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_PD_TEXT_NAME, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_PD_NAME, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_PD_TEXT_OPTIONS, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_PD_OPTIONS, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PD_UPDATE_PRESET, BOTTOM_RIGHT);
    AddAnchor(IDOK, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT);

    this->LoadWindowSettings();

    if (this->bShowGridLines == true)
        this->ShowGridlines(true);

    // ResetMinTrackSize();
    // ResetMaxTrackSize();
    // ResetMaximizedRect();

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
        CResizeDialog::OnPaint();
    }
}

HCURSOR CPresetsDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CPresetsDlg::ShowGridlines(bool bShow)
{
    DWORD dwExStyle = m_LstPresets.GetExtendedStyle();
    if (bShow == true)
    {
        dwExStyle |= LVS_EX_GRIDLINES;
        m_LstPresets.SetExtendedStyle(dwExStyle);
    }
    else
    {
        if (dwExStyle & LVS_EX_GRIDLINES)
        {
            dwExStyle = dwExStyle ^ LVS_EX_GRIDLINES;
            m_LstPresets.SetExtendedStyle(dwExStyle);
        }
    }
}

void CPresetsDlg::LoadWindowSettings()
{
    // set window rectangle and position
    if (szPresetsWndResize.Compare(_T("")) != 0)
        this->SetWindowRectStr(szPresetsWndResize);

    // load columns width for PresetsList
    if (szPresetsListColumns.Compare(_T("")) != 0)
    {
        int nColWidth[2];
        if (_stscanf(szPresetsListColumns, _T("%d %d"),
            &nColWidth[0],
            &nColWidth[1]) == 2)
        {
            for (int i = 0; i < 2; i++)
                m_LstPresets.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CPresetsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szPresetsWndResize = this->GetWindowRectStr();

    // save columns width from PresetsList
    int nColWidth[2];
    for (int i = 0; i < 2; i++)
        nColWidth[i] = m_LstPresets.GetColumnWidth(i);
    szPresetsListColumns.Format(_T("%d %d"),
        nColWidth[0],
        nColWidth[1]);
}

void CPresetsDlg::AddToList(CString szName, CString szOptions)
{
    // insert item to listview
    CString tmpBuf;
    LVITEM lvi;

    // prepare item struct
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;

    m_ListPresets.InsertNode(szName);

    // get next item position
    int nItem = m_ListPresets.GetSize() - 1;

    m_ListPresets.SetPresetOptions(szOptions, nItem);

    // name
    lvi.iItem = nItem; // position
    lvi.iSubItem = 0;

    lvi.pszText = (LPTSTR)(LPCTSTR)(szName);
    m_LstPresets.InsertItem(&lvi);

    // options
    lvi.iSubItem = 1;
    lvi.pszText = (LPTSTR)(LPCTSTR)(szOptions);
    m_LstPresets.SetItemText(lvi.iItem, 1, lvi.pszText);
}

void CPresetsDlg::ListSelectionChange(void)
{
    if (bUpdate == true)
        return;

    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        CString szName = m_ListPresets.GetPresetName(nItem);
        CString szOptions = m_ListPresets.GetPresetOptions(nItem);

        this->m_EdtName.SetWindowText(szName);
        this->m_EdtOptions.SetWindowText(szOptions);
    }
}

void CPresetsDlg::OnLvnKeydownListPdPresets(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

    // this->ListSelectionChange();

    *pResult = 0;
}

void CPresetsDlg::OnNMClickListPdPresets(NMHDR *pNMHDR, LRESULT *pResult)
{
    // this->ListSelectionChange();

    *pResult = 0;
}

void CPresetsDlg::OnLvnItemchangingListPdPresets(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // this->ListSelectionChange();

    *pResult = 0;
}

void CPresetsDlg::OnLvnItemchangedListPdPresets(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    this->ListSelectionChange();

    *pResult = 0;
}

void CPresetsDlg::OnBnClickedButtonPdRemoveAllPresets()
{
    // clear node list
    m_ListPresets.RemoveAllNodes();

    // clear list view
    m_LstPresets.DeleteAllItems();
}

void CPresetsDlg::OnBnClickedButtonPdRemovePresets()
{
    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);
        m_ListPresets.RemoveNode(nItem);
        m_LstPresets.DeleteItem(nItem);

        // select other item in list
        int nItems = m_LstPresets.GetItemCount();
        if (nItem < nItems && nItems >= 0)
            m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        else if (nItem >= nItems && nItems >= 0)
            m_LstPresets.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
    }
}

void CPresetsDlg::LoadConfigFile(CString szFileXml)
{
    ::UpdatePath();

    CXMLDocumentW doc;
    if (doc.LoadFileW(szFileXml) == true)
    {
        // Root = Presets
        tinyxml2::XMLElement* pRootElem = doc.FirstChildElement();
        if (!pRootElem)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        // check for "Presets"
        const char *szRoot = pRootElem->Value();
        const char *szRootName = "Presets";
        if (strcmp(szRootName, szRoot) != 0)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        // clear node list
        m_ListPresets.RemoveAllNodes();

        // clear list view
        m_LstPresets.DeleteAllItems();

        // Preset
        tinyxml2::XMLElement* pPresetsNode = pRootElem->FirstChildElement("Preset");
        for (pPresetsNode; pPresetsNode; pPresetsNode = pPresetsNode->NextSiblingElement())
        {
            const char *pszName = pPresetsNode->Attribute("name");
            if (pszName != NULL)
            {
                CString szNameData = GetConfigString(pszName);

                const char *pszOptions = pPresetsNode->Attribute("options");
                if (pszOptions != NULL)
                {
                    CString szOptionsData = GetConfigString(pszOptions);
                    this->AddToList(szNameData, szOptionsData);
                }
                else
                {
                    // ERROR
                }
            }
        }

        this->m_EdtConfigFile.SetWindowText(szFileXml);
        this->SetCurConfigFile(szFileXml);
    }
    else
    {
        MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CPresetsDlg::SaveConfigFile(CString szFileXml)
{
    if (szFileXml.Compare(_T("")) == 0)
    {
        this->OnBnClickedButtonPdSavePresets();
        return;
    }

    ::UpdatePath();

    CXMLDocumentW doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.LinkEndChild(decl);

    CString szBuff;
    CUtf8String m_Utf8;

    // root: Presets
    tinyxml2::XMLElement *pPresetsNode = doc.NewElement("Presets");
    doc.LinkEndChild(pPresetsNode);
    int nPresets = m_ListPresets.GetSize();
    for (int i = 0; i < nPresets; i++)
    {
        // Preset
        tinyxml2::XMLElement *preset = doc.NewElement("Preset");
        pPresetsNode->LinkEndChild(preset);

        CString szName = m_ListPresets.GetPresetName(i);
        CString szOptions = m_ListPresets.GetPresetOptions(i);

        if (szName.GetLength() == 0)
        {
            preset->SetAttribute("name", "");
        }
        else
        {
            preset->SetAttribute("name", m_Utf8.Create(szName));
            m_Utf8.Clear();
        }

        if (szOptions.GetLength() == 0)
        {
            preset->SetAttribute("options", "");
        }
        else
        {
            preset->SetAttribute("options", m_Utf8.Create(szOptions));
            m_Utf8.Clear();
        }
    }

    if (doc.SaveFileW(szFileXml) != true)
    {
        MessageBox(_T("Failed to save file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
    else
    {
        this->m_EdtConfigFile.SetWindowText(szFileXml);
        this->SetCurConfigFile(szFileXml);
    }
}

void CPresetsDlg::OnBnClickedButtonPdLoadPresets()
{
    CFileDialog fd(TRUE, _T("presets"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Presets Files (*.presets)|*.presets|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    int nPreset = this->m_CmbFormat.GetCurSel();
    ::SetBrowsePath(fd, szBrowsePath[(nPreset + nPreset)]);

    if (fd.DoModal() == IDOK)
    {
        szBrowsePath[(nPreset + nPreset)] = ::GetBrowsePath(fd);

        CString szFileXml = fd.GetPathName();
        this->LoadConfigFile(szFileXml);
    }
}

void CPresetsDlg::OnBnClickedButtonPdSavePresets()
{
    CFileDialog fd(FALSE, _T("presets"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Presets Files (*.presets)|*.presets|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    int nPreset = this->m_CmbFormat.GetCurSel();
    ::SetBrowsePath(fd, szBrowsePath[(nPreset + nPreset + 1)]);

    if (fd.DoModal() == IDOK)
    {
        szBrowsePath[(nPreset + nPreset + 1)] = ::GetBrowsePath(fd);

        CString szFileXml = fd.GetPathName();
        this->SaveConfigFile(szFileXml);
    }
}

void CPresetsDlg::OnBnClickedButtonPdAddPreset()
{
    CString szName;
    CString szOptions;

    this->m_EdtName.GetWindowText(szName);
    this->m_EdtOptions.GetWindowText(szOptions);

    AddToList(szName, szOptions);

    int nItem = m_LstPresets.GetItemCount() - 1;
    m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

    m_LstPresets.EnsureVisible(nItem, FALSE);
}

void CPresetsDlg::OnBnClickedButtonPdUpdatePreset()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        CString szName;
        CString szOptions;

        this->m_EdtName.GetWindowText(szName);
        this->m_EdtOptions.GetWindowText(szOptions);

        m_ListPresets.SetPresetName(szName, nItem);
        m_ListPresets.SetPresetOptions(szOptions, nItem);

        m_LstPresets.SetItemText(nItem, 0, szName);
        m_LstPresets.SetItemText(nItem, 1, szOptions);

        m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    }

    bUpdate = false;
}

void CPresetsDlg::OnBnClickedButtonPdUp()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    // move up item in ListView and list
    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        // don't process 1st item
        if (nItem > 0)
        {
            CString szName1 = m_ListPresets.GetPresetName(nItem);
            CString szOptions1 = m_ListPresets.GetPresetOptions(nItem);
            CString szName2 = m_ListPresets.GetPresetName(nItem - 1);
            CString szOptions2 = m_ListPresets.GetPresetOptions(nItem - 1);

            m_LstPresets.SetItemText(nItem, 0, szName2);
            m_LstPresets.SetItemText(nItem, 1, szOptions2);

            m_LstPresets.SetItemText(nItem - 1, 0, szName1);
            m_LstPresets.SetItemText(nItem - 1, 1, szOptions1);

            m_ListPresets.SetPresetName(szName2, nItem);
            m_ListPresets.SetPresetOptions(szOptions2, nItem);

            m_ListPresets.SetPresetName(szName1, nItem - 1);
            m_ListPresets.SetPresetOptions(szOptions1, nItem - 1);

            m_LstPresets.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);

            m_LstPresets.EnsureVisible(nItem - 1, FALSE);
        }
    }

    bUpdate = false;
}

void CPresetsDlg::OnBnClickedButtonPdDown()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    // move down item in ListView and list
    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        int nItems = m_LstPresets.GetItemCount();
        // don't process last item
        if (nItem != (nItems - 1) && nItem >= 0)
        {
            CString szName1 = m_ListPresets.GetPresetName(nItem);
            CString szOptions1 = m_ListPresets.GetPresetOptions(nItem);
            CString szName2 = m_ListPresets.GetPresetName(nItem + 1);
            CString szOptions2 = m_ListPresets.GetPresetOptions(nItem + 1);

            m_LstPresets.SetItemText(nItem, 0, szName2);
            m_LstPresets.SetItemText(nItem, 1, szOptions2);

            m_LstPresets.SetItemText(nItem + 1, 0, szName1);
            m_LstPresets.SetItemText(nItem + 1, 1, szOptions1);

            m_ListPresets.SetPresetName(szName2, nItem);
            m_ListPresets.SetPresetOptions(szOptions2, nItem);

            m_ListPresets.SetPresetName(szName1, nItem + 1);
            m_ListPresets.SetPresetOptions(szOptions1, nItem + 1);

            m_LstPresets.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);

            m_LstPresets.EnsureVisible(nItem + 1, FALSE);
        }
    }

    bUpdate = false;
}

void CPresetsDlg::OnBnClickedOk()
{
    // NOTE: when pressing OK button changes are saved to file
    this->SaveConfigFile(GetCurConfigFile());

    this->SaveWindowSettings();

    OnOK();
}

void CPresetsDlg::OnBnClickedCancel()
{
    this->SaveWindowSettings();

    OnCancel();
}

CString CPresetsDlg::GetCurConfigFile()
{
    if (this->bStandAlone == true)
    {
        return this->szStandAloneFile;
    }
    else
    {
        int nSelFormat = this->m_CmbFormat.GetCurSel();

        if ((nSelFormat >= 0) && (nSelFormat < NUM_PRESET_FILES))
            return this->szPresetsFile[nSelFormat];
        else
            return _T(""); // ERROR
    }
}

void CPresetsDlg::SetCurConfigFile(CString szFile)
{
    if (this->bStandAlone == true)
    {
        this->szStandAloneFile = szFile;
    }
    else
    {
        int nSelFormat = this->m_CmbFormat.GetCurSel();

        if ((nSelFormat >= 0) && (nSelFormat < NUM_PRESET_FILES))
            this->szPresetsFile[nSelFormat] = szFile;
    }
}

void CPresetsDlg::OnBnClickedButtonPdUpdate()
{
    this->SaveConfigFile(GetCurConfigFile());
}

void CPresetsDlg::OnCbnSelchangeComboPdFormat()
{
    this->LoadConfigFile(GetCurConfigFile());
}

void CPresetsDlg::OnClose()
{
    this->SaveWindowSettings();

    CResizeDialog::OnClose();
}

void CPresetsDlg::OnEnChangeEditPdName()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonPdUpdatePreset();
}

void CPresetsDlg::OnEnChangeEditPdOptions()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonPdUpdatePreset();
}
