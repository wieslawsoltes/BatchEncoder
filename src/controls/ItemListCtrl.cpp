// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "..\StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "ItemListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

    // don't update when pszText is NULL
    if (pDispInfo->item.pszText != NULL)
    {
        // after user finished editing entry text
        // get edit control and text from it and update
        // edit item text
        CString szText;
        CEdit *edt = this->GetEditControl();
        edt->GetWindowText(szText);

        if (szText.GetLength() > 0)
        {
            // update item text
            this->SetItemText(pDispInfo->item.iItem, 0, szText);

            // send message to parent
            GetParent()->SendMessage(WM_ITEMCHANGED,
                (WPARAM)pDispInfo->item.iItem,
                (LPARAM)szText.GetBuffer(szText.GetLength()));

            // release buffer
            szText.ReleaseBuffer();
        }
    }

    *pResult = 0;
}
