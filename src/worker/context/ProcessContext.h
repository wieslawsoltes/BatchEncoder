// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CProcessContext
{
public:
    HANDLE hReadPipeStderr;
    HANDLE hWritePipeStderr;
    HANDLE hReadPipeStdin;
    HANDLE hWritePipeStdin;
    HANDLE hReadPipeStdout;
    HANDLE hWritePipeStdout;
public:
    CProcessContext()
    {
        this->hReadPipeStderr = INVALID_HANDLE_VALUE;
        this->hWritePipeStderr = INVALID_HANDLE_VALUE;
        this->hReadPipeStdin = INVALID_HANDLE_VALUE;
        this->hWritePipeStdin = INVALID_HANDLE_VALUE;
        this->hReadPipeStdout = INVALID_HANDLE_VALUE;
        this->hWritePipeStdout = INVALID_HANDLE_VALUE;
    }
    virtual ~CProcessContext()
    {
    }
};
