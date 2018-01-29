// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef __AFXWIN_H__
#error "Include 'StdAfx.h' before including this file for PCH"
#endif // __AFXWIN_H__

#include <string>
#include "res\resource.h"

namespace app
{
    class CMainApp : public CWinAppEx
    {
    public:
        CMainApp();
    public:
        virtual BOOL InitInstance();
        DECLARE_MESSAGE_MAP()
    public:
        bool IsPortable();
    public:
        std::wstring szSettingsPath;
        std::wstring szFormatsPath;
        std::wstring szLanguagesPath;
        std::wstring szProgressPath;
        std::wstring szToolsPath;
        std::wstring szOptionsFile;
        std::wstring szFormatsFile;
        std::wstring szItemsFile;
        std::wstring szToolsFile;
    };

    extern CMainApp m_App;
}
