// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"

#define VAR_OUTPUT_SOURCE_DIRECTORY L"$SourceDirectory$"
#define VAR_OUTPUT_NAME             L"$Name$"
#define VAR_OUTPUT_EXTENSION        L"$Ext$"

// Output path:
// Empty string is same as the $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$ is equal to $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$\$Name$_converted.$Ext$
// $SourceDirectory$\Converted\$Name$.$Ext$
// C:\Output (is equal to C:\Output\Path\$Name$.$Ext$)
// C:\Output\$Name$.$Ext$
// C:\Output\$Name$_converted.$Ext$

namespace worker
{
    class COutputPath
    {
    public:
        std::wstring CreateFilePath(const std::wstring& szOutput, const std::wstring& szInputFile, const std::wstring& szName, const std::wstring& szExt)
        {
            std::wstring szPattern = szOutput;
            if (szPattern.length() <= 0)
            {
                szPattern = L"$SourceDirectory$\\$Name$.$Ext$";
            }
            else
            {
                bool bHaveName = util::StringHelper::FindNoCase(szOutput, VAR_OUTPUT_NAME) != std::wstring::npos;
                bool bHaveExt = util::StringHelper::FindNoCase(szOutput, VAR_OUTPUT_EXTENSION) != std::wstring::npos;
                if ((bHaveName == false) && (bHaveExt == false))
                {
                    if (szPattern.length() >= 1 && szPattern[szPattern.length() - 1] != '\\' && szPattern[szPattern.length() - 1] != '/')
                        szPattern += L"\\";
                    szPattern += L"$Name$.$Ext$";
                }
            }
            std::wstring szOutputFile = szPattern;
            std::wstring szInputPath = util::Utilities::GetFilePath(szInputFile);
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_SOURCE_DIRECTORY, szInputPath);
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXTENSION, util::StringHelper::TowLower(szExt));

            util::StringHelper::ReplaceNoCase(szOutputFile, L"\\\\", L"\\");
            util::StringHelper::ReplaceNoCase(szOutputFile, L"//", L"/");

            return szOutputFile;
        }
        bool CreateOutputPath(const std::wstring& szOutputFile)
        {
            std::wstring szOutputPath = util::Utilities::GetFilePath(szOutputFile);
            if (szOutputPath.length() > 0)
            {
                if (!util::Utilities::DirectoryExists(szOutputPath.c_str()))
                {
                    if (util::Utilities::MakeFullPath(szOutputPath) == false)
                        return false;
                }
            }
            return true;
        }
    };
}
