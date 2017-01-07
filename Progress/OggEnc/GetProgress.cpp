//
// BatchEncoder - GetProgress OggEnc
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
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
    char szPercentage[32];
    int nProgress = -1;
    int nStart;
    int nEnd;
    int j;

    memset(szPercentage, 0x00, sizeof(szPercentage));

    if(nLineLen >= 4)
    {
        // find: 'Done encoding file', if present we have done 100 %
        if(szLineBuff[0] == 'D' 
            && szLineBuff[1] == 'o' 
            && szLineBuff[2] == 'n' 
            && szLineBuff[3] == 'e')
        {
            nProgress = 100;
            return nProgress;
        }

        nStart = 0;
        nEnd = 0;

        // find: [  3,2%]
        for(j = 0; j < (int) nLineLen; j++)
        {
            if(szLineBuff[j] == '[')
            {
                nStart = j;
            }
            if(szLineBuff[j] == ']')
            {
                nEnd = j;
                break;
            }
        }

        if(nEnd > 0)
        {
            if(szLineBuff[nStart + 4] == ',')
            {
                if(szLineBuff[nStart + 1] == ' ') // not a 100.0 %
                {
                    if(szLineBuff[nStart + 2] == ' ') // 0 to 9 %
                    {
                        szPercentage[0] = szLineBuff[nStart + 3];
                        szPercentage[1] = '\0';

                        nProgress = atoi(szPercentage);
                        return nProgress;
                    }
                    else if(szLineBuff[nStart + 2] >= '0' && szLineBuff[nStart + 2] <= '9') // 10 to 99 %
                    {
                        szPercentage[0] = szLineBuff[nStart + 2];
                        szPercentage[1] = szLineBuff[nStart + 3];
                        szPercentage[2] = '\0';

                        nProgress = atoi(szPercentage);
                        return nProgress;
                    }
                }
                else if(szLineBuff[nStart + 1] == '1') // 100.0 %
                {
                    nProgress = 100;
                    return nProgress;
                }
            }
        }
    }

    return -1;
}
