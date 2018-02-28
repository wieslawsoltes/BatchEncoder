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
        volatile int nTotalFiles;
        volatile int nProcessedFiles;
        volatile int nErrors;
        volatile int nLastItemId;
    public:
        int nThreadCount;
        config::CConfiguration* pConfig;
    public:
        virtual ~IWorkerContext() { }
        virtual std::wstring GetString(int nKey) = 0;
        virtual bool IsRunning() = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void Next(int nItemId) = 0;
        virtual bool Progress(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
        virtual void Status(int nItemId, const std::wstring& szTime, const std::wstring& szStatus) = 0;
    };
}
