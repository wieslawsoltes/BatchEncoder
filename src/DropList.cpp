// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "DropList.h"
#include ".\droplist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDropList, CComboBox)
CDropList::CDropList()
{

}

CDropList::~CDropList()
{

}

BEGIN_MESSAGE_MAP(CDropList, CComboBox)
END_MESSAGE_MAP()
