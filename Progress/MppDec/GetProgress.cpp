//
// BatchEncoder - GetProgress MppDec
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
    char szPercentage[32];
    int j;
    int nProgress;

    memset(szPercentage, 0x00, sizeof(szPercentage));

    nStart = 0;
    nEnd = 0;

    for(j = 0; j < (int) nLineLen; j++)
    {
        if(szLineBuff[j] == '(')
        {
            nStart = j;
        }
        if(szLineBuff[j] == ')')
        {
            nEnd = j;
            break;
        }
    }

    if(nEnd > 0)
    {
        // find: 3:45.06 (runtime: 0.98 s  speed: 228.48x)
        if(szLineBuff[nEnd - 1] == 'x') // done
        {   
            nProgress = 100;
            return nProgress;
        }

        // find: 0:53.07/    3:45.07 decoded (23.6%)
        if(szLineBuff[nEnd - 1] == '%') // progress
        {
            if(szLineBuff[nEnd - 5] == ' ') // 0 to 9 %
            {
                szPercentage[0] = szLineBuff[nEnd - 4];
                szPercentage[1] = '\0';
                nProgress = atoi(szPercentage);

                return nProgress;
            }
            else if(szLineBuff[nEnd - 5] >= '0' && szLineBuff[nEnd - 5] >= '9') // 10 to 99%
            {
                szPercentage[0] = szLineBuff[nEnd - 5];
                szPercentage[1] = szLineBuff[nEnd - 4];
                szPercentage[2] = '\0';
                nProgress = atoi(szPercentage);

                return nProgress;
            }
        }
    }

    return -1;
}
