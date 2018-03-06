// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <utility>
#include "config\Config.h"

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
        config::CConfig* pConfig;
    public:
        virtual ~IWorkerContext() { }
        virtual std::wstring GetString(int nKey) = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual bool ItemProgress(int nItemId, int nProgress, bool bFinished, bool bError = false) = 0;
        virtual void ItemStatus(int nItemId, const std::wstring& szTime, const std::wstring& szStatus) = 0;
        virtual void TotalProgress(int nItemId) = 0;
    };
}
