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
    void Download(CTool& tool, bool bExtract, bool bInstall, int nIndex, CConfiguration *pConfig, std::function<void(int, CString)> callback)
    {
        CDownload m_Download;
        CString szUrl = tool.szUrl;
        CString szFilePath = m_App.CombinePath(m_App.szToolsPath, tool.szFile);
        CString szFolderPath = m_App.CombinePath(m_App.szToolsPath, ::GetOnlyFileName(tool.szFile));

        m_Download.Download(szUrl, szFilePath,
            [nIndex, pConfig, callback](int nProgress, CString szStatus)
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
        });

        if (tool.szExtract.CompareNoCase(_T("install")) == 0)
        {
            if (bInstall == true)
            {
                ::LaunchAndWait(szFilePath, _T(""), TRUE);
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
                        CString szStatus = pConfig->m_Language.GetString(0x00410001, pszExtractStatus[0]);
                        callback(nIndex, szStatus);
                    }
                }

                if (::DirectoryExists(szFolderPath) == TRUE)
                {
                    bool bUnzipResult = ::Unzip2Folder(file, folder);
                    if (bUnzipResult == true)
                    {
                        CString szStatus = pConfig->m_Language.GetString(0x00410002, pszExtractStatus[1]);
                        callback(nIndex, szStatus);
                    }
                    else
                    {
                        CString szStatus = pConfig->m_Language.GetString(0x00410003, pszExtractStatus[2]);
                        callback(nIndex, szStatus);
                    }
                }
            }
        }
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
