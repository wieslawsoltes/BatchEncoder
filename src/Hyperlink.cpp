//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
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

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Hyperlink.h"
#include ".\hyperlink.h"

IMPLEMENT_DYNAMIC(CHyperlink, CStatic)
CHyperlink::CHyperlink()
{
    m_hCursor = NULL;
    m_bVisited = false;
    m_bCaptured = false;
}

CHyperlink::~CHyperlink()
{

}

BEGIN_MESSAGE_MAP(CHyperlink, CMyStatic) 
    ON_CONTROL_REFLECT(STN_CLICKED, OnStnClicked)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

void CHyperlink::OnStnClicked()
{
    ::ShellExecute(NULL, _T("open"), m_szURL, NULL, NULL, SW_SHOW);
    m_bVisited = true;
}

HBRUSH CHyperlink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    SetTextColor(pDC->GetSafeHdc(), m_bVisited ? colorVisited : m_bCaptured ? colorHover : colorLink);
    SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);

    return (HBRUSH) ::GetStockObject(NULL_BRUSH);
}

void CHyperlink::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_bCaptured)
    {
        RECT rc;
        GetClientRect(&rc);
        if(PtInRect(&rc, point) == FALSE)
        {
            m_bCaptured = false;
            ReleaseCapture();
            RedrawWindow();
        }
    }
    else
    {
        SetCapture();
        m_bCaptured = true;
        RedrawWindow();
    }

    CMyStatic::OnMouseMove(nFlags, point);
}

BOOL CHyperlink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if(m_hCursor)
    {
        ::SetCursor(m_hCursor);
        return TRUE;
    }

    return CMyStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CHyperlink::OnDestroy()
{
    CMyStatic::OnDestroy();

    if(m_hCursor != NULL)
        ::DestroyCursor(m_hCursor);
}

void CHyperlink::PreSubclassWindow()
{
    colorHover = ::GetSysColor(COLOR_HIGHLIGHT);
    colorLink = RGB(0, 0, 255);
    colorVisited = RGB(128, 0, 128);

    SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, GetStyle() | SS_NOTIFY);
    m_hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));

    if(m_hCursor == NULL)
    {
        TCHAR szPath[MAX_PATH + 1];
        ::GetWindowsDirectory(szPath, sizeof(szPath));
        lstrcat(szPath, _T("\\winhlp32.exe"));
        HMODULE hModule = ::LoadLibrary(szPath);
        if(hModule)
        {
            HCURSOR hm_hCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if(hm_hCursor)
                m_hCursor = CopyCursor(hm_hCursor);
        }
        ::FreeLibrary(hModule);
    }

    CMyStatic::PreSubclassWindow();
}
