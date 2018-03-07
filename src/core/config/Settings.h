// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\FileSystem.h"

namespace config
{
    class CSettings
    {
    public:
        std::wstring szPortableFile;
    public:
        std::wstring szConfigDir;
        std::wstring szFormatsDir;
        std::wstring szLanguagesDir;
        std::wstring szProgressDir;
        std::wstring szToolsDir;
    public:
        std::wstring szLogFileName;
        std::wstring szOptionsFileName;
        std::wstring szItemsFileName;
        std::wstring szOutputsFileName;
    public:
        std::wstring szSettingsPath;
        std::wstring szFormatsPath;
        std::wstring szLanguagesPath;
        std::wstring szProgressPath;
        std::wstring szToolsPath;
    public:
        std::wstring szLogFile;
        std::wstring szOptionsFile;
        std::wstring szItemsFile;
        std::wstring szOutputsFile;
    public:
        CSettings()
        {
            this->szPortableFile = L"BatchEncoder.portable";
            this->szConfigDir = L"BatchEncoder";
            this->szFormatsDir = L"formats";
            this->szLanguagesDir = L"lang";
            this->szProgressDir = L"progress";
            this->szToolsDir = L"tools";
            this->szLogFileName = L"BatchEncoder.log";
            this->szOptionsFileName = L"Options.xml";
            this->szItemsFileName = L"Items.xml";
            this->szOutputsFileName = L"Outputs.xml";
        }
    public:
        bool IsPortable(util::IFileSystem* fs)
        {
            std::wstring szPath = fs->GetExeFilePath() + this->szPortableFile;
            return fs->PathFileExists_(szPath);
        }
    public:
        void InitPortableSettings(util::IFileSystem* fs)
        {
            this->szSettingsPath = fs->GetExeFilePath();
            this->szFormatsPath = fs->CombinePath(this->szSettingsPath, this->szFormatsDir);
            this->szLanguagesPath = fs->CombinePath(this->szSettingsPath, this->szLanguagesDir);
            this->szProgressPath = fs->CombinePath(this->szSettingsPath, this->szProgressDir);
            this->szToolsPath = fs->CombinePath(this->szSettingsPath, this->szToolsDir);

            try
            {
                fs->CreateDirectory_(this->szFormatsPath);
                fs->CreateDirectory_(this->szLanguagesPath);
                fs->CreateDirectory_(this->szProgressPath);
                fs->CreateDirectory_(this->szToolsPath);
            }
            catch (...) {}

            this->szLogFile = fs->CombinePath(this->szSettingsPath, this->szLogFileName);
            this->szOptionsFile = fs->CombinePath(this->szSettingsPath, this->szOptionsFileName);
            this->szItemsFile = fs->CombinePath(this->szSettingsPath, this->szItemsFileName);
            this->szOutputsFile = fs->CombinePath(this->szSettingsPath, this->szOutputsFileName);
        }
        void InitUserSettings(util::IFileSystem* fs)
        {
            this->szSettingsPath = fs->GetSettingsFilePath(L"", this->szConfigDir);
            this->szFormatsPath = fs->GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szFormatsDir);
            this->szLanguagesPath = fs->GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szLanguagesDir);
            this->szProgressPath = fs->GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szProgressDir);
            this->szToolsPath = fs->GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szToolsDir);

            try
            {
                fs->CreateDirectory_(szSettingsPath);
                fs->CreateDirectory_(szFormatsPath);
                fs->CreateDirectory_(szLanguagesPath);
                fs->CreateDirectory_(szProgressPath);
                fs->CreateDirectory_(szToolsPath);
            }
            catch (...) {}

            this->szLogFile = fs->GetSettingsFilePath(this->szLogFileName, this->szConfigDir);
            this->szOptionsFile = fs->GetSettingsFilePath(this->szOptionsFileName, this->szConfigDir);
            this->szItemsFile = fs->GetSettingsFilePath(this->szItemsFileName, this->szConfigDir);
            this->szOutputsFile = fs->GetSettingsFilePath(this->szOutputsFileName, this->szConfigDir);
        }
    public:
        void Init(util::IFileSystem* fs)
        {
            if (this->IsPortable(fs))
                this->InitPortableSettings(fs);
            else
                this->InitUserSettings(fs);
        }
    };
}
