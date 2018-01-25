// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"

// Output path:
// "" -> empty string is same as $SourceDirectory$
// $SourceDirectory$ -> same as $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$\$Name$_converted.$Ext$
// $SourceDirectory$\Converted\$Name$.$Ext$
// C:\Output -> same as C:\Output\Path\$Name$.$Ext$
// C:\Output\$Name$.$Ext$
// C:\Output\$Name$_converted.$Ext$

#define VAR_OUTPUT_SOURCE_DIRECTORY L"$SourceDirectory$"
#define VAR_OUTPUT_NAME             L"$Name$"
#define VAR_OUTPUT_EXTENSION        L"$Ext$"

namespace worker
{
    class COutputPath
    {
    public:
        bool bIsEmpty;
        bool bHaveSourceDirectory;
        bool bHaveName;
        bool bHaveExt;
    public:
        COutputPath()
        {
        }
        virtual ~COutputPath()
        {
        }
    public:
        bool Validate(std::wstring& szOutput)
        {
            this->bIsEmpty = szOutput.length() <= 0;
            this->bHaveSourceDirectory = util::StringHelper::FindNoCase(szOutput, VAR_OUTPUT_SOURCE_DIRECTORY) >= 0;
            this->bHaveName = util::StringHelper::FindNoCase(szOutput, VAR_OUTPUT_NAME) >= 0;
            this->bHaveExt = util::StringHelper::FindNoCase(szOutput, VAR_OUTPUT_EXTENSION) >= 0;
            if (this->bIsEmpty)
            {
                // Only input file directory is used (name and extension are added automatically).
                return true;
            }
            else
            {
                if ((this->bHaveSourceDirectory == true)
                    && (this->bHaveName == true) && (this->bHaveExt == true))
                {
                    // Input source directory variable is used with name and extension variables.
                    return true;
                }
                else if ((this->bHaveSourceDirectory == true)
                    && (this->bHaveName == false) && (this->bHaveExt == false))
                {
                    // Only input source directory variable is used (name and extension are added automatically).
                    return true;
                }
                else if ((this->bHaveSourceDirectory == false)
                    && (this->bHaveName == true) && (this->bHaveExt == true))
                {
                    // Only use name and extension variable (full output path must be provided by user).
                    return true;
                }
                else if ((this->bHaveSourceDirectory == false)
                    && (this->bHaveName == false) && (this->bHaveExt == false))
                {
                    // Full output path must be provided by user (ignore input file path and variables).
                    return true;
                }
            }
            return false;
        }
        std::wstring CreateFilePath(const std::wstring& szOutput, const std::wstring& szInputFile, const std::wstring& szName, const std::wstring& szExt)
        {
            if (this->bIsEmpty
                || ((this->bHaveSourceDirectory == false)
                    && (this->bHaveName == false) && (this->bHaveExt == false)))
            {
                std::wstring szOutPath;
                if (this->bIsEmpty)
                {
                    std::wstring szInputPath = util::Utilities::GetFilePath(szInputFile);
                    szOutPath = szInputPath;
                }
                else
                {
                    szOutPath = std::wstring(szOutput);
                }

                std::wstring szOutputFile = szName + L"." + util::StringHelper::TowLower(szExt);
                if (szOutPath.length() >= 1)
                {
                    if (szOutPath[szOutPath.length() - 1] == '\\' || szOutPath[szOutPath.length() - 1] == '/')
                        szOutputFile = szOutPath + szOutputFile;
                    else
                        szOutputFile = szOutPath + L"\\" + szOutputFile;
                }
                return szOutputFile;
            }
            else
            {
                if ((this->bHaveSourceDirectory == true)
                    && (this->bHaveName == true) && (this->bHaveExt == true))
                {
                    std::wstring szOutputFile = szOutput;
                    std::wstring szInputPath = util::Utilities::GetFilePath(szInputFile);
                    
                    util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_SOURCE_DIRECTORY, szInputPath);
                    util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);
                    util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXTENSION, util::StringHelper::TowLower(szExt));
                    return szOutputFile;
                }
                else if ((this->bHaveSourceDirectory == true)
                    && (this->bHaveName == false) && (this->bHaveExt == false))
                {
                    std::wstring szOutPath = szOutput;
                    std::wstring szInputPath = util::Utilities::GetFilePath(szInputFile);
                    util::StringHelper::ReplaceNoCase(szOutPath, VAR_OUTPUT_SOURCE_DIRECTORY, szInputPath);

                    std::wstring szOutputFile = szName + L"." + util::StringHelper::TowLower(szExt);
                    if (szOutPath.length() >= 1)
                    {
                        if (szOutPath[szOutPath.length() - 1] == '\\' || szOutPath[szOutPath.length() - 1] == '/')
                            szOutputFile = szOutPath + szOutputFile;
                        else
                            szOutputFile = szOutPath + L"\\" + szOutputFile;
                    }
                    return szOutputFile;
                }
                else if ((this->bHaveSourceDirectory == false) && (this->bHaveName == true) && (this->bHaveExt == true))
                {
                    std::wstring szOutputFile = szOutput;
                    util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);
                    util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXTENSION, util::StringHelper::TowLower(szExt));
                    return szOutputFile;
                }
            }
            return L"";
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
