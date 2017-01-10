// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "GetProgress.h"

int GetProgress(char *szLineBuff, int nLineLen)
{
    int j;
    int nPos = -1;
    char szPercentage[32];
    int nProgress = -1;

    memset(szPercentage, 0x00, sizeof(szPercentage));

    // find % (percentage) char
    for (j = 0; j < (int)nLineLen; j++)
    {
        if (szLineBuff[j] == '%')
        {
            nPos = j;
            break;
        }
    }

    // find: Decoding
    if (nLineLen > 8)
    {
        if (szLineBuff[0] == 'D' && szLineBuff[1] == 'e' && szLineBuff[2] == 'c')
        {
            nProgress = 100;
            return nProgress;
        }
    }

    if (nPos == 1) // 0 to 9 %
    {
        szPercentage[0] = szLineBuff[0];
        szPercentage[1] = '\0';

        nProgress = atoi(szPercentage);
        return nProgress;
    }
    else if (nPos == 2) // 10 to 99 %
    {
        szPercentage[0] = szLineBuff[0];
        szPercentage[1] = szLineBuff[1];
        szPercentage[2] = '\0';

        nProgress = atoi(szPercentage);
        return nProgress;
    }
    else if (nPos == 3) // 100 %
    {
        nProgress = 100;
        return nProgress;
    }

    return -1;
}
