//
// BatchEncoder - GetProgress OfrEnc
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

    // check if we have done the job
    // 'Compressing   done.'
    // 'Decompression done.'
    if(nLineLen >= 19)
    {
        if(szLineBuff[14] == 'd' 
            && szLineBuff[15] == 'o' 
            && szLineBuff[16] == 'n' 
            && szLineBuff[17] == 'e')
        {
            nProgress = 100;
            return nProgress;
        }
    }

    // search for:
    // 'Compressing    15.0%'

    // find '%' end char
    nEnd = -1;
    for(j = 0; j < nLineLen; j++)
    {
        if(szLineBuff[j] == '%')
        {
            nEnd = j;
            break;
        }
    }

    nStart = -1;
    if(nEnd > 0)
    {
        // find ' ' start char
        for(j = nEnd; j >= 0; j--)
        {
            if(szLineBuff[j] == ' ')
            {
                nStart = j;
                break;
            }
        }

        if(nStart > 0)
        {
            // (nEnd - 3) skip the period char
            // (nStart + 1)  skip the space char
            int nDiff = (nEnd - 2) - (nStart + 1);
            if(nDiff == 1)
            {
                szPercentage[0] = szLineBuff[(nStart + 1)];
                szPercentage[1] = '\0';
                nProgress = atoi(szPercentage);

                return nProgress;
            }
            else if(nDiff == 2)
            {
                szPercentage[0] = szLineBuff[(nStart + 1)];
                szPercentage[1] = szLineBuff[(nStart + 2)];
                szPercentage[2] = '\0';
                nProgress = atoi(szPercentage);

                return nProgress;
            }
            else if(nDiff == 3)
            {
                szPercentage[0] = szLineBuff[(nStart + 1)];
                szPercentage[1] = szLineBuff[(nStart + 2)];
                szPercentage[2] = szLineBuff[(nStart + 3)];
                szPercentage[3] = '\0';
                nProgress = atoi(szPercentage);

                return nProgress;
            }
        }
    }

    return -1;
}
