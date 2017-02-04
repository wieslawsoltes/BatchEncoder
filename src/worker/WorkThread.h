// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "WorkerContext.h"
#include "PipeContext.h"
#include "FileContext.h"
#include "ItemContext.h"
#include "ProcessContext.h"

enum Mode { None = -1, Encode = 0, Transcode = 1 };

typedef int GetProgressFunc(char *szLineBuff, int nLineLen);

typedef bool ConvertFileFunc(CFileContext *pContext);

bool ConvertItem(CItemContext* pContext, ConvertFileFunc *pConvertFile);

DWORD WINAPI WorkThread(LPVOID lpParam);
