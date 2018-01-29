// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>

namespace config
{
    class COptions
    {
    public:
        std::wstring szSelectedLanguage;
        int nSelectedFormat;
        std::wstring szOutputPath;
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
        bool bTryToDownloadTools;
        int nThreadCount;
        std::wstring szMainWindowResize;
        std::wstring szFileListColumns;
        std::wstring szPresetsDialogResize;
        std::wstring szPresetsListColumns;
        std::wstring szFormatsDialogResize;
        std::wstring szFormatsListColumns;
        std::wstring szToolsDialogResize;
        std::wstring szToolsListColumns;
    public:
        void Defaults()
        {
            this->szSelectedLanguage = _T("en-US");
            this->nSelectedFormat = 16;
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
            this->bTryToDownloadTools = true;
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
}
