// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>

class COptions
{
public:
    CString szSelectedLanguage;
    int nSelectedFormat;
    CString szOutputPath;
    bool bDeleteSourceFiles;
    bool bRecurseChecked;
    bool bShutdownWhenFinished;
    bool bDoNotSaveConfiguration;
    bool bDeleteOnErrors;
    bool bStopOnErrors;
    bool bHideConsoleWindow;
    bool bTryToFindDecoder;
    bool bEnsureItemIsVisible;
    bool bValidateInputFiles;
    bool bOverwriteExistingFiles;
    int nThreadCount;
    CString szMainWindowResize;
    CString szFileListColumns;
    CString szPresetsDialogResize;
    CString szPresetsListColumns;
    CString szFormatsDialogResize;
    CString szFormatsListColumns;
    CString szToolsDialogResize;
    CString szToolsListColumns;
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
        this->szSelectedLanguage = other.szSelectedLanguage;
        this->nSelectedFormat = other.nSelectedFormat;
        this->szOutputPath = other.szOutputPath;
        this->bDeleteSourceFiles = other.bDeleteSourceFiles;
        this->bRecurseChecked = other.bRecurseChecked;
        this->bShutdownWhenFinished = other.bShutdownWhenFinished;
        this->bDoNotSaveConfiguration = other.bDoNotSaveConfiguration;
        this->bDeleteOnErrors = other.bDeleteOnErrors;
        this->bStopOnErrors = other.bStopOnErrors;
        this->bHideConsoleWindow = other.bHideConsoleWindow;
        this->bTryToFindDecoder = other.bTryToFindDecoder;
        this->bEnsureItemIsVisible = other.bEnsureItemIsVisible;
        this->bValidateInputFiles = other.bValidateInputFiles;
        this->bOverwriteExistingFiles = other.bOverwriteExistingFiles;
        this->nThreadCount = other.nThreadCount;
        this->szMainWindowResize = other.szMainWindowResize;
        this->szFileListColumns = other.szFileListColumns;
        this->szPresetsDialogResize = other.szPresetsDialogResize;
        this->szPresetsListColumns = other.szPresetsListColumns;
        this->szFormatsDialogResize = other.szFormatsDialogResize;
        this->szFormatsListColumns = other.szFormatsListColumns;
        this->szToolsDialogResize = other.szToolsDialogResize;
        this->szToolsListColumns = other.szToolsListColumns;
    }
public:
    void Defaults()
    {
        this->szSelectedLanguage = _T("en-US");
        this->nSelectedFormat = 14;
        this->szOutputPath = _T("$SourceDirectory$\\$Name$.$Ext$");
        this->bDeleteSourceFiles = false;
        this->bRecurseChecked = true;
        this->bShutdownWhenFinished = false;
        this->bDoNotSaveConfiguration = false;
        this->bDeleteOnErrors = true;
        this->bStopOnErrors = false;
        this->bHideConsoleWindow = true;
        this->bTryToFindDecoder = false;
        this->bEnsureItemIsVisible = true;
        this->bValidateInputFiles = true;
        this->bOverwriteExistingFiles = true;
        this->nThreadCount = 0;
        this->szMainWindowResize = _T("");
        this->szFileListColumns = _T("");
        this->szPresetsDialogResize = _T("");
        this->szPresetsListColumns = _T("");
        this->szFormatsDialogResize = _T("");
        this->szFormatsListColumns = _T("");
        this->szToolsDialogResize = _T("");
        this->szToolsListColumns = _T("");
    }
};
