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

class CWorker
{
public:
    CThread m_Thread;
public:
    CWorker() { }
    virtual ~CWorker() { }
public:
    bool ConvertFileUsingConsole(IWorkerContext* pWorkerContext, CCommandLine &commandLine);
    bool ConvertFileUsingPipes(IWorkerContext* pWorkerContext, CCommandLine &commandLine);
    bool ConvertFileUsingOnlyPipes(IWorkerContext* pWorkerContext, CCommandLine &decoderCommandLine, CCommandLine &encoderCommandLine);
    bool ConvertItem(IWorkerContext* pWorkerContext, CItem& item, CSynchronize &syncDir);
    bool ConvertLoop(IWorkerContext* pWorkerContext, std::queue<CItem> &queue, CSynchronize &sync, CSynchronize &syncDir);
    void Convert(IWorkerContext* pWorkerContext);
};
