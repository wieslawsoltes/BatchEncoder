// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "OutputParser.h"

namespace worker
{
    class CDebugOutputParser : public IOutputParser
    {
    public:
        IWorkerContext * pWorkerContext;
        CCommandLine *pCommandLine;
    public:
        bool Init(IWorkerContext* pWorkerContext, CCommandLine* pCommandLine)
        {
            this->pWorkerContext = pWorkerContext;
            this->pCommandLine = pCommandLine;
            return true;
        }
        bool Parse(const char *szLine)
        {
            OutputDebugStringA(szLine);
            OutputDebugStringA("\n");
            return this->pWorkerContext->IsRunning();
        }
    };
}
