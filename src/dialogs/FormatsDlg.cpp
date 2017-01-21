// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "FormatsDlg.h"

IMPLEMENT_DYNAMIC(CFormatsDlg, CDialog)
CFormatsDlg::CFormatsDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CFormatsDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAYICON);
    szFormatsDialogResize = _T("");
    szFormatsListColumns = _T("");
    this->bShowGridLines = true;
}

CFormatsDlg::~CFormatsDlg()
{

}

void CFormatsDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BUTTON_LOAD_CONFIG, m_BtnLoad);
    DDX_Control(pDX, IDC_BUTTON_SAVE_CONFIG, m_BtnSave);
    DDX_Control(pDX, IDC_BUTTON_FD_BROWSE, m_BtnBrowse);
    DDX_Control(pDX, IDC_BUTTON_FD_UPDATE_PRESET, m_BtnChange);
    DDX_Control(pDX, IDC_EDIT_FD_FORMATS, m_LstFormats);
    DDX_Control(pDX, IDC_EDIT_FD_CLI_PATH, m_EdtPath);
    DDX_Control(pDX, IDC_EDIT_FD_CLI_TEMPLATE, m_EdtTemplate);
    DDX_Control(pDX, IDC_EDIT_FD_CLI_PROGRESS, m_EdtProgress);
    DDX_Control(pDX, IDC_STATIC_GROUP_FD_PIPES, m_GrpPipes);
    DDX_Control(pDX, IDC_STATIC_FD_TEXT_PATH, m_StcPath);
    DDX_Control(pDX, IDC_STATIC_FD_TEXT_TEMPLATE, m_StcTemplate);
    DDX_Control(pDX, IDC_STATIC_FD_TEXT_PROGRESS, m_StcProgress);
}

BEGIN_MESSAGE_MAP(CFormatsDlg, CResizeDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BUTTON_LOAD_CONFIG, OnBnClickedButtonLoadConfig)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_CONFIG, OnBnClickedButtonSaveConfig)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDIT_FD_FORMATS, OnLvnItemchangedEditFdFormats)
    ON_BN_CLICKED(IDC_BUTTON_FD_BROWSE, OnBnClickedButtonFdBrowse)
    ON_BN_CLICKED(IDC_BUTTON_FD_BROWSE_PROGRESS, OnBnClickedButtonFdBrowseProgress)
    ON_BN_CLICKED(IDC_BUTTON_FD_UPDATE_PRESET, OnBnClickedButtonFdUpdatePreset)
    ON_EN_CHANGE(IDC_EDIT_FD_CLI_PATH, OnEnChangeEditFdCliPath)
    ON_EN_CHANGE(IDC_EDIT_FD_CLI_TEMPLATE, OnEnChangeEditFdCliTemplate)
    ON_EN_CHANGE(IDC_EDIT_FD_CLI_PROGRESS, OnEnChangeEditFdCliProgress)
    ON_BN_CLICKED(IDC_CHECK_FD_PIPES_INPUT, OnBnClickedCheckFdPipesInput)
    ON_BN_CLICKED(IDC_CHECK_FD_PIPES_OUTPUT, OnBnClickedCheckFdPipesOutput)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CFormatsDlg::OnInitDialog()
{
    CResizeDialog::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // set pipes group font to normal
    m_GrpPipes.SetBold(false);

    // update list style
    DWORD dwExStyle = m_LstFormats.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    m_LstFormats.SetExtendedStyle(dwExStyle);

    // insert all ListCtrl columns
    m_LstFormats.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 80);
    m_LstFormats.InsertColumn(1, _T("Template"), LVCFMT_LEFT, 120);
    m_LstFormats.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 140);
    m_LstFormats.InsertColumn(3, _T("In Pipes"), LVCFMT_LEFT, 70);
    m_LstFormats.InsertColumn(4, _T("Out Pipes"), LVCFMT_LEFT, 70);
    m_LstFormats.InsertColumn(5, _T("Function"), LVCFMT_LEFT, 90);

    // insert all ListCtrl items and sub items
    this->InsertFormatsToListCtrl();

    // setup resize anchors
    AddAnchor(IDC_EDIT_FD_FORMATS, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_FD_TEXT_PATH, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_FD_CLI_PATH, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_FD_BROWSE, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_GROUP_FD_PIPES, BOTTOM_LEFT);
    AddAnchor(IDC_CHECK_FD_PIPES_INPUT, BOTTOM_LEFT);
    AddAnchor(IDC_CHECK_FD_PIPES_OUTPUT, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_FD_TEXT_TEMPLATE, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FD_CLI_TEMPLATE, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_FD_TEXT_PROGRESS, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FD_CLI_PROGRESS, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_FD_BROWSE_PROGRESS, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_LOAD_CONFIG, BOTTOM_LEFT);
    AddAnchor(IDC_BUTTON_SAVE_CONFIG, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT);

    this->LoadWindowSettings();

    if (this->bShowGridLines == true)
        this->ShowGridlines(true);

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
        CResizeDialog::OnPaint();
    }
}

HCURSOR CFormatsDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CFormatsDlg::ShowGridlines(bool bShow)
{
    DWORD dwExStyle = m_LstFormats.GetExtendedStyle();
    if (bShow == true)
    {
        dwExStyle |= LVS_EX_GRIDLINES;
        m_LstFormats.SetExtendedStyle(dwExStyle);
    }
    else
    {
        if (dwExStyle & LVS_EX_GRIDLINES)
        {
            dwExStyle = dwExStyle ^ LVS_EX_GRIDLINES;
            m_LstFormats.SetExtendedStyle(dwExStyle);
        }
    }
}

void CFormatsDlg::UpdateEditableFields(void)
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CString szTemplate = this->m_LstFormats.GetItemText(nItem, 1);
        CString szPath = this->m_LstFormats.GetItemText(nItem, 2);
        CString szCheckIn = this->m_LstFormats.GetItemText(nItem, 3);
        CString szCheckOut = this->m_LstFormats.GetItemText(nItem, 4);
        CString szFunctions = this->m_LstFormats.GetItemText(nItem, 5);

        this->m_EdtTemplate.SetWindowText(szTemplate);
        this->m_EdtPath.SetWindowText(szPath);

        if (szCheckIn.Compare(_T("true")) == 0)
            CheckDlgButton(IDC_CHECK_FD_PIPES_INPUT, BST_CHECKED);
        else
            CheckDlgButton(IDC_CHECK_FD_PIPES_INPUT, BST_UNCHECKED);

        if (szCheckOut.Compare(_T("true")) == 0)
            CheckDlgButton(IDC_CHECK_FD_PIPES_OUTPUT, BST_CHECKED);
        else
            CheckDlgButton(IDC_CHECK_FD_PIPES_OUTPUT, BST_UNCHECKED);

        this->m_EdtProgress.SetWindowText(szFunctions);
    }
    else
    {
        this->m_EdtTemplate.SetWindowText(_T(""));
        this->m_EdtPath.SetWindowText(_T(""));
        CheckDlgButton(IDC_CHECK_FD_PIPES_INPUT, BST_UNCHECKED);
        CheckDlgButton(IDC_CHECK_FD_PIPES_OUTPUT, BST_UNCHECKED);
        this->m_EdtProgress.SetWindowText(_T(""));
    }
}

void CFormatsDlg::LoadWindowSettings()
{
    // set window rectangle and position
    if (szFormatsDialogResize.Compare(_T("")) != 0)
        this->SetWindowRectStr(szFormatsDialogResize);

    // load columns width for FormatsList
    if (szFormatsListColumns.Compare(_T("")) != 0)
    {
        int nColWidth[6];
        if (_stscanf(szFormatsListColumns, _T("%d %d %d %d %d %d"),
            &nColWidth[0],
            &nColWidth[1],
            &nColWidth[2],
            &nColWidth[3],
            &nColWidth[4],
            &nColWidth[5]) == 6)
        {
            for (int i = 0; i < 6; i++)
                m_LstFormats.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CFormatsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szFormatsDialogResize = this->GetWindowRectStr();

    // save columns width from FormatsList
    int nColWidth[6];
    for (int i = 0; i < 6; i++)
        nColWidth[i] = m_LstFormats.GetColumnWidth(i);
    szFormatsListColumns.Format(_T("%d %d %d %d %d %d"),
        nColWidth[0],
        nColWidth[1],
        nColWidth[2],
        nColWidth[3],
        nColWidth[4],
        nColWidth[5]);
}

bool CFormatsDlg::BrowseForCliExe(CString szDefaultFName, CEdit *pEdit, int nID)
{
    CFileDialog fd(TRUE, _T("exe"), szDefaultFName,
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Exe Files (*.exe)|*.exe|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szPath = fd.GetPathName();
        pEdit->SetWindowText(szPath);
        return true;
    }
    return false;
}

bool CFormatsDlg::BrowseForProgress(CString szDefaultFName, CEdit *pEdit, int nID)
{
    CFileDialog fd(TRUE, _T("progress"), szDefaultFName,
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Progress Files (*.progress)|*.progress|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szPath;
        szPath = fd.GetPathName();
        pEdit->SetWindowText(szPath);
        return true;
    }
    return false;
}

void CFormatsDlg::OnBnClickedOk()
{
    this->UpdateFormatsFromListCtrl();
    this->SaveWindowSettings();

    OnOK();
}

void CFormatsDlg::OnBnClickedCancel()
{
    this->SaveWindowSettings();

    OnCancel();
}

void CFormatsDlg::InsertFormatsToListCtrl()
{
    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);

        LVITEM lvi;

        ZeroMemory(&lvi, sizeof(LVITEM));

        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;

        lvi.iSubItem = 0;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.szName);
        m_LstFormats.InsertItem(&lvi);

        lvi.iSubItem = 1;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.szTemplate);
        m_LstFormats.SetItemText(lvi.iItem, 1, lvi.pszText);

        lvi.iSubItem = 2;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.szPath);
        m_LstFormats.SetItemText(lvi.iItem, 2, lvi.pszText);

        lvi.iSubItem = 3;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.bInput) ? _T("true") : _T("false");
        m_LstFormats.SetItemText(lvi.iItem, 3, lvi.pszText);

        lvi.iSubItem = 4;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.bOutput) ? _T("true") : _T("false");
        m_LstFormats.SetItemText(lvi.iItem, 4, lvi.pszText);

        lvi.iSubItem = 5;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.szFunction);
        m_LstFormats.SetItemText(lvi.iItem, 5, lvi.pszText);
    }
}

void CFormatsDlg::UpdateFormatsFromListCtrl()
{
    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);

        format.szTemplate = m_LstFormats.GetItemText(i, 1);
        format.szPath = m_LstFormats.GetItemText(i, 2);
        format.bInput = (m_LstFormats.GetItemText(i, 3).Compare(_T("true")) == 0) ? true : false;
        format.bOutput = (m_LstFormats.GetItemText(i, 4).Compare(_T("true")) == 0) ? true : false;
        format.szFunction = m_LstFormats.GetItemText(i, 5);
    }
}

void CFormatsDlg::LoadFormatsFile(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.Open(szFileXml) == true)
    {
        tinyxml2::XMLElement *pFormatsElem = doc.FirstChildElement();
        if (!pFormatsElem)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        const char *szRoot = pFormatsElem->Value();
        const char *szRootName = "Formats";
        if (strcmp(szRootName, szRoot) != 0)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        m_Formats.RemoveAllNodes();
        m_LstFormats.DeleteAllItems();

        doc.GetFormats(pFormatsElem, m_Formats);

        this->InsertFormatsToListCtrl();
        this->UpdateEditableFields();
    }
    else
    {
        MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CFormatsDlg::SaveFormatsFile(CString szFileXml)
{
    XmlConfiguration doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    doc.LinkEndChild(decl);

    tinyxml2::XMLElement *pFormatsElem = doc.NewElement("Formats");
    doc.LinkEndChild(pFormatsElem);

    this->UpdateFormatsFromListCtrl();

    doc.SetFormats(pFormatsElem, m_Formats);

    if (doc.Save(szFileXml) != true)
        MessageBox(_T("Failed to save file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
}

void CFormatsDlg::OnBnClickedButtonLoadConfig()
{
    CFileDialog fd(TRUE, _T("formats"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        _T("Formats Files (*.formats)|*.formats|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadFormatsFile(szFileXml);
    }
}

void CFormatsDlg::OnBnClickedButtonSaveConfig()
{
    CFileDialog fd(FALSE, _T("formats"), _T("formats"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Formats Files (*.formats)|*.formats|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SaveFormatsFile(szFileXml);
    }
}

void CFormatsDlg::OnLvnItemchangedEditFdFormats(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    this->UpdateEditableFields();

    *pResult = 0;
}

void CFormatsDlg::OnBnClickedButtonFdBrowse()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szPath;
        this->m_EdtPath.GetWindowText(szPath);
        BrowseForCliExe(szPath, &m_EdtPath, nItem);
    }
    else
    {
        MessageBox(_T("Please select item in the list."),
            _T("INFO"),
            MB_OK | MB_ICONINFORMATION);
    }
}

void CFormatsDlg::OnBnClickedButtonFdBrowseProgress()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szFunction;
        this->m_EdtProgress.GetWindowText(szFunction);
        BrowseForProgress(szFunction, &m_EdtProgress, nItem);
    }
    else
    {
        MessageBox(_T("Please select item in the list."),
            _T("INFO"),
            MB_OK | MB_ICONINFORMATION);
    }
}

void CFormatsDlg::OnBnClickedButtonFdUpdatePreset()
{
    CString szTemplate = _T("");
    CString szPath = _T("");
    CString szCheckIn = _T("false");
    CString szCheckOut = _T("false");
    CString szFunction = _T("");

    this->m_EdtTemplate.GetWindowText(szTemplate);

    if (IsDlgButtonChecked(IDC_CHECK_FD_PIPES_INPUT) == BST_CHECKED)
        szCheckIn = _T("true");

    if (IsDlgButtonChecked(IDC_CHECK_FD_PIPES_OUTPUT) == BST_CHECKED)
        szCheckOut = _T("true");

    this->m_EdtPath.GetWindowText(szPath);

    this->m_EdtProgress.GetWindowText(szFunction);

    if (szPath.GetLength() > 0)
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);

            m_LstFormats.SetItemText(nItem, 1, szTemplate);
            m_LstFormats.SetItemText(nItem, 2, szPath);
            m_LstFormats.SetItemText(nItem, 3, szCheckIn);
            m_LstFormats.SetItemText(nItem, 4, szCheckOut);
            m_LstFormats.SetItemText(nItem, 5, szFunction);
            m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
}

void CFormatsDlg::OnEnChangeEditFdCliPath()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szPath;
        this->m_EdtPath.GetWindowText(szPath);
        m_LstFormats.SetItemText(nItem, 2, szPath);
    }
}

void CFormatsDlg::OnEnChangeEditFdCliTemplate()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szTemplate;
        this->m_EdtTemplate.GetWindowText(szTemplate);
        m_LstFormats.SetItemText(nItem, 1, szTemplate);
    }
}

void CFormatsDlg::OnEnChangeEditFdCliProgress()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szFunction;
        this->m_EdtProgress.GetWindowText(szFunction);
        m_LstFormats.SetItemText(nItem, 5, szFunction);
    }
}

void CFormatsDlg::OnBnClickedCheckFdPipesInput()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szCheckIn = _T("false");
        if (IsDlgButtonChecked(IDC_CHECK_FD_PIPES_INPUT) == BST_CHECKED)
            szCheckIn = _T("true");

        m_LstFormats.SetItemText(nItem, 3, szCheckIn);
    }
}

void CFormatsDlg::OnBnClickedCheckFdPipesOutput()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        CString szCheckOut = _T("false");
        if (IsDlgButtonChecked(IDC_CHECK_FD_PIPES_OUTPUT) == BST_CHECKED)
            szCheckOut = _T("true");

        m_LstFormats.SetItemText(nItem, 4, szCheckOut);
    }
}

void CFormatsDlg::OnClose()
{
    this->SaveWindowSettings();

    CResizeDialog::OnClose();
}
