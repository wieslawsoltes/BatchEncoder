// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"

PTCHAR* MyCommandLineToArgv(PTCHAR pszCmdLine, int *pnArgc)
{
    BOOLEAN bHaveQuotationMark, bHaveText, bHaveSpace;
    ULONG nLength, nArgc, nI, nJ;
    PTCHAR *pszArgv;
    PTCHAR pszTmpArgv;
    TCHAR cNext;

    nLength = _tcslen(pszCmdLine);
    nI = ((nLength + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);
    pszArgv = (PTCHAR *)GlobalAlloc(GMEM_FIXED, nI + (nLength + 2) * sizeof(TCHAR));
    pszTmpArgv = (PTCHAR)(((PUCHAR)pszArgv) + nI);
    nArgc = nI = nJ = 0;
    pszArgv[nArgc] = pszTmpArgv;
    bHaveQuotationMark = FALSE;
    bHaveText = FALSE;
    bHaveSpace = TRUE;

    while (cNext = pszCmdLine[nI])
    {
        if (bHaveQuotationMark)
        {
            if (cNext == '\"')
            {
                bHaveQuotationMark = FALSE;
            }
            else
            {
                pszTmpArgv[nJ] = cNext;
                nJ++;
            }
        }
        else
        {
            if (cNext == '\"')
            {
                bHaveQuotationMark = TRUE;
                bHaveText = TRUE;

                if (bHaveSpace)
                {
                    pszArgv[nArgc] = pszTmpArgv + nJ;
                    nArgc++;
                }

                bHaveSpace = FALSE;
            }
            else if ((cNext == ' ') || (cNext == '\t') || (cNext == '\n') || (cNext == '\r'))
            {
                if (bHaveText)
                {
                    pszTmpArgv[nJ] = '\0';
                    nJ++;
                }

                bHaveText = FALSE;
                bHaveSpace = TRUE;
            }
            else
            {
                bHaveText = TRUE;

                if (bHaveSpace)
                {
                    pszArgv[nArgc] = pszTmpArgv + nJ;
                    nArgc++;
                }

                pszTmpArgv[nJ] = cNext;
                nJ++;
                bHaveSpace = FALSE;
            }
        }

        nI++;
    }

    pszTmpArgv[nJ] = '\0';
    pszArgv[nArgc] = NULL;
    (*pnArgc) = nArgc;

    return pszArgv;
}
