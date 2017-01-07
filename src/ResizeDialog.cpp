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

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "ResizeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CResizeDialog::CResizeDialog()
{
    InitVars();
}

CResizeDialog::CResizeDialog(UINT nIDTemplate, CWnd* pParentWnd)
: CDialog(nIDTemplate, pParentWnd)
{
    InitVars();
}

CResizeDialog::CResizeDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
: CDialog(lpszTemplateName, pParentWnd)
{
    InitVars();
}

CResizeDialog::~CResizeDialog()
{
    Layout *pl;

    POSITION pos = m_plLayoutList.GetHeadPosition();
    while(pos != NULL)
    {
        pl = (Layout*)m_plLayoutList.GetNext(pos);
        delete pl;
    }
}

BEGIN_MESSAGE_MAP(CResizeDialog, CDialog)
    //ON_WM_NCHITTEST()
    ON_WM_GETMINMAXINFO()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CResizeDialog::InitVars() 
{
    m_bInitDone = FALSE;

    m_bUseMinTrack = TRUE;
    m_bUseMaxTrack = FALSE;
    m_bUseMaxRect = FALSE;

    // TODO: find howto create XP style sizegrip
    m_bShowGrip = FALSE;

    m_szGripSize.cx = GetSystemMetrics(SM_CXVSCROLL);
    m_szGripSize.cy = GetSystemMetrics(SM_CYHSCROLL);
}

BOOL CResizeDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    UpdateGripPos();
    CRect rc;
    GetWindowRect(&rc);
    m_ptMinTrackSize.x = rc.Width();
    m_ptMinTrackSize.y = rc.Height();
    m_bInitDone = TRUE;

    return TRUE;
}

void CResizeDialog::OnDestroy() 
{
    CDialog::OnDestroy();
}

void CResizeDialog::OnPaint() 
{
    CPaintDC dc(this);

    if(m_bShowGrip && !IsZoomed())
        dc.DrawFrameControl(&m_rcGripRect, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
}

void CResizeDialog::OnSize(UINT nType, int cx, int cy) 
{
    CWnd::OnSize(nType, cx, cy);

    if(nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
        return;

    if(m_bInitDone)
        ArrangeLayout();
}

UINT CResizeDialog::OnNcHitTest(CPoint point) 
{
    CPoint pt = point;
    ScreenToClient(&pt);

    if(m_bShowGrip && m_rcGripRect.PtInRect(pt) && pt.x >= 0 && pt.y >= 0)
        return HTBOTTOMRIGHT;

    return (UINT) CDialog::OnNcHitTest(point);
}

void CResizeDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
    if(!m_bInitDone)
        return;

    if(m_bUseMinTrack)
        lpMMI->ptMinTrackSize = m_ptMinTrackSize;

    if(m_bUseMaxTrack)
        lpMMI->ptMaxTrackSize = m_ptMaxTrackSize;

    if(m_bUseMaxRect)
    {
        lpMMI->ptMaxPosition = m_ptMaxPos;
        lpMMI->ptMaxSize = m_ptMaxSize;
    }
}

void CResizeDialog::AddAnchor(HWND wnd, CSize tl_type, CSize br_type)
{
    ASSERT(wnd != NULL && ::IsWindow(wnd));
    ASSERT(::IsChild(*this, wnd));
    ASSERT(tl_type != NOANCHOR);

    CString st;
    GetClassName(wnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
    st.ReleaseBuffer();
    st.MakeUpper();

    if(st == _T("BUTTON"))
    {
        DWORD style = GetWindowLong(wnd, GWL_STYLE);
        if(style & BS_GROUPBOX)
            SetWindowLong(wnd, GWL_STYLE, style | WS_CLIPSIBLINGS);
    }

    BOOL hscroll = FALSE;
    if(st == _T("LISTBOX"))
        hscroll = TRUE;

    BOOL refresh = FALSE;
    if(st == _T("STATIC"))
    {
        // FIXME: there is problem with refreshing non SS_SIMPLE STATIC text
        //        looks like disabling this refreshing work fine here

        //DWORD style = GetWindowLong(wnd, GWL_STYLE);
        //switch(style & SS_TYPEMASK)
        //{
        //case SS_LEFT:
        //case SS_CENTER:
        //case SS_RIGHT:
        //    refresh = TRUE;
        //}

        //if(style & SS_CENTERIMAGE)
        //    refresh = TRUE;

        //if((style & SS_TYPEMASK) == SS_SIMPLE)
        //    refresh = FALSE;
    }

    CRect wndrc, objrc;
    GetClientRect(&wndrc);
    ::GetWindowRect(wnd, &objrc);
    ScreenToClient(&objrc);

    CSize tl_margin, br_margin;

    if(br_type == NOANCHOR)
        br_type = tl_type;

    tl_margin.cx = objrc.left - wndrc.Width() * tl_type.cx / 100;
    tl_margin.cy = objrc.top - wndrc.Height() * tl_type.cy / 100;
    br_margin.cx = objrc.right - wndrc.Width() * br_type.cx / 100;
    br_margin.cy = objrc.bottom - wndrc.Height() * br_type.cy / 100;

    m_plLayoutList.AddTail(new Layout(wnd, tl_type, tl_margin, br_type, br_margin, hscroll, refresh));
}

void CResizeDialog::ArrangeLayout()
{
    CRect wndrc;
    GetClientRect(&wndrc);

    Layout *pl;
    POSITION pos = m_plLayoutList.GetHeadPosition();

    HDWP hdwp = BeginDeferWindowPos((int) m_plLayoutList.GetCount());

    while(pos != NULL)
    {
        pl = (Layout*)m_plLayoutList.GetNext(pos);

        CRect objrc, newrc;
        CWnd* wnd = CWnd::FromHandle(pl->hwnd);

        wnd->GetWindowRect(&objrc);
        ScreenToClient(&objrc);

        newrc.left = pl->tl_margin.cx + wndrc.Width() * pl->tl_type.cx / 100;
        newrc.top = pl->tl_margin.cy + wndrc.Height() * pl->tl_type.cy / 100;
        newrc.right = pl->br_margin.cx + wndrc.Width() * pl->br_type.cx / 100;
        newrc.bottom = pl->br_margin.cy + wndrc.Height() * pl->br_type.cy / 100;

        if(!newrc.EqualRect(&objrc))
        {
            if(pl->adj_hscroll)
            {
                int diff = newrc.Width() - objrc.Width();
                int max = wnd->GetScrollLimit(SB_HORZ);
                if(max > 0 && wnd->GetScrollPos(SB_HORZ) > max - diff)
                {
                    wnd->MoveWindow(&newrc);
                    wnd->Invalidate();
                }
            }

            if(pl->need_refresh)
            {
                wnd->MoveWindow(&newrc);
                wnd->Invalidate();
            }

            DeferWindowPos(hdwp, pl->hwnd, 
                NULL, 
                newrc.left, newrc.top,
                newrc.Width(), newrc.Height(), 
                SWP_NOZORDER | SWP_NOACTIVATE);
        }
    }

    InvalidateRect(&m_rcGripRect);
    UpdateGripPos();
    InvalidateRect(&m_rcGripRect);
    EndDeferWindowPos(hdwp);
}

void CResizeDialog::UpdateGripPos()
{
    GetClientRect(&m_rcGripRect);

    m_rcGripRect.left = m_rcGripRect.right - m_szGripSize.cx;
    m_rcGripRect.top = m_rcGripRect.bottom - m_szGripSize.cy;
}

void CResizeDialog::ShowSizeGrip(BOOL bShow)
{
    if(m_bShowGrip != bShow)
    {
        m_bShowGrip = bShow;
        InvalidateRect(&m_rcGripRect);
    }
}

void CResizeDialog::SetMaximizedRect(const CRect& rc)
{
    m_bUseMaxRect = TRUE;
    m_ptMaxPos = rc.TopLeft();
    m_ptMaxSize.x = rc.Width();
    m_ptMaxSize.y = rc.Height();
}

void CResizeDialog::ResetMaximizedRect()
{
    m_bUseMaxRect = FALSE;
}

void CResizeDialog::SetMinTrackSize(const CSize& size)
{
    m_bUseMinTrack = TRUE;
    m_ptMinTrackSize.x = size.cx;
    m_ptMinTrackSize.y = size.cy;
}

void CResizeDialog::ResetMinTrackSize()
{
    m_bUseMinTrack = FALSE;
}

void CResizeDialog::SetMaxTrackSize(const CSize& size)
{
    m_bUseMaxTrack = TRUE;
    m_ptMaxTrackSize.x = size.cx;
    m_ptMaxTrackSize.y = size.cy;
}

void CResizeDialog::ResetMaxTrackSize()
{
    m_bUseMaxTrack = FALSE;
}

CString CResizeDialog::GetWindowRectStr()
{
    CString data;
    WINDOWPLACEMENT wp;

    ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(&wp);

    RECT& rc = wp.rcNormalPosition;

    data.Format(_T("%d %d %d %d %d %d"), 
        rc.left, rc.top, rc.right, rc.bottom, wp.showCmd, wp.flags);

    return data;
}

void CResizeDialog::SetWindowRectStr(CString data)
{
    WINDOWPLACEMENT wp;

    if(data.IsEmpty())
        return;

    ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(&wp);

    RECT& rc = wp.rcNormalPosition;

    if(_stscanf(data, _T("%d %d %d %d %d %d"), 
        &rc.left, &rc.top, 
        &rc.right, &rc.bottom, 
        &wp.showCmd, &wp.flags) == 6)
    {
        SetWindowPlacement(&wp);
    }
}
