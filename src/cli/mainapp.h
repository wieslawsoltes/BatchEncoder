// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <string>
#include <utility>
#include <memory>
#include <array>
#include <vector>
#include <thread>
#include <algorithm>

#include "utilities\FileSystem.h"
#include "utilities\Log.h"
#include "utilities\ConsoleLog.h"
#include "utilities\String.h"
#include "utilities\TimeCount.h"
#include "utilities\Utf8String.h"
#include "utilities\Utilities.h"

#include "config\Config.h"

#include "worker\WorkerContext.h"
#include "worker\OutputPath.h"
#include "worker\ToolDownloader.h"
#include "worker\Worker.h"
#include "worker\Win32.h"

class CConsoleWorkerContext : public worker::IWorkerContext
{
    volatile bool bSafeCheck;
public:
    CConsoleWorkerContext()
    {
        this->bDone = true;
        this->bRunning = false;
        this->bSafeCheck = false;
        this->pFactory = std::make_shared<worker::Win32WorkerFactory>();
    }
    virtual ~CConsoleWorkerContext() { }
public:
    std::wstring GetString(int nKey)
    {
        return this->pConfig->GetString(nKey);
    }
    void Start()
    {
    }
    void Stop()
    {
        this->pConfig = nullptr;
        this->bRunning = false;
    }
    bool ItemProgress(int nItemId, int nProgress, bool bFinished, bool bError = false)
    {
        if (bError == true)
        {
            config::CItem &item = this->pConfig->m_Items[nItemId];
            item.bFinished = true;
            if (this->pConfig->m_Options.bStopOnErrors == true)
            {
                this->bRunning = false;
            }
            return this->bRunning;
        }

        if (bFinished == true)
        {
            config::CItem &item = this->pConfig->m_Items[nItemId];
            item.bFinished = true;
        }

        if ((bFinished == false) && (bRunning == true))
        {
            config::CItem &item = this->pConfig->m_Items[nItemId];
            item.nProgress = nProgress;
            if (item.nPreviousProgress > nProgress)
                item.nPreviousProgress = nProgress;

            if (item.bChecked == true)
            {
                int nItemProgress = item.nProgress;
                int nItemPreviousProgress = item.nPreviousProgress;
                if (item.bFinished == false)
                {
                    if (nItemProgress > 0 && nItemProgress < 100 && nItemProgress > nItemPreviousProgress)
                    {
                        item.szStatus = std::to_wstring(nItemProgress) + L"%";
                        item.nPreviousProgress = nItemProgress;
                    }
                    else if (nItemProgress == 100 && nItemProgress > nItemPreviousProgress)
                    {
                        item.nPreviousProgress = nItemProgress;
                    }
                }
            }

            if (this->bSafeCheck == false)
            {
                this->bSafeCheck = true;
                if (nItemId > this->nLastItemId)
                {
                    this->nLastItemId = nItemId;
                    if (this->pConfig->m_Options.bEnsureItemIsVisible == true)
                    {
                    }
                }
                this->bSafeCheck = false;
            }
        }
        return bRunning;
    }
    void ItemStatus(int nItemId, const std::wstring& szTime, const std::wstring& szStatus)
    {
        config::CItem &item = this->pConfig->m_Items[nItemId];
        item.szTime = szTime;
        item.szStatus = szStatus;
        if (bSafeCheck == false)
        {
            bSafeCheck = true;
            if (nItemId > this->nLastItemId)
            {
                this->nLastItemId = nItemId;
                if (pConfig->m_Options.bEnsureItemIsVisible == true)
                {
                }
            }
            bSafeCheck = false;
        }
    }
    void TotalProgress(int nItemId)
    {
        if (nItemId > this->nLastItemId)
        {
            this->nLastItemId = nItemId;
            if (this->pConfig->m_Options.bEnsureItemIsVisible == true)
            {
            }
        }

        int nPos = (int)(100.0f * ((double)nProcessedFiles / (double)nTotalFiles));
    }
};
