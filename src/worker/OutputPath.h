// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "..\utilities\Utilities.h"

// Output path:
// "" -> empty string is same as $SourceDirectory$
// $SourceDirectory$ -> same as $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$\$Name$.$Ext$
// $SourceDirectory$\$Name$_converted.$Ext$
// $SourceDirectory$\Converted\$Name$.$Ext$
// C:\Output -> same as C:\Output\Path\$Name$.$Ext$
// C:\Output\$Name$.$Ext$
// C:\Output\$Name$_converted.$Ext$

#define VAR_OUTPUT_SOURCE_DIRECTORY _T("$SourceDirectory$")
#define VAR_OUTPUT_NAME             _T("$Name$")
#define VAR_OUTPUT_EXTENSION        _T("$Ext$")

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
    bool Validate(CString szOutput)
    {
        this->bIsEmpty = szOutput.GetLength() <= 0;
        this->bHaveSourceDirectory = FindNoCase(szOutput, VAR_OUTPUT_SOURCE_DIRECTORY) >= 0;
        this->bHaveName = FindNoCase(szOutput, VAR_OUTPUT_NAME) >= 0;
        this->bHaveExt = FindNoCase(szOutput, VAR_OUTPUT_EXTENSION) >= 0;
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
    CString CreateFilePath(CString szOutput, CString szInputFile, CString szName, CString szExt)
    {
        if (this->bIsEmpty
            || ((this->bHaveSourceDirectory == false)
                && (this->bHaveName == false) && (this->bHaveExt == false)))
        {
            CString szOutPath;
            if (this->bIsEmpty)
            {
                CString szInputPath = ::GetFilePath(szInputFile);
                szOutPath = szInputPath;
            }
            else
            {
                szOutPath = CString(szOutput);
            }

            CString szOutputFile = szName + _T(".") + CString(szExt).MakeLower();
            if (szOutPath.GetLength() >= 1)
            {
                if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
                    szOutputFile = szOutPath + szOutputFile;
                else
                    szOutputFile = szOutPath + _T("\\") + szOutputFile;
            }
            return szOutputFile;
        }
        else
        {
            if ((this->bHaveSourceDirectory == true)
                && (this->bHaveName == true) && (this->bHaveExt == true))
            {
                CString szOutputFile = CString(szOutput);
                CString szInputPath = ::GetFilePath(szInputFile);
                szOutputFile = ReplaceNoCase(szOutputFile, VAR_OUTPUT_SOURCE_DIRECTORY, szInputPath);
                szOutputFile = ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);
                szOutputFile = ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXTENSION, CString(szExt).MakeLower());
                return szOutputFile;
            }
            else if ((this->bHaveSourceDirectory == true)
                && (this->bHaveName == false) && (this->bHaveExt == false))
            {
                CString szOutPath = CString(szOutput);
                CString szInputPath = ::GetFilePath(szInputFile);
                szOutPath = ReplaceNoCase(szOutPath, VAR_OUTPUT_SOURCE_DIRECTORY, szInputPath);

                CString szOutputFile = szName + _T(".") + CString(szExt).MakeLower();
                if (szOutPath.GetLength() >= 1)
                {
                    if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
                        szOutputFile = szOutPath + szOutputFile;
                    else
                        szOutputFile = szOutPath + _T("\\") + szOutputFile;
                }
                return szOutputFile;
            }
            else if ((this->bHaveSourceDirectory == false) && (this->bHaveName == true) && (this->bHaveExt == true))
            {
                CString szOutputFile = CString(szOutput);
                szOutputFile = ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);
                szOutputFile = ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXTENSION, CString(szExt).MakeLower());
                return szOutputFile;
            }
        }
        return NULL;
    }
    bool CreateOutputPath(CString szOutputFile)
    {
        CString szOutputPath = ::GetFilePath(szOutputFile);
        if (szOutputPath.GetLength() > 0)
        {
            if (!::DirectoryExists(szOutputPath))
            {
                if (::MakeFullPath(szOutputPath) == FALSE)
                    return false;
            }
        }
        return true;
    }
};
