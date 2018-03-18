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
        size_t nSelectedFormat;
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
        bool bRenameExistingFiles;
        int nRenameExistingFilesLimit;
        bool bTryToDownloadTools;
        int nThreadCount;
        std::wstring szOutputBrowse;
        std::wstring szDirectoryBrowse;
        std::wstring szMainWindowResize;
        std::wstring szFileListColumns;
        std::wstring szPathsDialogResize;
        std::wstring szPathsListColumns;
        std::wstring szPresetsDialogResize;
        std::wstring szPresetsListColumns;
        std::wstring szFormatsDialogResize;
        std::wstring szFormatsListColumns;
        std::wstring szToolsDialogResize;
        std::wstring szToolsListColumns;
    public:
        void Defaults()
        {
            this->szSelectedLanguage = L"en-US";
            this->nSelectedFormat = 16;
            this->szOutputPath = L"$InputPath$\\$Name$.$Ext$";
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
            this->bRenameExistingFiles = true;
            this->nRenameExistingFilesLimit = 100;
            this->bTryToDownloadTools = true;
            this->nThreadCount = 0;
            this->szOutputBrowse = L"";
            this->szDirectoryBrowse = L"";
            this->szMainWindowResize = L"";
            this->szFileListColumns = L"";
            this->szPathsDialogResize = L"";
            this->szPathsListColumns = L"";
            this->szPresetsDialogResize = L"";
            this->szPresetsListColumns = L"";
            this->szFormatsDialogResize = L"";
            this->szFormatsListColumns = L"";
            this->szToolsDialogResize = L"";
            this->szToolsListColumns = L"";
        }
    };
}
