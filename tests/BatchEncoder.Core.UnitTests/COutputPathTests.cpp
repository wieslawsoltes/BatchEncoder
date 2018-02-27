// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\OutputPath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(COutputPath_Tests)
    {
    public:
        TEST_METHOD(COutputPath_CreateFile_Empty)
        {
            std::wstring szOutput = L"";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_SourceDirectory)
        {
            std::wstring szOutput = L"$SourceDirectory$";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_SourceDirectory_Name_Ext)
        {
            std::wstring szOutput = L"$SourceDirectory$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_SourceDirectory_Name_String_Ext)
        {
            std::wstring szOutput = L"$SourceDirectory$\\$Name$_converted.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\CustomName_converted.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_SourceDirectory_String_Name_Ext)
        {
            std::wstring szOutput = L"$SourceDirectory$\\Converted\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\MusicFolder\\Converted\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_Path_NoSlash)
        {
            std::wstring szOutput = L"C:\\Output";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_Path_Slash)
        {
            std::wstring szOutput = L"C:\\Output\\";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_Path_Name_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(COutputPath_CreateFile_Path_Name_String_Ext)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$_converted.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            worker::COutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);

            Assert::AreEqual(L"C:\\Output\\CustomName_converted.ext", szOutputFile.c_str());
        }
    };
}
