// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

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

    for (j = 0; j < (int)nLineLen; j++)
    {
        if (szLineBuff[j] == '(')
        {
            nStart = j;
        }
        if (szLineBuff[j] == ')')
        {
            nEnd = j;
            break;
        }
    }

    if (nEnd > 0)
    {
        // find: 3:45.06 (runtime: 0.98 s  speed: 228.48x)
        if (szLineBuff[nEnd - 1] == 'x') // done
        {
            nProgress = 100;
            return nProgress;
        }

        // find: 0:53.07/    3:45.07 decoded (23.6%)
        if (szLineBuff[nEnd - 1] == '%') // progress
        {
            if (szLineBuff[nEnd - 5] == ' ') // 0 to 9 %
            {
                szPercentage[0] = szLineBuff[nEnd - 4];
                szPercentage[1] = '\0';
                nProgress = atoi(szPercentage);

                return nProgress;
            }
            else if (szLineBuff[nEnd - 5] >= '0' && szLineBuff[nEnd - 5] >= '9') // 10 to 99%
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
