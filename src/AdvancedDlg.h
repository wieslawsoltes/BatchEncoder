// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "afxwin.h"
#include "DropList.h"
#include "MyButton.h"

#define FROM_CCOLOR_TO_COLORREF(c) RGB(c.nRed,c.nGreen,c.nBlue)

#define FROM_COLORREF_TO_CCOLOR(c,cr) c.nRed   = GetRValue(cr); \
                           c.nGreen = GetGValue(cr); \
                           c.nBlue  = GetBValue(cr);

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
    CSettings m_Settings;
    CColors m_Colors;
public:
    int GetTheThreadPriority(void);
    DWORD GetTheProcessPriority(void);
public:
    void GetAdvSettings();
    void SetAdvSettings();
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
