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
            //fwprintf(stdout, L"Output\t\t%s\n", szOutput.c_str());
            //fwprintf(stdout, L"InputFile\t\t\t%s\n", szInputFile.c_str());
            //fwprintf(stdout, L"Name\t\t%s\n", szName.c_str());
            //fwprintf(stdout, L"Ext\t\t%s\n", szExt.c_str());

            worker::CInputPath m_Input(szInputFile.c_str());

            //fwprintf(stdout, L"$InputPath$\t\t%s\n", m_Input.szInputPath);
            //fwprintf(stdout, L"$InputDrive$\t\t%s\n", m_Input.szInputDrive);
            //fwprintf(stdout, L"$InputDir$\t\t%s\n", m_Input.szInputDir);
            //fwprintf(stdout, L"$InputName$\t\t%s\n", m_Input.szInputName);
            //fwprintf(stdout, L"$InputExt$\t\t%s\n", m_Input.szInputExt);

            //fwprintf(stdout, L"\nInputFolders:\t\t%d\n", m_Input.szSourceFolders.size());
            size_t nFoldersSize = m_Input.szSourceFolders.size();
            //for (size_t i = nFoldersSize; i > 0; i--)
            //{
            //    std::wstring& folder = m_Input.szSourceFolders[i - 1];
            //    std::wstring szVarIndex = std::to_wstring(nFoldersSize - i);
            //    std::wstring szVarName = L"$SourceFolder$[" + szVarIndex + L"]";
            //    fwprintf(stdout, L"%s\t%s\n", szVarName.c_str(), folder.c_str());
            //}

            std::wstring szOutputFile = szOutput;

            util::StringHelper::ReplaceNoCase(szOutputFile, L"$Name$", szName);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$Ext$", szExt);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputDrive$", m_Input.szInputDrive);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputDir$", m_Input.szInputDir);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputName$", m_Input.szInputName);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputExt$", m_Input.szInputExt);
            util::StringHelper::ReplaceNoCase(szOutputFile, L"$InputPath$", m_Input.szInputPath);

            // $InputFolder[index]$
            for (size_t i = nFoldersSize; i > 0; i--)
            {
                std::wstring& folder = m_Input.szSourceFolders[i - 1];
                std::wstring szVarIndex = std::to_wstring(nFoldersSize - i);
                std::wstring szVarName = L"$InputFolder[" + szVarIndex + L"]$";
                //fwprintf(stdout, L"%s\t%s\n", szVarName.c_str(), folder.c_str());
                util::StringHelper::ReplaceNoCase(szOutputFile, szVarName, folder);
            }

            // fix path after replacements
            util::StringHelper::ReplaceNoCase(szOutputFile, L"\\\\", L"\\");
            util::StringHelper::ReplaceNoCase(szOutputFile, L"//", L"/");
            //fwprintf(stdout, L"\nOutputFile\t\t%s\n", szOutputFile.c_str());

            return szOutputFile;
        }
    };

    TEST_CLASS(CNewOutputPath_Tests)
    {
    public:
        TEST_METHOD(CNewOutputPath_CreateFilePath0)
        {
            std::wstring szOutput = L"C:\\Output\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath1)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath2)
        {
            std::wstring szOutput = L"$InputPath$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath3)
        {
            std::wstring szOutput = L"$InputPath$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath4)
        {
            std::wstring szOutput = L"C:\\Output\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath5)
        {
            std::wstring szOutput = L"C:\\Output\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath6)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\ArtistFolder\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath7)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[1]$\\$InputFolder[0]$\\$Name$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\ArtistFolder\\AlbumFolder\\CustomName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath8)
        {
            std::wstring szOutput = L"C:\\Output\\$InputFolder[0]$\\$InputName$.$Ext$";
            std::wstring szInputFile = L"C:\\MusicFolder\\ArtistFolder\\AlbumFolder\\FileName.wav";
            std::wstring szName = L"CustomName";
            std::wstring szExt = L"ext";

            CNewOutputPath m_Output;
            std::wstring szOutputFile = m_Output.CreateFilePath(szOutput, szInputFile, szName, szExt);
            Assert::AreEqual(L"C:\\Output\\AlbumFolder\\FileName.ext", szOutputFile.c_str());
        }

        TEST_METHOD(CNewOutputPath_CreateFilePath9)
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
