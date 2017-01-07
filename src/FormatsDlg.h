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

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "ResizeDialog.h"
#include "MyButton.h"
#include "MyStatic.h"

class CFormatsDlg : public CResizeDialog
{
    DECLARE_DYNAMIC(CFormatsDlg)
public:
    CFormatsDlg(CWnd* pParent = NULL);
    virtual ~CFormatsDlg();
    enum { IDD = IDD_DIALOG_FORMATS };
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
public:
    CListCtrl m_LstFormats;
public:
    CString szFormatsWndResize;
    CString szFormatsListColumns;
public:
    void LoadWindowSettings();
    void SaveWindowSettings();
public:
    CEdit m_EdtPath;
    CEdit m_EdtTemplate;
    CEdit m_EdtProgress;
public:
    CMyStatic m_GrpPipes;
    CMyStatic m_StcPath;
    CMyStatic m_StcTemplate;
    CMyStatic m_StcProgress;
public:
    CMyButton m_BtnOK;
    CMyButton m_BtnCancel;
    CMyButton m_BtnLoad;
    CMyButton m_BtnSave;
    CMyButton m_BtnDefaults;
    CMyButton m_BtnBrowse;
    CMyButton m_BtnChange;
public:
    bool bShowGridLines;
    void ShowGridlines(bool bShow);
public:
    void UpdateEditableFields(void);
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonLoadConfig();
    afx_msg void OnBnClickedButtonSaveConfig();
    afx_msg void OnBnClickedButtonFdBrowse();
    afx_msg void OnBnClickedButtonFdBrowseProgress();
    afx_msg void OnBnClickedButtonFdUpdatePreset();
    afx_msg void OnBnClickedButtonDefaultConfig();
public:
    CString szBrowsePath[(NUM_BROWSE_PATH_FORMATS + NUM_BROWSE_PATH_PROGRESS)];
public:
    CString szFormatTemplate[NUM_FORMAT_NAMES];
    CString szFormatPath[NUM_FORMAT_NAMES];
    bool bFormatInput[NUM_FORMAT_NAMES];
    bool bFormatOutput[NUM_FORMAT_NAMES];
    CString szFormatFunction[NUM_FORMAT_NAMES];
public:
    bool BrowseForCliExe(CString szDefaultFName, CEdit *pEdit, int nID);
    bool BrowseForProgress(CString szDefaultFName, CEdit *pEdit, int nID);
public:
    afx_msg void OnLvnItemchangedEditFdFormats(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnEnChangeEditFdCliPath();
    afx_msg void OnEnChangeEditFdCliTemplate();
    afx_msg void OnEnChangeEditFdCliProgress();
public:
    afx_msg void OnBnClickedCheckFdPipesInput();
    afx_msg void OnBnClickedCheckFdPipesOutput();
public:
    afx_msg void OnClose();
};
