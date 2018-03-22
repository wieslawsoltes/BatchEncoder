// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(TestDownloader_Tests)
    {
    public:
        TEST_METHOD(TestDownloader_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestDownloader m_Downloader;
            #pragma warning(pop)
        }

        TEST_METHOD(TestDownloader_Download)
        {
        }
    };

    TEST_CLASS(TestProcess_Tests)
    {
    public:
        TEST_METHOD(TestProcess_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestProcess m_Process;
            #pragma warning(pop)
        }

        TEST_METHOD(TestProcess_ConnectStdInput)
        {
        }

        TEST_METHOD(TestProcess_ConnectStdOutput)
        {
        }

        TEST_METHOD(TestProcess_ConnectStdError)
        {
        }

        TEST_METHOD(TestProcess_Start)
        {
        }

        TEST_METHOD(TestProcess_Wait)
        {
        }

        TEST_METHOD(TestProcess_Wait_timeout)
        {
        }

        TEST_METHOD(TestProcess_Terminate)
        {
        }

        TEST_METHOD(TestProcess_Close)
        {
        }

        TEST_METHOD(TestProcess_Stop)
        {
        }

        TEST_METHOD(TestProcess_StdinHandle)
        {
        }

        TEST_METHOD(TestProcess_StdoutHandle)
        {
        }

        TEST_METHOD(TestProcess_StderrHandle)
        {
        }
    };

    TEST_CLASS(TestPipe_Tests)
    {
    public:
        TEST_METHOD(TestPipe_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestPipe m_Pipe;
            #pragma warning(pop)
        }

        TEST_METHOD(TestPipe_Create)
        {
        }

        TEST_METHOD(TestPipe_CloseRead)
        {
        }

        TEST_METHOD(TestPipe_CloseWrite)
        {
        }

        TEST_METHOD(TestPipe_InheritRead)
        {
        }

        TEST_METHOD(TestPipe_InheritWrite)
        {
        }

        TEST_METHOD(TestPipe_DuplicateRead)
        {
        }

        TEST_METHOD(TestPipe_DuplicateWrite)
        {
        }

        TEST_METHOD(TestPipe_ReadHandle)
        {
        }

        TEST_METHOD(TestPipe_WriteHandle)
        {
        }
    };

    TEST_CLASS(TestPipeToFileWriter_Tests)
    {
    public:
        TEST_METHOD(TestPipeToFileWriter_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestPipeToFileWriter m_Writer;
            #pragma warning(pop)
        }

        TEST_METHOD(TestPipeToFileWriter_WriteLoop)
        {
        }
    };

    TEST_CLASS(TestFileToPipeReader_Tests)
    {
    public:
        TEST_METHOD(TestFileToPipeReader_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestFileToPipeReader m_Reader;
            #pragma warning(pop)
        }

        TEST_METHOD(TestFileToPipeReader_ReadLoop)
        {
        }
    };

    TEST_CLASS(TestOutputParser_Tests)
    {
    public:
        TEST_METHOD(TestOutputParser_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestOutputParser m_Parser;
            #pragma warning(pop)
        }

        TEST_METHOD(TestOutputParser_Open)
        {
        }

        TEST_METHOD(TestOutputParser_Parse)
        {
        }
    };

    TEST_CLASS(TestPipeToStringWriter_Tests)
    {
    public:
        TEST_METHOD(TestPipeToStringWriter_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestPipeToStringWriter m_Writer;
            #pragma warning(pop)
        }

        TEST_METHOD(TestPipeToStringWriter_WriteLoop)
        {
        }
    };

    TEST_CLASS(TestFileSystem_Tests)
    {
    public:
        TEST_METHOD(TestFileSystem_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestFileSystem m_FileSystem;
            #pragma warning(pop)
        }

        TEST_METHOD(TestFileSystem_GenerateUuidString)
        {
        }

        TEST_METHOD(TestFileSystem_CombinePath)
        {
        }

        TEST_METHOD(TestFileSystem_GetFileName)
        {
        }

        TEST_METHOD(TestFileSystem_GetFilePath)
        {
        }

        TEST_METHOD(TestFileSystem_GetFileExtension)
        {
        }

        TEST_METHOD(TestFileSystem_GetOnlyFileName)
        {
        }

        TEST_METHOD(TestFileSystem_GetFileSize64_handle)
        {
        }

        TEST_METHOD(TestFileSystem_GetFileSize64_filename)
        {
        }

        TEST_METHOD(TestFileSystem_GetFileSizeInt64)
        {
        }

        TEST_METHOD(TestFileSystem_GetExeFilePath)
        {
        }

        TEST_METHOD(TestFileSystem_GetSettingsFilePath)
        {
        }

        TEST_METHOD(TestFileSystem_GetFullPathName_)
        {
        }

        TEST_METHOD(TestFileSystem_FileExists)
        {
        }

        TEST_METHOD(TestFileSystem_PathFileExists_)
        {
        }

        TEST_METHOD(TestFileSystem_DeleteFile_)
        {
        }

        TEST_METHOD(TestFileSystem_CreateDirectory_)
        {
        }

        TEST_METHOD(TestFileSystem_GetCurrentDirectory_)
        {
        }

        TEST_METHOD(TestFileSystem_SetCurrentDirectory_)
        {
        }

        TEST_METHOD(TestFileSystem_DirectoryExists)
        {
        }

        TEST_METHOD(TestFileSystem_MakeFullPath)
        {
        }

        TEST_METHOD(TestFileSystem_FindFiles_pattern)
        {
        }

        TEST_METHOD(TestFileSystem_FindFiles_path)
        {
        }
    };

    TEST_CLASS(TestWorkerFactory_Tests)
    {
    public:
        TEST_METHOD(TestWorkerFactory_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            TestWorkerFactory m_Factory;
            #pragma warning(pop)
        }

        TEST_METHOD(TestWorkerFactory_CreateDownloaderPtr)
        {
        }

        TEST_METHOD(TestWorkerFactory_CreateProcessPtr)
        {
        }

        TEST_METHOD(TestWorkerFactory_CreatePipePtr)
        {
        }

        TEST_METHOD(TestWorkerFactory_CreateFileReaderPtr)
        {
        }

        TEST_METHOD(TestWorkerFactory_CreateFileWriterPtr)
        {
        }

        TEST_METHOD(TestWorkerFactory_CreateOutputParserPtr)
        {
        }

        TEST_METHOD(TestWorkerFactory_CreateStringWriterPtr)
        {
        }
    };

    TEST_CLASS(TestWorkerContext_Tests)
    {
    public:
        TEST_METHOD(TestWorkerContext_Constructor)
        {
            TestWorkerContext ctx;

            Assert::IsTrue(ctx.bDone);
            Assert::IsFalse(ctx.bRunning);
            Assert::IsNull(ctx.pConfig);
        }

        TEST_METHOD(TestWorkerContext_GetString)
        {
        }

        TEST_METHOD(TestWorkerContext_Init)
        {
            TestWorkerContext ctx;
            ctx.Init();

            Assert::IsTrue(ctx.bRunning);
            Assert::IsFalse(ctx.bDone);
            Assert::IsTrue(ctx.nTotalFiles == 0);
            Assert::IsTrue(ctx.nProcessedFiles == 0);
            Assert::IsTrue(ctx.nErrors == 0);
            Assert::IsTrue(ctx.nLastItemId == -1);
        }

        TEST_METHOD(TestWorkerContext_Start)
        {
        }

        TEST_METHOD(TestWorkerContext_Stop)
        {
        }

        TEST_METHOD(TestWorkerContext_ItemProgress)
        {
        }

        TEST_METHOD(TestWorkerContext_ItemStatus)
        {
        }

        TEST_METHOD(TestWorkerContext_TotalProgress)
        {
        }
    };
}
