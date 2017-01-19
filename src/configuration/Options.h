// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>

class COptions
{
public:
    int nSelectedFormat;
    CString szOutputPath;
    bool bOutputPathChecked;
    bool bLogConsoleOutput;
    bool bDeleteSourceFiles;
    bool bStayOnTop;
    bool bRecurseChecked;
    CString szMainWindowResize;
    CString szFileListColumns;
    bool bShowGridLines;
    bool bShowTrayIcon;
    bool bShutdownWhenFinished;
    bool bDoNotSaveConfiguration;
    CString szPresetsDialogResize;
    CString szPresetsListColumns;
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
    bool bDeleteOnError;
    bool bStopOnErrors;
    CString szLogFileName;
    int nLogEncoding; // 0 - ANSI, 1 - UNICODE, 2 - UTF-8
    bool bForceConsoleWindow;
public:
    COptions()
    {
    }
    COptions(const COptions &other)
    {
        Copy(other);
    }
    COptions& operator=(const COptions &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~COptions()
    {
    }
public:
    void Copy(const COptions &other)
    {
        this->nSelectedFormat = other.nSelectedFormat;
        this->szOutputPath = other.szOutputPath;
        this->bOutputPathChecked = other.bOutputPathChecked;
        this->bLogConsoleOutput = other.bLogConsoleOutput;
        this->bDeleteSourceFiles = other.bDeleteSourceFiles;
        this->bStayOnTop = other.bStayOnTop;
        this->bRecurseChecked = other.bRecurseChecked;
        this->szMainWindowResize = other.szMainWindowResize;
        this->szFileListColumns = other.szFileListColumns;
        this->bShowGridLines = other.bShowGridLines;
        this->bShowTrayIcon = other.bShowTrayIcon;
        this->bShutdownWhenFinished = other.bShutdownWhenFinished;
        this->bDoNotSaveConfiguration = other.bDoNotSaveConfiguration;
        this->szPresetsDialogResize = other.szPresetsDialogResize;
        this->szPresetsListColumns = other.szPresetsListColumns;
        this->szFormatsDialogResize = other.szFormatsDialogResize;
        this->szFormatsListColumns = other.szFormatsListColumns;
        this->bDeleteOnError = other.bDeleteOnError;
        this->bStopOnErrors = other.bStopOnErrors;
        this->szLogFileName = other.szLogFileName;
        this->nLogEncoding = other.nLogEncoding;
        this->bForceConsoleWindow = other.bForceConsoleWindow;
    }
public:
    void Defaults()
    {
        this->nSelectedFormat = 0;
        this->szOutputPath = _T("");
        this->bOutputPathChecked = false;
        this->bLogConsoleOutput = false;
        this->bDeleteSourceFiles = false;
        this->bStayOnTop = false;
        this->bRecurseChecked = true;
        this->szMainWindowResize = _T("");
        this->szFileListColumns = _T("");
        this->bShowGridLines = true;
        this->bShowTrayIcon = false;
        this->bShutdownWhenFinished = false;
        this->bDoNotSaveConfiguration = false;
        this->szPresetsDialogResize = _T("");
        this->szPresetsListColumns = _T("");
        this->szFormatsDialogResize = _T("");
        this->szFormatsListColumns = _T("");
        this->bDeleteOnError = true;
        this->bStopOnErrors = false;
        this->szLogFileName = _T("BatchEncoder.log");
        this->nLogEncoding = 2;
        this->bForceConsoleWindow = false;
    }
};
