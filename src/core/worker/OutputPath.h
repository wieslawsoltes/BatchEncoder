// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "InputPath.h"
#include "utilities\FileSystem.h"
#include "utilities\String.h"

#define VAR_INPUT_DRIVE         L"$InputDrive$"
#define VAR_INPUT_DIR           L"$InputDir$"
#define VAR_INPUT_NAME          L"$InputName$"
#define VAR_INPUT_EXT           L"$InputExt$"
#define VAR_INPUT_PATH          L"$InputPath$"
#define VAR_INPUT_FOLDER_INDEX  L"index"
#define VAR_INPUT_FOLDER        L"$InputFolder[index]$"
#define VAR_OUTPUT_NAME         L"$Name$"
#define VAR_OUTPUT_EXT          L"$Ext$"
#define VAR_OUTPUT_FULL         L"$InputPath$\\$Name$.$Ext$"
#define VAR_OUTPUT_RELATIVE     L"$Name$.$Ext$"

namespace worker
{
    class COutputPath
    {
    public:
        std::wstring CreateFilePath(util::IFileSystem* fs, const std::wstring& szOutput, const std::wstring& szInputFile, const std::wstring& szName, const std::wstring& szExt)
        {
            CInputPath m_Input(szInputFile.c_str());

            // set output file pattern
            std::wstring szOutputFile = szOutput;

            if (szOutputFile.empty())
            {
                // empty string
                szOutputFile = VAR_OUTPUT_FULL;
            }
            else if (szOutputFile == VAR_INPUT_PATH)
            {
                // input path only
                szOutputFile += L"\\";
                szOutputFile += VAR_OUTPUT_RELATIVE;
            }
            else
            {
                // no variables are present and string is not empty e.g.: "C:\Output", "C:\Output\" or relative path
                if (szOutputFile.find('$') == std::wstring::npos)
                {
                    wchar_t szValidPath[_MAX_PATH]{ 0 };
                    _wmakepath_s(szValidPath, nullptr, szOutputFile.c_str(), nullptr, nullptr);
                    szOutputFile = szValidPath;
                    szOutputFile += VAR_OUTPUT_RELATIVE;
                }
            }

            // replace custom name
            util::string::ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);

            // replace custom ext
            util::string::ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXT, util::string::TowLower(szExt));

            // replace input path parts
            util::string::ReplaceNoCase(szOutputFile, VAR_INPUT_DRIVE, m_Input.szInputDrive);
            util::string::ReplaceNoCase(szOutputFile, VAR_INPUT_DIR, m_Input.szInputDir);
            util::string::ReplaceNoCase(szOutputFile, VAR_INPUT_NAME, m_Input.szInputName);
            util::string::ReplaceNoCase(szOutputFile, VAR_INPUT_EXT, util::string::TowLower(m_Input.szInputExt));
            util::string::ReplaceNoCase(szOutputFile, VAR_INPUT_PATH, m_Input.szInputPath);

            // replace input path folders
            size_t nFoldersSize = m_Input.szSourceFolders.size();
            for (size_t i = nFoldersSize; i > 0; i--)
            {
                std::wstring& folder = m_Input.szSourceFolders[i - 1];
                std::wstring szVarIndex = std::to_wstring(nFoldersSize - i);
                std::wstring szVarName = VAR_INPUT_FOLDER;
                util::string::Replace(szVarName, VAR_INPUT_FOLDER_INDEX, szVarIndex);
                util::string::ReplaceNoCase(szOutputFile, szVarName, folder);
            }

            // fix path after replacements
            util::string::ReplaceNoCase(szOutputFile, L"\\\\", L"\\");
            util::string::ReplaceNoCase(szOutputFile, L"//", L"/");

            // make valid full path
            wchar_t szValidOutputFile[_MAX_PATH];
            wchar_t *absPath = _wfullpath(szValidOutputFile, szOutputFile.c_str(), _MAX_PATH);
            if (absPath == nullptr)
                throw "Failed to create an absolute or full path name.";

            szOutputFile = szValidOutputFile;

            return szOutputFile;
        }
        bool CreateOutputPath(util::IFileSystem* fs, const std::wstring& szOutputFile)
        {
            std::wstring szOutputPath = fs->GetFilePath(szOutputFile);
            if (szOutputPath.length() > 0)
            {
                if (!fs->DirectoryExists(szOutputPath.c_str()))
                {
                    if (fs->MakeFullPath(szOutputPath) == false)
                        return false;
                }
            }
            return true;
        }
    };
}
