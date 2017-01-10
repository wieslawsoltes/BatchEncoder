// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "XmlResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// undefine to deprecate all of the unsafe functions
#define STRSAFE_NO_DEPRECATE

#include <strsafe.h>

LPVOID LoadXmlResource(LPCTSTR lpType, int nResID, PINT64 lpdwSize)
{
    HINSTANCE hInstance = ::GetModuleHandle(NULL);
    HANDLE hResInfo;
    HANDLE hResHandle;

    *lpdwSize = 0UL;

    hResInfo = ::FindResource(hInstance, MAKEINTRESOURCE(nResID), lpType);
    if (hResInfo == NULL)
        return NULL;

    hResHandle = ::LoadResource(hInstance, (HRSRC)hResInfo);
    if (hResHandle == NULL)
        return NULL;

    LPVOID pTempBuf = NULL;
    pTempBuf = (LPVOID) ::LockResource(hResHandle);
    if (pTempBuf == NULL)
        return NULL;

    size_t dwBuffSize = 0L;
    if (SUCCEEDED(StringCbLength((LPCTSTR)pTempBuf, STRSAFE_MAX_CCH * sizeof(TCHAR), &dwBuffSize)))
    {
        // don't count terminating '\0' char
        dwBuffSize = dwBuffSize - 1UL;

        LPVOID lpvBuf;
        lpvBuf = malloc(dwBuffSize);
        if (lpvBuf == NULL)
            return FALSE;

        memset(lpvBuf, 0x00, dwBuffSize);
        memcpy(lpvBuf, pTempBuf, dwBuffSize);
        *lpdwSize = dwBuffSize;

        return lpvBuf;
    }
    return NULL;
}

VOID FreeXmlResource(LPVOID lpvBuf)
{
    if (lpvBuf != NULL)
        free(lpvBuf);
}
