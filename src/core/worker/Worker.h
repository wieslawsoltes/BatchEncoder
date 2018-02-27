// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include <utility>
#include <string>
#include <atlstr.h>
#include <thread>
#include "OutputPath.h"
#include "utilities\Pipe.h"
#include "utilities\StringHelper.h"
#include "utilities\Process.h"
#include "utilities\Synchronize.h"
#include "WorkerContext.h"
#include "CommandLine.h"
#include "OutputParser.h"

namespace worker
{
    class CWorker
    {
    public:
        bool ConvertFileUsingConsole(IWorkerContext* ctx, CCommandLine &cl, util::CSynchronize &syncDown);
        bool ConvertFileUsingPipes(IWorkerContext* ctx, CCommandLine &cl, util::CSynchronize &syncDown);
        bool ConvertFileUsingOnlyPipes(IWorkerContext* ctx, CCommandLine &dcl, CCommandLine &ecl, util::CSynchronize &syncDown);
        bool ConvertItem(IWorkerContext* ctx, int nId, util::CSynchronize &syncDir, util::CSynchronize &syncDown);
        bool ConvertLoop(IWorkerContext* ctx, std::queue<int> &queue, util::CSynchronize &sync, util::CSynchronize &syncDir, util::CSynchronize &syncDown);
        void Convert(IWorkerContext* ctx);
    };
}
