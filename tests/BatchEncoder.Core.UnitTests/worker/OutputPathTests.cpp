// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(COutputPath_Tests)
    {
        TestFileSystem FileSystem;
    public:
        TEST_METHOD(COutputPath_CreateFile_Empty)
        {
            std::wstring szOutput = L"";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_Relative_NoSlash)
        {
            std::wstring szOutput = L"relative\\path";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            std::wstring szCurrentDirectory = FileSystem.GetCurrentDirectory_();
            std::wstring szExpectedOutputFile = szCurrentDirectory + L"\\" + szOutput + L"\\CustomName.ext";

            Assert::AreEqual(szExpectedOutputFile.c_str(), szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_Relative_Slash)
        {
            std::wstring szOutput = L"relative\\path\\";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            std::wstring szCurrentDirectory = FileSystem.GetCurrentDirectory_();
            std::wstring szExpectedOutputFile = szCurrentDirectory + L"\\" + szOutput + L"CustomName.ext";

            Assert::AreEqual(szExpectedOutputFile.c_str(), szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_NoVariables_NoSlash)
        {
            std::wstring szOutput = L"C:\\Output";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_NoVariables_Slash)
        {
            std::wstring szOutput = L"C:\\Output\\";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_InputPath_Only)
        {
            std::wstring szOutput = L"$InputPath$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputPath_InputName_Ext)
        {
            std::wstring szOutput = L"$InputPath$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputPath_InputName_Ext_NoCase)
        {
            std::wstring szOutput = L"$inputpath$\\$inputname$.$ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputPath_Name_Ext)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputName_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputExt)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$$InputExt$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.wav.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputExt_Ext_ToLower)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$$InputExt$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.WAV";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"EXT";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.wav.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputName_String_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputName$_converted.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\FileName_converted.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_Name_String_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$_converted.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName_converted.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputFolder1_InputFolder0_InputName_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());

        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputFolder1_InputFolder0_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputFolder0_InputName_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[0]$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputFolder0_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[0]$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputDrive_Name_Ext)
        {
            std::wstring szOutput = L"$InputDrive$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputDrive_InputName_Ext)
        {
            std::wstring szOutput = L"$InputDrive$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputDrive_InputDir_String_Name_Ext)
        {
            std::wstring szOutput = L"$InputDrive$\\$InputDir$\\Converted\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\Converted\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputDrive_InputDir_String_InputName_Ext)
        {
            std::wstring szOutput = L"$InputDrive$\\$InputDir$\\Converted\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\Converted\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputDrive_String_Name_Ext)
        {
            std::wstring szOutput = L"$InputDrive$\\Converted\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Converted\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_InputDrive_String_InputName_Ext)
        {
            std::wstring szOutput = L"$InputDrive$\\Converted\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Converted\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputDir_Name_Ext)
        {
            std::wstring szOutput = L"D:\\$InputDir$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"D:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_InputDir_InputName_Ext)
        {
            std::wstring szOutput = L"D:\\$InputDir$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"D:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_String_InputDir_Name_Ext)
        {
            std::wstring szOutput = L"D:\\Converted\\$InputDir$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"D:\\Converted\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFilePath_Prefix_String_InputDir_InputName_Ext)
        {
            std::wstring szOutput = L"D:\\Converted\\$InputDir$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(&FileSystem, szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"D:\\Converted\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }
    };
}
