#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\InputPath.h"
#include "utilities\StringHelper.h"

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
            Assert::AreEqual(L"FileName.wav", m_Source.szInputFile);
            Assert::AreEqual(L"FileName", m_Source.szSourceName);
            Assert::AreEqual(L".wav", m_Source.szSourceExt);
        }

        TEST_METHOD(CInputPath_Constructor_Drive_FileName)
        {
            std::wstring szInputFile = L"C:\\FileName.wav";
            worker::CInputPath m_Source(szInputFile.c_str());
            Assert::AreEqual(L"C:\\FileName.wav", m_Source.szInputFile);
            Assert::AreEqual(L"C:\\FileName.wav", m_Source.szInputFilePath);
            Assert::AreEqual(L"C:", m_Source.szSourceDrive);
            Assert::AreEqual(L"\\", m_Source.szSourceDir);
            Assert::AreEqual(L"FileName", m_Source.szSourceName);
            Assert::AreEqual(L".wav", m_Source.szSourceExt);
            Assert::AreEqual(L"C:\\", m_Source.szSourcePath);
            Assert::AreEqual(size_t(0), m_Source.szSourceFolders.size());
        }

        TEST_METHOD(CInputPath_Constructor_Drive_OneFolder_FileName)
        {
            std::wstring szInputFile = L"C:\\MusicFolder\\FileName.wav";
            worker::CInputPath m_Source(szInputFile.c_str());
            Assert::AreEqual(L"C:\\MusicFolder\\FileName.wav", m_Source.szInputFile);
            Assert::AreEqual(L"C:\\MusicFolder\\FileName.wav", m_Source.szInputFilePath);
            Assert::AreEqual(L"C:", m_Source.szSourceDrive);
            Assert::AreEqual(L"\\MusicFolder\\", m_Source.szSourceDir);
            Assert::AreEqual(L"FileName", m_Source.szSourceName);
            Assert::AreEqual(L".wav", m_Source.szSourceExt);
            Assert::AreEqual(L"C:\\MusicFolder\\", m_Source.szSourcePath);
            Assert::AreEqual(size_t(1), m_Source.szSourceFolders.size());
            Assert::AreEqual(L"MusicFolder", m_Source.szSourceFolders[0].c_str());
        }

        TEST_METHOD(CInputPath_Constructor_Drive_ThreeFolders_FileName)
        {
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            worker::CInputPath m_Source(szInputFile.c_str());
            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav", m_Source.szInputFile);
            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav", m_Source.szInputFilePath);
            Assert::AreEqual(L"C:", m_Source.szSourceDrive);
            Assert::AreEqual(L"\\MusicFolder\\ArtistFolder\\AlbumFolder\\", m_Source.szSourceDir);
            Assert::AreEqual(L"FileName", m_Source.szSourceName);
            Assert::AreEqual(L".wav", m_Source.szSourceExt);
            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\", m_Source.szSourcePath);
            Assert::AreEqual(size_t(3), m_Source.szSourceFolders.size());
            Assert::AreEqual(L"MusicFolder", m_Source.szSourceFolders[0].c_str());
            Assert::AreEqual(L"ArtistFolder", m_Source.szSourceFolders[1].c_str());
            Assert::AreEqual(L"AlbumFolder", m_Source.szSourceFolders[2].c_str());
        }
    };
}
