// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "ConversionStatus.h"
#include "BatchEncoderDlg.h"
#include ".\conversionstatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CConversionStatus, CStatic)
CConversionStatus::CConversionStatus()
{
    m_MemDC = NULL;
    m_pFontNormal = NULL;
    m_pFontBold = NULL;

    // Text Color
    crText = RGB(0x00, 0x00, 0x00);

    // Text Error Color
    crTextError = RGB(0xFF, 0x00, 0x00);

    // Progress Color
    crProgress = RGB(0x00, 0xFF, 0x00);

    // Progress Border Color
    crBorder = RGB(0x80, 0x80, 0x80);

    // Background Color
    crBack = RGB(0xFF, 0xFF, 0xFF);
}

CConversionStatus::~CConversionStatus()
{
    this->Clean();
}

BEGIN_MESSAGE_MAP(CConversionStatus, CStatic)
    ON_WM_PAINT()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CConversionStatus::Create(const RECT &rc, CWnd *pParentWnd, UINT uID, bool bVisible = false)
{
    DWORD dwStyle = WS_CHILD | SS_SIMPLE | SS_SUNKEN | SS_NOTIFY;
    if (bVisible == true)
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
        (HMENU)(UINT_PTR)uID);
    if (!bRet)
        return FALSE;

    return TRUE;
}

BOOL CConversionStatus::Init(void)
{
    CClientDC dc(this);

    CRect rc;
    GetClientRect(rc);

    // create new CDC object
    m_MemDC = new CDC;
    if (!m_MemDC)
        return FALSE;

    if (m_MemDC->GetSafeHdc())
        return FALSE;

    // create compatible memory DC
    if (!m_MemDC->CreateCompatibleDC(&dc))
        return FALSE;

    // create compatible bitmap object
    CBitmap bmp;
    if (!bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height()))
        return FALSE;

    if (!m_MemDC->SelectObject(bmp))
        return FALSE;

    // allocate memory for new fonts
    m_pFontNormal = new CFont;
    if (m_pFontNormal == NULL)
        return FALSE;

    m_pFontBold = new CFont;
    if (m_pFontBold == NULL)
        return FALSE;

    // get font from parent dialog/window
    CFont* pFont = this->GetParent()->GetFont();
    if (!pFont)
    {
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        if (hFont == NULL)
            hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
        if (hFont)
            pFont = CFont::FromHandle(hFont);
    }

    ASSERT(pFont->GetSafeHandle());

    // get logical font used for creating MemDC fonts
    LOGFONT lf;
    pFont->GetLogFont(&lf);

    // create normal style font
    lf.lfWeight = FW_NORMAL;
    m_pFontNormal->CreateFontIndirect(&lf);

    // create bold style font
    lf.lfWeight = FW_BOLD;
    m_pFontBold->CreateFontIndirect(&lf);

    return TRUE;
}

void CConversionStatus::Clean(void)
{
    if (m_MemDC)
    {
        m_MemDC->DeleteDC();
        delete m_MemDC;
        m_MemDC = NULL;
    }

    if (m_pFontNormal)
    {
        m_pFontNormal->DeleteObject();
        delete m_pFontNormal;
        m_pFontNormal = NULL;
    }

    if (m_pFontBold)
    {
        m_pFontBold->DeleteObject();
        delete m_pFontBold;
        m_pFontBold = NULL;
    }
}

void CConversionStatus::Paint()
{
    if (m_MemDC == NULL)
        return;

    CDC *pDC = this->GetDC();
    CRect rc;
    this->GetClientRect(rc);

    ASSERT((pDC != NULL) && (m_MemDC != NULL));

    pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), m_MemDC, 0, 0, SRCCOPY);

    ReleaseDC(pDC);
}

void CConversionStatus::SetFontMemDC(bool bBold, COLORREF crBack, COLORREF crText)
{
    // select proper font to MemDC
    m_MemDC->SelectObject((bBold == true) ? this->m_pFontBold : this->m_pFontNormal);

    // set text face and background color
    m_MemDC->SetBkColor(crBack);
    m_MemDC->SetTextColor(crText);
    m_MemDC->SetTextAlign(TA_LEFT | TA_NOUPDATECP);
}

void CConversionStatus::DrawTextMemDC(CRect &rcText, CString &szText)
{
    // draw text to memory DC
    m_MemDC->ExtTextOut(rcText.left,
        rcText.top,
        ETO_OPAQUE,
        rcText,
        szText,
        NULL);
}

void CConversionStatus::DrawPercentageMemDC(bool bBold, CRect &rcText, CString &szText, COLORREF crText)
{
    // select proper font to MemDC
    m_MemDC->SelectObject((bBold == true) ? this->m_pFontBold : this->m_pFontNormal);

    // set text face and background mode
    m_MemDC->SetBkMode(TRANSPARENT);
    m_MemDC->SetTextColor(crText);
    m_MemDC->SetTextAlign(TA_CENTER | TA_NOUPDATECP);

    // draw text to memory DC
    m_MemDC->ExtTextOut(rcText.left,
        rcText.top,
        0,
        rcText,
        szText,
        NULL);
}

void CConversionStatus::DrawStatus(int nCurrentProgress)
{
    if (m_MemDC == NULL)
        return;

    // int nFile = 2; // 1..nTotalFiles
    // int nTotalFiles = 10; // nTotalFiles > 0
    // int nCurrentProgress = 75; // 0..100

    // int nTotalProgress = (int)(double) (((double) (nFile - 1)* 100.0) / (double) nTotalFiles); // 0..100, nTotalFiles > 0
    double fFactor = (100.0 / (double)nTotalFiles);
    int nTotalProgress = (int)(double)((double)(nFile - 1) * fFactor + (fFactor / 100.0 * (double)nCurrentProgress));

    // int nStatusDone = 1; // 1..nTotalFiles
    int nStatusErrors = (nFile - 1) - nStatusDone; // nFile >= 1, 1..nTotalFiles

    // TODO: check if string is not to long for drawing szInFile, szOutFile, szOutPath
    // and maybe use as solution some kind of Word Ellipsis function

    // get control rectangle
    CRect rc, rcText;
    this->GetClientRect(rc);

    CString szText;
    CSize size;
    LONG nLastPos = 0L;

    // TEXT: Processing item: [b]%d[/b] of [b]%d[/b]

    szText.Format(_T("Processing item: "));

    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = 2 * size.cy; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = 2 * size.cy + size.cy; // NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%d"), nFile);
    this->SetFontMemDC(true, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T(" of "));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%d"), nTotalFiles);
    this->SetFontMemDC(true, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    // TEXT: Input file: [b]%s[/b]

    szText.Format(_T("Input file: "));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.bottom + 5; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = rcText.top + size.cy;// NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%s"), szInFile);
    this->SetFontMemDC(true, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    // TEXT: Output file: [b]%s[/b]

    szText.Format(_T("Output file: "));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.bottom + 5; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = rcText.top + size.cy;// NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%s"), szOutFile);
    this->SetFontMemDC(true, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    // TEXT: Output path: [b]%s[/b]

    szText.Format(_T("Output path: "));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.bottom + 5; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = rcText.top + size.cy;// NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%s"), szOutPath);
    this->SetFontMemDC(true, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    // TEXT: Current progress:

    szText.Format(_T("Current progress:"));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.bottom + 2 * size.cy; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = rcText.top + size.cy;// NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    // PROGRESS: current progress

    CRect rcCurProgress;

    rcCurProgress.top = rcText.bottom + 5;
    rcCurProgress.left = size.cy; // NOTE: maybe I will set this up to 10px at start of this function
    rcCurProgress.right = rc.right - size.cy;
    rcCurProgress.bottom = rcCurProgress.top + 20; // NOTE: this will be floating value

    CRect rcCurProgressOffset = rcCurProgress;
    rcCurProgressOffset.InflateRect(-2, -2, -1, -1);

    // calculate progress position
    int nCurProgressPos = rcCurProgressOffset.Width();
    nCurProgressPos = (int)(double)(((double)nCurProgressPos / 100.0) * (double)nCurrentProgress);

    rcCurProgressOffset.right = rcCurProgressOffset.left + nCurProgressPos;

    m_MemDC->FillSolidRect(rcCurProgressOffset, crProgress);

    // draw bounding rectangle
    CPen penCurrent;
    penCurrent.CreatePen(PS_SOLID, 1, crBorder);
    m_MemDC->SelectObject(penCurrent);

    CPoint m_CurrentPoints[5];

    m_CurrentPoints[0].x = rcCurProgress.left;
    m_CurrentPoints[0].y = rcCurProgress.top;
    m_CurrentPoints[1].x = rcCurProgress.right; // rcCurProgress.right - 1
    m_CurrentPoints[1].y = m_CurrentPoints[0].y;
    m_CurrentPoints[2].x = m_CurrentPoints[1].x;
    m_CurrentPoints[2].y = rcCurProgress.bottom;
    m_CurrentPoints[3].x = m_CurrentPoints[0].x;
    m_CurrentPoints[3].y = m_CurrentPoints[2].y;
    m_CurrentPoints[4].x = m_CurrentPoints[0].x;
    m_CurrentPoints[4].y = m_CurrentPoints[0].y;

    m_MemDC->Polyline(m_CurrentPoints, 5);

    penCurrent.DeleteObject();

    // TEXT: [b]%d %[/b]

    CRect rcPercentageCur = rcCurProgress;

    szText.Format(_T("%d %%"), nCurrentProgress);

    CSize sizeCur = m_MemDC->GetTextExtent(szText);

    rcPercentageCur.top = (rcPercentageCur.bottom - rcPercentageCur.Height() / 2) - (sizeCur.cy / 2);
    rcPercentageCur.left = rcPercentageCur.right - rcPercentageCur.Width() / 2;

    this->DrawPercentageMemDC(true, rcPercentageCur, szText, crText);

    // TEXT: Total progress:

    szText.Format(_T("Total progress:"));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcCurProgress.bottom + size.cy; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = rcText.top + size.cy;// NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    // PROGRESS: total progress

    CRect rcTotProgress;

    rcTotProgress.top = rcText.bottom + 5;
    rcTotProgress.left = size.cy; // NOTE: maybe I will set this up to 10px at start of this function
    rcTotProgress.right = rc.right - size.cy;
    rcTotProgress.bottom = rcTotProgress.top + 20; // NOTE: this will be floating value

    CRect rcTotProgressOffset = rcTotProgress;
    rcTotProgressOffset.InflateRect(-2, -2, -1, -1);

    // calculate progress position
    int nTotProgressPos = rcTotProgressOffset.Width();
    nTotProgressPos = (int)(double)(((double)nTotProgressPos / 100.0) * (double)nTotalProgress);

    rcTotProgressOffset.right = rcTotProgressOffset.left + nTotProgressPos;

    m_MemDC->FillSolidRect(rcTotProgressOffset, crProgress);

    // draw bounding rectangle
    CPen penTotal;
    penTotal.CreatePen(PS_SOLID, 1, crBorder);
    m_MemDC->SelectObject(penTotal);

    CPoint m_TotalPoints[5];

    m_TotalPoints[0].x = rcTotProgress.left;
    m_TotalPoints[0].y = rcTotProgress.top;
    m_TotalPoints[1].x = rcTotProgress.right; // rcTotProgress.right - 1
    m_TotalPoints[1].y = m_TotalPoints[0].y;
    m_TotalPoints[2].x = m_TotalPoints[1].x;
    m_TotalPoints[2].y = rcTotProgress.bottom;
    m_TotalPoints[3].x = m_TotalPoints[0].x;
    m_TotalPoints[3].y = m_TotalPoints[2].y;
    m_TotalPoints[4].x = m_TotalPoints[0].x;
    m_TotalPoints[4].y = m_TotalPoints[0].y;

    m_MemDC->Polyline(m_TotalPoints, 5);

    penTotal.DeleteObject();

    // TEXT: [b]%d %[/b]

    CRect rcPercentageTot = rcTotProgress;

    szText.Format(_T("%d %%"), nTotalProgress);

    CSize sizeTot = m_MemDC->GetTextExtent(szText);

    rcPercentageTot.top = (rcPercentageTot.bottom - rcPercentageTot.Height() / 2) - (sizeTot.cy / 2);
    rcPercentageTot.left = rcPercentageTot.right - rcPercentageTot.Width() / 2;

    this->DrawPercentageMemDC(true, rcPercentageTot, szText, crText);

    // TEXT: Overall status: [b]%d[b] Done, [b]%[/b] Errors

    szText.Format(_T("Overall status: "));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcTotProgress.bottom + 2 * size.cy; // NOTE: initial value
    rcText.left = size.cy; // NOTE: initial value
    rcText.bottom = rcText.top + size.cy;// NOTE: initial value
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%d"), nStatusDone);
    this->SetFontMemDC(true, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T(" Done "));
    this->SetFontMemDC(false, crBack, crText);
    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    szText.Format(_T("%d"), nStatusErrors);

    if (nStatusErrors == 0)
        this->SetFontMemDC(true, crBack, crText);
    else
        this->SetFontMemDC(true, crBack, crTextError);

    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);

    if ((nStatusErrors == 0) || (nStatusErrors > 1))
        szText.Format(_T(" Errors"));
    else if (nStatusErrors == 1)
        szText.Format(_T(" Error"));

    this->SetFontMemDC(false, crBack, crText);

    size = m_MemDC->GetTextExtent(szText);

    rcText.top = rcText.top;
    rcText.left = nLastPos;
    rcText.bottom = rcText.bottom;
    rcText.right = rcText.left + size.cx;
    nLastPos = rcText.right;

    this->DrawTextMemDC(rcText, szText);
}

void CConversionStatus::Erase(bool bPaint = true)
{
    if (m_MemDC == NULL)
        return;

    // get control rectangle
    CRect rc;
    this->GetClientRect(rc);

    // draw background rectangle
    rc.right += 1; // because we are using FillSolidRect
    m_MemDC->FillSolidRect(rc, crBack);

    if (bPaint)
        this->Paint();
}

void CConversionStatus::Draw(int nCurrentProgress)
{
    if (m_MemDC == NULL)
        return;

    this->Erase(false);

    this->DrawStatus(nCurrentProgress);

    this->Paint();
}

void CConversionStatus::SetCurrentInfo(int nFile,
    int nTotalFiles,
    int nStatusDone,
    CString szInFile,
    CString szOutFile,
    CString szOutPath)
{
    this->nFile = nFile;
    this->nTotalFiles = nTotalFiles;
    this->nStatusDone = nStatusDone;
    this->szInFile = szInFile;
    this->szOutFile = szOutFile;
    this->szOutPath = szOutPath;
}

void CConversionStatus::OnPaint()
{
    CPaintDC dc(this);

    // draw data from memory DC when on demand
    this->Paint();
}

void CConversionStatus::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // NOTE: erase control area to background color when user double-clicked
    if (((CBatchEncoderDlg *) this->GetParent())->bRunning == false)
        this->Erase(true);

    CStatic::OnLButtonDblClk(nFlags, point);
}

void CConversionStatus::OnMButtonDown(UINT nFlags, CPoint point)
{
    // NOTE: 
    // when user pressed mouse Middle Button show file list/show conversion status
    // only if clicked over file list/conversion status area
    ((CBatchEncoderDlg *) this->GetParent())->OnShowCnvStatus();

    CStatic::OnMButtonDown(nFlags, point);
}
