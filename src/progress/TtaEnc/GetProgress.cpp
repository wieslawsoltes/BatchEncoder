// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "GetProgress.h"

int GetProgress(char *szLineBuff, int nLineLen)
{
    // NOTE:
    // using my patched console encoder/decoder ttaenc.exe 3.3
    // added fflush(...) after Encode/Decode fprintf(...) calls in ttaenc project
    // because of delayed output from original console encoder/decoder
    // and added UnicoWS support to ttaenc for Win9x compatibility

    // NOTE: this is same code as FlacEnc

    // search for:
    // 'Encode:  wrote 6090426 bytes, 27% complete, ratio: 0,64, time: 1'
    // 'Encode:  wrote 25027712 bytes, done, ratio: 0,73, time: 3'

    int j;
    int nPos = 0;
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

    if (nPos >= 3)
    {
        if (szLineBuff[nPos - 3] == ' ') // not a 100.0 %
        {
            if (szLineBuff[nPos - 2] == ' ') // 0 to 9 %
            {
                szPercentage[0] = szLineBuff[nPos - 1];
                szPercentage[1] = '\0';

                nProgress = atoi(szPercentage);
                return nProgress;
            }
            else if (szLineBuff[nPos - 2] >= '0' && szLineBuff[nPos - 2] <= '9') // 10 to 99 %
            {
                szPercentage[0] = szLineBuff[nPos - 2];
                szPercentage[1] = szLineBuff[nPos - 1];
                szPercentage[2] = '\0';

                nProgress = atoi(szPercentage);
                return nProgress;
            }
        }
        else if (szLineBuff[nPos - 3] == '1') // 100.0 %
        {
            nProgress = 100;
            return nProgress;
        }
    }

    return -1;
}
