// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\InputPath.h"
#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

namespace BatchEncoderCoreUnitTests
{
    class CNewOutputPath
    {
    public:
        std::wstring CreateFilePath(const std::wstring& szOutput, const std::wstring& szInputFile, const std::wstring& szName, const std::wstring& szExt)
        {
            worker::CInputPath m_Input(szInputFile.c_str());

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
                    wchar_t szValidPath[_MAX_PATH];
                    _wmakepath_s(szValidPath, nullptr, szOutputFile.c_str(), nullptr, nullptr);
                    szOutputFile = szValidPath;
                    szOutputFile += VAR_OUTPUT_RELATIVE;
                }
            }

            // replace custom name
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_NAME, szName);

            // replace custom ext
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_OUTPUT_EXT, util::StringHelper::TowLower(szExt));

            // replace input path parts
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_INPUT_DRIVE, m_Input.szInputDrive);
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_INPUT_DIR, m_Input.szInputDir);
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_INPUT_NAME, m_Input.szInputName);
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_INPUT_EXT, util::StringHelper::TowLower(m_Input.szInputExt));
            util::StringHelper::ReplaceNoCase(szOutputFile, VAR_INPUT_PATH, m_Input.szInputPath);

            // replace input path folders
            size_t nFoldersSize = m_Input.szSourceFolders.size();
            for (size_t i = nFoldersSize; i > 0; i--)
            {
                std::wstring& folder = m_Input.szSourceFolders[i - 1];
                std::wstring szVarIndex = std::to_wstring(nFoldersSize - i);
                std::wstring szVarName = VAR_INPUT_FOLDER;
                util::StringHelper::Replace(szVarName, VAR_INPUT_FOLDER_INDEX, szVarIndex);
                util::StringHelper::ReplaceNoCase(szOutputFile, szVarName, folder);
            }

            // fix path after replacements
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

    TEST_CLASS(CNewOutputPath_Tests)
    {
    public:
        TEST_METHOD(CNewOutputPath_CreateFile_Empty)
        {
            std::wstring szOutput = L"";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFile_Relative)
        {
            std::wstring szOutput = L"relative\\path";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            wchar_t szCurrentDirectory[_MAX_PATH];
            ::GetCurrentDirectory(_MAX_PATH, szCurrentDirectory);
            std::wstring szExpectedOutputFile = szCurrentDirectory;
            szExpectedOutputFile += L"\\" + szOutput + L"\\CustomName.ext";

            Assert::AreEqual(szExpectedOutputFile.c_str(), szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFile_NoVariables_NoSlash)
        {
            std::wstring szOutput = L"C:\\Output";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFile_NoVariables_Slash)
        {
            std::wstring szOutput = L"C:\\Output\\";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFile_InputPath_Only)
        {
            std::wstring szOutput = L"$InputPath$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_InputPath_InputName_Ext)
        {
            std::wstring szOutput = L"$InputPath$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_InputPath_InputName_Ext_NoCase)
        {
            std::wstring szOutput = L"$inputpath$\\$inputname$.$ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_InputPath_Name_Ext)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_InputName_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_InputExt)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$$InputExt$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.wav.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFile_InputExt_Ext_ToLower)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$$InputExt$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.WAV";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"EXT";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.wav.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_InputName_String_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputName$_converted.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\FileName_converted.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_Name_String_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$_converted.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName_converted.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_InputFolder1_InputFolder0_InputName_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());

        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_InputFolder1_InputFolder0_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_InputFolder0_InputName_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[0]$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath_Prefix_InputFolder0_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[0]$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }
    };
}
