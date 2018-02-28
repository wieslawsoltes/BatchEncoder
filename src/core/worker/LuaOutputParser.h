// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "OutputParser.h"
#include "LuaProgess.h"
#include "utilities\StringHelper.h"

namespace worker
{
    class CLuaOutputParser : public IOutputParser
    {
        CLuaProgess luaProgress;
    public:
        IWorkerContext * ctx;
        CCommandLine *pCommandLine;
        int nProgress;
        int nPreviousProgress;
    public:
        bool Init(IWorkerContext* ctx, CCommandLine* pCommandLine)
        {
            this->ctx = ctx;
            this->pCommandLine = pCommandLine;
            this->nProgress = 0;
            this->nPreviousProgress = 0;

            std::string szFunction = util::StringHelper::StringHelper::Convert(this->pCommandLine->pFormat->szFunction);
            if (this->luaProgress.Open(szFunction.c_str()) == false)
            {
                this->ctx->Status(this->pCommandLine->nItemId, ctx->pConfig->GetString(0x00150001), this->ctx->pConfig->GetString(0x00110001));
                this->ctx->Callback(this->pCommandLine->nItemId, -1, true, true);
                return false;
            }

            if (this->luaProgress.Init() == false)
            {
                this->ctx->Status(this->pCommandLine->nItemId, ctx->pConfig->GetString(0x00150001), this->ctx->pConfig->GetString(0x00110002));
                this->ctx->Callback(this->pCommandLine->nItemId, -1, true, true);
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
                bool bRunning = this->ctx->Callback(this->pCommandLine->nItemId, nProgress, false);
                return bRunning;
            }
            else
            {
                bool bRunning = this->ctx->IsRunning();
                return bRunning;
            }

            return true;
        }
    };
}
