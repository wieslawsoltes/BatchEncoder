// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "GetProgress.h"

int GetProgress(char *szLineBuff, int nLineLen)
{
    // NOTE: using VorbisTools decoder

    char szPercentage[32];
    int nProgress = -1;
    int nStart;
    int nEnd;
    int j;

    memset(szPercentage, 0x00, sizeof(szPercentage));

    if (nLineLen >= 4)
    {
        nStart = 0;
        nEnd = 0;

        // find: [  3,2%]
        for (j = 0; j < (int)nLineLen; j++)
        {
            if (szLineBuff[j] == '[')
            {
                nStart = j;
            }
            if (szLineBuff[j] == ']')
            {
                nEnd = j;
                break;
            }
        }

        if (nEnd > 0)
        {
            if (szLineBuff[nStart + 4] == '.')
            {
                if (szLineBuff[nStart + 1] == ' ') // not a 100.0 %
                {
                    if (szLineBuff[nStart + 2] == ' ') // 0 to 9 %
                    {
                        szPercentage[0] = szLineBuff[nStart + 3];
                        szPercentage[1] = '\0';

                        nProgress = atoi(szPercentage);
                        return nProgress;
                    }
                    else if (szLineBuff[nStart + 2] >= '0' && szLineBuff[nStart + 2] <= '9') // 10 to 99 %
                    {
                        szPercentage[0] = szLineBuff[nStart + 2];
                        szPercentage[1] = szLineBuff[nStart + 3];
                        szPercentage[2] = '\0';

                        nProgress = atoi(szPercentage);
                        return nProgress;
                    }
                }
                else if (szLineBuff[nStart + 1] == '1') // 100.0 %
                {
                    nProgress = 100;
                    return nProgress;
                }
            }
        }
    }

    return -1;
}
