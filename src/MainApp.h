// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef __AFXWIN_H__
#error "Include 'StdAfx.h' before including this file for PCH"
#endif // __AFXWIN_H__

#include "res\resource.h"

class CMainApp : public CWinApp
{
public:
    CMainApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMainApp mainApp;
