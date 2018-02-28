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
        TEST_METHOD(CWorker_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CWorker m_Worker;
            #pragma warning(pop)
        }
    };
}
