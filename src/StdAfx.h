// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "version.h"

#define MAIN_APP_NAME           _T(VER_PRODUCTNAME_STR)
#define MAIN_APP_NAME_VER       _T(VER_PRODUCTNAME_STR " v" VER_FILE_VERSION_SHORT_STR " " VER_COPYRIGHT_STR)
#define MAIN_APP_WEBSITE        _T("https://github.com/wieslawsoltes/BatchEncoder")
#define MAIN_APP_EMAIL          _T("wieslaw.soltes@gmail.com")
#define MAIN_APP_CONFIG         _T("BatchEncoder.config")
#define MAIN_APP_LOG            _T("BatchEncoder.log")

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS
#define _AFX_PORTABLE
#define _FORCENAMELESSUNION
#define _AFX_NO_DAO_SUPPORT
#define _AFX_NO_DHTML_SUPPORT
#define _AFX_NO_OLEDB_SUPPORT
#define _AFX_NO_RICHEDIT_SUPPORT

#include <afxwin.h>
#include <afxext.h>
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#ifndef _UNICODE
#include <direct.h>
#endif
#include <afxdhtml.h>

#include <VersionHelpers.h>

#include "XMLDocumentW.h"
#include "Configuration.h"
