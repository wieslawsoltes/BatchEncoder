// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "..\Configuration.h"
#include "context\WorkerContext.h"
#include "context\PipeContext.h"
#include "context\FileContext.h"
#include "context\ItemContext.h"

DWORD WINAPI ReadThread(LPVOID lpParam);

DWORD WINAPI WriteThread(LPVOID lpParam);

bool ConvertFile(CFileContext* pContext);

bool ConvertItem(CItemContext* pContext);

DWORD WINAPI ConvertThread(LPVOID lpParam);

DWORD WINAPI WorkThread(LPVOID lpParam);
