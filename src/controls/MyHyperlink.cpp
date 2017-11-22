// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MyHyperlink.h"

IMPLEMENT_DYNAMIC(CMyHyperlink, CStatic)

CMyHyperlink::CMyHyperlink()
{
    m_hCursor = NULL;
    m_bVisited = false;
    m_bCaptured = false;
}

CMyHyperlink::~CMyHyperlink()
{

}

BEGIN_MESSAGE_MAP(CMyHyperlink, CMyStatic)
    ON_CONTROL_REFLECT(STN_CLICKED, OnStnClicked)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

void CMyHyperlink::OnStnClicked()
{
    ::ShellExecute(NULL, _T("open"), m_szURL, NULL, NULL, SW_SHOW);
    m_bVisited = true;
}

HBRUSH CMyHyperlink::CtlColor(CDC* pDC, UINT nCtlColor)
{
    SetTextColor(pDC->GetSafeHdc(), m_bVisited ? colorVisited : m_bCaptured ? colorHover : colorLink);
    SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);

    return (HBRUSH) ::GetStockObject(NULL_BRUSH);
}

void CMyHyperlink::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bCaptured)
    {
        RECT rc;
        GetClientRect(&rc);
        if (PtInRect(&rc, point) == FALSE)
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

BOOL CMyHyperlink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_hCursor)
    {
        ::SetCursor(m_hCursor);
        return TRUE;
    }

    return CMyStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CMyHyperlink::OnDestroy()
{
    CMyStatic::OnDestroy();

    if (m_hCursor != NULL)
        ::DestroyCursor(m_hCursor);
}

void CMyHyperlink::PreSubclassWindow()
{
    colorHover = ::GetSysColor(COLOR_HIGHLIGHT);
    colorLink = RGB(0, 0, 255);
    colorVisited = RGB(128, 0, 128);

    SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, GetStyle() | SS_NOTIFY);
    m_hCursor = ::LoadCursor(NULL, IDC_HAND);

    if (m_hCursor == NULL)
    {
        TCHAR szPath[MAX_PATH + 1];
        ::GetWindowsDirectory(szPath, sizeof(szPath));
        lstrcat(szPath, _T("\\winhlp32.exe"));
        HMODULE hModule = ::LoadLibrary(szPath);
        if (hModule)
        {
            HCURSOR hm_hCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hm_hCursor)
                m_hCursor = CopyCursor(hm_hCursor);
        }
        ::FreeLibrary(hModule);
    }

    CMyStatic::PreSubclassWindow();
}
