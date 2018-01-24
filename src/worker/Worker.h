// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include <string>
#include "OutputPath.h"
#include "utilities\Pipe.h"
#include "utilities\StringHelper.h"
#include "utilities\Thread.h"
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
        CWorker() { }
        virtual ~CWorker() { }
    public:
        bool ConvertFileUsingConsole(IWorkerContext* pWorkerContext, CCommandLine &commandLine, util::CSynchronize &syncDown);
        bool ConvertFileUsingPipes(IWorkerContext* pWorkerContext, CCommandLine &commandLine, util::CSynchronize &syncDown);
        bool ConvertFileUsingOnlyPipes(IWorkerContext* pWorkerContext, CCommandLine &decoderCommandLine, CCommandLine &encoderCommandLine, util::CSynchronize &syncDown);
        bool ConvertItem(IWorkerContext* pWorkerContext, config::CItem& item, util::CSynchronize &syncDir, util::CSynchronize &syncDown);
        bool ConvertLoop(IWorkerContext* pWorkerContext, std::queue<config::CItem> &queue, util::CSynchronize &sync, util::CSynchronize &syncDir, util::CSynchronize &syncDown);
        void Convert(IWorkerContext* pWorkerContext);
    };
}
