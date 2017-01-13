// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

#include "MyStatic.h"

class CHyperlink : public CMyStatic
{
    DECLARE_DYNAMIC(CHyperlink)
public:
    CHyperlink();
    virtual ~CHyperlink();
protected:
    DECLARE_MESSAGE_MAP()
protected:
    HCURSOR m_hCursor;
    CString m_szURL;
    bool m_bCaptured;
    bool m_bVisited;
    COLORREF colorHover;
    COLORREF colorLink;
    COLORREF colorVisited;
public:
    void SetTargetUrl(LPCTSTR szUrl) { m_szURL = szUrl; }
    LPCTSTR GetTargetUrl() { return (LPCTSTR)m_szURL; }
public:
    afx_msg void OnStnClicked();
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnDestroy();
protected:
    virtual void PreSubclassWindow();
};
