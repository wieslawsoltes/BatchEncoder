// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef __AFXWIN_H__
#error "Include 'StdAfx.h' before including this file for PCH"
#endif // __AFXWIN_H__

#include "res\resource.h"

class CMainApp : public CWinAppEx
{
public:
    CMainApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
public:
    bool m_bIsPortable = true;
public:
    CString CombinePath(CString szPath, CString szFile);
public:
    CString szSettingsPath;
public:
    CString szFormatsPath;
    CString szPresetsPath;
    CString szLanguagesPath;
    CString szProgressPath;
    CString szToolsPath;
public:
    CString szOptionsFile;
    CString szFormatsFile;
    CString szItemsFile;
    CString szToolsFile;
};

extern CMainApp m_App;
