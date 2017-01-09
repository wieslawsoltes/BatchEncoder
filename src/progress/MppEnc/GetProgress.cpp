//
// BatchEncoder - GetProgress MppEnc
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
    int nProgress = -1;
    char szPercentage[32];

    memset(szPercentage, 0x00, sizeof(szPercentage));

    if(nLineLen >= 5)
    {
        // we have progress line
        if(szLineBuff[3] == '.')
        {
            if(szLineBuff[0] == ' ') // not a 100.0 %
            {
                if(szLineBuff[1] == ' ') // 0 to 9 %
                {
                    szPercentage[0] = szLineBuff[2];
                    szPercentage[1] = '\0';

                    nProgress = atoi(szPercentage);
                    return nProgress;
                }
                else if(szLineBuff[1] >= '0' && szLineBuff[1] <= '9') // 10 to 99 %
                {
                    szPercentage[0] = szLineBuff[1];
                    szPercentage[1] = szLineBuff[2];
                    szPercentage[2] = '\0';

                    nProgress = atoi(szPercentage);
                    return nProgress;
                }
            }
            else if(szLineBuff[0] == '1') // 100.0 %
            {
                nProgress = 100;
                return nProgress;
            }
        }
    }

    return -1;
}
