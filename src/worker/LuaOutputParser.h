// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "OutputParser.h"
#include "LuaProgess.h"

namespace worker
{
    class CLuaOutputParser : public IOutputParser
    {
        CLuaProgess luaProgress;
    public:
        IWorkerContext * pWorkerContext;
        CCommandLine *pCommandLine;
        int nProgress;
        int nPreviousProgress;
    public:
        CLuaOutputParser() { }
        virtual ~CLuaOutputParser() { }
    public:
        bool Init(IWorkerContext* pWorkerContext, CCommandLine* pCommandLine)
        {
            this->pWorkerContext = pWorkerContext;
            this->pCommandLine = pCommandLine;
            this->nProgress = 0;
            this->nPreviousProgress = 0;

            if (this->luaProgress.Open(CT2CA(this->pCommandLine->pFormat->szFunction)) == false)
            {
                this->pWorkerContext->Status(this->pCommandLine->nItemId, pszDefaulTime, this->pWorkerContext->GetString(0x00110001, pszProgresssLoop[0]));
                this->pWorkerContext->Callback(this->pCommandLine->nItemId, -1, true, true);
                return false;
            }

            if (this->luaProgress.Init() == false)
            {
                this->pWorkerContext->Status(this->pCommandLine->nItemId, pszDefaulTime, this->pWorkerContext->GetString(0x00110002, pszProgresssLoop[1]));
                this->pWorkerContext->Callback(this->pCommandLine->nItemId, -1, true, true);
                return false;
            }

            return true;
        }
        bool Parse(const char *szLine)
        {
            int nRet = (int)this->luaProgress.GetProgress(szLine);
            if (nRet != -1)
            {
                this->nProgress = nRet;
            }

            if (this->nProgress != this->nPreviousProgress)
            {
                nPreviousProgress = nProgress;
                bool bRunning = this->pWorkerContext->Callback(this->pCommandLine->nItemId, nProgress, false);
                return bRunning;
            }
            else
            {
                bool bRunning = this->pWorkerContext->IsRunning();
                return bRunning;
            }

            return true;
        }
    };
}
