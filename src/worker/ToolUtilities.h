// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <functional>
#include "utilities\Download.h"
#include "utilities\Thread.h"
#include "configuration\ToolsList.h"
#include "configuration\FormatsList.h"
#include "configuration\Configuration.h"

class CToolUtilities
{
public:
    volatile bool bDownload;
public:
    CToolUtilities() { }
    virtual ~CToolUtilities() { }
public:
    bool Download(CTool& tool, bool bExtract, bool bInstall, int nIndex, CConfiguration *pConfig, std::function<void(int, CString)> callback = nullptr)
    {
        CDownload m_Download;
        CString szUrl = tool.szUrl;
        CString szFilePath = m_App.CombinePath(m_App.szToolsPath, tool.szFile);
        CString szFolderPath = m_App.CombinePath(m_App.szToolsPath, ::GetOnlyFileName(tool.szFile));

        bool bResult = m_Download.Download(szUrl, szFilePath,
            [nIndex, pConfig, callback](int nProgress, CString szStatus)
        {
            if (callback != nullptr)
            {
                for (int s = 0; s < 8; s++)
                {
                    if (szStatus.Find(pszDownloadStatus[s]) >= 0)
                    {
                        CString szTranslation = pConfig->m_Language.GetString(0x00400001 + s, pszDownloadStatus[s]);
                        szStatus.Replace(pszDownloadStatus[s], szTranslation);
                    }
                }
                callback(nIndex, szStatus);
            }
        });

        if (bResult == false)
        {
            return false;
        }

        if (tool.szExtract.CompareNoCase(_T("install")) == 0)
        {
            if (bInstall == true)
            {
                ::LaunchAndWait(szFilePath, _T(""), TRUE);
                return true;
            }
        }

        if (tool.szExtract.CompareNoCase(_T("zip")) == 0)
        {
            if (bExtract == true)
            {
                CComBSTR file(szFilePath);
                CComBSTR folder(szFolderPath);

                if (!::DirectoryExists(szFolderPath))
                {
                    if (::MakeFullPath(szFolderPath) == false)
                    {
                        if (callback != nullptr)
                        {
                            CString szStatus = pConfig->m_Language.GetString(0x00410001, pszExtractStatus[0]);
                            callback(nIndex, szStatus);
                        }
                        return false;
                    }
                }

                if (::DirectoryExists(szFolderPath) == TRUE)
                {
                    bool bUnzipResult = ::Unzip2Folder(file, folder);
                    if (bUnzipResult == true)
                    {
                        if (callback != nullptr)
                        {
                            CString szStatus = pConfig->m_Language.GetString(0x00410002, pszExtractStatus[1]);
                            callback(nIndex, szStatus);
                        }
                        return true;
                    }
                    else
                    {
                        if (callback != nullptr)
                        {
                            CString szStatus = pConfig->m_Language.GetString(0x00410003, pszExtractStatus[2]);
                            callback(nIndex, szStatus);
                        }
                        return false;
                    }
                }
            }
        }

        return true;
    }
    int FindTool(CToolsList& m_Tools, CString szPlatform, CString szFormatId)
    {
        int nTool = m_Tools.GetToolByFormatAndPlatform(szFormatId, szPlatform);
        if (nTool >= 0)
        {
            return nTool;
        }
        return -1;
    }
    int FindTool(CToolsList& m_Tools, CString szFormatId)
    {
#if defined(_WIN32) & !defined(_WIN64)
        CString szPlatform = _T("x86");
#else
        CString szPlatform = _T("x64");
#endif
        return FindTool(m_Tools, szPlatform, szFormatId);
    }
    void SetFormatPaths(CFormatsList& m_Formats, CToolsList& m_Tools, CString szPlatform)
    {
        int nFormats = m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = m_Formats.Get(i);
            int nTool = m_Tools.GetToolByFormatAndPlatform(format.szId, szPlatform);
            if (nTool >= 0)
            {
                CTool& tool = m_Tools.Get(nTool);
                format.szPath = tool.szPath;
            }
        }
    }
    void SetFormatPaths(CFormatsList& m_Formats, CToolsList& m_Tools, std::function<bool(int, CTool&)> filter)
    {
        int nTools = m_Tools.Count();
        int nFormats = m_Formats.Count();
        if ((nTools > 0) && (nFormats > 0))
        {
            for (int i = 0; i < nTools; i++)
            {
                CTool& tool = m_Tools.Get(i);
                if (filter(i, tool) == true)
                {
                    for (int i = 0; i < nFormats; i++)
                    {
                        CFormat& format = m_Formats.Get(i);
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
