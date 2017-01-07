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
    LPCTSTR GetTargetUrl() { return (LPCTSTR) m_szURL; }
public:
    afx_msg void OnStnClicked();
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnDestroy();
protected:
    virtual void PreSubclassWindow();
};
