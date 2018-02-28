// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <utility>
#include "config\Configuration.h"

namespace worker
{
    class IWorkerContext
    {
    public:
        volatile bool bRunning;
        volatile bool bDone;
        volatile int nThreadCount;
        volatile int nTotalFiles;
        volatile int nProcessedFiles;
        volatile int nErrors;
        volatile int nLastItemId;
    public:
        config::CConfiguration* pConfig;
    public:
        IWorkerContext(config::CConfiguration* pConfig)
            : pConfig(pConfig) { }
        virtual ~IWorkerContext() { }
    public:
        virtual std::wstring GetString(int nKey) = 0;
    public:
        virtual void Init() = 0;
        virtual void Next(int nItemId) = 0;
        virtual void Done() = 0;
        virtual bool IsRunning() = 0;
        virtual bool Callback(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
        virtual void Status(int nItemId, const std::wstring& szTime, const std::wstring& szStatus) = 0;
    };
}
