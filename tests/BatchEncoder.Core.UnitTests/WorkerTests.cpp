// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\Worker.h"
#include "worker\WorkerContext.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    class CTestWorkerContext : public worker::IWorkerContext
    {
    public:
        CTestWorkerContext()
        {
            this->bDone = true;
            this->bRunning = false;
            this->pConfig = nullptr;
        }
        virtual ~CTestWorkerContext() { }
    public:
        std::wstring GetString(int nKey)
        {
            return pConfig->GetString(nKey);
        }
        bool IsRunning()
        {
            return this->bRunning;
        }
        void Start()
        {
        }
        void Stop()
        {
            this->pConfig = nullptr;
            this->bRunning = false;
        }
        bool ItemProgress(int nItemId, int nProgress, bool bFinished, bool bError = false)
        {
            if (bError == true)
            {
                if (this->pConfig->m_Options.bStopOnErrors == true)
                {
                    this->bRunning = false;
                }
                return this->bRunning;
            }

            if (bFinished == true)
            {
            }

            if ((bFinished == false) && (this->bRunning == true))
            {
            }

            return this->bRunning;
        }
        void ItemStatus(int nItemId, const std::wstring& szTime, const std::wstring& szStatus)
        {
        }
        void TotalProgress(int nItemId)
        {
            if (nItemId > this->nLastItemId)
            {
                this->nLastItemId = nItemId;
            }
        }
    };

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
