// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <cstring>
#include "utilities\Pipe.h"
#include "utilities\Synchronize.h"
#include "OutputParser.h"
#include "WorkerContext.h"
#include "Strings.h"

namespace worker
{
    class CPipeToStringWriter
    {
    public:
        CPipeToStringWriter() { }
        virtual ~CPipeToStringWriter() { }
    public:
        bool ReadLoop(IWorkerContext* pWorkerContext, CCommandLine &commandLine, util::CPipe &Stderr, IOutputParser &parser, util::CSynchronize &syncDown)
        {
            const int nBuffSize = 4096;
            char szReadBuff[nBuffSize];
            char szLineBuff[nBuffSize];
            DWORD dwReadBytes = 0L;
            BOOL bRes = FALSE;
            bool bLineStart = false;
            bool bLineEnd = false;
            bool bRunning = true;
            int nLineLen = 0;

            syncDown.Wait();
            ::SetCurrentDirectory(app::m_App.szSettingsPath);

            if (parser.Init(pWorkerContext, &commandLine) == false)
            {
                syncDown.Release();
                return false; // ERROR
            }

            syncDown.Release();

            // initialize buffers
            std::memset(szReadBuff, 0, sizeof(szReadBuff));
            std::memset(szLineBuff, 0, sizeof(szLineBuff));

            do
            {
                std::memset(szReadBuff, 0, sizeof(szReadBuff));
                bRes = ::ReadFile(Stderr.hRead, szReadBuff, 100, &dwReadBytes, 0);
                if (bRes == FALSE || dwReadBytes == 0)
                    break;

                // terminate read data by '\0'
                szReadBuff[dwReadBytes] = '\0';

                for (int i = 0; i < (int)dwReadBytes; i++)
                {
                    // processed escape chars ( \r \n \t \b )
                    if (szReadBuff[i] == '\r') // '\r'
                    {
                        if ((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if (szReadBuff[i] == '\n') // '\n'
                    {
                        // do nothing
                    }
                    else if (szReadBuff[i] == '\t') // '\t'
                    {
                        // do nothing
                    }
                    else if (szReadBuff[i] == '\b') // '\b'
                    {
                        // do nothing (most of the tools)
                        if ((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if (bLineEnd == false)
                    {
                        bLineStart = true; // we have start
                        nLineLen++;
                        if (nLineLen > nBuffSize)
                        {
                            pWorkerContext->Status(commandLine.nItemId, app::pszDefaulTime, pWorkerContext->GetString(0x00110003, app::pszProgresssLoop[2]));
                            pWorkerContext->Callback(commandLine.nItemId, -1, true, true);
                            return false;
                        }

                        szLineBuff[nLineLen - 1] = szReadBuff[i];
                    }

                    // now we have correct full line of text
                    if ((bLineEnd == true) && (bLineStart == false))
                    {
                        // don't include empty lines
                        if (strlen(szLineBuff) > 0)
                        {
                            bRunning = parser.Parse(szLineBuff);
                            std::memset(szLineBuff, 0, sizeof(szLineBuff));
                            if ((pWorkerContext->bRunning == false) || (bRunning == false))
                                break;
                        }

                        // reset line counter
                        nLineLen = 0;

                        // find next start
                        bLineStart = true;
                        bLineEnd = false;
                    }
                }

                if (bRunning == false)
                    break;
            } while (bRes);

            return true;
        }
    };
}
