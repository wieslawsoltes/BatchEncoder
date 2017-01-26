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

    // check for percentage progress from console output (stdout)
    // '( 6%)'

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
        if ((nEnd - nStart) == 3)
        {
            szPercentage[0] = szLineBuff[nStart + 1];
            szPercentage[1] = '\0';
            nProgress = atoi(szPercentage);

            return nProgress;
        }
        else if ((nEnd - nStart) == 4)
        {
            szPercentage[0] = szLineBuff[nStart + 1];
            szPercentage[1] = szLineBuff[nStart + 2];
            szPercentage[2] = '\0';
            nProgress = atoi(szPercentage);

            return nProgress;
        }
        else if ((nEnd - nStart) == 5)
        {
            szPercentage[0] = szLineBuff[nStart + 1];
            szPercentage[1] = szLineBuff[nStart + 2];
            szPercentage[2] = szLineBuff[nStart + 3];
            szPercentage[3] = '\0';
            nProgress = atoi(szPercentage);

            return nProgress;
        }
    }

    return -1;
}
