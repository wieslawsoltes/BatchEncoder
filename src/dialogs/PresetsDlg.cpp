// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "PresetsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CPresetsDlg, CDialog)
CPresetsDlg::CPresetsDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CPresetsDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    nSelectedFormat = 0;
    szPresetsDialogResize = _T("");
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

    // file format combo-box
    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);
        m_CmbFormat.InsertString(i, format.szName);
    }

    ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PD_FORMAT);

    nSelectedFormat = 0;
    m_CmbFormat.SetCurSel(nSelectedFormat);

    // load configuration file for current format
    this->OnCbnSelchangeComboPdFormat();

    // select current preset
    int nItem = m_Formats.GetData(nSelectedFormat).nDefaultPreset;

    CString szName = this->m_LstPresets.GetItemText(nItem, 0);
    CString szOptions = this->m_LstPresets.GetItemText(nItem, 1);

    this->m_EdtName.SetWindowText(szName);
    this->m_EdtOptions.SetWindowText(szOptions);

    m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    m_LstPresets.EnsureVisible(nItem, FALSE);

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
    if (szPresetsDialogResize.Compare(_T("")) != 0)
        this->SetWindowRectStr(szPresetsDialogResize);

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
    this->szPresetsDialogResize = this->GetWindowRectStr();

    // save columns width from PresetsList
    int nColWidth[2];
    for (int i = 0; i < 2; i++)
        nColWidth[i] = m_LstPresets.GetColumnWidth(i);
    szPresetsListColumns.Format(_T("%d %d"),
        nColWidth[0],
        nColWidth[1]);
}

void CPresetsDlg::AddToList(CString szName, CString szOptions, int nItem)
{
    // insert item to list view
    CString tmpBuf;
    LVITEM lvi;

    // prepare item struct
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;

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

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        CPreset& preset = format.m_Presets.GetData(nItem);

        this->m_EdtName.SetWindowText(preset.szName);
        this->m_EdtOptions.SetWindowText(preset.szOptions);
    }
}

void CPresetsDlg::OnLvnItemchangedListPdPresets(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    this->ListSelectionChange();

    *pResult = 0;
}

void CPresetsDlg::OnBnClickedButtonPdRemoveAllPresets()
{
    CFormat& format = m_Formats.GetData(nSelectedFormat);
    format.m_Presets.RemoveAllNodes();

    m_LstPresets.DeleteAllItems();
}

void CPresetsDlg::OnBnClickedButtonPdRemovePresets()
{
    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        format.m_Presets.RemoveNode(nItem);

        m_LstPresets.DeleteItem(nItem);

        int nItems = m_LstPresets.GetItemCount();
        if (nItem < nItems && nItems >= 0)
            m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        else if (nItem >= nItems && nItems >= 0)
            m_LstPresets.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
    }
}

void CPresetsDlg::LoadPresetsFile(CString szFileXml)
{
    ::UpdatePath();

    CXMLDocumentW doc;
    if (doc.LoadFileW(szFileXml) == true)
    {
        tinyxml2::XMLElement *pPresetsElem = doc.FirstChildElement();
        if (!pPresetsElem)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        const char *szRoot = pPresetsElem->Value();
        const char *szRootName = "Presets";
        if (strcmp(szRootName, szRoot) != 0)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        //format.m_Presets.RemoveAllNodes();

        //m_LstPresets.DeleteAllItems();

        doc.LoadPresets(pPresetsElem, format.m_Presets);

        int nPresets = format.m_Presets.GetSize();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset& preset = format.m_Presets.GetData(i);
            this->AddToList(preset.szName, preset.szOptions, i);
        }

        this->m_EdtConfigFile.SetWindowText(szFileXml);
    }
    else
    {
        MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CPresetsDlg::SavePresetsFile(CString szFileXml)
{
    if (szFileXml.Compare(_T("")) == 0)
    {
        this->OnBnClickedButtonPdSavePresets();
        return;
    }

    ::UpdatePath();

    CXMLDocumentW doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    doc.LinkEndChild(decl);

    tinyxml2::XMLElement *pPresetsElem = doc.NewElement("Presets");
    doc.LinkEndChild(pPresetsElem);

    CFormat& format = m_Formats.GetData(nSelectedFormat);
    doc.SavePresets(pPresetsElem, format.m_Presets);

    if (doc.SaveFileW(szFileXml) != true)
    {
        MessageBox(_T("Failed to save file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CPresetsDlg::OnBnClickedButtonPdLoadPresets()
{
    CFileDialog fd(TRUE, _T("presets"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Presets Files (*.presets)|*.presets|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    int nPreset = this->m_CmbFormat.GetCurSel();

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadPresetsFile(szFileXml);
    }
}

void CPresetsDlg::OnBnClickedButtonPdSavePresets()
{
    CFileDialog fd(FALSE, _T("presets"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Presets Files (*.presets)|*.presets|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    int nPreset = this->m_CmbFormat.GetCurSel();

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SavePresetsFile(szFileXml);
    }
}

void CPresetsDlg::OnBnClickedButtonPdAddPreset()
{
    CString szName;
    CString szOptions;

    this->m_EdtName.GetWindowText(szName);
    this->m_EdtOptions.GetWindowText(szOptions);

    int nItem = m_LstPresets.GetItemCount();

    AddToList(szName, szOptions, nItem);

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

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        CPreset& preset = format.m_Presets.GetData(nItem);
        preset.szName = szName;
        preset.szOptions = szOptions;

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

    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);
        if (nItem > 0)
        {
            CFormat& format = m_Formats.GetData(nSelectedFormat);
            CPreset& preset1 = format.m_Presets.GetData(nItem);
            CPreset& preset2 = format.m_Presets.GetData(nItem - 1);

            m_LstPresets.SetItemText(nItem, 0, preset2.szName);
            m_LstPresets.SetItemText(nItem, 1, preset2.szOptions);
            m_LstPresets.SetItemText(nItem - 1, 0, preset1.szName);
            m_LstPresets.SetItemText(nItem - 1, 1, preset1.szOptions);

            format.m_Presets.SwapItems(nItem, nItem - 1);

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

    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);
        int nItems = m_LstPresets.GetItemCount();
        if (nItem != (nItems - 1) && nItem >= 0)
        {
            CFormat& format = m_Formats.GetData(nSelectedFormat);
            CPreset& preset1 = format.m_Presets.GetData(nItem);
            CPreset& preset2 = format.m_Presets.GetData(nItem + 1);

            m_LstPresets.SetItemText(nItem, 0, preset2.szName);
            m_LstPresets.SetItemText(nItem, 1, preset2.szOptions);
            m_LstPresets.SetItemText(nItem + 1, 0, preset1.szName);
            m_LstPresets.SetItemText(nItem + 1, 1, preset1.szOptions);

            format.m_Presets.SwapItems(nItem, nItem + 1);

            m_LstPresets.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);

            m_LstPresets.EnsureVisible(nItem + 1, FALSE);
        }
    }

    bUpdate = false;
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

void CPresetsDlg::OnBnClickedButtonPdUpdate()
{
    // TODO: Remove this handler.
}

void CPresetsDlg::OnCbnSelchangeComboPdFormat()
{
    // TODO: Remove this handler.
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
