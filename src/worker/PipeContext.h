// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CPipeContext
{
public:
    CString szFileName;
    HANDLE hPipe;
    int nIndex;
    volatile bool bError;
    volatile bool bFinished;
};
