// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include <cstring>
#include "utilities\OutputPath.h"
#include "utilities\Pipe.h"
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
        CThread m_Thread;
    public:
        CWorker() { }
        virtual ~CWorker() { }
    public:
        bool ConvertFileUsingConsole(IWorkerContext* pWorkerContext, CCommandLine &commandLine, CSynchronize &syncDown);
        bool ConvertFileUsingPipes(IWorkerContext* pWorkerContext, CCommandLine &commandLine, CSynchronize &syncDown);
        bool ConvertFileUsingOnlyPipes(IWorkerContext* pWorkerContext, CCommandLine &decoderCommandLine, CCommandLine &encoderCommandLine, CSynchronize &syncDown);
        bool ConvertItem(IWorkerContext* pWorkerContext, config::CItem& item, CSynchronize &syncDir, CSynchronize &syncDown);
        bool ConvertLoop(IWorkerContext* pWorkerContext, std::queue<config::CItem> &queue, CSynchronize &sync, CSynchronize &syncDir, CSynchronize &syncDown);
        void Convert(IWorkerContext* pWorkerContext);
    };
}
