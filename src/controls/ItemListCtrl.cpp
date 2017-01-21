// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "ItemListCtrl.h"

IMPLEMENT_DYNAMIC(CItemListCtrl, CListCtrl)
CItemListCtrl::CItemListCtrl()
{

}

CItemListCtrl::~CItemListCtrl()
{

}

BEGIN_MESSAGE_MAP(CItemListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
END_MESSAGE_MAP()

void CItemListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    if (pDispInfo->item.pszText != NULL)
    {
        CString szText;
        CEdit *edt = this->GetEditControl();

        edt->GetWindowText(szText);
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
