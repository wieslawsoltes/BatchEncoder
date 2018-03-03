// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <memory>
#include "OutputParser.h"
#include "LuaProgess.h"
#include "utilities\Log.h"
#include "utilities\MemoryLog.h"
#include "utilities\StringHelper.h"

namespace worker
{
    class CLuaOutputParser : public IOutputParser
    {
        CLuaProgess luaProgress;
    public:
        std::unique_ptr<util::ILog> Log;
        IWorkerContext *ctx;
        CCommandLine *cl;
        int nProgress;
        int nPreviousProgress;
    public:
        bool Init(IWorkerContext* ctx, CCommandLine* cl)
        {
            this->ctx = ctx;
            this->cl = cl;
            this->nProgress = 0;
            this->nPreviousProgress = 0;

            std::string szFunction = util::StringHelper::StringHelper::Convert(this->cl->pFormat->szFunction);
            if (this->luaProgress.Open(szFunction.c_str()) == false)
            {
                this->ctx->ItemStatus(this->cl->nItemId, ctx->GetString(0x00150001), this->ctx->GetString(0x00110001));
                this->ctx->ItemProgress(this->cl->nItemId, -1, true, true);
                return false;
            }

            if (this->luaProgress.Init() == false)
            {
                this->ctx->ItemStatus(this->cl->nItemId, ctx->GetString(0x00150001), this->ctx->GetString(0x00110002));
                this->ctx->ItemProgress(this->cl->nItemId, -1, true, true);
                return false;
            }

            return true;
        }
        bool Parse(const char *szLine)
        {
            if (Log != nullptr)
            {
                std::wstring szUnicode = util::StringHelper::Convert(szLine);
                Log->Log(szUnicode, false);
            }

            int nRet = (int)this->luaProgress.GetProgress(szLine);
            if (nRet != -1)
            {
                this->nProgress = nRet;
            }

            if (this->nProgress != this->nPreviousProgress)
            {
                nPreviousProgress = nProgress;
                bool bRunning = this->ctx->ItemProgress(this->cl->nItemId, nProgress, false);
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
