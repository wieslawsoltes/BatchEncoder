// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxdialogex.h>

const CSize AnchorNone(-1, -1);
const CSize AnchorTopLeft(0, 0);
const CSize AnchorTopCenter(50, 0);
const CSize AnchorTopRight(100, 0);
const CSize AnchorMiddleLeft(0, 50);
const CSize AnchorMiddleCenter(50, 50);
const CSize AnchorMiddleRight(100, 50);
const CSize AnchorBottomLeft(0, 100);
const CSize AnchorBottomCenter(50, 100);
const CSize AnchorBottomRight(100, 100);

class CMyResizeDialog : public CDialogEx
{
private:
    typedef struct
    {
        HWND hWnd;
        BOOL bAdjHscroll;
        SIZE typeTL, marginTL;
        SIZE typeBR, marginBR;
    } Layout;
private:
    void InitLayout(Layout &layout, HWND hw, SIZE tTL, SIZE mTL, SIZE tBR, SIZE mBR, BOOL hscroll)
    {
        layout.hWnd = hw;
        layout.typeTL = tTL;
        layout.marginTL = mTL;
        layout.typeBR = tBR;
        layout.marginBR = mBR;
        layout.bAdjHscroll = hscroll;
    };
private:
    BOOL m_bUseMaxTrack, m_bUseMinTrack, m_bUseMaxRect;
    BOOL m_bInitDone;
    POINT m_ptMinTrackSize, m_ptMaxTrackSize;
    POINT m_ptMaxPos, m_ptMaxSize;
    CList<Layout, Layout> m_LayoutList;
private:
    void InitVars();
public:
    void CleanUp();
protected:
    virtual BOOL OnInitDialog();
protected:
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR *lpMMI);
    afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
    DECLARE_MESSAGE_MAP()
public:
    CMyResizeDialog();
    CMyResizeDialog(UINT nIDTemplate, CWnd *pParentWnd = NULL);
    CMyResizeDialog(LPCTSTR lpszTemplateName, CWnd *pParentWnd = NULL);
    virtual ~CMyResizeDialog();
public:
    void UpdateLayout();
    void ArrangeLayout();
    void AddAnchor(HWND newWnd, CSize typeTL, CSize typeBR = AnchorNone);
    void AddAnchor(UINT nCtrlID, CSize typeTL, CSize typeBR = AnchorNone);
    void SetMaximizedRect(const CRect &rc);
    void ResetMaximizedRect();
    void SetMinTrackSize(const CSize &size);
    void ResetMinTrackSize();
    void SetMaxTrackSize(const CSize &size);
    void ResetMaxTrackSize();
    void UpdateWindowPos(HWND hWnd, CRect newRC);
    CString GetWindowRectStr();
    void SetWindowRectStr(CString szData);
};
