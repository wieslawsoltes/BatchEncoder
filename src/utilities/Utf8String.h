// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CUtf8String
{
public:
    char *m_Result;
public:
    CUtf8String();
    CUtf8String(CString szText);
    virtual ~CUtf8String();
public:
    char *Convert(CString szText);
    void Free();
};
