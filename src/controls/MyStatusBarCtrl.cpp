// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
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
    ON_COMMAND(ID_STATUSMENU_RESET, OnStatusMenuReset)
END_MESSAGE_MAP()

void CMyStatusBarCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    POINT point;
    CMenu m_Menu;

    GetCursorPos(&point);
    m_Menu.LoadMenu(IDR_MENU_STAT);
    (m_Menu.GetSubMenu(0))->TrackPopupMenu(0, point.x, point.y, this, NULL);

    *pResult = 0;
}

void CMyStatusBarCtrl::OnStatusMenuReset()
{
    SetText(_T(""), 1, 0);
}
