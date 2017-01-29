// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CFileContext
{
public:
    CWorkerContext* pWorkerContext;
    CString szInputFile;
    CString szOutputFile;
    TCHAR *szCommandLine;
    int nItemId;
    CString szFunction;
    bool bUseReadPipes;
    bool bUseWritePipes;
};
