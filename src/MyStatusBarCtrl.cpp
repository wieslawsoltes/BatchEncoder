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
#include "MyStatusBarCtrl.h"
#include "BatchEncoderDlg.h"

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
    // check if the converion process is on
    CBatchEncoderDlg *pDlg = (CBatchEncoderDlg *)GetParent();
    if (pDlg->bRunning == true)
        return;

    // show statusbar popup menu
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
