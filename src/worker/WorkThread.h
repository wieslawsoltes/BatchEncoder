// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "..\Configuration.h"
#include "WorkerContext.h"
#include "PipeContext.h"
#include "FileContext.h"
#include "ItemContext.h"
#include "ProcessContext.h"

bool ConvertFile(CFileContext* pContext);

bool ConvertItem(CItemContext* pContext);

DWORD WINAPI WorkThread(LPVOID lpParam);
