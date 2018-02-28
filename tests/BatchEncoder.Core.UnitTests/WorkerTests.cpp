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
        CTestWorkerContext(config::CConfiguration* pConfig)
        {
            this->pConfig = pConfig;
            this->bDone = true;
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
        }
        void Next(int nItemId)
        {
        }
        bool Progress(int nItemId, int nProgress, bool bFinished, bool bError = false)
        {
            return this->bRunning;
        }
        void Status(int nItemId, const std::wstring& szTime, const std::wstring& szStatus)
        {
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
