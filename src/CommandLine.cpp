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

PTCHAR* MyCommandLineToArgv(PTCHAR pszCmdLine, int *pnArgc)
{
    BOOLEAN bHaveQuotationMark, bHaveText, bHaveSpace;
    ULONG nLength, nArgc, nI, nJ;
    PTCHAR *pszArgv;
    PTCHAR pszTmpArgv;
    TCHAR cNext;

    nLength = (ULONG) _tcslen(pszCmdLine);
    nI = ((nLength + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);
    pszArgv = (PTCHAR *) GlobalAlloc(GMEM_FIXED, nI + (nLength + 2) * sizeof(TCHAR));
    pszTmpArgv = (PTCHAR) (((PUCHAR) pszArgv) + nI);
    nArgc = nI = nJ = 0;
    pszArgv[nArgc] = pszTmpArgv;
    bHaveQuotationMark = FALSE;
    bHaveText = FALSE;
    bHaveSpace = TRUE;

    while(cNext = pszCmdLine[nI]) 
    {
        if(bHaveQuotationMark) 
        {
            if(cNext == '\"') 
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
            if(cNext == '\"')
            {
                bHaveQuotationMark = TRUE;
                bHaveText = TRUE;

                if(bHaveSpace) 
                {
                    pszArgv[nArgc] = pszTmpArgv + nJ;
                    nArgc++;
                }

                bHaveSpace = FALSE;
            }
            else if((cNext == ' ') || (cNext == '\t') || (cNext == '\n') || (cNext == '\r'))
            {
                if(bHaveText) 
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

                if(bHaveSpace) 
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
