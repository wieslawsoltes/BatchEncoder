//
// BatchEncoder - GetProgress WavPackEnc
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

/* $Id: GetProgress.cpp 437 2008-03-24 12:15:45Z wisodev $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "GetProgress.h"

int GetProgress(char *szLineBuff, int nLineLen)
{
    // NOTE:
    // using my patched console encoder wavpack.exe 4.31
    // added fflush(...) after all fprintf(...) calls in WavPack project
    // because of delayed output from original console encoder

    int j;
    int nPos = 0;
    char szPercentage[32];
    int nProgress = -1;

    memset(szPercentage, 0x00, sizeof(szPercentage));

    // check if we have done the job
    // 'created 1.wv in 7.95 secs (lossless, 26.56%)'
    if(nLineLen >= 7)
    {
        if(strncmp(szLineBuff, "created", 7) == 0)
        {
            nProgress = 100;
            return nProgress;
        }
    }

    // search for:
    // 'creating 1.wv,  77% done...'

    // find % (percentage) char
    for(j = 0; j < (int) nLineLen; j++)
    {
        if(szLineBuff[j] == '%')
        {
            nPos = j;
            break;
        }
    }

    if(nPos >= 3)
    {
        if(szLineBuff[nPos - 3] == ' ') // not a 100.0 %
        {
            if(szLineBuff[nPos - 2] == ' ') // 0 to 9 %
            {
                szPercentage[0] = szLineBuff[nPos - 1];
                szPercentage[1] = '\0';

                nProgress = atoi(szPercentage);
                return nProgress;
            }
            else if(szLineBuff[nPos - 2] >= '0' && szLineBuff[nPos - 2] <= '9') // 10 to 99 %
            {
                szPercentage[0] = szLineBuff[nPos - 2];
                szPercentage[1] = szLineBuff[nPos - 1];
                szPercentage[2] = '\0';

                nProgress = atoi(szPercentage);
                return nProgress;
            }
        }
        else if(szLineBuff[nPos - 3] == '1') // 100.0 %
        {
            nProgress = 100;
            return nProgress;
        }
    }

    return -1;
}
