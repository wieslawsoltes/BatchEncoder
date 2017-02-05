// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\Strings.h"
#include "..\configuration\LanguageHelper.h"
#include "..\utilities\Utilities.h"
#include "..\utilities\UnicodeUtf8.h"
#include "..\utilities\Utf8String.h"
#include "..\XmlConfiguration.h"
#include "FormatsDlg.h"

IMPLEMENT_DYNAMIC(CFormatsDlg, CDialog)
CFormatsDlg::CFormatsDlg(CWnd* pParent /*=NULL*/)
    : CResizeDialog(CFormatsDlg::IDD, pParent)
{
    this->m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAYICON);
    this->szFormatsDialogResize = _T("");
    this->szFormatsListColumns = _T("");
    this->bUpdate = false;
    this->nSelectedFormat = 0;
}

CFormatsDlg::~CFormatsDlg()
{

}

void CFormatsDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_GROUP_FORMAT_PIPES, m_GrpPipes);
    DDX_Control(pDX, IDC_STATIC_GROUP_FORMAT_TYPE, m_GrpTypes);
    DDX_Control(pDX, IDC_STATIC_FORMAT_ID, m_StcId);
    DDX_Control(pDX, IDC_STATIC_FORMAT_NAME, m_StcName);
    DDX_Control(pDX, IDC_STATIC_FORMAT_EXTENSION, m_StcExtension);
    DDX_Control(pDX, IDC_STATIC_FORMAT_FORMATS, m_StcFormats);
    DDX_Control(pDX, IDC_STATIC_FORMAT_DEFAULT, m_StcDefault);
    DDX_Control(pDX, IDC_STATIC_FORMAT_PATH, m_StcPath);
    DDX_Control(pDX, IDC_STATIC_FORMAT_TEMPLATE, m_StcTemplate);
    DDX_Control(pDX, IDC_STATIC_FORMAT_FUNCTION, m_StcProgress);
    DDX_Control(pDX, IDC_LIST_FORMATS, m_LstFormats);
    DDX_Control(pDX, IDC_EDIT_FORMAT_ID, m_EdtId);
    DDX_Control(pDX, IDC_EDIT_FORMAT_NAME, m_EdtName);
    DDX_Control(pDX, IDC_EDIT_FORMAT_EXTENSION, m_EdtExtension);
    DDX_Control(pDX, IDC_EDIT_FORMAT_FORMATS, m_EdtFormats);
    DDX_Control(pDX, IDC_EDIT_FORMAT_DEFAULT, m_EdtDefault);
    DDX_Control(pDX, IDC_EDIT_FORMAT_PATH, m_EdtPath);
    DDX_Control(pDX, IDC_EDIT_FORMAT_TEMPLATE, m_EdtTemplate);
    DDX_Control(pDX, IDC_EDIT_FORMAT_FUNCTION, m_EdtFunction);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_REMOVE_ALL, m_BtnRemoveAll);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_REMOVE, m_BtnRemove);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_ADD, m_BtnAdd);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_UP, m_BtnMoveUp);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_DOWN, m_BtnMoveDown);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_UPDATE, m_BtnUpdate);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_LOAD, m_BtnLoad);
    DDX_Control(pDX, IDC_BUTTON_FORMAT_SAVE, m_BtnSave);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_PATH, m_BtnBrowsePath);
    DDX_Control(pDX, IDC_BUTTON_BROWSE_FUNCTION, m_BtnBrowseFunction);
}

BEGIN_MESSAGE_MAP(CFormatsDlg, CResizeDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FORMATS, OnLvnItemchangedListFormats)
    ON_BN_CLICKED(IDC_RADIO_TYPE_ENCODER, OnBnClickedRadioTypeEncoder)
    ON_BN_CLICKED(IDC_RADIO_TYPE_DECODER, OnBnClickedRadioTypeDecoder)
    ON_BN_CLICKED(IDC_CHECK_FORMAT_PIPES_INPUT, OnBnClickedCheckPipesInput)
    ON_BN_CLICKED(IDC_CHECK_FORMAT_PIPES_OUTPUT, OnBnClickedCheckPipesOutput)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_ID, OnEnChangeEditFormatId)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_NAME, OnEnChangeEditFormatName)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_EXTENSION, OnEnChangeEditFormatExtension)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_FORMATS, OnEnChangeEditFormatFormats)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_DEFAULT, OnEnChangeEditFormatDefault)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_PATH, OnEnChangeEditFormatPath)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_TEMPLATE, OnEnChangeEditFormatTemplate)
    ON_EN_CHANGE(IDC_EDIT_FORMAT_FUNCTION, OnEnChangeEditFormatFunction)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_REMOVE_ALL, OnBnClickedButtonRemoveAllFormats)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_REMOVE, OnBnClickedButtonRemoveFormat)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_ADD, OnBnClickedButtonAddFormat)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_UP, OnBnClickedButtonFormatUp)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_DOWN, OnBnClickedButtonFormatDown)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_UPDATE, OnBnClickedButtonUpdateFormat)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_LOAD, OnBnClickedButtonLoadFormats)
    ON_BN_CLICKED(IDC_BUTTON_FORMAT_SAVE, OnBnClickedButtonSaveFormats)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_PATH, OnBnClickedButtonBrowsePath)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_FUNCTION, OnBnClickedButtonBrowseProgress)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CFormatsDlg::OnInitDialog()
{
    CResizeDialog::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // update list style
    DWORD dwExStyle = m_LstFormats.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
    m_LstFormats.SetExtendedStyle(dwExStyle);

    // insert all ListCtrl columns
    m_LstFormats.InsertColumn(FORMAT_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 195);
    m_LstFormats.InsertColumn(FORMAT_COLUMN_TEMPLATE, _T("Template"), LVCFMT_LEFT, 295);

    // insert all ListCtrl items and sub items
    this->InsertFormatsToListCtrl();

    m_LstFormats.SetItemState(nSelectedFormat, LVIS_SELECTED, LVIS_SELECTED);
    m_LstFormats.EnsureVisible(nSelectedFormat, FALSE);

    // setup resize anchors
    AddAnchor(IDC_LIST_FORMATS, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_GROUP_FORMAT_TYPE, BOTTOM_LEFT);
    AddAnchor(IDC_RADIO_TYPE_ENCODER, BOTTOM_LEFT);
    AddAnchor(IDC_RADIO_TYPE_DECODER, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_FORMAT_ID, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_FORMAT_ID, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_FORMAT_EXTENSION, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_FORMAT_EXTENSION, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_FORMAT_NAME, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FORMAT_NAME, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_FORMAT_FORMATS, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FORMAT_FORMATS, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_FORMAT_DEFAULT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FORMAT_DEFAULT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_FORMAT_PATH, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT_FORMAT_PATH, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_BROWSE_PATH, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_GROUP_FORMAT_PIPES, BOTTOM_LEFT);
    AddAnchor(IDC_CHECK_FORMAT_PIPES_INPUT, BOTTOM_LEFT);
    AddAnchor(IDC_CHECK_FORMAT_PIPES_OUTPUT, BOTTOM_LEFT);
    AddAnchor(IDC_STATIC_FORMAT_TEMPLATE, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FORMAT_TEMPLATE, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_STATIC_FORMAT_FUNCTION, BOTTOM_RIGHT);
    AddAnchor(IDC_EDIT_FORMAT_FUNCTION, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_BROWSE_FUNCTION, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_FORMAT_UP, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_FORMAT_DOWN, TOP_RIGHT);
    AddAnchor(IDC_BUTTON_FORMAT_ADD, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_FORMAT_REMOVE_ALL, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_FORMAT_REMOVE, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_FORMAT_LOAD, BOTTOM_LEFT);
    AddAnchor(IDC_BUTTON_FORMAT_SAVE, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT);

    this->LoadWindowSettings();
    this->SetLanguage();

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

void CFormatsDlg::OnBnClickedOk()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
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

void CFormatsDlg::OnBnClickedButtonRemoveAllFormats()
{
    if (m_Formats.GetSize() > 0)
    {
        CFormat& format = m_Formats.GetData(nSelectedFormat);
        m_Formats.RemoveAllNodes();

        m_LstFormats.DeleteAllItems();

        this->ListSelectionChange();
    }
}

void CFormatsDlg::OnBnClickedButtonRemoveFormat()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CFormat& format = m_Formats.GetData(nItem);
        m_Formats.RemoveNode(nItem);

        m_LstFormats.DeleteItem(nItem);

        int nItems = m_LstFormats.GetItemCount();
        if (nItem < nItems && nItems >= 0)
            m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        else if (nItem >= nItems && nItems >= 0)
            m_LstFormats.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);

        this->ListSelectionChange();
    }
}

void CFormatsDlg::OnBnClickedButtonAddFormat()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    int nItem = m_LstFormats.GetItemCount();

    CFormat format;
    format.szId = _T("ID");
    format.szName = pConfig->GetString(0x00230004, pszFormatsDialog[3]);
    format.szTemplate = _T("$EXE $OPTIONS $INFILE $OUTFILE");
    format.bPipeInput = true;
    format.bPipeOutput = false;
    format.szFunction = _T("- none -");
    format.szPath = _T("program.exe");
    format.nType = 0;
    format.szInputExtensions = _T("WAV");
    format.szOutputExtension = _T("EXT");
    format.nDefaultPreset = 0;

    CPreset preset;
    preset.szName = pConfig->GetString(0x00230005, pszFormatsDialog[4]);
    preset.szOptions = _T("");
    format.m_Presets.InsertNode(preset);

    m_Formats.InsertNode(format);

    AddToList(format, nItem);

    m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    m_LstFormats.EnsureVisible(nItem, FALSE);

    bUpdate = false;

    this->ListSelectionChange();
}

void CFormatsDlg::OnBnClickedButtonFormatUp()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        if (nItem > 0)
        {
            CFormat& format1 = m_Formats.GetData(nItem);
            CFormat& format2 = m_Formats.GetData(nItem - 1);

            m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_NAME, format2.szName);
            m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_TEMPLATE, format2.szTemplate);
            m_LstFormats.SetItemText(nItem - 1, FORMAT_COLUMN_NAME, format1.szName);
            m_LstFormats.SetItemText(nItem - 1, FORMAT_COLUMN_TEMPLATE, format1.szTemplate);

            m_Formats.SwapItems(nItem, nItem - 1);

            m_LstFormats.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstFormats.EnsureVisible(nItem - 1, FALSE);
        }
    }

    bUpdate = false;
}

void CFormatsDlg::OnBnClickedButtonFormatDown()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);
        int nItems = m_LstFormats.GetItemCount();
        if (nItem != (nItems - 1) && nItem >= 0)
        {
            CFormat& format1 = m_Formats.GetData(nItem);
            CFormat& format2 = m_Formats.GetData(nItem + 1);

            m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_NAME, format2.szName);
            m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_TEMPLATE, format2.szTemplate);
            m_LstFormats.SetItemText(nItem + 1, FORMAT_COLUMN_NAME, format1.szName);
            m_LstFormats.SetItemText(nItem + 1, FORMAT_COLUMN_TEMPLATE, format1.szTemplate);

            m_Formats.SwapItems(nItem, nItem + 1);

            m_LstFormats.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstFormats.EnsureVisible(nItem + 1, FALSE);
        }
    }

    bUpdate = false;
}

void CFormatsDlg::OnBnClickedButtonUpdateFormat()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CString szId = _T("");
        CString szName = _T("");
        CString szExtension = _T("");
        CString szFormats = _T("");
        CString szDefault = _T("");
        CString szTemplate = _T("");
        CString szPath = _T("");
        int nType = 0;
        bool bInput = false;
        bool bOutput = false;
        CString szFunction = _T("");

        m_EdtId.GetWindowText(szId);
        m_EdtName.GetWindowText(szName);
        m_EdtExtension.GetWindowText(szExtension);
        m_EdtFormats.GetWindowText(szFormats);
        m_EdtDefault.GetWindowText(szDefault);
        this->m_EdtName.GetWindowText(szName);
        this->m_EdtTemplate.GetWindowText(szTemplate);

        int nCheckID = this->GetCheckedRadioButton(IDC_RADIO_TYPE_ENCODER, IDC_RADIO_TYPE_DECODER);
        if (nCheckID == IDC_RADIO_TYPE_ENCODER)
            nType = 0;
        else if (nCheckID == IDC_RADIO_TYPE_DECODER)
            nType = 1;
        else
            nType = 0;

        if (IsDlgButtonChecked(IDC_CHECK_FORMAT_PIPES_INPUT) == BST_CHECKED)
            bInput = true;

        if (IsDlgButtonChecked(IDC_CHECK_FORMAT_PIPES_OUTPUT) == BST_CHECKED)
            bOutput = true;

        this->m_EdtPath.GetWindowText(szPath);
        this->m_EdtFunction.GetWindowText(szFunction);

        CFormat& format = m_Formats.GetData(nItem);
        format.szId = szId;
        format.szName = szName;
        format.szOutputExtension = szExtension;
        format.szInputExtensions = szFormats;
        format.nDefaultPreset = _tstoi(szDefault);
        format.szTemplate = szTemplate;
        format.nType = nType;
        format.bPipeInput = bInput;
        format.bPipeOutput = bOutput;
        format.szPath = szPath;
        format.szFunction = szFunction;

        m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_NAME, szName);
        m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_TEMPLATE, szTemplate);

        m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
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

void CFormatsDlg::OnEnChangeEditFormatDefault()
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

void CFormatsDlg::OnBnClickedButtonLoadFormats()
{
    CString szFilter;
    szFilter.Format(_T("%s (*.formats)|*.formats|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310005, pszFileDialogs[4]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(TRUE, _T("formats"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadFormats(szFileXml);
    }
}

void CFormatsDlg::OnBnClickedButtonSaveFormats()
{
    CString szFilter;
    szFilter.Format(_T("%s (*.formats)|*.formats|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310005, pszFileDialogs[4]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(FALSE, _T("formats"), _T("formats"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SaveFormats(szFileXml);
    }
}

void CFormatsDlg::OnBnClickedButtonBrowsePath()
{
    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
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
    if (pos != NULL)
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

    CResizeDialog::OnClose();
}

void CFormatsDlg::LoadWindowSettings()
{
    // set window rectangle and position
    if (szFormatsDialogResize.CompareNoCase(_T("")) != 0)
        this->SetWindowRectStr(szFormatsDialogResize);

    // load columns width for FormatsList
    if (szFormatsListColumns.CompareNoCase(_T("")) != 0)
    {
        int nColWidth[2];
        if (_stscanf(szFormatsListColumns, _T("%d %d"),
            &nColWidth[0],
            &nColWidth[1]) == 2)
        {
            for (int i = 0; i < 2; i++)
                m_LstFormats.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CFormatsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szFormatsDialogResize = this->GetWindowRectStr();

    // save columns width from FormatsList
    int nColWidth[11];
    for (int i = 0; i < 2; i++)
        nColWidth[i] = m_LstFormats.GetColumnWidth(i);
    szFormatsListColumns.Format(_T("%d %d"),
        nColWidth[0],
        nColWidth[1]);
}

void CFormatsDlg::SetLanguage()
{
    CLanguageHelper helper(pConfig);

    helper.SetColumnText(m_LstFormats, FORMAT_COLUMN_NAME, 0x000C0001);
    helper.SetColumnText(m_LstFormats, FORMAT_COLUMN_TEMPLATE, 0x000C0002);

    helper.SetWndText(this, 0x000C0010);
    helper.SetWndText(&m_BtnCancel, 0x000C0011);
    helper.SetItemText(this, IDC_RADIO_TYPE_ENCODER, 0x000C0012);
    helper.SetItemText(this, IDC_RADIO_TYPE_ENCODER, 0x000C0013);
    helper.SetWndText(&m_GrpTypes, 0x000C0014);
    helper.SetItemText(this, IDC_CHECK_FORMAT_PIPES_INPUT, 0x000C0015);
    helper.SetItemText(this, IDC_CHECK_FORMAT_PIPES_OUTPUT, 0x000C0016);
    helper.SetWndText(&m_GrpPipes, 0x000C0017);
    helper.SetWndText(&m_StcId, 0x000C0018);
    helper.SetWndText(&m_StcName, 0x000C0019);
    helper.SetWndText(&m_StcExtension, 0x000C001A);
    helper.SetWndText(&m_StcFormats, 0x000C001B);
    helper.SetWndText(&m_StcDefault, 0x000C001C);
    helper.SetWndText(&m_StcPath, 0x000C001D);
    helper.SetWndText(&m_BtnBrowsePath, 0x000C001E);
    helper.SetWndText(&m_StcTemplate, 0x000C001F);
    helper.SetWndText(&m_StcProgress, 0x000C0020);
    helper.SetWndText(&m_BtnBrowseFunction, 0x000C0021);
    helper.SetWndText(&m_BtnMoveUp, 0x000C0022);
    helper.SetWndText(&m_BtnMoveDown, 0x000C0023);
    helper.SetWndText(&m_BtnRemoveAll, 0x000C0024);
    helper.SetWndText(&m_BtnRemove, 0x000C0025);
    helper.SetWndText(&m_BtnAdd, 0x000C0026);
    helper.SetWndText(&m_BtnLoad, 0x000C0027);
    helper.SetWndText(&m_BtnSave, 0x000C0028);
    helper.SetWndText(&m_BtnUpdate, 0x000C0029);
    helper.SetWndText(&m_BtnOK, 0x000C002A);
}

void CFormatsDlg::AddToList(CFormat &format, int nItem)
{
    LVITEM lvi;

    ZeroMemory(&lvi, sizeof(LVITEM));

    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iItem = nItem;

    lvi.iSubItem = FORMAT_COLUMN_NAME;
    lvi.pszText = (LPTSTR)(LPCTSTR)(format.szName);
    m_LstFormats.InsertItem(&lvi);

    lvi.iSubItem = FORMAT_COLUMN_TEMPLATE;
    lvi.pszText = (LPTSTR)(LPCTSTR)(format.szTemplate);
    m_LstFormats.SetItemText(lvi.iItem, FORMAT_COLUMN_TEMPLATE, lvi.pszText);
}

void CFormatsDlg::InsertFormatsToListCtrl()
{
    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);
        this->AddToList(format, i);
    }
}

void CFormatsDlg::UpdateFields(CFormat &format)
{
    this->m_EdtId.SetWindowText(format.szId);
    this->m_EdtName.SetWindowText(format.szName);
    this->m_EdtExtension.SetWindowText(format.szOutputExtension);
    this->m_EdtFormats.SetWindowText(format.szInputExtensions);

    CString szDefaultPreset;
    szDefaultPreset.Format(_T("%d\0"), format.nDefaultPreset);
    this->m_EdtDefault.SetWindowText(szDefaultPreset);

    this->m_EdtTemplate.SetWindowText(format.szTemplate);
    this->m_EdtPath.SetWindowText(format.szPath);

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

    this->m_EdtFunction.SetWindowText(format.szFunction);
}

void CFormatsDlg::ListSelectionChange()
{
    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstFormats.GetNextSelectedItem(pos);

        CFormat& format = this->m_Formats.GetData(nItem);

        this->UpdateFields(format);
    }
    else
    {
        this->m_EdtId.SetWindowText(_T(""));
        this->m_EdtName.SetWindowText(_T(""));
        this->m_EdtExtension.SetWindowText(_T(""));
        this->m_EdtFormats.SetWindowText(_T(""));
        this->m_EdtDefault.SetWindowText(_T(""));
        this->m_EdtTemplate.SetWindowText(_T(""));
        this->m_EdtPath.SetWindowText(_T(""));
        this->CheckRadioButton(IDC_RADIO_TYPE_ENCODER,
            IDC_RADIO_TYPE_DECODER,
            IDC_RADIO_TYPE_ENCODER);
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_INPUT, BST_UNCHECKED);
        CheckDlgButton(IDC_CHECK_FORMAT_PIPES_OUTPUT, BST_UNCHECKED);
        this->m_EdtFunction.SetWindowText(_T(""));
    }

    bUpdate = false;
}

void CFormatsDlg::LoadFormats(CString szFileXml)
{
    XmlConfiguration doc;
    if (doc.Open(szFileXml) == true)
    {
        this->m_Formats.RemoveAllNodes();
        this->m_LstFormats.DeleteAllItems();

        doc.GetFormats(this->m_Formats);

        if (this->m_Formats.GetSize() > 0)
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

void CFormatsDlg::SaveFormats(CString szFileXml)
{
    XmlConfiguration doc;
    doc.SetFormats(this->m_Formats);
    if (doc.Save(szFileXml) != true)
    {
        MessageBox(
            pConfig->GetString(0x00230003, pszFormatsDialog[2]),
            pConfig->GetString(0x00230001, pszFormatsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

bool CFormatsDlg::BrowseForPath(CString szDefaultFName, CEdit *pEdit, int nID)
{
    CString szFilter;
    szFilter.Format(_T("%s (*.exe)|*.exe|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310006, pszFileDialogs[5]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

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
    szFilter.Format(_T("%s (*.progress)|*.progress|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310007, pszFileDialogs[6]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(TRUE, _T("progress"), szDefaultFName,
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
