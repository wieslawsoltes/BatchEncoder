// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "..\XmlConfiguration.h"
#include "PresetsDlg.h"

IMPLEMENT_DYNAMIC(CPresetsDlg, CDialog)
CPresetsDlg::CPresetsDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CPresetsDlg::IDD, pParent)
{
    this->m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAYICON);
    this->szPresetsDialogResize = _T("");
    this->szPresetsListColumns = _T("");
    this->bUpdate = false;
    this->nSelectedFormat = 0;
}

CPresetsDlg::~CPresetsDlg()
{

}

void CPresetsDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_PRESET_NAME, m_StcName);
    DDX_Control(pDX, IDC_STATIC_PRESET_OPTIONS, m_StcOptions);
    DDX_Control(pDX, IDC_LIST_PRESETS, m_LstPresets);
    DDX_Control(pDX, IDC_COMBO_PRESET_FORMAT, m_CmbFormat);
    DDX_Control(pDX, IDC_EDIT_PRESET_NAME, m_EdtName);
    DDX_Control(pDX, IDC_EDIT_PRESET_OPTIONS, m_EdtOptions);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BUTTON_PRESET_REMOVE_ALL, m_BtnRemoveAll);
    DDX_Control(pDX, IDC_BUTTON_PRESET_REMOVE, m_BtnRemove);
    DDX_Control(pDX, IDC_BUTTON_PRESET_ADD, m_BtnAdd);
    DDX_Control(pDX, IDC_BUTTON_PRESET_UP, m_BtnMoveUp);
    DDX_Control(pDX, IDC_BUTTON_PRESET_DOWN, m_BtnMoveDown);
    DDX_Control(pDX, IDC_BUTTON_PRESET_UPDATE, m_BtnUpdate);
    DDX_Control(pDX, IDC_BUTTON_PRESET_LOAD, m_BtnLoad);
    DDX_Control(pDX, IDC_BUTTON_PRESET_SAVE, m_BtnSave);
}

BEGIN_MESSAGE_MAP(CPresetsDlg, CResizeDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRESETS, OnLvnItemchangedListPresets)
    ON_CBN_SELCHANGE(IDC_COMBO_PRESET_FORMAT, OnCbnSelchangeComboPresetFormat)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_REMOVE_ALL, OnBnClickedButtonRemoveAllPresets)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_REMOVE, OnBnClickedButtonRemovePreset)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_ADD, OnBnClickedButtonAddPreset)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_UP, OnBnClickedButtonPresetUp)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_DOWN, OnBnClickedButtonPresetDown)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_UPDATE, OnBnClickedButtonUpdatePreset)
    ON_EN_CHANGE(IDC_EDIT_PRESET_NAME, OnEnChangeEditPresetName)
    ON_EN_CHANGE(IDC_EDIT_PRESET_OPTIONS, OnEnChangeEditPresetOptions)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_LOAD, OnBnClickedButtonLoadPresets)
    ON_BN_CLICKED(IDC_BUTTON_PRESET_SAVE, OnBnClickedButtonSavePresets)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CPresetsDlg::OnInitDialog()
{
    CResizeDialog::OnInitDialog();

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
    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);
        m_CmbFormat.InsertString(i, format.szName);
    }

    ::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_PRESET_FORMAT, 15);

    m_CmbFormat.SetCurSel(nSelectedFormat);

    this->OnCbnSelchangeComboPresetFormat();

    // setup resize anchors
    AddAnchor(IDC_COMBO_PRESET_FORMAT, TOP_LEFT);
    AddAnchor(IDC_LIST_PRESETS, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_UP, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_DOWN, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_ADD, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_REMOVE_ALL, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_REMOVE, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_LOAD, BOTTOM_LEFT);
    AddAnchor(IDC_BUTTON_PRESET_SAVE, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_PRESET_NAME, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_PRESET_NAME, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_PRESET_OPTIONS, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_PRESET_OPTIONS, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_PRESET_UPDATE, BOTTOM_RIGHT);
    AddAnchor(IDOK, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT);

    this->LoadWindowSettings();

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

void CPresetsDlg::OnBnClickedButtonRemoveAllPresets()
{
    if (m_Formats.GetSize() > 0)
    {
        CFormat& format = m_Formats.GetData(nSelectedFormat);
        format.m_Presets.RemoveAllNodes();

        m_LstPresets.DeleteAllItems();

        this->ListSelectionChange();
    }
}

void CPresetsDlg::OnBnClickedButtonRemovePreset()
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

        this->ListSelectionChange();
    }
}

void CPresetsDlg::OnBnClickedButtonAddPreset()
{
    if (m_Formats.GetSize() > 0)
    {
        if (bUpdate == true)
            return;

        bUpdate = true;

        int nItem = m_LstPresets.GetItemCount();

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        CPreset preset;
        preset.szName = _T("Preset");
        preset.szOptions = _T("");
        format.m_Presets.InsertNode(preset);

        AddToList(preset, nItem);

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
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);
        if (nItem > 0)
        {
            CFormat& format = m_Formats.GetData(nSelectedFormat);
            CPreset& preset1 = format.m_Presets.GetData(nItem);
            CPreset& preset2 = format.m_Presets.GetData(nItem - 1);

            m_LstPresets.SetItemText(nItem, PRESET_COLUMN_NAME, preset2.szName);
            m_LstPresets.SetItemText(nItem, PRESET_COLUMN_OPTIONS, preset2.szOptions);
            m_LstPresets.SetItemText(nItem - 1, PRESET_COLUMN_NAME, preset1.szName);
            m_LstPresets.SetItemText(nItem - 1, PRESET_COLUMN_OPTIONS, preset1.szOptions);

            format.m_Presets.SwapItems(nItem, nItem - 1);

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
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);
        int nItems = m_LstPresets.GetItemCount();
        if (nItem != (nItems - 1) && nItem >= 0)
        {
            CFormat& format = m_Formats.GetData(nSelectedFormat);
            CPreset& preset1 = format.m_Presets.GetData(nItem);
            CPreset& preset2 = format.m_Presets.GetData(nItem + 1);

            m_LstPresets.SetItemText(nItem, PRESET_COLUMN_NAME, preset2.szName);
            m_LstPresets.SetItemText(nItem, PRESET_COLUMN_OPTIONS, preset2.szOptions);
            m_LstPresets.SetItemText(nItem + 1, PRESET_COLUMN_NAME, preset1.szName);
            m_LstPresets.SetItemText(nItem + 1, PRESET_COLUMN_OPTIONS, preset1.szOptions);

            format.m_Presets.SwapItems(nItem, nItem + 1);

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
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        CString szName = _T("");
        CString szOptions = _T("");

        this->m_EdtName.GetWindowText(szName);
        this->m_EdtOptions.GetWindowText(szOptions);

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        CPreset& preset = format.m_Presets.GetData(nItem);
        preset.szName = szName;
        preset.szOptions = szOptions;

        m_LstPresets.SetItemText(nItem, PRESET_COLUMN_NAME, szName);
        m_LstPresets.SetItemText(nItem, PRESET_COLUMN_OPTIONS, szOptions);

        m_LstPresets.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    }

    bUpdate = false;
}

void CPresetsDlg::OnCbnSelchangeComboPresetFormat()
{
    m_LstPresets.DeleteAllItems();

    nSelectedFormat = this->m_CmbFormat.GetCurSel();

    this->InsertPresetsToListCtrl();

    if (this->m_Formats.GetSize() > 0)
    {
        CFormat& format = this->m_Formats.GetData(nSelectedFormat);

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

void CPresetsDlg::OnBnClickedButtonLoadPresets()
{
    CFileDialog fd(TRUE, _T("presets"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Presets Files (*.presets)|*.presets|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    int nPreset = this->m_CmbFormat.GetCurSel();

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadPresets(szFileXml);
    }
}

void CPresetsDlg::OnBnClickedButtonSavePresets()
{
    CFileDialog fd(FALSE, _T("presets"), _T("presets"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Presets Files (*.presets)|*.presets|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    int nPreset = this->m_CmbFormat.GetCurSel();

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SavePresets(szFileXml);
    }
}

void CPresetsDlg::OnClose()
{
    this->SaveWindowSettings();

    CResizeDialog::OnClose();
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

void CPresetsDlg::AddToList(CPreset &preset, int nItem)
{
    LVITEM lvi;

    ZeroMemory(&lvi, sizeof(LVITEM));

    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iItem = nItem;

    lvi.iSubItem = PRESET_COLUMN_NAME;
    lvi.pszText = (LPTSTR)(LPCTSTR)(preset.szName);
    m_LstPresets.InsertItem(&lvi);

    lvi.iSubItem = PRESET_COLUMN_OPTIONS;
    lvi.pszText = (LPTSTR)(LPCTSTR)(preset.szOptions);
    m_LstPresets.SetItemText(lvi.iItem, PRESET_COLUMN_OPTIONS, lvi.pszText);
}

void CPresetsDlg::InsertPresetsToListCtrl()
{
    if (this->m_Formats.GetSize() > 0)
    {
        CFormat& format = this->m_Formats.GetData(this->nSelectedFormat);
        int nPresets = format.m_Presets.GetSize();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset& preset = format.m_Presets.GetData(i);
            this->AddToList(preset, i);
        }
    }
}

void CPresetsDlg::UpdateFields(CPreset& preset)
{
    this->m_EdtName.SetWindowText(preset.szName);
    this->m_EdtOptions.SetWindowText(preset.szOptions);
}

void CPresetsDlg::ListSelectionChange()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstPresets.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstPresets.GetNextSelectedItem(pos);

        CFormat& format = m_Formats.GetData(nSelectedFormat);
        CPreset& preset = format.m_Presets.GetData(nItem);
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

void CPresetsDlg::LoadPresets(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.Open(szFileXml) == true)
    {
        this->m_LstPresets.DeleteAllItems();

        CFormat& format = this->m_Formats.GetData(this->nSelectedFormat);
        format.m_Presets.RemoveAllNodes();

        doc.GetPresets(format.m_Presets);

        this->InsertPresetsToListCtrl();
    }
    else
    {
        MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CPresetsDlg::SavePresets(CString szFileXml)
{
    CFormat& format = this->m_Formats.GetData(this->nSelectedFormat);

    XmlConfiguration doc;
    doc.SetPresets(format.m_Presets);
    if (doc.Save(szFileXml) != true)
    {
        MessageBox(_T("Failed to save file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}
