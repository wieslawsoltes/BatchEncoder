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
    bool Wait()
    {
        DWORD dwRet = ::WaitForSingleObject(this->pInfo.hProcess, INFINITE);
        switch (dwRet)
        {
        case WAIT_FAILED:
            return false;
            break;
        case WAIT_ABANDONED:
            break;
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return false;
            break;
        };
        return true;
    }
    bool Wait(int milliseconds)
    {
        DWORD dwRet = ::WaitForSingleObject(this->pInfo.hProcess, (DWORD)milliseconds);
        switch (dwRet)
        {
        case WAIT_FAILED:
            return false;
            break;
        case WAIT_ABANDONED:
            break;
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return false;
            break;
        };
        return true;

    }
    bool Terminate(int code = 0)
    {
        return ::TerminateProcess(this->pInfo.hProcess, (DWORD)code) == TRUE;
    }
    bool Close()
    {
        BOOL bCloseProcess = ::CloseHandle(this->pInfo.hProcess);
        BOOL bCloseThread = ::CloseHandle(this->pInfo.hThread);
        return (bCloseProcess == TRUE) && (bCloseThread == TRUE);
    }
    bool Stop(bool bWait, int nExitCodeSucess)
    {
        bool bResult = false;
        if (bWait == true)
        {
            if (this->Wait() == false)
            {
                this->Terminate();
                DWORD dwExitCode;
                ::GetExitCodeProcess(this->pInfo.hProcess, &dwExitCode);
                bResult = false;
            }
            else
            {
                DWORD dwExitCode;
                ::GetExitCodeProcess(this->pInfo.hProcess, &dwExitCode);
                bResult = (DWORD)nExitCodeSucess == dwExitCode;
            }
        }
        else
        {
            this->Terminate();
            this->Wait(5000);
            DWORD dwExitCode;
            ::GetExitCodeProcess(this->pInfo.hProcess, &dwExitCode);
            bResult = false;
        }
        this->Close();
        return bResult;
    }
};
