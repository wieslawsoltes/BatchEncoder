// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CInputPath_Tests)
    {
    public:
        TEST_METHOD(CInputPath_Constructor_FileName)
        {
            std::wstring szInputFile = L"FileName.wav";
            worker::CInputPath m_Source(szInputFile.c_str());

            Assert::AreEqual(L"FileName", m_Source.szInputName);
            Assert::AreEqual(L".wav", m_Source.szInputExt);
        }

        TEST_METHOD(CInputPath_Constructor_Drive_FileName)
        {
            std::wstring szInputFile = L"C:\\FileName.wav";
            worker::CInputPath m_Source(szInputFile.c_str());

            Assert::AreEqual(L"C:", m_Source.szInputDrive);
            Assert::AreEqual(L"\\", m_Source.szInputDir);
            Assert::AreEqual(L"FileName", m_Source.szInputName);
            Assert::AreEqual(L".wav", m_Source.szInputExt);
            Assert::AreEqual(L"C:\\", m_Source.szInputPath);
            Assert::AreEqual(size_t(0), m_Source.szSourceFolders.size());
        }

        TEST_METHOD(CInputPath_Constructor_Drive_OneFolder_FileName)
        {
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            worker::CInputPath m_Source(szInputFile.c_str());

            Assert::AreEqual(L"C:", m_Source.szInputDrive);
            Assert::AreEqual(L"\\MusicFolder\\", m_Source.szInputDir);
            Assert::AreEqual(L"FileName", m_Source.szInputName);
            Assert::AreEqual(L".wav", m_Source.szInputExt);
            Assert::AreEqual(L"C:\\MusicFolder\\", m_Source.szInputPath);
            Assert::AreEqual(size_t(1), m_Source.szSourceFolders.size());
            Assert::AreEqual(L"MusicFolder", m_Source.szSourceFolders[0].c_str());
        }

        TEST_METHOD(CInputPath_Constructor_Drive_ThreeFolders_FileName)
        {
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            worker::CInputPath m_Input(szInputFile.c_str());

            Assert::AreEqual(L"C:", m_Input.szInputDrive);
            Assert::AreEqual(L"\\MusicFolder\\ArtistFolder\\AlbumFolder\\", m_Input.szInputDir);
            Assert::AreEqual(L"FileName", m_Input.szInputName);
            Assert::AreEqual(L".wav", m_Input.szInputExt);
            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\", m_Input.szInputPath);
            Assert::AreEqual(size_t(3), m_Input.szSourceFolders.size());
            Assert::AreEqual(L"MusicFolder", m_Input.szSourceFolders[0].c_str());
            Assert::AreEqual(L"ArtistFolder", m_Input.szSourceFolders[1].c_str());
            Assert::AreEqual(L"AlbumFolder", m_Input.szSourceFolders[2].c_str());
        }

        TEST_METHOD(CInputPath_AppendInputName)
        {
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            worker::CInputPath m_Input(szInputFile.c_str());
            std::wstring szAppendedInputFile = m_Input.AppendInputName(L"_");

            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName_.wav", szAppendedInputFile.c_str());
        }
    };
}
