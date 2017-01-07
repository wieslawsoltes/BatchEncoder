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

#include "afxwin.h"
#include "DropList.h"
#include "MyButton.h"

// nLogEncoding setting values:
// 0 - ANSI
// 1 - UNICODE
// 2 - UTF-8

class CAdvancedDlg : public CDialog
{
    DECLARE_DYNAMIC(CAdvancedDlg)
public:
    CAdvancedDlg(CWnd* pParent = NULL);
    virtual ~CAdvancedDlg();
    enum { IDD = IDD_DIALOG_ADVANCED };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
public:
    CMyButton m_BtnCancel;
    CMyButton m_BtnOK;
public:
    CDropList m_CmbThread;
    CDropList m_CmbProcess;
public:
    CEdit m_EdtLog;
    CButton m_BtnBrowse;
public:
    CString szBrowsePath[NUM_BROWSE_PATH_ADVANCED];
public:
    int nThreadPriorityIndex;
    int nProcessPriorityIndex;
    bool bDeleteOnError;
    bool bStopOnErrors;
    CString szLogFileName;
    int nLogEncoding;
public:
    int GetTheThreadPriority(void);
    DWORD GetTheProcessPriority(void);
public:
    void GetAdvSettings();
    void SetAdvSettings();
public:
    COLORREF m_Color[NUM_PROGRAM_COLORS];
public:
    HWND GetHWND(int nID);
    void PaintRect(int nID, COLORREF cr);
    void OnPaintProc(void);
    COLORREF ChangeTheColor(COLORREF cr);
    void ClickedOnColorRect(int nID);
protected:
    virtual void OnOK();
    virtual void OnCancel();
public:
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
public:
    afx_msg void OnBnClickedButtonBrowseLog();
public:
    afx_msg void OnStnClickedColor0();
    afx_msg void OnStnClickedColor1();
    afx_msg void OnStnClickedColor2();
    afx_msg void OnStnClickedColor3();
    afx_msg void OnStnClickedColor4();
    afx_msg void OnStnClickedColor5();
    afx_msg void OnStnClickedColor6();
    afx_msg void OnStnClickedColor7();
    afx_msg void OnStnClickedColor8();
};
