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

class CWorker
{
public:
    CWorker() { }
    virtual ~CWorker() { }
public:
    bool ProgresssLoop(CWorkerContext* pWorkerContext, CFileContext* pContext, CPipe &Stderr, int &nProgress);
    bool ReadLoop(CWorkerContext* pWorkerContext, CPipeContext* pContext);
    bool WriteLoop(CWorkerContext* pWorkerContext, CPipeContext* pContext);
    bool ConvertFileUsingConsole(CWorkerContext* pWorkerContext, CFileContext* pContext);
    bool ConvertFileUsingPipes(CWorkerContext* pWorkerContext, CFileContext* pContext);
    bool ConvertFileUsingOnlyPipes(CWorkerContext* pWorkerContext, CFileContext* pDecoderContext, CFileContext* pEncoderContext);
    bool ConvertItem(CWorkerContext* pWorkerContext, CItem& item, CSynchronize &syncDir);
    bool ConvertLoop(CWorkerContext* pWorkerContext, std::queue<CItem> &queue, CSynchronize &sync, CSynchronize &syncDir);
    void Convert(CWorkerContext* pWorkerContext);
};
