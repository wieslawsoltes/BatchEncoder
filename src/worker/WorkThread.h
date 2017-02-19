// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "WorkerContext.h"
#include "PipeContext.h"
#include "FileContext.h"
#include "ItemContext.h"
#include "Pipe.h"
#include "Thread.h"
#include "Process.h"

bool ConvertItem(CItemContext* pContext);

int WorkThread(void *param);
