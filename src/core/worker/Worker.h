// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include <utility>
#include <string>
#include <mutex>
#include <thread>
#include <atlstr.h>
#include "WorkerContext.h"
#include "CommandLine.h"

namespace worker
{
    class CWorker
    {
    public:
        bool ConvertFileUsingConsole(IWorkerContext* ctx, CCommandLine &cl, std::mutex &syncDown);
        bool ConvertFileUsingPipes(IWorkerContext* ctx, CCommandLine &cl, std::mutex &syncDown);
        bool ConvertFileUsingOnlyPipes(IWorkerContext* ctx, CCommandLine &dcl, CCommandLine &ecl, std::mutex &syncDown);
        bool ConvertItem(IWorkerContext* ctx, int nId, std::mutex &syncDir, std::mutex &syncDown);
        bool ConvertLoop(IWorkerContext* ctx, std::queue<int> &queue, std::mutex &sync, std::mutex &syncDir, std::mutex &syncDown);
        void Convert(IWorkerContext* ctx);
    };
}
