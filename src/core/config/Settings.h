// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\Utilities.h"

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
        bool IsPortable()
        {
            std::wstring szPath = util::Utilities::GetExeFilePath() + this->szPortableFile;
            return util::Utilities::PathFileExists(szPath);
        }
    public:
        void InitPortableSettings()
        {
            this->szSettingsPath = util::Utilities::GetExeFilePath();
            this->szFormatsPath = util::Utilities::CombinePath(this->szSettingsPath, this->szFormatsDir);
            this->szLanguagesPath = util::Utilities::CombinePath(this->szSettingsPath, this->szLanguagesDir);
            this->szProgressPath = util::Utilities::CombinePath(this->szSettingsPath, this->szProgressDir);
            this->szToolsPath = util::Utilities::CombinePath(this->szSettingsPath, this->szToolsDir);

            try
            {
                util::Utilities::CreateDirectory(this->szFormatsPath);
                util::Utilities::CreateDirectory(this->szLanguagesPath);
                util::Utilities::CreateDirectory(this->szProgressPath);
                util::Utilities::CreateDirectory(this->szToolsPath);
            }
            catch (...) {}

            this->szLogFile = util::Utilities::CombinePath(this->szSettingsPath, this->szLogFileName);
            this->szOptionsFile = util::Utilities::CombinePath(this->szSettingsPath, this->szOptionsFileName);
            this->szItemsFile = util::Utilities::CombinePath(this->szSettingsPath, this->szItemsFileName);
            this->szOutputsFile = util::Utilities::CombinePath(this->szSettingsPath, this->szOutputsFileName);
        }
        void InitUserSettings()
        {
            this->szSettingsPath = util::Utilities::GetSettingsFilePath(L"", this->szConfigDir);
            this->szFormatsPath = util::Utilities::GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szFormatsDir);
            this->szLanguagesPath = util::Utilities::GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szLanguagesDir);
            this->szProgressPath = util::Utilities::GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szProgressDir);
            this->szToolsPath = util::Utilities::GetSettingsFilePath(L"", this->szConfigDir + L"\\" + this->szToolsDir);

            try
            {
                util::Utilities::CreateDirectory(szSettingsPath);
                util::Utilities::CreateDirectory(szFormatsPath);
                util::Utilities::CreateDirectory(szLanguagesPath);
                util::Utilities::CreateDirectory(szProgressPath);
                util::Utilities::CreateDirectory(szToolsPath);
            }
            catch (...) {}

            this->szLogFile = util::Utilities::GetSettingsFilePath(this->szLogFileName, this->szConfigDir);
            this->szOptionsFile = util::Utilities::GetSettingsFilePath(this->szOptionsFileName, this->szConfigDir);
            this->szItemsFile = util::Utilities::GetSettingsFilePath(this->szItemsFileName, this->szConfigDir);
            this->szOutputsFile = util::Utilities::GetSettingsFilePath(this->szOutputsFileName, this->szConfigDir);
        }
    public:
        void Init()
        {
            if (this->IsPortable())
                this->InitPortableSettings();
            else
                this->InitUserSettings();
        }
    };
}
