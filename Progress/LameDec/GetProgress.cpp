//
// BatchEncoder - GetProgress LameDec
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "GetProgress.h"

int GetProgress(char *szLineBuff, int nLineLen)
{
    int nStart;
    int nEnd;
    int nPos;
    char szBuff[32];
    int nCurFrame;
    int nTotalFrames;
    int j;
    int nProgress;

    memset(szBuff, 0x00, sizeof(szBuff));

    // check the decode output (stdout)
    // 'Frame# 10007/10007'

    nStart = 0;
    nEnd = 0;
    nPos = 0;

    // find separator '/'
    for(j = 0; j < (int) nLineLen; j++)
    {
        if(szLineBuff[j] == '/')
        {
            nPos = j;
            break;
        }
    }

    if(nPos != 0)
    {
        // find first space before separator
        for(j = nPos; j > 0; j--)
        {
            if(szLineBuff[j] == ' ')
            {
                nStart = j;
                break;
            }
        }

        // find first space after separator
        for(j = nPos; j < (int) nLineLen; j++)
        {
            if(szLineBuff[j] == ' ')
            {
                nEnd = j;
                break;
            }
        }

        strncpy_s(szBuff, (char *) (szLineBuff + nStart + 1), nPos - nStart - 1);
        szBuff[(nPos - nStart) - 1] = '\0';
        nCurFrame = atoi(szBuff);

        strncpy_s(szBuff, (char *) (szLineBuff + nPos + 1), nEnd - nPos - 1);
        szBuff[(nEnd - nPos) - 1] = '\0';
        nTotalFrames = atoi(szBuff);

        nProgress = (100 * nCurFrame) / nTotalFrames;

        return nProgress;
    }

    return -1;
}
