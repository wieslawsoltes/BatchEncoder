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
        void Init(worker::IWorkerContext& ctx)
        {
            ctx.bRunning = true;
            ctx.bDone = false;
            ctx.nTotalFiles = 0;
            ctx.nProcessedFiles = 0;
            ctx.nErrors = 0;
            ctx.nLastItemId = -1;
        }
    public:
        TEST_METHOD(CWorker_Convert_Items_Empty)
        {
            config::CConfiguration m_Config;
            CTestWorkerContext ctx;

            Init(ctx);
            ctx.nThreadCount = 1;
            ctx.pConfig = &m_Config;

            Assert::AreEqual(1, ctx.nThreadCount);
            Assert::IsNotNull(ctx.pConfig);

            worker::CWorker m_Worker;
            m_Worker.Convert(&ctx, m_Config.m_Items);

            Assert::IsFalse(ctx.bRunning);
            Assert::IsTrue(ctx.bDone);
            Assert::IsTrue(ctx.nTotalFiles == 0);
            Assert::IsTrue(ctx.nProcessedFiles == 0);
            Assert::IsTrue(ctx.nErrors == 0);
            Assert::IsTrue(ctx.nLastItemId == -1);

            Assert::AreEqual(1, ctx.nThreadCount);
            Assert::IsNull(ctx.pConfig);
        }
    };
}
