// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CConsoleConverter_Tests)
    {
    public:
        TEST_METHOD(CConsoleConverter_Run)
        {
        }
    };

    TEST_CLASS(CPipesConverter_Tests)
    {
    public:
        TEST_METHOD(CPipesConverter_Run)
        {
        }
    };

    TEST_CLASS(CPipesTranscoder_Tests)
    {
    public:
        TEST_METHOD(CPipesTranscoder_Run)
        {
        }
    };

    TEST_CLASS(CWorker_Tests)
    {
        config::CFormat m_Format
        {
            L"TEST_ID",
            L"Name",
            config::FormatType::Encoder,
            0,
            L"WAV",
            L"MP3",
            L"$EXE $OPTIONS $INFILE $OUTFILE",
            false,
            false,
            L"script.lua",
            L"program.exe",
            0,
            0,
            {
                { L"Default1", L"--option 1" },
                { L"Default2", L"--option 2" }
            }
        };
        config::CItem m_Item
        {
            0,
            L"File",
            L"WAV",
            L"TEST_ID",
            0,
            L"",
            true,
            1024,
            {
                { L"C:\\Output\\File.WAV" }
            },
            L"", L"", false, 0, 0
        };
    public:
        TEST_METHOD(CWorker_Transcode)
        {
        }

        TEST_METHOD(CWorker_Decode)
        {
        }

        TEST_METHOD(CWorker_Encode)
        {
        }

        TEST_METHOD(CWorker_Convert_item_mutex)
        {
        }

        TEST_METHOD(CWorker_Convert_vector)
        {
        }

        TEST_METHOD(CWorker_Convert_queue)
        {
        }

        TEST_METHOD(CWorker_Convert_item)
        {
        }

        TEST_METHOD(CWorker_Convert_items_Empty)
        {
            config::CConfig m_Config;

            m_Config.FileSystem = std::make_unique<TestFileSystem>();

            m_Config.Log = std::make_unique<util::MemoryLog>();
            m_Config.Log->Open();

            TestWorkerContext ctx;

            ctx.Init();
            ctx.nThreadCount = 1;
            ctx.pConfig = &m_Config;

            Assert::AreEqual(1, ctx.nThreadCount);
            Assert::IsNotNull(ctx.pConfig);

            auto pWorker = std::make_unique<worker::CWorker>();
            pWorker->ConsoleConverter = std::make_unique<worker::CConsoleConverter>();
            pWorker->PipesConverter = std::make_unique<worker::CPipesConverter>();
            pWorker->PipesTranscoder = std::make_unique<worker::CPipesTranscoder>();
            pWorker->Convert(&ctx, m_Config.m_Items);
            m_Config.Log->Close();

            Assert::IsFalse(ctx.bRunning);
            Assert::IsTrue(ctx.bDone);
            Assert::IsTrue(ctx.nTotalFiles == 0);
            Assert::IsTrue(ctx.nProcessedFiles == 0);
            Assert::IsTrue(ctx.nErrors == 0);
            Assert::IsTrue(ctx.nLastItemId == -1);

            Assert::AreEqual(1, ctx.nThreadCount);
            Assert::IsNull(ctx.pConfig);
        }

        TEST_METHOD(CWorker_Convert_items_One_Error)
        {
            config::CConfig m_Config;

            config::CFormat format = m_Format;
            m_Config.m_Formats.emplace_back(format);

            config::CItem item = m_Item;
            m_Config.m_Items.emplace_back(item);

            m_Config.FileSystem = std::make_unique<TestFileSystem>();

            m_Config.Log = std::make_unique<util::MemoryLog>();
            m_Config.Log->Open();

            TestWorkerContext ctx;

            ctx.Init();
            ctx.nThreadCount = 1;
            ctx.pConfig = &m_Config;

            Assert::AreEqual(1, ctx.nThreadCount);
            Assert::IsNotNull(ctx.pConfig);

            auto pWorker = std::make_unique<worker::CWorker>();
            pWorker->ConsoleConverter = std::make_unique<worker::CConsoleConverter>();
            pWorker->PipesConverter = std::make_unique<worker::CPipesConverter>();
            pWorker->PipesTranscoder = std::make_unique<worker::CPipesTranscoder>();
            pWorker->Convert(&ctx, m_Config.m_Items);
            m_Config.Log->Close();

            Assert::IsFalse(ctx.bRunning);
            Assert::IsTrue(ctx.bDone);
            Assert::IsTrue(ctx.nTotalFiles == 1);
            Assert::IsTrue(ctx.nProcessedFiles == 1);
            Assert::IsTrue(ctx.nErrors == 1);
            Assert::IsTrue(ctx.nLastItemId == 0);

            Assert::AreEqual(1, ctx.nThreadCount);
            Assert::IsNull(ctx.pConfig);
        }
    };
}
