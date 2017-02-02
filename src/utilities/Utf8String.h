// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CUtf8String
{
public:
    char *szBuffUtf8;
public:
    CUtf8String();
    virtual ~CUtf8String();
public:
    char *Create(CString szData);
    void Free();
};
