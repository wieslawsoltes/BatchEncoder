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

/* $Id: PresetsDlg.h 437 2008-03-24 12:15:45Z wisodev $ */

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "ResizeDialog.h"
#include "LListPresets.h"
#include "MyStatic.h"
#include "MyButton.h"

class CPresetsDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CPresetsDlg)
public:
    CPresetsDlg(CWnd* pParent = NULL);
    virtual ~CPresetsDlg();
    enum { IDD = IDD_DIALOG_PRESETS };
protected:
    HICON m_hIcon;
    afx_msg HCURSOR OnQueryDragIcon();
protected:
    afx_msg void OnPaint();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
private:
    volatile bool bUpdate;
public:
    bool bStandAlone;
    CString szStandAloneFile;
public:
    int nSelFormat;
    int nSelPreset;
public:
    CLListPresets m_ListPresets;
public:
    CString szPresetsWndResize;
    CString szPresetsListColumns;
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
public:
    CString szPresetsFile[NUM_PRESET_FILES];
public:
    CString szBrowsePath[NUM_BROWSE_PATH_PRESETS];
public:
    CMyStatic m_StcName;
    CMyStatic m_StcOptions;
public:
    CListCtrl m_LstPresets;
public:
    CComboBox m_CmbFormat;
public:
    CEdit m_EdtName;
    CEdit m_EdtOptions;
    CEdit m_EdtConfigFile;
public:
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
public:
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnUpdateChanges;
    CMyButton m_BtnMoveUp;
    CMyButton m_BtnMoveDown;
    CMyButton m_BtnRemoveAll;
    CMyButton m_BtnRemove;
    CMyButton m_BtnUpdate;
    CMyButton m_BtnAdd;
public:
    bool bShowGridLines;
    void ShowGridlines(bool bShow);
public:
    void LoadConfigFile(CString szFileXml);
    void SaveConfigFile(CString szFileXml);
public:
    void AddToList(CString szName, CString szOptions);
    void ListSelectionChange(void);
public:
    CString GetCurConfigFile();
    void SetCurConfigFile(CString szFile);
public:
    afx_msg void OnLvnKeydownListPdPresets(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickListPdPresets(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangingListPdPresets(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedListPdPresets(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnCbnSelchangeComboPdFormat();
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    afx_msg void OnBnClickedButtonPdRemoveAllPresets();
    afx_msg void OnBnClickedButtonPdRemovePresets();
    afx_msg void OnBnClickedButtonPdLoadPresets();
    afx_msg void OnBnClickedButtonPdSavePresets();
    afx_msg void OnBnClickedButtonPdAddPreset();
    afx_msg void OnBnClickedButtonPdUpdatePreset();
    afx_msg void OnBnClickedButtonPdUp();
    afx_msg void OnBnClickedButtonPdDown();
    afx_msg void OnBnClickedButtonPdUpdate();
public:
    afx_msg void OnClose();
    afx_msg void OnEnChangeEditPdName();
    afx_msg void OnEnChangeEditPdOptions();
};
