// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CProcess
{
    PROCESS_INFORMATION pInfo;
    STARTUPINFO sInfo;
public:
    CProcess()
    {
        ZeroMemory(&this->pInfo, sizeof(this->pInfo));
        ZeroMemory(&this->sInfo, sizeof(this->sInfo));
        this->sInfo.cb = sizeof(this->sInfo);
        this->sInfo.dwFlags = STARTF_USESTDHANDLES;
    }
    virtual ~CProcess()
    {
    }
public:
    void ConnectStdInput(HANDLE hPipeStdin)
    {
        this->sInfo.hStdInput = hPipeStdin;
    }
    void ConnectStdOutput(HANDLE hPipeStdout)
    {
        this->sInfo.hStdOutput = hPipeStdout;
    }
    void ConnectStdError(HANDLE hPipeStderr)
    {
        this->sInfo.hStdError = hPipeStderr;
    }
public:
    bool Start(TCHAR *szCommandLine, bool bNoWindow)
    {
        DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS;
        if (bNoWindow == true)
            dwCreationFlags |= CREATE_NO_WINDOW;
        BOOL bResult = ::CreateProcess(NULL,
            szCommandLine,
            NULL,
            NULL,
            TRUE,
            dwCreationFlags,
            NULL,
            NULL,
            &this->sInfo,
            &this->pInfo);
        return bResult == TRUE;
    }
    void Stop(bool bWait)
    {
        // terminate console process if there was an error
        HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, this->pInfo.dwProcessId); // PROCESS_TERMINATE
        if (hProcess != NULL)
        {
            if (bWait == true)
            {
                // when properly finishing we need to wait
                // for process to terminate = 2 seconds max
                DWORD dwRet = ::WaitForSingleObject(hProcess, 2000);
                switch (dwRet)
                {
                case WAIT_FAILED:
                    // failed, probably process has finished, but error could be to
                    ::TerminateProcess(hProcess, 0);
                    break;
                case WAIT_ABANDONED:
                    // skip, only for mutex objects
                    break;
                case WAIT_OBJECT_0:
                    // skip, object is signaled, process has terminated
                    break;
                case WAIT_TIMEOUT:
                    // time-out interval elapsed
                    // object's state is non-signaled
                    // used when user had pressed stop button
                    ::TerminateProcess(hProcess, 0);
                    break;
                };
            }
            else
            {
                // don't wait for process to terminate because
                // this code is only executed when user pressed Stop
                ::TerminateProcess(hProcess, 0);

                // wait for process to terminate = 5 seconds max
                // this is because process must release file handles
                // and we need to delete it if there was an error
                ::WaitForSingleObject(hProcess, 5000);
            }

            // release process handle
            ::CloseHandle(hProcess);
        }

        ::CloseHandle(this->pInfo.hProcess);
        ::CloseHandle(this->pInfo.hThread);
    }
};
