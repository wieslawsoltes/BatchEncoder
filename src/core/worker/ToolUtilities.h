// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <functional>
#include "utilities\Download.h"
#include "utilities\String.h"
#include "utilities\Utilities.h"
#include "config\Settings.h"
#include "config\Tool.h"
#include "config\Format.h"
#include "config\Config.h"

namespace worker
{
    class CToolUtilities
    {
    public:
        volatile bool bDownload;
    public:
        bool Download(config::CTool& tool, bool bExtract, bool bInstall, int nIndex, config::CConfig *config, std::function<bool(int, std::wstring)> callback = nullptr)
        {
            util::CDownload m_Download;
            std::wstring szUrl = tool.szUrl;
            std::wstring szFilePath = util::Utilities::CombinePath(config->m_Settings.szToolsPath, tool.szFile);
            std::wstring szFolderPath = util::Utilities::CombinePath(config->m_Settings.szToolsPath, util::Utilities::GetOnlyFileName(tool.szFile));

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
                    util::Utilities::LaunchAndWait(szFilePath, L"", TRUE);
                    return true;
                }
            }

            if (util::string::CompareNoCase(tool.szExtract, L"zip"))
            {
                if (bExtract == true)
                {
                    CComBSTR file(szFilePath.c_str());
                    CComBSTR folder(szFolderPath.c_str());

                    if (!util::Utilities::DirectoryExists(szFolderPath))
                    {
                        if (util::Utilities::MakeFullPath(szFolderPath) == false)
                        {
                            if (callback != nullptr)
                            {
                                std::wstring szStatus = config->GetString(0x00410001);
                                callback(nIndex, szStatus);
                            }
                            return false;
                        }
                    }

                    if (util::Utilities::DirectoryExists(szFolderPath))
                    {
                        bool bUnzipResult = util::Utilities::Unzip2Folder(file, folder);
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
        int FindTool(std::vector<config::CTool>& m_Tools, const std::wstring& szPlatform, const std::wstring& szFormatId)
        {
            int nTool = config::CTool::GetToolByFormatAndPlatform(m_Tools, szFormatId, szPlatform);
            if (nTool >= 0)
            {
                return nTool;
            }
            return -1;
        }
        int FindTool(std::vector<config::CTool>& m_Tools, const std::wstring& szFormatId)
        {
            const std::wstring szPlatformX86 = L"x86";
            const std::wstring szPlatformX64 = L"x64";
#if defined(_WIN32) & !defined(_WIN64)
            return FindTool(m_Tools, szPlatformX86, szFormatId);
#else
            int nTool = FindTool(m_Tools, szPlatformX64, szFormatId);
            if (nTool == -1)
                return FindTool(m_Tools, szPlatformX86, szFormatId);
            return nTool;
#endif
        }
        void SetFormatPaths(std::vector<config::CFormat>& m_Formats, std::vector<config::CTool>& m_Tools, const std::wstring& szPlatform)
        {
            size_t nFormats = m_Formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                config::CFormat& format = m_Formats[i];
                int nTool = config::CTool::GetToolByFormatAndPlatform(m_Tools, format.szId, szPlatform);
                if (nTool >= 0)
                {
                    config::CTool& tool = m_Tools[nTool];
                    format.szPath = tool.szPath;
                }
            }
        }
        void SetFormatPaths(std::vector<config::CFormat>& m_Formats, std::vector<config::CTool>& m_Tools, std::function<bool(int, config::CTool&)> filter)
        {
            size_t nTools = m_Tools.size();
            size_t nFormats = m_Formats.size();
            if ((nTools > 0) && (nFormats > 0))
            {
                for (size_t i = 0; i < nTools; i++)
                {
                    config::CTool& tool = m_Tools[i];
                    if (filter(i, tool) == true)
                    {
                        for (size_t i = 0; i < nFormats; i++)
                        {
                            config::CFormat& format = m_Formats[i];
                            if (tool.IsValidFormat(format.szId))
                            {
                                format.szPath = tool.szPath;
                            }
                        }
                    }
                }
            }
        }
    };
}
