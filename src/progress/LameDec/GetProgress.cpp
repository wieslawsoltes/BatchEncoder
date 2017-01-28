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
    for (j = 0; j < (int)nLineLen; j++)
    {
        if (szLineBuff[j] == '/')
        {
            nPos = j;
            break;
        }
    }

    if (nPos != 0)
    {
        // find first space before separator
        for (j = nPos; j > 0; j--)
        {
            if (szLineBuff[j] == ' ')
            {
                nStart = j;
                break;
            }
        }

        // find first space after separator
        for (j = nPos; j < (int)nLineLen; j++)
        {
            if (szLineBuff[j] == ' ')
            {
                nEnd = j;
                break;
            }
        }

        strncpy_s(szBuff, (char *)(szLineBuff + nStart + 1), nPos - nStart - 1);
        szBuff[(nPos - nStart) - 1] = '\0';
        nCurFrame = atoi(szBuff);

        strncpy_s(szBuff, (char *)(szLineBuff + nPos + 1), nEnd - nPos - 1);
        szBuff[(nEnd - nPos) - 1] = '\0';
        nTotalFrames = atoi(szBuff);

        if (nCurFrame == 0 || nTotalFrames == 0)
            nProgress = 0;
        else
            nProgress = (100 * nCurFrame) / nTotalFrames;

        return nProgress;
    }

    return -1;
}
