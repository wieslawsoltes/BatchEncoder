// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif
#include "Configuration.h"

class CLanguageHelper
{
public:
    CConfiguration *pConfig;
public:
    CLanguageHelper(CConfiguration *pConfig)
    {
        this->pConfig = pConfig;
    }
    virtual ~CLanguageHelper()
    {
    }
public:
    void SetColumnText(CListCtrl& listCtrl, int nCol, int nKey)
    {
        CString rValue;
        if (this->pConfig->LookupString(nKey, rValue))
        {
            LVCOLUMN lvCol;
            ::ZeroMemory((void *)&lvCol, sizeof(LVCOLUMN));

            lvCol.mask = LVCF_TEXT;
            listCtrl.GetColumn(nCol, &lvCol);

            lvCol.pszText = (LPTSTR)(LPCTSTR)rValue;
            listCtrl.SetColumn(nCol, &lvCol);
        }
    }
    void SetMenuPopupText(CMenu *hMenu, UINT nPosition, int nKey)
    {
        CString rValue;
        if (this->pConfig->LookupString(nKey, rValue))
        {
            hMenu->ModifyMenu(nPosition, MF_STRING | MF_BYPOSITION, nPosition, rValue);
        }
    }
    void SetMenuItemText(CMenu *hMenu, UINT nID, int nKey)
    {
        CString rValue;
        if (this->pConfig->LookupString(nKey, rValue))
        {
            UINT nState = hMenu->GetMenuState(nID, MF_BYCOMMAND);
            hMenu->ModifyMenu(nID, MF_BYCOMMAND, nID, rValue);
            if (nState & MF_CHECKED)
                hMenu->CheckMenuItem(nID, MF_CHECKED | MF_BYCOMMAND);
            else
                hMenu->CheckMenuItem(nID, MF_UNCHECKED | MF_BYCOMMAND);
        }
    }
    void SetWndText(CWnd *hWnd, int nKey)
    {
        CString rValue;
        if (this->pConfig->LookupString(nKey, rValue))
        {
            hWnd->SetWindowText(rValue);
        }
    }
};
