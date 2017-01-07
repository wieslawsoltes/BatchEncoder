//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

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

    for(int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        szFormatTemplate[i] = g_szDefaultTemplate[i];
        szFormatPath[i] = g_szDefaultPath[i];
        bFormatInput[i] = g_bDefaultInPipes[i];
        bFormatOutput[i] = g_bDefaultOutPipes[i];
        szFormatFunction[i] = g_bDefaultFunction[i];
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
    DWORD dwExStyle = m_LstFormats.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT;
    m_LstFormats.SetExtendedStyle(dwExStyle);

    // insert all ListCtl columns
    m_LstFormats.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 80);
    m_LstFormats.InsertColumn(1, _T("Template"), LVCFMT_LEFT, 120);
    m_LstFormats.InsertColumn(2, _T("Path"), LVCFMT_LEFT, 140);
    m_LstFormats.InsertColumn(3, _T("In Pipes"), LVCFMT_LEFT, 70);
    m_LstFormats.InsertColumn(4, _T("Out Pipes"), LVCFMT_LEFT, 70);
    m_LstFormats.InsertColumn(5, _T("Function"), LVCFMT_LEFT, 90);

    // insert all ListCtl items and subitems
    for(int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        LVITEM lvi;

        ZeroMemory(&lvi, sizeof(LVITEM));

        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;

        lvi.iSubItem = 0;
        lvi.pszText = (LPTSTR) (LPCTSTR) (g_szFormatNames[i]);
        m_LstFormats.InsertItem(&lvi);

        lvi.iSubItem = 1;
        lvi.pszText = (LPTSTR) (LPCTSTR) (szFormatTemplate[i]);
        m_LstFormats.SetItemText(lvi.iItem, 1, lvi.pszText);

        lvi.iSubItem = 2;
        lvi.pszText = (LPTSTR) (LPCTSTR) (szFormatPath[i]);
        m_LstFormats.SetItemText(lvi.iItem, 2, lvi.pszText);

        lvi.iSubItem = 3;
        lvi.pszText = (LPTSTR) (LPCTSTR) (bFormatInput[i]) ? _T("true") : _T("false");
        m_LstFormats.SetItemText(lvi.iItem, 3, lvi.pszText);

        lvi.iSubItem = 4;
        lvi.pszText = (LPTSTR) (LPCTSTR) (bFormatOutput[i]) ? _T("true") : _T("false");
        m_LstFormats.SetItemText(lvi.iItem, 4, lvi.pszText);

        lvi.iSubItem = 5;
        lvi.pszText = (LPTSTR) (LPCTSTR) (szFormatFunction[i]);
        m_LstFormats.SetItemText(lvi.iItem, 5, lvi.pszText);
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

    if(this->bShowGridLines == true)
        this->ShowGridlines(true);

    // ResetMinTrackSize();
    // ResetMaxTrackSize();
    // ResetMaximizedRect();

    return TRUE;
}

void CFormatsDlg::OnPaint()
{
    if(IsIconic())
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
    if(bShow == true)
    {
        dwExStyle |= LVS_EX_GRIDLINES;
        m_LstFormats.SetExtendedStyle(dwExStyle);
    }
    else
    {
        if(dwExStyle & LVS_EX_GRIDLINES)
        {
            dwExStyle = dwExStyle ^ LVS_EX_GRIDLINES;
            m_LstFormats.SetExtendedStyle(dwExStyle);
        }
    }
}

void CFormatsDlg::UpdateEditableFields(void)
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if(pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CString szTemplate = this->m_LstFormats.GetItemText(nItem, 1);
        CString szPath = this->m_LstFormats.GetItemText(nItem, 2);
        CString szCheckIn = this->m_LstFormats.GetItemText(nItem, 3);
        CString szCheckOut = this->m_LstFormats.GetItemText(nItem, 4);
        CString szFunctions = this->m_LstFormats.GetItemText(nItem, 5);

        this->m_EdtTemplate.SetWindowText(szTemplate);
        this->m_EdtPath.SetWindowText(szPath);

        if(szCheckIn.Compare(_T("true")) == 0)
            CheckDlgButton(IDC_CHECK_FD_PIPES_INPUT, BST_CHECKED);
        else
            CheckDlgButton(IDC_CHECK_FD_PIPES_INPUT, BST_UNCHECKED);

        if(szCheckOut.Compare(_T("true")) == 0)
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
    if(szFormatsWndResize.Compare(_T("")) != 0)
        this->SetWindowRectStr(szFormatsWndResize);

    // load columns width for FormatsList
    if(szFormatsListColumns.Compare(_T("")) != 0)
    {
        int nColWidth[6];
        if(_stscanf(szFormatsListColumns, _T("%d %d %d %d %d %d"), 
            &nColWidth[0], 
            &nColWidth[1],
            &nColWidth[2],
            &nColWidth[3],
            &nColWidth[4],
            &nColWidth[5]) == 6)
        {
            for(int i = 0; i < 6; i++)
                m_LstFormats.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CFormatsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szFormatsWndResize = this->GetWindowRectStr();

    // save columns width from FormatsList
    int nColWidth[6];
    for(int i = 0; i < 6; i++)
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

    ::SetBrowsePath(fd, szBrowsePath[(2 + nID)]);

    if(fd.DoModal() == IDOK)
    {
        szBrowsePath[(2 + nID)] = ::GetBrowsePath(fd);

        CString szPath;
        szPath = fd.GetPathName();
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

    ::SetBrowsePath(fd, szBrowsePath[(NUM_BROWSE_PATH_FORMATS + nID)]);

    if(fd.DoModal() == IDOK)
    {
        szBrowsePath[(NUM_BROWSE_PATH_FORMATS + nID)] = ::GetBrowsePath(fd);

        CString szPath;
        szPath = fd.GetPathName();
        pEdit->SetWindowText(szPath);

        return true;
    }
    return false;
}

void CFormatsDlg::OnBnClickedOk()
{
    for(int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        szFormatTemplate[i]  = m_LstFormats.GetItemText(i, 1);
        szFormatPath[i]  = m_LstFormats.GetItemText(i, 2);
        bFormatInput[i] = (m_LstFormats.GetItemText(i, 3).Compare(_T("true")) == 0) ? true : false;
        bFormatOutput[i] = (m_LstFormats.GetItemText(i, 4).Compare(_T("true")) == 0) ? true : false;
        szFormatFunction[i]  = m_LstFormats.GetItemText(i, 5);
    }

    this->SaveWindowSettings();

    OnOK();
}

void CFormatsDlg::OnBnClickedCancel()
{
    this->SaveWindowSettings();

    OnCancel();
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

    ::SetBrowsePath(fd, szBrowsePath[0]);

    if(fd.DoModal() == IDOK)
    {
        szBrowsePath[0] = ::GetBrowsePath(fd);

        CString szFileXml = fd.GetPathName();

        CTiXmlDocumentW doc;
        if(doc.LoadFileW(szFileXml) == true)
        {
            TiXmlHandle hDoc(&doc);
            TiXmlElement* pElem;

            TiXmlHandle hRoot(0);

            // Root = Formats
            pElem = hDoc.FirstChildElement().Element();
            if(!pElem) 
            {
                MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
                return;
            }

            hRoot = TiXmlHandle(pElem);

            // check for "Formats"
            const char *szRoot = pElem->Value(); 
            const char *szRootName = "Formats";
            if(strcmp(szRootName, szRoot) != 0)
            {
                MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
                return;
            }

            // load all elemetns
            pElem = hRoot.FirstChild("Format").Element();
            for(pElem; pElem; pElem = pElem->NextSiblingElement())
            {
                int nFormat = -1;

                const char *pszName = pElem->Attribute("name");
                if(pszName != NULL)
                {
                    CString szBuff = GetConfigString(pszName);

                    nFormat = ::GetFormatId(szBuff);

                    // check if this is valid format name
                    if((nFormat < 0) || (nFormat >= NUM_FORMAT_NAMES))
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

                const char *pszTemplate = pElem->Attribute("template");
                if(pszTemplate != NULL)
                {
                    szFormatTemplate[nFormat] = GetConfigString(pszTemplate);
                }

                const char *pszPipesInput = pElem->Attribute("input");
                if(pszPipesInput != NULL)
                {
                    CString szBuff = GetConfigString(pszPipesInput);
                    if(szBuff.CompareNoCase(_T("true")) == 0)
                        bFormatInput[nFormat] = true;
                    else
                        bFormatInput[nFormat] = false;
                }

                const char *pszPipesOutput = pElem->Attribute("output");
                if(pszPipesOutput != NULL)
                {
                    CString szBuff = GetConfigString(pszPipesOutput);
                    if(szBuff.CompareNoCase(_T("true")) == 0)
                        bFormatOutput[nFormat] = true;
                    else
                        bFormatOutput[nFormat] = false;
                }

                const char *pszFunction = pElem->Attribute("function");
                if(pszFunction != NULL)
                {
                    szFormatFunction[nFormat] = GetConfigString(pszFunction);
                }

                const char *tmpBuff = pElem->GetText();
                szFormatPath[nFormat] = GetConfigString(tmpBuff);

                m_LstFormats.SetItemText(nFormat, 1, szFormatTemplate[nFormat]);
                m_LstFormats.SetItemText(nFormat, 2, szFormatPath[nFormat]);
                m_LstFormats.SetItemText(nFormat, 3, (bFormatInput[nFormat]) ? _T("true") : _T("false"));
                m_LstFormats.SetItemText(nFormat, 4, (bFormatOutput[nFormat]) ? _T("true") : _T("false"));
                m_LstFormats.SetItemText(nFormat, 5, szFormatFunction[nFormat]);
            }

            this->UpdateEditableFields();
        }
        else
        {
            MessageBox(_T("Failed to load file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
        }
    }
}

void CFormatsDlg::OnBnClickedButtonSaveConfig()
{
    // save config to .formats file
    CFileDialog fd(FALSE, _T("formats"), _T(""), 
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT, 
        _T("Formats Files (*.formats)|*.formats|Xml Files (*.xml)|*.xml|All Files|*.*||"), this);

    ::SetBrowsePath(fd, szBrowsePath[1]);

    if(fd.DoModal() == IDOK)
    {
        szBrowsePath[1] = ::GetBrowsePath(fd);

        CString szFileXml = fd.GetPathName();

        CTiXmlDocumentW doc;

        TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", ""); 
        doc.LinkEndChild(decl);  

        // root: Formats
        TiXmlElement *formats = new TiXmlElement("Formats");  
        doc.LinkEndChild(formats);  
        TiXmlElement *format;  

        for(int i = 0; i < NUM_FORMAT_NAMES; i++)
        {
            CUtf8String m_Utf8;

            szFormatTemplate[i] = m_LstFormats.GetItemText(i, 1);
            szFormatPath[i] = m_LstFormats.GetItemText(i, 2);
            bFormatInput[i] = (m_LstFormats.GetItemText(i, 3).Compare(_T("true")) == 0) ? true : false;
            bFormatOutput[i] = (m_LstFormats.GetItemText(i, 4).Compare(_T("true")) == 0) ? true : false;
            szFormatFunction[i] = m_LstFormats.GetItemText(i, 5);

            format = new TiXmlElement("Format");

            format->LinkEndChild(new TiXmlText(m_Utf8.Create(szFormatPath[i])));
            m_Utf8.Clear();

            format->SetAttribute("name", m_Utf8.Create(g_szFormatNames[i]));
            m_Utf8.Clear();

            format->SetAttribute("template", m_Utf8.Create(szFormatTemplate[i]));
            m_Utf8.Clear();

            format->SetAttribute("input", (bFormatInput[i]) ? "true" : "false");
            format->SetAttribute("output", (bFormatOutput[i]) ? "true" : "false");

            format->SetAttribute("function", m_Utf8.Create(szFormatFunction[i]));
            m_Utf8.Clear();

            formats->LinkEndChild(format); 
        }

        // save file
        if(doc.SaveFileW(szFileXml) != true)
            MessageBox(_T("Failed to save file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
    }
}

void CFormatsDlg::OnBnClickedButtonDefaultConfig()
{
    // load defaults
    for(int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        szFormatTemplate[i] = g_szDefaultTemplate[i];
        szFormatPath[i] = g_szDefaultPath[i];
        bFormatInput[i] = g_bDefaultInPipes[i];
        bFormatOutput[i] = g_bDefaultOutPipes[i];
        szFormatFunction[i] = g_bDefaultFunction[i];

        m_LstFormats.SetItemText(i, 1, g_szDefaultTemplate[i]);
        m_LstFormats.SetItemText(i, 2, g_szDefaultPath[i]);
        m_LstFormats.SetItemText(i, 3, (bFormatInput[i]) ? _T("true") : _T("false"));
        m_LstFormats.SetItemText(i, 4, (bFormatOutput[i]) ? _T("true") : _T("false"));
        m_LstFormats.SetItemText(i, 5, g_bDefaultFunction[i]);
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
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if(pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

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
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if(pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

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
    
    if(IsDlgButtonChecked(IDC_CHECK_FD_PIPES_INPUT) == BST_CHECKED)
        szCheckIn = _T("true");

    if(IsDlgButtonChecked(IDC_CHECK_FD_PIPES_OUTPUT) == BST_CHECKED)
        szCheckOut = _T("true");

    this->m_EdtPath.GetWindowText(szPath);

    this->m_EdtProgress.GetWindowText(szFunction);

    if(szPath.GetLength() > 0)
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if(pos != NULL)
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
    if(pos != NULL)
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
    if(pos != NULL)
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
    if(pos != NULL)
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
    if(pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CString szCheckIn = _T("false");
        if(IsDlgButtonChecked(IDC_CHECK_FD_PIPES_INPUT) == BST_CHECKED)
            szCheckIn = _T("true");

        m_LstFormats.SetItemText(nItem, 3, szCheckIn);
    }
}

void CFormatsDlg::OnBnClickedCheckFdPipesOutput()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if(pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CString szCheckOut = _T("false");
        if(IsDlgButtonChecked(IDC_CHECK_FD_PIPES_OUTPUT) == BST_CHECKED)
            szCheckOut = _T("true");

        m_LstFormats.SetItemText(nItem, 4, szCheckOut);
    }
}

void CFormatsDlg::OnClose()
{
    this->SaveWindowSettings();

    CResizeDialog::OnClose();
}
