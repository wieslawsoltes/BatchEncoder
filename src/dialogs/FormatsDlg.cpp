// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "utilities\LanguageHelper.h"
#include "utilities\Utilities.h"
#include "utilities\Utf8String.h"
#include "xml\XmlConfig.h"
#include "FormatsDlg.h"
#include "PresetsDlg.h"

namespace app
{
    DWORD WINAPI FormatsDlgDropThread(LPVOID lpParam)
    {
        FormatsDlgDropContext* pDD = (FormatsDlgDropContext*)lpParam;
        pDD->pDlg->HandleDropFiles(pDD->hDrop);
        pDD->bHandled = true;
        return ::CloseHandle(pDD->hThread);
    }

    IMPLEMENT_DYNAMIC(CFormatsDlg, CDialog)
    CFormatsDlg::CFormatsDlg(CWnd* pParent /*=nullptr*/)
        : CMyDialogEx(CFormatsDlg::IDD, pParent)
    {
        this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
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
        CMyDialogEx::DoDataExchange(pDX);
        DDX_Control(pDX, IDC_STATIC_GROUP_FORMAT_PIPES, m_GrpPipes);
        DDX_Control(pDX, IDC_STATIC_GROUP_FORMAT_TYPE, m_GrpTypes);
        DDX_Control(pDX, IDC_STATIC_FORMAT_ID, m_StcId);
        DDX_Control(pDX, IDC_STATIC_FORMAT_NAME, m_StcName);
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
        DDX_Control(pDX, IDC_BUTTON_FORMAT_UP, m_BtnMoveUp);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_DOWN, m_BtnMoveDown);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_UPDATE, m_BtnUpdate);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_LOAD, m_BtnLoad);
        DDX_Control(pDX, IDC_BUTTON_FORMAT_SAVE, m_BtnSave);
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
        ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FORMATS, OnLvnItemchangedListFormats)
        ON_BN_CLICKED(IDC_RADIO_TYPE_ENCODER, OnBnClickedRadioTypeEncoder)
        ON_BN_CLICKED(IDC_RADIO_TYPE_DECODER, OnBnClickedRadioTypeDecoder)
        ON_BN_CLICKED(IDC_CHECK_FORMAT_PIPES_INPUT, OnBnClickedCheckPipesInput)
        ON_BN_CLICKED(IDC_CHECK_FORMAT_PIPES_OUTPUT, OnBnClickedCheckPipesOutput)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_ID, OnEnChangeEditFormatId)
        ON_EN_CHANGE(IDC_EDIT_FORMAT_NAME, OnEnChangeEditFormatName)
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
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_UP, OnBnClickedButtonFormatUp)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_DOWN, OnBnClickedButtonFormatDown)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_UPDATE, OnBnClickedButtonUpdateFormat)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_LOAD, OnBnClickedButtonLoadFormats)
        ON_BN_CLICKED(IDC_BUTTON_FORMAT_SAVE, OnBnClickedButtonSaveFormats)
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

        // enable drag & drop
        this->DragAcceptFiles(TRUE);

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
            CMyDialogEx::OnPaint();
        }
    }

    HCURSOR CFormatsDlg::OnQueryDragIcon()
    {
        return static_cast<HCURSOR>(m_hIcon);
    }

    void CFormatsDlg::OnDropFiles(HDROP hDropInfo)
    {
        if (this->m_DD.bHandled == true)
        {
            this->m_DD.bHandled = false;
            this->m_DD.pDlg = this;
            this->m_DD.hDrop = hDropInfo;
            this->m_DD.hThread = ::CreateThread(nullptr, 0, FormatsDlgDropThread, (LPVOID)&this->m_DD, 0, &this->m_DD.dwThreadID);
            if (this->m_DD.hThread == nullptr)
                this->m_DD.bHandled = true;
        }
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

    void CFormatsDlg::OnBnClickedButtonImport()
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310008).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("xml"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->LoadFormat(szFileXml);
        }
    }

    void CFormatsDlg::OnBnClickedButtonExport()
    {
        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
        if (pos != nullptr)
        {
            int nSelected = m_LstFormats.GetNextSelectedItem(pos);
            if (nSelected >= 0)
            {
                config::CFormat& format = m_Formats.Get(nSelected);

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
                config::CFormat& format = m_Formats.Get(nSelected);
                config::CFormat copy = format;

                m_Formats.Insert(copy);

                int nItem = m_LstFormats.GetItemCount();
                AddToList(copy, nItem);

                m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
                m_LstFormats.EnsureVisible(nItem, FALSE);
            }
        }

        bUpdate = false;

        this->ListSelectionChange();
    }

    void CFormatsDlg::OnBnClickedButtonRemoveAllFormats()
    {
        if (m_Formats.Count() > 0)
        {
            m_Formats.RemoveAll();
            m_LstFormats.DeleteAllItems();
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
            return;

        for (int i = (nItems - 1); i >= 0; i--)
        {
            if (m_LstFormats.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                m_Formats.Remove(i);
                m_LstFormats.DeleteItem(i);
                nItemLastRemoved = i;
            }
        }

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

        int nItem = m_LstFormats.GetItemCount();

        config::CFormat format;
        format.szId = _T("ID");
        format.szName = pConfig->GetString(0x00230004);
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

        config::CPreset preset;
        preset.szName = pConfig->GetString(0x00230005);
        preset.szOptions = _T("");
        format.m_Presets.Insert(preset);

        m_Formats.Insert(format);

        AddToList(format, nItem);

        m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
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
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);
            if (nItem > 0)
            {
                config::CFormat& format1 = m_Formats.Get(nItem);
                config::CFormat& format2 = m_Formats.Get(nItem - 1);

                m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_NAME, format2.szName.c_str());
                m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_TEMPLATE, format2.szTemplate.c_str());
                m_LstFormats.SetItemText(nItem - 1, FORMAT_COLUMN_NAME, format1.szName.c_str());
                m_LstFormats.SetItemText(nItem - 1, FORMAT_COLUMN_TEMPLATE, format1.szTemplate.c_str());

                m_Formats.Swap(nItem, nItem - 1);

                m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
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
        if (pos != nullptr)
        {
            int nItem = m_LstFormats.GetNextSelectedItem(pos);
            int nItems = m_LstFormats.GetItemCount();
            if (nItem != (nItems - 1) && nItem >= 0)
            {
                config::CFormat& format1 = m_Formats.Get(nItem);
                config::CFormat& format2 = m_Formats.Get(nItem + 1);

                m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_NAME, format2.szName.c_str());
                m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_TEMPLATE, format2.szTemplate.c_str());
                m_LstFormats.SetItemText(nItem + 1, FORMAT_COLUMN_NAME, format1.szName.c_str());
                m_LstFormats.SetItemText(nItem + 1, FORMAT_COLUMN_TEMPLATE, format1.szTemplate.c_str());

                m_Formats.Swap(nItem, nItem + 1);

                m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
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
            int nType = 0;
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

            config::CFormat& format = m_Formats.Get(nItem);
            format.szId = szId;
            format.szName = szName;
            format.szOutputExtension = szExtension;
            format.szInputExtensions = szFormats;
            format.nDefaultPreset = nDefaultPreset;
            format.szTemplate = szTemplate;
            format.nExitCodeSuccess = _tstoi(szExitCodeSuccess);
            format.nType = nType;
            format.bPipeInput = bInput;
            format.bPipeOutput = bOutput;
            format.szPath = szPath;
            format.szFunction = szFunction;

            m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_NAME, szName);
            m_LstFormats.SetItemText(nItem, FORMAT_COLUMN_TEMPLATE, szTemplate);

            m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
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

    void CFormatsDlg::OnBnClickedButtonLoadFormats()
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310005).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(TRUE, _T("xml"), _T(""),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->LoadFormats(szFileXml, true);
        }
    }

    void CFormatsDlg::OnBnClickedButtonSaveFormats()
    {
        CString szFilter;
        szFilter.Format(_T("%s (*.xml)|*.xml|%s (*.*)|*.*||"),
            pConfig->GetString(0x00310005).c_str(),
            pConfig->GetString(0x00310001).c_str());

        CFileDialog fd(FALSE, _T("xml"), _T("Formats"),
            OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
            szFilter, this);

        if (fd.DoModal() == IDOK)
        {
            std::wstring szFileXml = fd.GetPathName();
            this->SaveFormats(szFileXml, true);
        }
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
                this->m_Formats.RemoveAll();
                this->m_Formats = dlg.m_Formats;

                this->m_LstFormats.DeleteAllItems();

                this->InsertFormatsToListCtrl();
                m_LstFormats.SetItemState(-1, 0, LVIS_SELECTED);
                m_LstFormats.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
                m_LstFormats.EnsureVisible(nItem, FALSE);

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
            auto widths = util::StringHelper::Split(szFormatsListColumns.c_str(), ' ');
            if (widths.size() == 2)
            {
                for (int i = 0; i < 2; i++)
                {
                    std::wstring szWidth = widths[i];
                    int nWidth = util::StringHelper::ToInt(szWidth);
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
        lang::CLanguageHelper helper(&pConfig->m_Language);

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

    void CFormatsDlg::AddToList(config::CFormat &format, int nItem)
    {
        LVITEM lvi;

        ZeroMemory(&lvi, sizeof(LVITEM));

        lvi.mask = LVIF_TEXT | LVIF_STATE;
        lvi.state = 0;
        lvi.stateMask = 0;
        lvi.iItem = nItem;

        lvi.iSubItem = FORMAT_COLUMN_NAME;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.szName.c_str());
        m_LstFormats.InsertItem(&lvi);

        lvi.iSubItem = FORMAT_COLUMN_TEMPLATE;
        lvi.pszText = (LPTSTR)(LPCTSTR)(format.szTemplate.c_str());
        m_LstFormats.SetItemText(lvi.iItem, FORMAT_COLUMN_TEMPLATE, lvi.pszText);
    }

    void CFormatsDlg::InsertFormatsToListCtrl()
    {
        int nFormats = m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            config::CFormat& format = m_Formats.Get(i);
            this->AddToList(format, i);
        }
    }

    void CFormatsDlg::HandleDropFiles(HDROP hDropInfo)
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

                    if (util::StringHelper::CompareNoCase(szExt, L"xml"))
                    {
                        xml::XmlDocumnent doc;
                        std::string szName = xml::CXmlConfig::GetRootName(szPath, doc);
                        if (!szName.empty())
                        {
                            if (util::StringHelper::CompareNoCase(szName, "Formats"))
                            {
                                ::SetCurrentDirectory(util::Utilities::GetFilePath(szPath).c_str());
                                this->LoadFormats(doc, true);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Format"))
                            {
                                this->LoadFormat(doc);
                            }
                            else if (util::StringHelper::CompareNoCase(szName, "Presets"))
                            {
                                this->LoadPresets(doc);
                            }
                        }
                    }
                    else if (util::StringHelper::CompareNoCase(szExt, L"exe"))
                    {
                        // Set current format exe path.
                        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
                        if (pos != nullptr)
                        {
                            int nItem = m_LstFormats.GetNextSelectedItem(pos);
                            config::CFormat& format = this->m_Formats.Get(nItem);
                            format.szPath = szPath;
                            this->m_EdtPath.SetWindowText(format.szPath.c_str());
                        }
                    }
                    else if (util::StringHelper::CompareNoCase(szExt, L"lua"))
                    {
                        // Set current format progress path.
                        POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
                        if (pos != nullptr)
                        {
                            int nItem = m_LstFormats.GetNextSelectedItem(pos);
                            config::CFormat& format = this->m_Formats.Get(nItem);
                            format.szFunction = szPath;
                            this->m_EdtFunction.SetWindowText(format.szFunction.c_str());
                        }
                    }
                }

                szFile.ReleaseBuffer();
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

        CString szExitCodeSuccess;
        szExitCodeSuccess.Format(_T("%d\0"), format.nExitCodeSuccess);
        this->m_EdtCode.SetWindowText(szExitCodeSuccess);

        this->m_EdtTemplate.SetWindowText(format.szTemplate.c_str());
        this->m_EdtPath.SetWindowText(format.szPath.c_str());

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

        this->m_EdtFunction.SetWindowText(format.szFunction.c_str());
    }

    void CFormatsDlg::UpdateDefaultComboBox(config::CFormat &format)
    {
        this->m_CmbDefault.ResetContent();

        int nPreset = -1;

        int nPresets = format.m_Presets.Count();
        for (int i = 0; i < nPresets; i++)
        {
            config::CPreset& preset = format.m_Presets.Get(i);
            this->m_CmbDefault.InsertString(i, preset.szName.c_str());
        }

        nPreset = format.nDefaultPreset;

        static bool bResizeDefaultComboBox = false;
        if (bResizeDefaultComboBox == false)
        {
            util::Utilities::SetComboBoxHeight(this->GetSafeHwnd(), IDC_COMBO_FORMAT_DEFAULT, 15);
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

            config::CFormat& format = this->m_Formats.Get(nItem);

            this->UpdateFields(format);
            this->UpdateDefaultComboBox(format);
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

    bool CFormatsDlg::LoadFormat(const std::wstring& szFileXml)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Format"))
        {
            return this->LoadFormat(doc);
        }
        return false;
    }

    bool CFormatsDlg::LoadFormat(xml::XmlDocumnent &doc)
    {
        config::CFormat format;
        if (xml::CXmlConfig::LoadFormat(doc, format))
        {
            m_Formats.Insert(format);
            int nItem = m_Formats.Count() - 1;
            this->AddToList(format, nItem);
            return true;
        }
        return false;
    }

    bool CFormatsDlg::SaveFormat(const std::wstring& szFileXml, config::CFormat &format)
    {
        return xml::CXmlConfig::SaveFormat(szFileXml, format);
    }

    bool CFormatsDlg::LoadFormats(const std::wstring& szFileXml, bool bOnlyIds)
    {
        xml::XmlDocumnent doc;
        std::string szName = xml::CXmlConfig::GetRootName(szFileXml, doc);
        if (!szName.empty() && util::StringHelper::CompareNoCase(szName, "Formats"))
        {
            ::SetCurrentDirectory(util::Utilities::GetFilePath(szFileXml).c_str());
            return this->LoadFormats(doc, bOnlyIds);
        }
        return false;
    }

    bool CFormatsDlg::LoadFormats(xml::XmlDocumnent &doc, bool bOnlyIds)
    {
        config::CFormatsList formats;
        if (xml::CXmlConfig::LoadFormats(doc, formats, bOnlyIds))
        {
            this->m_LstFormats.DeleteAllItems();
            if (bOnlyIds == true)
            {
                this->m_Formats.RemoveAll();
                for (auto& format : formats.m_Items)
                {
                    std::wstring path = app::m_App.m_Settings.szFormatsDir + L"\\" + format.szId + L".xml";
                    if (this->LoadFormat(path) == false)
                        return false;
                }
                if (this->m_Formats.Count() > 0)
                    nSelectedFormat = 0;
            }
            else
            {
                this->m_Formats = std::move(formats);
                if (this->m_Formats.Count() > 0)
                    nSelectedFormat = 0;

                this->InsertFormatsToListCtrl();
            }
            this->ListSelectionChange();
            return true;
        }
        return false;
    }

    bool CFormatsDlg::SaveFormats(const std::wstring& szFileXml, bool bOnlyIds)
    {
        bool bResult = xml::CXmlConfig::SaveFormats(szFileXml, this->m_Formats, bOnlyIds);
        if (bResult == false)
            return false;

        if (bOnlyIds == true)
        {
            std::wstring szFilePath = util::Utilities::GetFilePath(szFileXml);
            std::wstring szPath = util::Utilities::CombinePath(szFilePath, app::m_App.m_Settings.szFormatsDir);
            ::SetCurrentDirectory(szFilePath.c_str());
            ::CreateDirectory(szPath.c_str(), NULL);
            for (auto& format : this->m_Formats.m_Items)
            {
                std::wstring path = util::Utilities::CombinePath(szPath, format.szId + L".xml");
                if (this->SaveFormat(path, format) == false)
                    return false;
            }
        }

        return true;
    }

    bool CFormatsDlg::LoadPresets(xml::XmlDocumnent &doc)
    {
        config::CPresetsList presets;
        if (xml::CXmlConfig::LoadPresets(doc, presets))
        {
            POSITION pos = m_LstFormats.GetFirstSelectedItemPosition();
            if (pos != nullptr)
            {
                int nItem = m_LstFormats.GetNextSelectedItem(pos);
                config::CFormat& format = this->m_Formats.Get(nItem);
                format.m_Presets = std::move(presets);
                this->UpdateDefaultComboBox(format);
                return true;
            }
        }
        return false;
    }
}
