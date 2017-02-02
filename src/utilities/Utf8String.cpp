// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "Utf8String.h"
#include "UnicodeUtf8.h"

CUtf8String::CUtf8String()
{
    szBuffUtf8 = NULL;
}

CUtf8String::~CUtf8String()
{
    Free();
}

char *CUtf8String::Create(CString szData)
{
#ifdef _UNICODE
    // UNICODE to UTF-8
    if (szData.GetLength() > 0)
    {
        szBuffUtf8 = (char *)MakeUtf8String(szData);
    }
    else
    {
        szBuffUtf8 = (char *)malloc(1);
        szBuffUtf8[0] = '\0';
    }

    return szBuffUtf8;
#else
    // ANSI to UTF-8
    if (szData.GetLength() > 0)
    {
        Utf8Encode(szData, &szBuffUtf8);
    }
    else
    {
        szBuffUtf8 = (char *)malloc(1);
        szBuffUtf8[0] = '\0';
    }

    return szBuffUtf8;
#endif
}

void CUtf8String::Free()
{
    if (szBuffUtf8 != NULL)
    {
        free(szBuffUtf8);
        szBuffUtf8 = NULL;
    }
}
