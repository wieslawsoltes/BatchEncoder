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

/* $Id: Histogram.cpp 437 2008-03-24 12:15:45Z wisodev $ */

#pragma once

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Histogram.h"
#include "BatchEncoderDlg.h"
#include ".\histogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CHistogram, CStatic)
CHistogram::CHistogram()
{
    m_MemDC = NULL;
    m_pFont = NULL;

    // LR Stereo Color
    crLR = RGB(0xEF, 0xC7, 0x7B);

    // MS Stereo Color
    crMS = RGB(0xFE, 0xF6, 0xE4);

    // Column Border Color
    crBorder = RGB(0x00, 0x00, 0x00);

    // Histogram Background Color
    crBack = ::GetSysColor(COLOR_BTNFACE);
}

CHistogram::~CHistogram()
{
    this->Clean();
}

BEGIN_MESSAGE_MAP(CHistogram, CStatic)
    ON_WM_PAINT()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CHistogram::Create(const RECT &rc, CWnd *pParentWnd, UINT uID, bool bVisible = false)
{
    DWORD dwStyle = WS_CHILD | SS_SIMPLE | SS_SUNKEN | SS_NOTIFY;
    if(bVisible == true)
        dwStyle |= WS_VISIBLE;

    // NOTE: STATIC control should have SIMPLE style
    BOOL bRet = CStatic::CreateEx(0 /* WS_EX_CLIENTEDGE */, 
        _T("STATIC") /* AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) */,
        NULL,
        dwStyle,
        rc.left,
        rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        pParentWnd->GetSafeHwnd(),
        (HMENU) uID);
	if(!bRet)
		return FALSE;

	return TRUE;
}

BOOL CHistogram::Init(bool bBoldFont)
{
    CClientDC dc(this);

    CRect rc;
    GetClientRect(rc);

    // create new CDC object
    m_MemDC = new CDC;
    if(!m_MemDC)
        return FALSE;

    if(m_MemDC->GetSafeHdc())
        return FALSE;

    // create compatible memory DC
    if(!m_MemDC->CreateCompatibleDC(&dc))
        return FALSE;

    // create compatible bitmap object
    CBitmap bmp;
    if(!bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height()))
        return FALSE;

    if(!m_MemDC->SelectObject(bmp))
        return FALSE;

    // allocate memory for new font
    m_pFont = new CFont;
    if(m_pFont == NULL)
        return FALSE;

    // get font from parent dialog/window
    CFont* pFont = this->GetParent()->GetFont();
    if(!pFont)
    {
        HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
        if(hFont == NULL)
            hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
        if(hFont)
            pFont = CFont::FromHandle(hFont);
    }

    ASSERT(pFont->GetSafeHandle());

    // get logical font used for creating MemDC fonts
    LOGFONT lf;
    pFont->GetLogFont(&lf);
    
    // create normal/bold style font
    lf.lfWeight = (bBoldFont == true) ? FW_BOLD : FW_NORMAL;
    m_pFont->CreateFontIndirect(&lf);

    return TRUE;
}

void CHistogram::Clean(void)
{
    if(m_MemDC)
    {
        m_MemDC->DeleteDC();
        delete m_MemDC;
        m_MemDC = NULL;
    }

    if(m_pFont)
    {
        m_pFont->DeleteObject();
        delete m_pFont;
        m_pFont = NULL;
    }
}

void CHistogram::Paint()
{
    if(m_MemDC == NULL)
        return;

    CDC *pDC = this->GetDC();
    CRect rc;
    this->GetClientRect(rc);

    ASSERT((pDC != NULL) && (m_MemDC != NULL));

    pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), m_MemDC, 0, 0, SRCCOPY);

    ReleaseDC(pDC);
}

void CHistogram::DrawColumn(int nColumns, 
                            int nPos,
                            int nLR, 
                            int nMS,
                            bool bText = true,
                            CString szText = _T(""))
{
    if(m_MemDC == NULL)
        return;

    if(nColumns <= 0)
        return;

    if((nPos < 0) || (nPos >= nColumns))
        return;

    // get control rectangle
    CRect rc, rcText, rcOrg, rcLR, rcMS;
    this->GetClientRect(rc);
    rcOrg = rc;

    double fColWidth = (double) rc.Width() / (double) nColumns;
    double fStart = (double) nPos * fColWidth;
    double fEnd = ((double) nPos * fColWidth) + fColWidth;

    rc.left = (int) fStart;
    rc.right = (int) fEnd;

    if(bText)
    {
        // set new font for memory DC
        m_MemDC->SelectObject(m_pFont);

        // set text face and backgound color
        m_MemDC->SetBkColor(crBack);
        m_MemDC->SetTextColor(crBorder);
        m_MemDC->SetTextAlign(TA_CENTER);

        // draw text to memory DC
        CSize size;
        size = m_MemDC->GetTextExtent(szText);

        rcText = rc;
        rcText.top = rc.bottom - size.cy;

        m_MemDC->ExtTextOut(rcText.left + rcText.Width() / 2, 
            rcText.top,
            ETO_OPAQUE, 
            rcText, 
            szText, 
            NULL);

        // if drawing text update rectangle
        rc.bottom -= size.cy;
    }

    // inside of the bounding rectangle
    if((nLR == 100) || (nMS == 100))
        rc.top += 1;

    rc.bottom -= 1;

    // set LR rectangle
    rcLR = rc;
    double fValLR = (double) rc.Height() * ((double) nLR / 100.0f);
    rcLR.top = rc.bottom - (int) fValLR;

    // set MS rectangle
    rcMS = rc;
    double fValMS = (double) rc.Height()  * ((double) nMS / 100.0f);
    // rcMS.top = (int)(double) ((double) rcLR.top - fValMS + 1.0f);
    rcMS.top = rcLR.top - (int) fValMS;
    rcMS.bottom = rcLR.top;

    // NOTE: temporary solution
    if(rcMS.top < 1)
        rcMS.top = 1;

    // draw LR rectangle
    if(nLR > 0)
        m_MemDC->FillSolidRect(rcLR, crLR);

    // draw MS rectangle
    if(nMS > 0)
        m_MemDC->FillSolidRect(rcMS, crMS);

    // draw bounding rectangle
    CPen pen;
    pen.CreatePen(PS_SOLID, 1, crBorder);
    m_MemDC->SelectObject(pen);

    CPoint m_Points[5];

    m_Points[0].x = rc.left;
    m_Points[0].y = ((nLR + nMS) == 0) ? rc.bottom : min(rcMS.top - 1, rcLR.top - 1);
    m_Points[1].x = (rcOrg.right == rc.right) ? (rc.right - 1) : rc.right;
    m_Points[1].y = m_Points[0].y;
    m_Points[2].x = m_Points[1].x;
    m_Points[2].y = rc.bottom;
    m_Points[3].x = m_Points[0].x;
    m_Points[3].y = m_Points[2].y;
    m_Points[4].x = m_Points[0].x;
    m_Points[4].y = m_Points[0].y;

    m_MemDC->Polyline(m_Points, 5);

    pen.DeleteObject();
}

void CHistogram::Erase(bool bPaint = true)
{
    if(m_MemDC == NULL)
        return;

    // get control rectangle
    CRect rc;
    this->GetClientRect(rc);

    // draw background rectangle
    rc.right += 1; // becose we are using FillSolidRect
    m_MemDC->FillSolidRect(rc, crBack);

    if(bPaint)
        this->Paint();
}

void CHistogram::Draw(PLAME_ENC_HISTOGRAM plehData)
{
    if(m_MemDC == NULL)
        return;

    if(plehData != NULL)
    {        
        this->Erase(false);

        // find maximum
        int nMax = 0;
        int nSum[HISTOGRAM_COLUMNS];
        for(int i = 0; i < HISTOGRAM_COLUMNS; i++)
        {
            nSum[i] = plehData->nCountPercent[i] + plehData->nCountAsterisk[i];
            if(nSum[i] > nMax)
                nMax = nSum[i];
        }

        CString szText;
        for(int i = 0; i < HISTOGRAM_COLUMNS; i++)
        {
            int nLR = plehData->nCountPercent[i];
            int nMS = plehData->nCountAsterisk[i];

            // correct values to fit 0..100 range
            if(nMax > 0)
            {
                double fFactor = 100.0f / (double) nMax;
                nLR = (int) ((double) nLR * fFactor + 0.5);
                nMS = (int) ((double) nMS * fFactor + 0.5);
            }

            szText.Format(_T("%d"), plehData->nBitrate[i]);
            this->DrawColumn(HISTOGRAM_COLUMNS, 
                i, 
                nLR, 
                nMS, 
                true, 
                szText);
        }

        this->Paint();
    }
}

void CHistogram::OnPaint()
{
    CPaintDC dc(this);

    // draw data from memory DC when on demand
    this->Paint();
}

void CHistogram::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // NOTE: erase control area to backgound color when user double-clicked
    if(((CBatchEncoderDlg *) this->GetParent())->bRunning == false)
        this->Erase(true);

    CStatic::OnLButtonDblClk(nFlags, point);
}

void CHistogram::OnMButtonDown(UINT nFlags, CPoint point)
{
    // NOTE: 
    // when user pressed mouse Middle Button show filelist/show histogram
    // only if clicked over filelist/histogram area
    ((CBatchEncoderDlg *) this->GetParent())->OnShowHistogram();

    CStatic::OnMButtonDown(nFlags, point);
}
