// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "Utf8String.h"
#include "UnicodeUtf8.h"

CUtf8String::CUtf8String()
{
    m_Result = NULL;
}

CUtf8String::CUtf8String(CString szText)
{
    Convert(szText);
}

CUtf8String::~CUtf8String()
{
    Free();
}

#ifdef _UNICODE
char *CUtf8String::Convert(CString szText)
{
    if (szText.GetLength() > 0)
    {
        m_Result = (char *)MakeUtf8String(szText);
    }
    else
    {
        m_Result = (char *)malloc(1);
        m_Result[0] = '\0';
    }
    return m_Result;
}
#else
char *CUtf8String::Convert(CString szText)
{
    if (szText.GetLength() > 0)
    {
        Utf8Encode(szText, &m_Result);
    }
    else
    {
        m_Result = (char *)malloc(1);
        m_Result[0] = '\0';
    }
    return m_Result;
}
#endif

void CUtf8String::Free()
{
    if (m_Result != NULL)
    {
        free(m_Result);
        m_Result = NULL;
    }
}
