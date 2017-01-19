// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "MyStatusBarCtrl.h"

IMPLEMENT_DYNAMIC(CMyStatusBarCtrl, CStatusBarCtrl)
CMyStatusBarCtrl::CMyStatusBarCtrl()
{

}

CMyStatusBarCtrl::~CMyStatusBarCtrl()
{

}

BEGIN_MESSAGE_MAP(CMyStatusBarCtrl, CStatusBarCtrl)
    ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
    ON_COMMAND(ID_STATUSMENU_RESETCONVERSIONSTATUS, OnStatusMenuResetConversionStatus)
END_MESSAGE_MAP()

void CMyStatusBarCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    // check if the conversion process is on
    CBatchEncoderDlg *pDlg = (CBatchEncoderDlg *)GetParent();
    if (pDlg->pWorkerContext->bRunning == true)
        return;

    // show status-bar popup menu
    POINT point;
    GetCursorPos(&point);

    CMenu m_Menu;
    m_Menu.LoadMenu(IDR_MENU_STAT);

    CMenu *m_SubMenu;
    m_SubMenu = m_Menu.GetSubMenu(0);

    m_SubMenu->TrackPopupMenu(0, point.x, point.y, this, NULL);

    *pResult = 0;
}

void CMyStatusBarCtrl::OnStatusMenuResetConversionStatus()
{
    SetText(_T(""), 1, 0);
}
