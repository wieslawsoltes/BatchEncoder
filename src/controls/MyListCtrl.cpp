// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\MainApp.h"
#include "MyListCtrl.h"

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)
CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{

}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
END_MESSAGE_MAP()

void CMyListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    if (pDispInfo->item.pszText != NULL)
    {
        CString szText;
        this->GetEditControl()->GetWindowText(szText);
        if (szText.GetLength() > 0)
        {
            this->SetItemText(pDispInfo->item.iItem, 0, szText);
            GetParent()->SendMessage(WM_ITEMCHANGED,
                (WPARAM)pDispInfo->item.iItem,
                (LPARAM)szText.GetBuffer(szText.GetLength()));
            szText.ReleaseBuffer();
        }
    }
    *pResult = 0;
}
