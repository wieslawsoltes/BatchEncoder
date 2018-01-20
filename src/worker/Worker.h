// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <queue>
#include <memory>
#include "utilities\OutputPath.h"
#include "utilities\Pipe.h"
#include "utilities\Thread.h"
#include "utilities\Process.h"
#include "utilities\Synchronize.h"
#include "WorkerContext.h"
#include "PipeContext.h"
#include "FileContext.h"

class IOutputParser
{
public:
    virtual bool Init(IWorkerContext* pWorkerContext, CFileContext* pFileContext) = 0;
    virtual bool Parse(const char *szLine) = 0;
};

class CWorker
{
public:
    CThread m_Thread;
public:
    CWorker() { }
    virtual ~CWorker() { }
public:
    bool OutputLoop(IWorkerContext* pWorkerContext, CFileContext &context, CPipe &Stderr, IOutputParser &parser);
    bool ReadLoop(IWorkerContext* pWorkerContext, CPipeContext &context);
    bool WriteLoop(IWorkerContext* pWorkerContext, CPipeContext &context);
    bool ConvertFileUsingConsole(IWorkerContext* pWorkerContext, CFileContext &context);
    bool ConvertFileUsingPipes(IWorkerContext* pWorkerContext, CFileContext &context);
    bool ConvertFileUsingOnlyPipes(IWorkerContext* pWorkerContext, CFileContext &decoderContext, CFileContext &encoderContext);
    bool ConvertItem(IWorkerContext* pWorkerContext, CItem& item, CSynchronize &syncDir);
    bool ConvertLoop(IWorkerContext* pWorkerContext, std::queue<CItem> &queue, CSynchronize &sync, CSynchronize &syncDir);
    void Convert(IWorkerContext* pWorkerContext);
};
