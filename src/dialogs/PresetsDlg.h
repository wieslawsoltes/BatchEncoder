// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxcmn.h>
#include <afxwin.h>
#include "..\controls\Controls.h"
#include "..\Configuration.h"
#include "..\XmlConfiguration.h"

class CPresetsDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CPresetsDlg)
public:
    CPresetsDlg(CWnd* pParent = NULL);
    virtual ~CPresetsDlg();
    enum { IDD = IDD_DIALOG_PRESETS };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
protected:
    HICON m_hIcon;
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnPaint();
public:
    virtual BOOL OnInitDialog();
public:
    volatile bool bUpdate;
    int nSelectedFormat;
    CFormatsList m_Formats;
    CString szPresetsDialogResize;
    CString szPresetsListColumns;
    bool bShowGridLines;
public:
    CMyStatic m_StcName;
    CMyStatic m_StcOptions;
    CMyListCtrl m_LstPresets;
    CMyComboBox m_CmbFormat;
    CEdit m_EdtName;
    CEdit m_EdtOptions;
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnMoveUp;
    CMyButton m_BtnMoveDown;
    CMyButton m_BtnRemoveAll;
    CMyButton m_BtnRemove;
    CMyButton m_BtnUpdate;
    CMyButton m_BtnAdd;
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
    void ShowGridlines(bool bShow);
    void AddToList(CPreset &preset, int nItem);
    void InsertPresetsToListCtrl();
    void ListSelectionChange();
    void LoadPresets(CString szFileXml);
    void SavePresets(CString szFileXml);
public:
    afx_msg void OnLvnItemchangedListPdPresets(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeComboPdFormat();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonPdRemoveAllPresets();
    afx_msg void OnBnClickedButtonPdRemovePresets();
    afx_msg void OnBnClickedButtonPdLoadPresets();
    afx_msg void OnBnClickedButtonPdSavePresets();
    afx_msg void OnBnClickedButtonPdAddPreset();
    afx_msg void OnBnClickedButtonPdUpdatePreset();
    afx_msg void OnBnClickedButtonPdUp();
    afx_msg void OnBnClickedButtonPdDown();
    afx_msg void OnClose();
    afx_msg void OnEnChangeEditPdName();
    afx_msg void OnEnChangeEditPdOptions();
};
