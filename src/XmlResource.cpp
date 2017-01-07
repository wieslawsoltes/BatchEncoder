//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

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
    if(hResInfo == NULL)
        return NULL;

    hResHandle = ::LoadResource(hInstance, (HRSRC) hResInfo);
    if(hResHandle == NULL)
        return NULL;

    LPVOID pTempBuf = NULL;
    pTempBuf = (LPVOID) ::LockResource(hResHandle);
    if(pTempBuf == NULL)
        return NULL;

    size_t dwBuffSize = 0L;
    if(SUCCEEDED(StringCbLength((LPCTSTR) pTempBuf, STRSAFE_MAX_CCH * sizeof(TCHAR), &dwBuffSize)))
    {
        // don't count terminating '\0' char
        dwBuffSize = dwBuffSize - 1UL;

        LPVOID lpvBuf;
        lpvBuf = malloc(dwBuffSize);
        if(lpvBuf == NULL)
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
    if(lpvBuf != NULL)
        free(lpvBuf);
}
