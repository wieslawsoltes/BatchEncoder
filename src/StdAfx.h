// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#define WIN32_LEAN_AND_MEAN

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "version.h"
#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS
#define _AFX_PORTABLE
#define _FORCENAMELESSUNION
#define _AFX_NO_DAO_SUPPORT
#define _AFX_NO_DHTML_SUPPORT
#define _AFX_NO_OLEDB_SUPPORT
#define _AFX_NO_RICHEDIT_SUPPORT

#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS

#include <afxwin.h>
#include <afxext.h>

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

#include "controls\MyButton.h"
#include "controls\MyComboBox.h"
#include "controls\MyComboBoxEdit.h"
#include "controls\MyDialogEx.h"
#include "controls\MyEdit.h"
#include "controls\MyHyperlink.h"
#include "controls\MyListCtrl.h"
#include "controls\MyProgressCtrl.h"
#include "controls\MySliderCtrl.h"
#include "controls\MySpinButtonCtrl.h"
#include "controls\MyStatic.h"
#include "controls\MyStatusBarCtrl.h"

#include <string>
#include <utility>
#include <memory>
#include <array>
#include <vector>
#include <thread>
#include <algorithm>

#include "utilities\Log.h"
#include "utilities\FileLog.h"
#include "utilities\String.h"
#include "utilities\TimeCount.h"
#include "utilities\Utf8String.h"
#include "utilities\Utilities.h"

#include "config\Config.h"

#include "worker\WorkerContext.h"
#include "worker\OutputPath.h"
#include "worker\ToolUtilities.h"
#include "worker\Worker.h"
