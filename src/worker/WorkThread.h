// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "utilities\OutputPath.h"
#include "utilities\Pipe.h"
#include "utilities\Thread.h"
#include "utilities\Process.h"
#include "WorkerContext.h"
#include "PipeContext.h"
#include "FileContext.h"
#include "ItemContext.h"

bool ProgresssLoop(CFileContext* pContext, CPipe &Stderr, int &nProgress);

bool ReadLoop(CPipeContext* pContext);

bool WriteLoop(CPipeContext* pContext);

bool ConvertFileUsingConsole(CFileContext* pContext);

bool ConvertFileUsingPipes(CFileContext* pContext);

bool ConvertFileUsingOnlyPipes(CFileContext* pDecoderContext, CFileContext* pEncoderContext);

bool ConvertItem(CItemContext* pContext);

bool ConvertLoop(CWorkerContext* pWorkerContext);

void Convert(CWorkerContext* pWorkerContext);
