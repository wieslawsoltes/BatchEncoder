// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <functional>
#include "utilities\Download.h"
#include "utilities\StringHelper.h"
#include "utilities\Thread.h"
#include "utilities\Utilities.h"
#include "configuration\Settings.h"
#include "configuration\ToolsList.h"
#include "configuration\FormatsList.h"
#include "configuration\Configuration.h"

namespace worker
{
    class CToolUtilities
    {
    public:
        volatile bool bDownload;
    public:
        CToolUtilities() { }
        virtual ~CToolUtilities() { }
    public:
        bool Download(config::CTool& tool, bool bExtract, bool bInstall, int nIndex, config::CConfiguration *pConfig, std::function<void(int, std::wstring)> callback = nullptr)
        {
            util::CDownload m_Download;
            std::wstring szUrl = tool.szUrl;
            std::wstring szFilePath = util::Utilities::CombinePath(config::m_Settings.szToolsPath, tool.szFile);
            std::wstring szFolderPath = util::Utilities::CombinePath(config::m_Settings.szToolsPath, util::Utilities::GetOnlyFileName(tool.szFile));

            bool bResult = m_Download.Download(szUrl, szFilePath,
                [nIndex, pConfig, callback](int nProgress, std::wstring szStatus)
            {
                if (callback != nullptr)
                {
                    for (int s = 0; s < 8; s++)
                    {
                        auto str = config::m_Strings.at(0x00400001 + s);
                        auto pos = szStatus.find(str);
                        if (pos != std::string::npos)
                        {
                            std::wstring szTranslation = pConfig->GetString(0x00400001 + s);
                            szStatus.replace(pos, str.length(), szTranslation);
                        }
                    }
                    callback(nIndex, szStatus);
                }
            });

            if (bResult == false)
            {
                return false;
            }

            if (util::StringHelper::CompareNoCase(tool.szExtract, L"install"))
            {
                if (bInstall == true)
                {
                    util::Utilities::LaunchAndWait(szFilePath, L"", TRUE);
                    return true;
                }
            }

            if (util::StringHelper::CompareNoCase(tool.szExtract, L"zip"))
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
                                std::wstring szStatus = pConfig->GetString(0x00410001);
                                callback(nIndex, szStatus);
                            }
                            return false;
                        }
                    }

                    if (util::Utilities::DirectoryExists(szFolderPath) == true)
                    {
                        bool bUnzipResult = util::Utilities::Unzip2Folder(file, folder);
                        if (bUnzipResult == true)
                        {
                            if (callback != nullptr)
                            {
                                std::wstring szStatus = pConfig->GetString(0x00410002);
                                callback(nIndex, szStatus);
                            }
                            return true;
                        }
                        else
                        {
                            if (callback != nullptr)
                            {
                                std::wstring szStatus = pConfig->GetString(0x00410003);
                                callback(nIndex, szStatus);
                            }
                            return false;
                        }
                    }
                }
            }

            return true;
        }
        int FindTool(config::CToolsList& m_Tools, const std::wstring& szPlatform, const std::wstring& szFormatId)
        {
            int nTool = m_Tools.GetToolByFormatAndPlatform(szFormatId, szPlatform);
            if (nTool >= 0)
            {
                return nTool;
            }
            return -1;
        }
        int FindTool(config::CToolsList& m_Tools, const std::wstring& szFormatId)
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
        void SetFormatPaths(config::CFormatsList& m_Formats, config::CToolsList& m_Tools, const std::wstring& szPlatform)
        {
            int nFormats = m_Formats.Count();
            for (int i = 0; i < nFormats; i++)
            {
                config::CFormat& format = m_Formats.Get(i);
                int nTool = m_Tools.GetToolByFormatAndPlatform(format.szId, szPlatform);
                if (nTool >= 0)
                {
                    config::CTool& tool = m_Tools.Get(nTool);
                    format.szPath = tool.szPath;
                }
            }
        }
        void SetFormatPaths(config::CFormatsList& m_Formats, config::CToolsList& m_Tools, std::function<bool(int, config::CTool&)> filter)
        {
            int nTools = m_Tools.Count();
            int nFormats = m_Formats.Count();
            if ((nTools > 0) && (nFormats > 0))
            {
                for (int i = 0; i < nTools; i++)
                {
                    config::CTool& tool = m_Tools.Get(i);
                    if (filter(i, tool) == true)
                    {
                        for (int i = 0; i < nFormats; i++)
                        {
                            config::CFormat& format = m_Formats.Get(i);
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
