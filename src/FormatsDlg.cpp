// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "UnicodeUtf8.h"
#include "Utf8String.h"
#include "FormatsDlg.h"
#include ".\formatsdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CFormatsDlg, CDialog)
CFormatsDlg::CFormatsDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CFormatsDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        m_Format[i].szTemplate = g_szDefaultTemplate[i];
        m_Format[i].szPath = g_szDefaultPath[i];
        m_Format[i].bInput = g_bDefaultInPipes[i];
        m_Format[i].bOutput = g_bDefaultOutPipes[i];
        m_Format[i].szFunction = g_bDefaultFunction[i];
    }

    szFormatsWndResize = _T("");
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
    DDX_Control(pDX, IDC_BUTTON_DEFAULT_CONFIG, m_BtnDefaults);
    DDX_Control(pDX, IDC_BUTTON_FD_BROWSE, m_BtnBrowse);
    DDX_Control(pDX, IDC_BUTTON_FD_UPDATE_PRESET, m_BtnChange);
    DDX_Control(pDX, IDC_EDIT_FD_FORMATS, m_Formats);
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
    ON_BN_CLICKED(IDC_BUTTON_DEFAULT_CONFIG, OnBnClickedButtonDefaultConfig)
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
    DWORD dwExStyle = m_Formats.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    m_Formats.SetExtendedStyle(dwExStyle);

    // insert all ListCtl columns
    m_Formats.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 80);
    m_Formats.InsertColumn(1, _T("Template"), LVCFMT_LEFT, 120);
    m_Formats.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 140);
    m_Formats.InsertColumn(3, _T("In Pipes"), LVCFMT_LEFT, 70);
    m_Formats.InsertColumn(4, _T("Out Pipes"), LVCFMT_LEFT, 70);
    m_Formats.InsertColumn(5, _T("Function"), LVCFMT_LEFT, 90);

    // insert all ListCtl items and subitems
    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        LVITEM lvi;

        ZeroMemory(&lvi, sizeof(LVITEM));

        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;

        lvi.iSubItem = 0;
        lvi.pszText = (LPTSTR)(LPCTSTR)(g_szFormatNames[i]);
        m_Formats.InsertItem(&lvi);

        lvi.iSubItem = 1;
        lvi.pszText = (LPTSTR)(LPCTSTR)(m_Format[i].szTemplate);
        m_Formats.SetItemText(lvi.iItem, 1, lvi.pszText);

        lvi.iSubItem = 2;
        lvi.pszText = (LPTSTR)(LPCTSTR)(m_Format[i].szPath);
        m_Formats.SetItemText(lvi.iItem, 2, lvi.pszText);

        lvi.iSubItem = 3;
        lvi.pszText = (LPTSTR)(LPCTSTR)(m_Format[i].bInput) ? _T("true") : _T("false");
        m_Formats.SetItemText(lvi.iItem, 3, lvi.pszText);

        lvi.iSubItem = 4;
        lvi.pszText = (LPTSTR)(LPCTSTR)(m_Format[i].bOutput) ? _T("true") : _T("false");
        m_Formats.SetItemText(lvi.iItem, 4, lvi.pszText);

        lvi.iSubItem = 5;
        lvi.pszText = (LPTSTR)(LPCTSTR)(m_Format[i].szFunction);
        m_Formats.SetItemText(lvi.iItem, 5, lvi.pszText);
    }

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
    AddAnchor(IDC_BUTTON_DEFAULT_CONFIG, BOTTOM_LEFT);
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
    DWORD dwExStyle = m_Formats.GetExtendedStyle();
    if (bShow == true)
    {
        dwExStyle |= LVS_EX_GRIDLINES;
        m_Formats.SetExtendedStyle(dwExStyle);
    }
    else
    {
        if (dwExStyle & LVS_EX_GRIDLINES)
        {
            dwExStyle = dwExStyle ^ LVS_EX_GRIDLINES;
            m_Formats.SetExtendedStyle(dwExStyle);
        }
    }
}

void CFormatsDlg::UpdateEditableFields(void)
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);

        CString szTemplate = this->m_Formats.GetItemText(nItem, 1);
        CString szPath = this->m_Formats.GetItemText(nItem, 2);
        CString szCheckIn = this->m_Formats.GetItemText(nItem, 3);
        CString szCheckOut = this->m_Formats.GetItemText(nItem, 4);
        CString szFunctions = this->m_Formats.GetItemText(nItem, 5);

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
    if (szFormatsWndResize.Compare(_T("")) != 0)
        this->SetWindowRectStr(szFormatsWndResize);

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
                m_Formats.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CFormatsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szFormatsWndResize = this->GetWindowRectStr();

    // save columns width from FormatsList
    int nColWidth[6];
    for (int i = 0; i < 6; i++)
        nColWidth[i] = m_Formats.GetColumnWidth(i);
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
    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        m_Format[i].szTemplate = m_Formats.GetItemText(i, 1);
        m_Format[i].szPath = m_Formats.GetItemText(i, 2);
        m_Format[i].bInput = (m_Formats.GetItemText(i, 3).Compare(_T("true")) == 0) ? true : false;
        m_Format[i].bOutput = (m_Formats.GetItemText(i, 4).Compare(_T("true")) == 0) ? true : false;
        m_Format[i].szFunction = m_Formats.GetItemText(i, 5);
    }

    this->SaveWindowSettings();

    OnOK();
}

void CFormatsDlg::OnBnClickedCancel()
{
    this->SaveWindowSettings();

    OnCancel();
}

void CFormatsDlg::LoadFormatsFile(CString szFileXml)
{
    CXMLDocumentW doc;
    if (doc.LoadFileW(szFileXml) == true)
    {
        // Root = Formats
        tinyxml2::XMLElement* pRootElem = doc.FirstChildElement();
        if (!pRootElem)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        // check for "Formats"
        const char *szRoot = pRootElem->Value();
        const char *szRootName = "Formats";
        if (strcmp(szRootName, szRoot) != 0)
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
            return;
        }

        // load all elements
        tinyxml2::XMLElement* pFormatElem = pRootElem->FirstChildElement("Format");
        for (pFormatElem; pFormatElem; pFormatElem = pFormatElem->NextSiblingElement())
        {
            int nFormat = -1;

            const char *pszName = pFormatElem->Attribute("name");
            if (pszName != NULL)
            {
                CString szBuff = GetConfigString(pszName);

                nFormat = ::GetFormatId(szBuff);

                // check if this is valid format name
                if ((nFormat < 0) || (nFormat >= NUM_FORMAT_NAMES))
                {
                    // invalid format Id
                    continue;
                }
            }
            else
            {
                // unknown or invalid format
                continue;
            }

            const char *pszTemplate = pFormatElem->Attribute("template");
            if (pszTemplate != NULL)
            {
                m_Format[nFormat].szTemplate = GetConfigString(pszTemplate);
            }

            const char *pszPipesInput = pFormatElem->Attribute("input");
            if (pszPipesInput != NULL)
            {
                CString szBuff = GetConfigString(pszPipesInput);
                if (szBuff.CompareNoCase(_T("true")) == 0)
                    m_Format[nFormat].bInput = true;
                else
                    m_Format[nFormat].bInput = false;
            }

            const char *pszPipesOutput = pFormatElem->Attribute("output");
            if (pszPipesOutput != NULL)
            {
                CString szBuff = GetConfigString(pszPipesOutput);
                if (szBuff.CompareNoCase(_T("true")) == 0)
                    m_Format[nFormat].bOutput = true;
                else
                    m_Format[nFormat].bOutput = false;
            }

            const char *pszFunction = pFormatElem->Attribute("function");
            if (pszFunction != NULL)
            {
                m_Format[nFormat].szFunction = GetConfigString(pszFunction);
            }

            const char *tmpBuff = pFormatElem->GetText();
            m_Format[nFormat].szPath = GetConfigString(tmpBuff);

            m_Formats.SetItemText(nFormat, 1, m_Format[nFormat].szTemplate);
            m_Formats.SetItemText(nFormat, 2, m_Format[nFormat].szPath);
            m_Formats.SetItemText(nFormat, 3, (m_Format[nFormat].bInput) ? _T("true") : _T("false"));
            m_Formats.SetItemText(nFormat, 4, (m_Format[nFormat].bOutput) ? _T("true") : _T("false"));
            m_Formats.SetItemText(nFormat, 5, m_Format[nFormat].szFunction);
        }

        this->UpdateEditableFields();
    }
    else
    {
        MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CFormatsDlg::SaveFormatsFile(CString szFileXml)
{
    CXMLDocumentW doc;

    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.LinkEndChild(decl);

    // root: Formats
    tinyxml2::XMLElement *formats = doc.NewElement("Formats");
    doc.LinkEndChild(formats);

    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        CUtf8String m_Utf8;

        m_Format[i].szTemplate = m_Formats.GetItemText(i, 1);
        m_Format[i].szPath = m_Formats.GetItemText(i, 2);
        m_Format[i].bInput = (m_Formats.GetItemText(i, 3).Compare(_T("true")) == 0) ? true : false;
        m_Format[i].bOutput = (m_Formats.GetItemText(i, 4).Compare(_T("true")) == 0) ? true : false;
        m_Format[i].szFunction = m_Formats.GetItemText(i, 5);

        tinyxml2::XMLElement *format = doc.NewElement("Format");

        format->LinkEndChild(doc.NewText(m_Utf8.Create(m_Format[i].szPath)));
        m_Utf8.Clear();

        format->SetAttribute("name", m_Utf8.Create(g_szFormatNames[i]));
        m_Utf8.Clear();

        format->SetAttribute("template", m_Utf8.Create(m_Format[i].szTemplate));
        m_Utf8.Clear();

        format->SetAttribute("input", (m_Format[i].bInput) ? "true" : "false");
        format->SetAttribute("output", (m_Format[i].bOutput) ? "true" : "false");

        format->SetAttribute("function", m_Utf8.Create(m_Format[i].szFunction));
        m_Utf8.Clear();

        formats->LinkEndChild(format);
    }

    // save file
    if (doc.SaveFileW(szFileXml) != true)
        MessageBox(_T("Failed to save file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
}

void CFormatsDlg::OnBnClickedButtonLoadConfig()
{
    // NOTE:
    // the presets file do not need to contain all formats
    // this enables loading partial patches and configurations
    // for specific encoder/decoder created by other users

    // load config from .formats file
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
    // save config to .formats file
    CFileDialog fd(FALSE, _T("formats"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        _T("Formats Files (*.formats)|*.formats|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SaveFormatsFile(szFileXml);
    }
}

void CFormatsDlg::OnBnClickedButtonDefaultConfig()
{
    // load defaults
    for (int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        m_Format[i].szTemplate = g_szDefaultTemplate[i];
        m_Format[i].szPath = g_szDefaultPath[i];
        m_Format[i].bInput = g_bDefaultInPipes[i];
        m_Format[i].bOutput = g_bDefaultOutPipes[i];
        m_Format[i].szFunction = g_bDefaultFunction[i];

        m_Formats.SetItemText(i, 1, g_szDefaultTemplate[i]);
        m_Formats.SetItemText(i, 2, g_szDefaultPath[i]);
        m_Formats.SetItemText(i, 3, (g_bDefaultInPipes[i]) ? _T("true") : _T("false"));
        m_Formats.SetItemText(i, 4, (g_bDefaultOutPipes[i]) ? _T("true") : _T("false"));
        m_Formats.SetItemText(i, 5, g_bDefaultFunction[i]);
    }

    this->UpdateEditableFields();
}

void CFormatsDlg::OnLvnItemchangedEditFdFormats(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    this->UpdateEditableFields();

    *pResult = 0;
}

void CFormatsDlg::OnBnClickedButtonFdBrowse()
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        BrowseForCliExe(g_szDefaultPath[nItem], &m_EdtPath, nItem);
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
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        BrowseForProgress(g_bDefaultFunction[nItem], &m_EdtProgress, nItem);
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
    // NOTE: 
    // button is disabled instead live update is working
    // with EditBox change event and CheckBoxes click event
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
        POSITION pos = m_Formats.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int nItem = m_Formats.GetNextSelectedItem(pos);

            m_Formats.SetItemText(nItem, 1, szTemplate);
            m_Formats.SetItemText(nItem, 2, szPath);
            m_Formats.SetItemText(nItem, 3, szCheckIn);
            m_Formats.SetItemText(nItem, 4, szCheckOut);
            m_Formats.SetItemText(nItem, 5, szFunction);
            m_Formats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
}

void CFormatsDlg::OnEnChangeEditFdCliPath()
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        CString szPath;
        this->m_EdtPath.GetWindowText(szPath);
        m_Formats.SetItemText(nItem, 2, szPath);
    }
}

void CFormatsDlg::OnEnChangeEditFdCliTemplate()
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        CString szTemplate;
        this->m_EdtTemplate.GetWindowText(szTemplate);
        m_Formats.SetItemText(nItem, 1, szTemplate);
    }
}

void CFormatsDlg::OnEnChangeEditFdCliProgress()
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        CString szFunction;
        this->m_EdtProgress.GetWindowText(szFunction);
        m_Formats.SetItemText(nItem, 5, szFunction);
    }
}

void CFormatsDlg::OnBnClickedCheckFdPipesInput()
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        CString szCheckIn = _T("false");
        if (IsDlgButtonChecked(IDC_CHECK_FD_PIPES_INPUT) == BST_CHECKED)
            szCheckIn = _T("true");

        m_Formats.SetItemText(nItem, 3, szCheckIn);
    }
}

void CFormatsDlg::OnBnClickedCheckFdPipesOutput()
{
    POSITION pos = m_Formats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_Formats.GetNextSelectedItem(pos);
        CString szCheckOut = _T("false");
        if (IsDlgButtonChecked(IDC_CHECK_FD_PIPES_OUTPUT) == BST_CHECKED)
            szCheckOut = _T("true");

        m_Formats.SetItemText(nItem, 4, szCheckOut);
    }
}

void CFormatsDlg::OnClose()
{
    this->SaveWindowSettings();

    CResizeDialog::OnClose();
}
