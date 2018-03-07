// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <functional>
#include "utilities\Download.h"
#include "utilities\String.h"
#include "config\Settings.h"
#include "config\Tool.h"
#include "config\Config.h"

namespace worker
{
    class IToolDownloader
    {
    public:
        volatile bool bDownload;
    public:
        virtual ~IToolDownloader() { }
        virtual bool Download(config::CConfig *config, config::CTool& tool, bool bExtract, bool bInstall, int nIndex, std::function<bool(int, std::wstring)> callback = nullptr) = 0;
    };

    class CToolDownloader : public IToolDownloader
    {
    public:
        bool Download(config::CConfig *config, config::CTool& tool, bool bExtract, bool bInstall, int nIndex, std::function<bool(int, std::wstring)> callback = nullptr)
        {
            util::CDownload m_Download;
            std::wstring szUrl = tool.szUrl;
            std::wstring szFilePath = util::CombinePath(config->m_Settings.szToolsPath, tool.szFile);
            std::wstring szFolderPath = util::CombinePath(config->m_Settings.szToolsPath, util::GetOnlyFileName(tool.szFile));

            auto statusCallback = [&](int nProgress, std::wstring szStatus)
            {
                if (callback != nullptr)
                {
                    for (int s = 0; s < 8; s++)
                    {
                        auto str = config::m_Strings.at(0x00400001 + s);
                        auto pos = szStatus.find(str);
                        if (pos != std::string::npos)
                        {
                            std::wstring szTranslation = config->GetString(0x00400001 + s);
                            szStatus.replace(pos, str.length(), szTranslation);
                        }
                    }
                    return callback(nIndex, szStatus);
                }
                return false; // Do not abort.
            };

            if (m_Download.Download(szUrl, szFilePath, statusCallback) == false)
            {
                return false;
            }

            if (util::string::CompareNoCase(tool.szExtract, L"install"))
            {
                if (bInstall == true)
                {
                    util::LaunchAndWait(szFilePath, L"", TRUE);
                    return true;
                }
            }

            if (util::string::CompareNoCase(tool.szExtract, L"zip"))
            {
                if (bExtract == true)
                {
                    CComBSTR file(szFilePath.c_str());
                    CComBSTR folder(szFolderPath.c_str());

                    if (!util::DirectoryExists(szFolderPath))
                    {
                        if (util::MakeFullPath(szFolderPath) == false)
                        {
                            if (callback != nullptr)
                            {
                                std::wstring szStatus = config->GetString(0x00410001);
                                callback(nIndex, szStatus);
                            }
                            return false;
                        }
                    }

                    if (util::DirectoryExists(szFolderPath))
                    {
                        bool bUnzipResult = util::Unzip2Folder(file, folder);
                        if (bUnzipResult == true)
                        {
                            if (callback != nullptr)
                            {
                                std::wstring szStatus = config->GetString(0x00410002);
                                callback(nIndex, szStatus);
                            }
                            return true;
                        }
                        else
                        {
                            if (callback != nullptr)
                            {
                                std::wstring szStatus = config->GetString(0x00410003);
                                callback(nIndex, szStatus);
                            }
                            return false;
                        }
                    }
                }
            }

            return true;
        }
    };
}
