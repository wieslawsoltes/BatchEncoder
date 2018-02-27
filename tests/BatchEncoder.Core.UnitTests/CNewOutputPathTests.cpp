// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\InputPath.h"
#include "utilities\StringHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
                szOutputFile = L"$InputPath$\\$Name$.$Ext$";
            }
            else if (szOutputFile == L"$InputPath$")
            {
                // input path only
                szOutputFile += L"\\$Name$.$Ext$";
            }
            else
            {
                // no variables are present and string is not empty e.g.: "C:\Output" or "C:\Output\"
                if (szOutputFile.find('$') == std::wstring::npos)
                {
                    wchar_t szValidPath[_MAX_PATH];
                    _wmakepath_s(szValidPath, nullptr, szOutputFile.c_str(), nullptr, nullptr);
                    szOutputFile = szValidPath;
                    szOutputFile += L"$Name$.$Ext$";
                }
            }

            // replace custom name
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$Name$", szName);

            // replace custom ext
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$Ext$", util::StringHelper::TowLower(szExt));

            // replace input path parts
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputDrive$", m_Input.szInputDrive);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputDir$", m_Input.szInputDir);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputName$", m_Input.szInputName);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputExt$", util::StringHelper::TowLower(m_Input.szInputExt));
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputPath$", m_Input.szInputPath);

            // replace input path folders $InputFolder[index]$
            size_t nFoldersSize = m_Input.szSourceFolders.size();
            for (size_t i = nFoldersSize; i > 0; i--)
            {
                std::wstring& folder = m_Input.szSourceFolders[i - 1];
                std::wstring szVarIndex = std::to_wstring(nFoldersSize - i);
                std::wstring szVarName = L"$InputFolder[" + szVarIndex + L"]$";
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
