// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "FileListCtrl.h"
#include "BatchEncoderDlg.h"
#include ".\filelistctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CFileListCtrl, CListCtrl)
CFileListCtrl::CFileListCtrl()
{

}

CFileListCtrl::~CFileListCtrl()
{

}

BEGIN_MESSAGE_MAP(CFileListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnLvnEndlabeledit)
    ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

void CFileListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
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

void CFileListCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
    // NOTE: 
    // when user pressed mouse Middle Button show file list/show histogram
    // only if clicked over file list/histogram area
    ((CBatchEncoderDlg *) this->GetParent())->OnShowHistogram();

    CListCtrl::OnMButtonDown(nFlags, point);
}
