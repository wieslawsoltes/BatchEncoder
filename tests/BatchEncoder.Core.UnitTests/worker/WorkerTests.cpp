// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\Worker.h"
#include "worker\WorkerContext.h"
#include "TestWorkerContext.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CWorker_Tests)
    {
    public:
        TEST_METHOD(CWorker_Convert_Nothing)
        {
            config::CConfiguration m_Config;

            CTestWorkerContext ctx;
            Assert::IsTrue(ctx.bDone);
            Assert::IsNull(ctx.pConfig);

            ctx.bRunning = true;
            ctx.bDone = false;
            ctx.nTotalFiles = 0;
            ctx.nProcessedFiles = 0;
            ctx.nErrors = 0;
            ctx.nLastItemId = -1;
            ctx.nThreadCount = m_Config.m_Options.nThreadCount;
            ctx.pConfig = &m_Config;

            Assert::AreEqual(0, ctx.nThreadCount);
            Assert::IsNotNull(ctx.pConfig);

            worker::CWorker m_Worker;
            m_Worker.Convert(&ctx);

            Assert::IsFalse(ctx.bRunning);
            Assert::IsTrue(ctx.bDone);
            Assert::IsTrue(ctx.nTotalFiles == 0);
            Assert::IsTrue(ctx.nProcessedFiles == 0);
            Assert::IsTrue(ctx.nErrors == 0);
            Assert::IsTrue(ctx.nLastItemId == -1);
            Assert::AreEqual(0, ctx.nThreadCount);
            Assert::IsNull(ctx.pConfig);
        }
    };
}
