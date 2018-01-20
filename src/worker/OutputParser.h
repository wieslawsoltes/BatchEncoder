// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "WorkerContext.h"
#include "CommandLine.h"

class IOutputParser
{
public:
    virtual bool Init(IWorkerContext* pWorkerContext, CCommandLine* pCommandLine) = 0;
    virtual bool Parse(const char *szLine) = 0;
};
