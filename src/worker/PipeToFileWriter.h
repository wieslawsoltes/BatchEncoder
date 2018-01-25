// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\Pipe.h"
#include "WorkerContext.h"

namespace worker
{
    class CPipeToFileWriter
    {
    public:
        std::wstring szFileName;
        int nIndex;
    public:
        volatile bool bError;
        volatile bool bFinished;
    public:
        CPipeToFileWriter() { }
        virtual ~CPipeToFileWriter() { }
    public:
        bool WriteLoop(IWorkerContext* pWorkerContext, util::CPipe &Stdout)
        {
            HANDLE hPipe = Stdout.hRead;
            HANDLE hFile = INVALID_HANDLE_VALUE;
            BYTE pReadBuff[4096];
            BOOL bRes = FALSE;
            DWORD dwReadBytes = 0;
            DWORD dwWriteBytes = 0;
            ULONGLONG nTotalBytesWrite = 0;

            bError = false;
            bFinished = false;

            hFile = ::CreateFile(szFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                bError = true;
                bFinished = true;
                return false;
            }

            do
            {
                ::Sleep(0);

                DWORD dwAvailableBytes;
                if (FALSE == PeekNamedPipe(hPipe, 0, 0, 0, &dwAvailableBytes, 0))
                    break;

                if (dwAvailableBytes > 0)
                {
                    bRes = ::ReadFile(hPipe, pReadBuff, 4096, &dwReadBytes, 0);
                    if ((bRes == FALSE) || (dwReadBytes == 0))
                        break;

                    bRes = ::WriteFile(hFile, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
                    if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
                        break;

                    nTotalBytesWrite += dwReadBytes;
                }
                else
                    bRes = TRUE;

                if (pWorkerContext->bRunning == false)
                    break;
            } while (bRes != FALSE);

            ::CloseHandle(hFile);

            if (nTotalBytesWrite <= 0)
            {
                bError = true;
                bFinished = true;
                return false;
            }
            else
            {
                bError = false;
                bFinished = true;
                return true;
            }
        }
    };
}
