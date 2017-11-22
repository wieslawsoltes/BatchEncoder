// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MyComboBoxEdit.h"

IMPLEMENT_DYNAMIC(CMyComboBoxEdit, CEdit)

CMyComboBoxEdit::CMyComboBoxEdit()
{
    this->szToolTipText = _T("");
    this->bHaveToolTipText = false;
}

CMyComboBoxEdit::~CMyComboBoxEdit()
{

}

void CMyComboBoxEdit::PreSubclassWindow()
{
    CEdit::PreSubclassWindow();

    EnableToolTips(TRUE);
}

BEGIN_MESSAGE_MAP(CMyComboBoxEdit, CEdit)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
    ON_CONTROL_REFLECT(EN_CHANGE, &CMyComboBoxEdit::OnEnChange)
END_MESSAGE_MAP()

void CMyComboBoxEdit::SetTooltipText(CString szText)
{
    this->szToolTipText = szText;
    this->bHaveToolTipText = true;
}

CString CMyComboBoxEdit::GetTooltipText()
{
    return this->szToolTipText;
}

void CMyComboBoxEdit::DelTooltipText()
{
    this->bHaveToolTipText = false;
    this->szToolTipText = _T("");
}

bool CMyComboBoxEdit::HaveTooltipText()
{
    return this->bHaveToolTipText;
}

INT_PTR CMyComboBoxEdit::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
    CRect rcTemp;
    GetClientRect(rcTemp);
    if (!rcTemp.PtInRect(point))
        return -1;

    pTI->hwnd = m_hWnd;
    pTI->uId = 1;
    pTI->lpszText = LPSTR_TEXTCALLBACK;
    pTI->rect = rcTemp;

    return pTI->uId;
    // return CEdit::OnToolHitTest(point, pTI);
}

BOOL CMyComboBoxEdit::OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
    TOOLTIPTEXTA *pTTTA = (TOOLTIPTEXTA *)pNMHDR;
    TOOLTIPTEXTW *pTTTW = (TOOLTIPTEXTW *)pNMHDR;
    static CString szTipText;
    UINT nID = pNMHDR->idFrom;

    if (nID == 0)
        return FALSE;

    if (this->HaveTooltipText())
        szTipText = this->GetTooltipText();
    else
        GetWindowText(szTipText);

    // length of tooltips buffer
    static const int nMyTooltipsWidth = 4096;

    // check the tooltip text length
    if (szTipText.GetLength() > nMyTooltipsWidth)
        return FALSE;

    ::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, (LPARAM)nMyTooltipsWidth);

    TCHAR szBuff[nMyTooltipsWidth] = _T("");
    _stprintf_s(szBuff, _T("%s"), (LPCTSTR)szTipText);

#ifndef _UNICODE
    if (pNMHDR->code == TTN_NEEDTEXTA)
    {
        pTTTA->lpszText = szBuff;
    }
    else
    {
        wchar_t szTmpBuff[nMyTooltipsWidth];
        _mbstowcsz(szTmpBuff, szBuff, szTipText.GetLength() + 1);
        pTTTW->lpszText = szTmpBuff;
    }
#else
    if (pNMHDR->code == TTN_NEEDTEXTA)
    {
        char szTmpBuff[nMyTooltipsWidth];
        _wcstombsz(szTmpBuff, szBuff, szTipText.GetLength() + 1);
        pTTTA->lpszText = szTmpBuff;
    }
    else
    {
        pTTTW->lpszText = szBuff;
    }
#endif

    *pResult = 0;
    return TRUE;
}

void CMyComboBoxEdit::OnEnChange()
{
    CString szText;
    this->GetWindowText(szText);
    this->GetParent()->GetParent()->SendMessage(WM_MY_EN_CHANGE, (WPARAM)&szText, 0);
}
