// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

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

    if (nLineLen >= 5)
    {
        // we have progress line
        if (szLineBuff[3] == '.')
        {
            if (szLineBuff[0] == ' ') // not a 100.0 %
            {
                if (szLineBuff[1] == ' ') // 0 to 9 %
                {
                    szPercentage[0] = szLineBuff[2];
                    szPercentage[1] = '\0';

                    nProgress = atoi(szPercentage);
                    return nProgress;
                }
                else if (szLineBuff[1] >= '0' && szLineBuff[1] <= '9') // 10 to 99 %
                {
                    szPercentage[0] = szLineBuff[1];
                    szPercentage[1] = szLineBuff[2];
                    szPercentage[2] = '\0';

                    nProgress = atoi(szPercentage);
                    return nProgress;
                }
            }
            else if (szLineBuff[0] == '1') // 100.0 %
            {
                nProgress = 100;
                return nProgress;
            }
        }
    }

    return -1;
}
