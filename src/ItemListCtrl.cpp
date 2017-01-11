// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "ItemListCtrl.h"
#include "BatchEncoderDlg.h"
#include ".\itemlistctrl.h"

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
    ON_WM_MBUTTONDOWN()
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

void CItemListCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
    // NOTE: 
    // when user pressed mouse Middle Button show file list/show histogram
    // only if clicked over file list/histogram area
    ((CBatchEncoderDlg *) this->GetParent())->OnShowHistogram();

    CListCtrl::OnMButtonDown(nFlags, point);
}
