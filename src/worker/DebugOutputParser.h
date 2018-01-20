// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "OutputParser.h"

class CDebugOutputParser : public IOutputParser
{
public:
    IWorkerContext * pWorkerContext;
    CFileContext *pFileContext;
public:
    CDebugOutputParser() { }
    virtual ~CDebugOutputParser() { }
public:
    bool Init(IWorkerContext* pWorkerContext, CFileContext* pFileContext)
    {
        this->pWorkerContext = pWorkerContext;
        this->pFileContext = pFileContext;
        return true;
    }
    bool Parse(const char *szLine)
    {
        OutputDebugStringA(szLine);
        OutputDebugStringA("\n");
        return this->pWorkerContext->IsRunning();
    }
};
