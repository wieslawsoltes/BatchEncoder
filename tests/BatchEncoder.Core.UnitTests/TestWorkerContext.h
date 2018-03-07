// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "worker\WorkerContext.h"
#include "worker\Win32.h"

namespace BatchEncoderCoreUnitTests
{
    class TestOutputParser : public worker::IOutputParser
    {
    public:
        bool Open(worker::IWorkerContext* ctx, const std::wstring& szFunction)
        {
            return true;
        }
        bool Parse(worker::IWorkerContext* ctx, const char *szLine)
        {
            return true;
        }
    };

    class TestWorkerContext : public worker::IWorkerContext
    {
    public:
        TestWorkerContext()
        {
            this->bDone = true;
            this->bRunning = false;
            this->pConfig = nullptr;
            this->pFactory = std::make_shared<worker::Win32WorkerFactory>();
        }
        virtual ~TestWorkerContext() { }
    public:
        std::wstring GetString(int nKey)
        {
            return pConfig->GetString(nKey);
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
}
