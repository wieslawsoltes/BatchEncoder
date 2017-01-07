//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
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
    if(pDispInfo->item.pszText != NULL)
    {
        // after user finished editing entry text
        // get edit control and text from it and update
        // edit item text
        CString szText;
        CEdit *edt = this->GetEditControl();
        edt->GetWindowText(szText);

        if(szText.GetLength() > 0)
        {
            // update item text
            this->SetItemText(pDispInfo->item.iItem, 0, szText);

            // send message to parent
            GetParent()->SendMessage(WM_ITEMCHANGED, 
                (WPARAM) pDispInfo->item.iItem,
                (LPARAM) szText.GetBuffer(szText.GetLength()));

            // release buffer
            szText.ReleaseBuffer();
        }
    }

    *pResult = 0;
}

void CFileListCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{
    // NOTE: 
    // when user pressed mouse Middle Button show filelist/show histogram
    // only if clicked over filelist/histogram area
    ((CBatchEncoderDlg *) this->GetParent())->OnShowHistogram();

    CListCtrl::OnMButtonDown(nFlags, point);
}
