// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CProcessContext
{
public:
    SECURITY_ATTRIBUTES secattr;
    HANDLE hReadPipeStderr;
    HANDLE hWritePipeStderr;
    HANDLE hReadPipeStdin;
    HANDLE hWritePipeStdin;
    HANDLE hReadPipeStdout;
    HANDLE hWritePipeStdout;
    PROCESS_INFORMATION pInfo;
    STARTUPINFO sInfo;
public:
    CProcessContext()
    {
        // security attributes
        ZeroMemory(&this->secattr, sizeof(this->secattr));
        this->secattr.nLength = sizeof(this->secattr);
        this->secattr.bInheritHandle = TRUE;
        // stderr pipe
        this->hReadPipeStderr = INVALID_HANDLE_VALUE;
        this->hWritePipeStderr = INVALID_HANDLE_VALUE;
        // stdin pipe
        this->hReadPipeStdin = INVALID_HANDLE_VALUE;
        this->hWritePipeStdin = INVALID_HANDLE_VALUE;
        // stdout pipes
        this->hReadPipeStdout = INVALID_HANDLE_VALUE;
        this->hWritePipeStdout = INVALID_HANDLE_VALUE;
        // process info
        ZeroMemory(&this->pInfo, sizeof(this->pInfo));
        // startup info
        ZeroMemory(&this->sInfo, sizeof(this->sInfo));
        this->sInfo.cb = sizeof(this->sInfo);
        this->sInfo.dwFlags = STARTF_USESTDHANDLES;
    }
    virtual ~CProcessContext()
    {
    }
public:
    bool CreateStderrPipe()
    {
        BOOL bResult = ::CreatePipe(&this->hReadPipeStderr, &this->hWritePipeStderr, &this->secattr, 0);
        if (bResult == FALSE)
            return false;
        return true;
    }
    void CloseStderrReadPipe()
    {
        ::CloseHandle(this->hReadPipeStderr);
    }
    void CloseStderrWritePipe()
    {
        ::CloseHandle(this->hWritePipeStderr);
    }
    bool DuplicateStderrReadPipe()
    {
        BOOL bResult = ::DuplicateHandle(::GetCurrentProcess(),
            this->hReadPipeStderr,
            ::GetCurrentProcess(),
            &this->hReadPipeStderr,
            0,
            TRUE,
            DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS);
        if (bResult == FALSE)
        {
            this->CloseStderrReadPipe();
            this->CloseStderrWritePipe();
            return false;
        }
        return true;
    }
public:
    bool CreateStdinPipe()
    {
        BOOL bResult = ::CreatePipe(&this->hReadPipeStdin, &this->hWritePipeStdin, &this->secattr, 0);
        if (bResult == FALSE)
            return false;
        return true;
    }
    void CloseStdinReadPipe()
    {
        ::CloseHandle(this->hReadPipeStdin);
    }
    void CloseStdinWritePipe()
    {
        ::CloseHandle(this->hWritePipeStdin);
    }
    bool InheritStdinWritePipe()
    {
        BOOL bResult = ::SetHandleInformation(this->hWritePipeStdin, HANDLE_FLAG_INHERIT, 0);
        if (bResult == FALSE)
        {
            this->CloseStdinReadPipe();
            this->CloseStdinWritePipe();
            return false;
        }
        return true;
    }
public:
    bool CreateStdoutPipe()
    {
        BOOL bResult = ::CreatePipe(&this->hReadPipeStdout, &this->hWritePipeStdout, &this->secattr, 0);
        if (bResult == FALSE)
            return false;
        return true;
    }
    void CloseStdoutReadPipe()
    {
        ::CloseHandle(this->hReadPipeStdout);
    }
    void CloseStdoutWritePipe()
    {
        ::CloseHandle(this->hWritePipeStdout);
    }
    bool InheritStdoutReadPipe()
    {
        BOOL bResult = ::SetHandleInformation(this->hReadPipeStdout, HANDLE_FLAG_INHERIT, 0);
        if (bResult == FALSE)
        {
            this->CloseStdoutReadPipe();
            this->CloseStdoutWritePipe();
            return false;
        }
        return true;
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
    bool Start(TCHAR *szCommandLine)
    {
        BOOL bResult = ::CreateProcess(NULL,
            szCommandLine,
            NULL,
            NULL,
            TRUE,
            NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
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
