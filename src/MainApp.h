// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef __AFXWIN_H__
#error "Include 'StdAfx.h' before including this file for PCH"
#endif // __AFXWIN_H__

#include "res\resource.h"

#include <string>
#include "config\Config.h"

namespace app
{
    class CMainApp : public CWinAppEx
    {
    public:
        CMainApp();
    public:
        virtual BOOL InitInstance();
        DECLARE_MESSAGE_MAP()
    };

    class CLanguageHelper
    {
    public:
        config::CConfig * pConfig;
    public:
        CLanguageHelper(config::CConfig *pConfig) : pConfig(pConfig) { }
    public:
        void SetColumnText(CListCtrl& listCtrl, int nCol, int nKey)
        {
            std::wstring rValue;
            if (this->pConfig->LookupString(nKey, rValue))
            {
                LVCOLUMN lvCol;
                ::ZeroMemory((void *)&lvCol, sizeof(LVCOLUMN));

                lvCol.mask = LVCF_TEXT;
                listCtrl.GetColumn(nCol, &lvCol);

                lvCol.pszText = (LPTSTR)(LPCTSTR)rValue.c_str();
                listCtrl.SetColumn(nCol, &lvCol);
            }
        }
        void SetMenuPopupText(CMenu *hMenu, UINT nPosition, int nKey)
        {
            std::wstring rValue;
            if (this->pConfig->LookupString(nKey, rValue))
                hMenu->ModifyMenu(nPosition, MF_STRING | MF_BYPOSITION, nPosition, rValue.c_str());
        }
        void SetMenuItemText(CMenu *hMenu, UINT nID, int nKey)
        {
            std::wstring rValue;
            if (this->pConfig->LookupString(nKey, rValue))
            {
                UINT nState = hMenu->GetMenuState(nID, MF_BYCOMMAND);
                hMenu->ModifyMenu(nID, MF_BYCOMMAND, nID, rValue.c_str());
                if (nState & MF_CHECKED)
                    hMenu->CheckMenuItem(nID, MF_CHECKED | MF_BYCOMMAND);
                else
                    hMenu->CheckMenuItem(nID, MF_UNCHECKED | MF_BYCOMMAND);
            }
        }
        void SetWndText(CWnd *hWnd, int nKey)
        {
            std::wstring rValue;
            if (this->pConfig->LookupString(nKey, rValue))
                hWnd->SetWindowText(rValue.c_str());
        }
        void SetItemText(CWnd *hWnd, UINT nID, int nKey)
        {
            std::wstring rValue;
            if (this->pConfig->LookupString(nKey, rValue))
                hWnd->GetDlgItem(nID)->SetWindowText(rValue.c_str());
        }
    };

    extern CMainApp m_App;
}
