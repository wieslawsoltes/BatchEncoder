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
#include "NewItemData.h"

void InitNewItemData(NewItemData &nid)
{
    nid.nAction = -1;
    nid.szFileName = _T("");
    nid.nItem = -1;
    nid.szName = _T("");
    nid.szOutExt = _T("");
    nid.nPreset = -1;
    nid.bCheck = TRUE;
    nid.szTime = _T("");
    nid.szStatus = _T("");
}
