// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "Strings.h"
#include "language\LanguageHelper.h"
#include "utilities\Utilities.h"
#include "utilities\UnicodeUtf8.h"
#include "utilities\Utf8String.h"
#include "xml\XmlFormats.h"
#include "xml\XmlPresets.h"
#include "ToolsDlg.h"
#include "PresetsDlg.h"

DWORD WINAPI ToolsDlgDropThread(LPVOID lpParam)
{
    ToolsDlgDropContext* pDD = (ToolsDlgDropContext*)lpParam;
    pDD->pDlg->HandleDropFiles(pDD->hDrop);
    pDD->bHandled = true;
    return ::CloseHandle(pDD->hThread);
}

IMPLEMENT_DYNAMIC(CToolsDlg, CDialog)
CToolsDlg::CToolsDlg(CWnd* pParent /*=NULL*/)
    : CMyDialogEx(CToolsDlg::IDD, pParent)
{
    this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
    this->szToolsDialogResize = _T("");
    this->szToolsListColumns = _T("");
    this->bUpdate = false;
    this->nSelectedFormat = 0;
}

CToolsDlg::~CToolsDlg()
{

}

void CToolsDlg::DoDataExchange(CDataExchange* pDX)
{
    CMyDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_TOOL_NAME, m_StcName);
    DDX_Control(pDX, IDC_STATIC_TOOL_PLATFORM, m_StcPlatform);
    DDX_Control(pDX, IDC_STATIC_TOOL_FORMATS, m_StcFormats);
    DDX_Control(pDX, IDC_STATIC_TOOL_URL, m_StcUrl);
    DDX_Control(pDX, IDC_STATIC_TOOL_FILE, m_StcFile);
    DDX_Control(pDX, IDC_STATIC_TOOL_EXTRACT, m_StcExtract);
    DDX_Control(pDX, IDC_STATIC_TOOL_PATH, m_StcPath);
    DDX_Control(pDX, IDC_LIST_TOOLS, m_LstTools);
    DDX_Control(pDX, IDC_EDIT_TOOL_NAME, m_EdtName);
    DDX_Control(pDX, IDC_EDIT_TOOL_PLATFORM, m_EdtPlatform);
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
    DDX_Control(pDX, IDC_BUTTON_TOOL_UP, m_BtnMoveUp);
    DDX_Control(pDX, IDC_BUTTON_TOOL_DOWN, m_BtnMoveDown);
    DDX_Control(pDX, IDC_BUTTON_TOOL_UPDATE, m_BtnUpdate);
    DDX_Control(pDX, IDC_BUTTON_TOOL_LOAD, m_BtnLoad);
    DDX_Control(pDX, IDC_BUTTON_TOOL_SAVE, m_BtnSave);
    DDX_Control(pDX, IDC_BUTTON_EDIT_FORMATS, m_BtnEditFormats);
}

BEGIN_MESSAGE_MAP(CToolsDlg, CMyDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOOLS, OnLvnItemchangedListTools)
    ON_EN_CHANGE(IDC_EDIT_TOOL_NAME, OnEnChangeEditToolName)
    ON_EN_CHANGE(IDC_EDIT_TOOL_PLATFORM, OnEnChangeEditToolPlatform)
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
    ON_BN_CLICKED(IDC_BUTTON_TOOL_UP, OnBnClickedButtonToolUp)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_DOWN, OnBnClickedButtonToolDown)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_UPDATE, OnBnClickedButtonUpdateTool)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_LOAD, OnBnClickedButtonLoadTools)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_SAVE, OnBnClickedButtonSaveTools)
    ON_BN_CLICKED(IDC_BUTTON_EDIT_FORMATS, OnBnClickedButtonEditFormats)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CToolsDlg::OnInitDialog()
{
    CMyDialogEx::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // update list style
    DWORD dwExStyle = m_LstTools.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
    m_LstTools.SetExtendedStyle(dwExStyle);

    // insert all ListCtrl columns
    m_LstTools.InsertColumn(TOOL_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 195);
    m_LstTools.InsertColumn(TOOL_COLUMN_URL, _T("Template"), LVCFMT_LEFT, 295);

    // insert all ListCtrl items and sub items
    this->InsertFormatsToListCtrl();

    m_LstTools.SetItemState(nSelectedFormat, LVIS_SELECTED, LVIS_SELECTED);
    m_LstTools.EnsureVisible(nSelectedFormat, FALSE);

    // enable drag & drop
    this->DragAcceptFiles(TRUE);

    this->LoadWindowSettings();
    this->SetLanguage();

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
    if (this->m_DD.bHandled == true)
    {
        this->m_DD.bHandled = false;
        this->m_DD.pDlg = this;
        this->m_DD.hDrop = hDropInfo;
        this->m_DD.hThread = ::CreateThread(NULL, 0, ToolsDlgDropThread, (LPVOID)&this->m_DD, 0, &this->m_DD.dwThreadID);
        if (this->m_DD.hThread == NULL)
            this->m_DD.bHandled = true;
    }
    CMyDialogEx::OnDropFiles(hDropInfo);
}

void CToolsDlg::OnBnClickedOk()
{
    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
        nSelectedFormat = m_LstTools.GetNextSelectedItem(pos);
    else
        nSelectedFormat = -1;

    this->SaveWindowSettings();

    OnOK();
}

void CToolsDlg::OnBnClickedCancel()
{
    this->SaveWindowSettings();

    OnCancel();
}

void CToolsDlg::OnLvnItemchangedListTools(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    this->ListSelectionChange();

    *pResult = 0;
}

void CToolsDlg::OnBnClickedButtonImport()
{
    CString szFilter;
    szFilter.Format(_T("%s (*.tool)|*.tool|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310010, pszFileDialogs[9]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(TRUE, _T("tool"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadTool(szFileXml);
    }
}

void CToolsDlg::OnBnClickedButtonExport()
{
    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nSelected = m_LstTools.GetNextSelectedItem(pos);
        if (nSelected >= 0)
        {
            CFormat& format = m_Formats.Get(nSelected);

            CString szFilter;
            szFilter.Format(_T("%s (*.tool)|*.tool|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
                pConfig->GetString(0x00310010, pszFileDialogs[9]),
                pConfig->GetString(0x00310002, pszFileDialogs[1]),
                pConfig->GetString(0x00310001, pszFileDialogs[0]));

            CFileDialog fd(FALSE, _T("tool"), format.szId,
                OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
                szFilter, this);

            if (fd.DoModal() == IDOK)
            {
                CString szFileXml = fd.GetPathName();
                this->SaveTool(szFileXml, format);
            }
        }
    }
}

void CToolsDlg::OnBnClickedButtonDuplicate()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nSelected = m_LstTools.GetNextSelectedItem(pos);
        if (nSelected >= 0)
        {
            CFormat& format = m_Formats.Get(nSelected);
            CFormat copy = format;

            m_Formats.Insert(copy);

            int nItem = m_LstTools.GetItemCount();
            AddToList(copy, nItem);

            m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem, FALSE);
        }
    }

    bUpdate = false;

    this->ListSelectionChange();
}

void CToolsDlg::OnBnClickedButtonRemoveAllTools()
{
    if (m_Formats.Count() > 0)
    {
        CFormat& format = m_Formats.Get(nSelectedFormat);
        m_Formats.RemoveAll();

        m_LstTools.DeleteAllItems();

        this->ListSelectionChange();
    }
}

void CToolsDlg::OnBnClickedButtonRemoveTool()
{
    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);

        CFormat& format = m_Formats.Get(nItem);
        m_Formats.Remove(nItem);

        m_LstTools.DeleteItem(nItem);

        int nItems = m_LstTools.GetItemCount();
        if (nItem < nItems && nItems >= 0)
            m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        else if (nItem >= nItems && nItems >= 0)
            m_LstTools.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);

        this->ListSelectionChange();
    }
}

void CToolsDlg::OnBnClickedButtonAddTool()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    int nItem = m_LstTools.GetItemCount();

    CFormat format;
    format.szId = _T("ID");
    format.szName = pConfig->GetString(0x00230004, pszFormatsDialog[3]);
    format.szTemplate = _T("$EXE $OPTIONS $INFILE $OUTFILE");
    format.bPipeInput = true;
    format.bPipeOutput = false;
    format.szFunction = _T("- none -");
    format.szPath = _T("program.exe");
    format.nExitCodeSuccess = 0;
    format.nType = 0;
    format.szInputExtensions = _T("WAV");
    format.szOutputExtension = _T("EXT");
    format.nDefaultPreset = 0;

    CPreset preset;
    preset.szName = pConfig->GetString(0x00230005, pszFormatsDialog[4]);
    preset.szOptions = _T("");
    format.m_Presets.Insert(preset);

    m_Formats.Insert(format);

    AddToList(format, nItem);

    m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    m_LstTools.EnsureVisible(nItem, FALSE);

    bUpdate = false;

    this->ListSelectionChange();
}

void CToolsDlg::OnBnClickedButtonToolUp()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);
        if (nItem > 0)
        {
            CFormat& format1 = m_Formats.Get(nItem);
            CFormat& format2 = m_Formats.Get(nItem - 1);

            m_LstTools.SetItemText(nItem, TOOL_COLUMN_NAME, format2.szName);
            m_LstTools.SetItemText(nItem, TOOL_COLUMN_URL, format2.szTemplate);
            m_LstTools.SetItemText(nItem - 1, TOOL_COLUMN_NAME, format1.szName);
            m_LstTools.SetItemText(nItem - 1, TOOL_COLUMN_URL, format1.szTemplate);

            m_Formats.Swap(nItem, nItem - 1);

            m_LstTools.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem - 1, FALSE);
        }
    }

    bUpdate = false;
}

void CToolsDlg::OnBnClickedButtonToolDown()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);
        int nItems = m_LstTools.GetItemCount();
        if (nItem != (nItems - 1) && nItem >= 0)
        {
            CFormat& format1 = m_Formats.Get(nItem);
            CFormat& format2 = m_Formats.Get(nItem + 1);

            m_LstTools.SetItemText(nItem, TOOL_COLUMN_NAME, format2.szName);
            m_LstTools.SetItemText(nItem, TOOL_COLUMN_URL, format2.szTemplate);
            m_LstTools.SetItemText(nItem + 1, TOOL_COLUMN_NAME, format1.szName);
            m_LstTools.SetItemText(nItem + 1, TOOL_COLUMN_URL, format1.szTemplate);

            m_Formats.Swap(nItem, nItem + 1);

            m_LstTools.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem + 1, FALSE);
        }
    }

    bUpdate = false;
}

void CToolsDlg::OnBnClickedButtonUpdateTool()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);

        CString szId = _T("");
        CString szName = _T("");
        CString szExtension = _T("");
        CString szFormats = _T("");
        CString szTemplate = _T("");
        CString szPath = _T("");
        CString szFunction = _T("");

        this->m_EdtName.GetWindowText(szId);
        this->m_EdtPlatform.GetWindowText(szName);
        this->m_EdtFormats.GetWindowText(szFormats);
        this->m_EdtPlatform.GetWindowText(szName);
        this->m_EdtExtract.GetWindowText(szTemplate);
        this->m_EdtFile.GetWindowText(szPath);
        this->m_EdtPath.GetWindowText(szFunction);

        CFormat& format = m_Formats.Get(nItem);
        format.szId = szId;
        format.szName = szName;
        format.szOutputExtension = szExtension;
        format.szInputExtensions = szFormats;
        format.szTemplate = szTemplate;
        format.szPath = szPath;
        format.szFunction = szFunction;

        m_LstTools.SetItemText(nItem, TOOL_COLUMN_NAME, szName);
        m_LstTools.SetItemText(nItem, TOOL_COLUMN_URL, szTemplate);

        m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    }

    bUpdate = false;
}

void CToolsDlg::OnEnChangeEditToolName()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolPlatform()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolFormats()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolUrl()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolFile()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolExtract()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolPath()
{
    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnBnClickedButtonLoadTools()
{
    CString szFilter;
    szFilter.Format(_T("%s (*.tools)|*.tools|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310009, pszFileDialogs[8]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(TRUE, _T("tools"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadTools(szFileXml);
    }
}

void CToolsDlg::OnBnClickedButtonSaveTools()
{
    CString szFilter;
    szFilter.Format(_T("%s (*.tools)|*.tools|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310009, pszFileDialogs[8]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(FALSE, _T("tools"), _T("tools"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SaveTools(szFileXml);
    }
}

void CToolsDlg::OnBnClickedButtonEditFormats()
{
    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);

        CPresetsDlg dlg;
        dlg.pConfig = pConfig;
        dlg.nSelectedFormat = nItem;
        dlg.m_Formats = m_Formats;
        dlg.szPresetsDialogResize = pConfig->m_Options.szPresetsDialogResize;
        dlg.szPresetsListColumns = pConfig->m_Options.szPresetsListColumns;

        INT_PTR nRet = dlg.DoModal();
        if (nRet == IDOK)
        {
            this->m_Formats.RemoveAll();
            this->m_Formats = dlg.m_Formats;

            this->m_LstTools.DeleteAllItems();

            this->InsertFormatsToListCtrl();
            m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem, FALSE);

            this->ListSelectionChange();
        }

        pConfig->m_Options.szPresetsDialogResize = dlg.szPresetsDialogResize;
        pConfig->m_Options.szPresetsListColumns = dlg.szPresetsListColumns;
    }
}

void CToolsDlg::OnClose()
{
    this->SaveWindowSettings();

    CMyDialogEx::OnClose();
}

void CToolsDlg::LoadWindowSettings()
{
    // set window rectangle and position
    if (szToolsDialogResize.CompareNoCase(_T("")) != 0)
        this->SetWindowRectStr(szToolsDialogResize);

    // load columns width for FormatsList
    if (szToolsListColumns.CompareNoCase(_T("")) != 0)
    {
        int nColWidth[2];
        if (_stscanf(szToolsListColumns, _T("%d %d"),
            &nColWidth[0],
            &nColWidth[1]) == 2)
        {
            for (int i = 0; i < 2; i++)
                m_LstTools.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CToolsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szToolsDialogResize = this->GetWindowRectStr();

    // save columns width from FormatsList
    int nColWidth[11];
    for (int i = 0; i < 2; i++)
        nColWidth[i] = m_LstTools.GetColumnWidth(i);
    szToolsListColumns.Format(_T("%d %d"),
        nColWidth[0],
        nColWidth[1]);
}

void CToolsDlg::SetLanguage()
{
    CLanguageHelper helper(pConfig);

    helper.SetColumnText(m_LstTools, TOOL_COLUMN_NAME, 0x000C0001);
    helper.SetColumnText(m_LstTools, TOOL_COLUMN_URL, 0x000C0002);

    helper.SetWndText(this, 0x000C0010);
    helper.SetWndText(&m_BtnCancel, 0x000C0011);
    helper.SetWndText(&m_StcName, 0x000C0018);
    helper.SetWndText(&m_StcPlatform, 0x000C0019);
    helper.SetWndText(&m_StcFormats, 0x000C001B);
    helper.SetWndText(&m_StcUrl, 0x000C002B);
    helper.SetWndText(&m_StcFile, 0x000C001D);
    helper.SetWndText(&m_BtnEditFormats, 0x000C002C);
    helper.SetWndText(&m_StcExtract, 0x000C001F);
    helper.SetWndText(&m_StcPath, 0x000C0020);
    helper.SetWndText(&m_BtnMoveUp, 0x000C0022);
    helper.SetWndText(&m_BtnMoveDown, 0x000C0023);
    helper.SetWndText(&m_BtnImport, 0x000C002D);
    helper.SetWndText(&m_BtnExport, 0x000C002E);
    helper.SetWndText(&m_BtnDuplicate, 0x000C002F);
    helper.SetWndText(&m_BtnRemoveAll, 0x000C0024);
    helper.SetWndText(&m_BtnRemove, 0x000C0025);
    helper.SetWndText(&m_BtnAdd, 0x000C0026);
    helper.SetWndText(&m_BtnLoad, 0x000C0027);
    helper.SetWndText(&m_BtnSave, 0x000C0028);
    helper.SetWndText(&m_BtnUpdate, 0x000C0029);
    helper.SetWndText(&m_BtnOK, 0x000C002A);
}

void CToolsDlg::AddToList(CFormat &format, int nItem)
{
    LVITEM lvi;

    ZeroMemory(&lvi, sizeof(LVITEM));

    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iItem = nItem;

    lvi.iSubItem = TOOL_COLUMN_NAME;
    lvi.pszText = (LPTSTR)(LPCTSTR)(format.szName);
    m_LstTools.InsertItem(&lvi);

    lvi.iSubItem = TOOL_COLUMN_URL;
    lvi.pszText = (LPTSTR)(LPCTSTR)(format.szTemplate);
    m_LstTools.SetItemText(lvi.iItem, TOOL_COLUMN_URL, lvi.pszText);
}

void CToolsDlg::InsertFormatsToListCtrl()
{
    int nFormats = m_Formats.Count();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.Get(i);
        this->AddToList(format, i);
    }
}

void CToolsDlg::HandleDropFiles(HDROP hDropInfo)
{
    int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, NULL, 0);
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            int nReqChars = ::DragQueryFile(hDropInfo, i, NULL, 0);

            CString szFile;
            ::DragQueryFile(hDropInfo, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
            if (!(::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY))
            {
                CString szPath = szFile;
                CString szExt = ::GetFileExtension(szPath);

                if (szExt.CompareNoCase(_T("formats")) == 0)
                {
                    this->LoadTools(szPath);
                }
                else if (szExt.CompareNoCase(_T("format")) == 0)
                {
                    // Add format to formats list.
                    XmlFormats doc;
                    if (doc.Open(szPath) == true)
                    {
                        CFormat format;
                        doc.GetFormat(format);
                        m_Formats.Insert(format);

                        int nItem = m_Formats.Count() - 1;
                        this->AddToList(format, nItem);
                    }
                }
                else if (szExt.CompareNoCase(_T("presets")) == 0)
                {
                    // Add presets to current format presets list.
                    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
                    if (pos != NULL)
                    {
                        int nItem = m_LstTools.GetNextSelectedItem(pos);
                        CFormat& format = this->m_Formats.Get(nItem);

                        XmlPresets doc;
                        if (doc.Open(szPath) == true)
                        {
                            CFormat& format = this->m_Formats.Get(this->nSelectedFormat);
                            format.m_Presets.RemoveAll();
                            doc.GetPresets(format.m_Presets);
                        }
                    }
                }
                else if (szExt.CompareNoCase(_T("exe")) == 0)
                {
                    // Set current format exe path.
                    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
                    if (pos != NULL)
                    {
                        int nItem = m_LstTools.GetNextSelectedItem(pos);
                        CFormat& format = this->m_Formats.Get(nItem);
                        format.szPath = szPath;
                        this->m_EdtFile.SetWindowText(format.szPath);
                    }
                }
                else if (szExt.CompareNoCase(_T("progress")) == 0)
                {
                    // Set current format progress path.
                    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
                    if (pos != NULL)
                    {
                        int nItem = m_LstTools.GetNextSelectedItem(pos);
                        CFormat& format = this->m_Formats.Get(nItem);
                        format.szFunction = szPath;
                        this->m_EdtPath.SetWindowText(format.szFunction);
                    }
                }
            }

            szFile.ReleaseBuffer();
        }
    }

    ::DragFinish(hDropInfo);
}

void CToolsDlg::UpdateFields(CFormat &format)
{
    this->m_EdtName.SetWindowText(format.szId);
    this->m_EdtPlatform.SetWindowText(format.szName);
    this->m_EdtFormats.SetWindowText(format.szInputExtensions);

    CString szExitCodeSuccess;
    szExitCodeSuccess.Format(_T("%d\0"), format.nExitCodeSuccess);
    this->m_EdtUrl.SetWindowText(szExitCodeSuccess);

    this->m_EdtExtract.SetWindowText(format.szTemplate);
    this->m_EdtFile.SetWindowText(format.szPath);

    switch (format.nType)
    {
    case 0:
        this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER,
            IDC_RADIO_TYPE_DECODER,
            IDC_RADIO_TYPE_ENCODER);
        break;
    case 1:
        this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER,
            IDC_RADIO_TYPE_DECODER,
            IDC_RADIO_TYPE_DECODER);
        break;
    default:
        this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER,
            IDC_RADIO_TYPE_DECODER,
            IDC_RADIO_TYPE_ENCODER);
        break;
    };

    if (format.bPipeInput)
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_INPUT, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_INPUT, BST_UNCHECKED);

    if (format.bPipeOutput)
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_OUTPUT, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_OUTPUT, BST_UNCHECKED);

    this->m_EdtPath.SetWindowText(format.szFunction);
}

void CToolsDlg::ListSelectionChange()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);

        CFormat& format = this->m_Formats.Get(nItem);

        this->UpdateFields(format);
    }
    else
    {
        this->m_EdtName.SetWindowText(_T(""));
        this->m_EdtPlatform.SetWindowText(_T(""));
        this->m_EdtFormats.SetWindowText(_T(""));
        this->m_EdtUrl.SetWindowText(_T(""));

        this->m_EdtExtract.SetWindowText(_T(""));
        this->m_EdtFile.SetWindowText(_T(""));
        this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER,
            IDC_RADIO_TYPE_DECODER,
            IDC_RADIO_TYPE_ENCODER);
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_INPUT, BST_UNCHECKED);
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_OUTPUT, BST_UNCHECKED);
        this->m_EdtPath.SetWindowText(_T(""));
    }

    bUpdate = false;
}

void CToolsDlg::LoadTool(CString szFileXml)
{
    XmlFormats doc;
    if (doc.Open(szFileXml) == true)
    {
        CFormat format;
        doc.GetFormat(format);
        m_Formats.Insert(format);

        int nItem = m_Formats.Count() - 1;
        this->AddToList(format, nItem);
    }
    else
    {
        MessageBox(
            pConfig->GetString(0x00230002, pszFormatsDialog[1]),
            pConfig->GetString(0x00230001, pszFormatsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::SaveTool(CString szFileXml, CFormat &format)
{
    XmlFormats doc;
    doc.SetFormat(format);
    if (doc.Save(szFileXml) != true)
    {
        MessageBox(
            pConfig->GetString(0x00230003, pszFormatsDialog[2]),
            pConfig->GetString(0x00230001, pszFormatsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::LoadTools(CString szFileXml)
{
    XmlFormats doc;
    if (doc.Open(szFileXml) == true)
    {
        this->m_Formats.RemoveAll();
        this->m_LstTools.DeleteAllItems();

        doc.GetFormats(this->m_Formats);

        if (this->m_Formats.Count() > 0)
            nSelectedFormat = 0;

        this->InsertFormatsToListCtrl();
        this->ListSelectionChange();
    }
    else
    {
        MessageBox(
            pConfig->GetString(0x00230002, pszFormatsDialog[1]),
            pConfig->GetString(0x00230001, pszFormatsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::SaveTools(CString szFileXml)
{
    XmlFormats doc;
    doc.SetFormats(this->m_Formats);
    if (doc.Save(szFileXml) != true)
    {
        MessageBox(
            pConfig->GetString(0x00230003, pszFormatsDialog[2]),
            pConfig->GetString(0x00230001, pszFormatsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}
