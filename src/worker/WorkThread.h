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

bool ConvertItem(CItemContext* pContext);

int WorkThread(void *param);
