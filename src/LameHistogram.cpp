//
// BatchEncoder (Audio Conversion GUI)
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

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "LameHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TODO: 
// - check if LAME is showing histogram,
//   first output lines shows some info,
//   or check command-line args for vbr stuff
int GetHistogram_LameEnc(char *szLineBuff, int nLineLen, PLAME_ENC_HISTOGRAM plehData)
{
    // search for histogram data
    // NOTE: LAME 3.97 Beta 2

    char szBuff[64];

    // position of closing ']' is hard-coded
    if (nLineLen >= 10)
    {
        // NOTE: if we get starting '[' then we don't need
        //       more checks, it's only in one case that '[' occurs

        // check for starting '[', position is hard-coded
        if (szLineBuff[4] == '[')
        {
            szBuff[0] = szLineBuff[0];
            szBuff[1] = szLineBuff[1];
            szBuff[2] = szLineBuff[2];
            szBuff[3] = '\0';

            plehData->nBitrate[plehData->nCount] = atoi(szBuff);

            // find closing ']'
            //
            // NOTE: start of search position is hard-coded to 5th char
            //       if you get errors check LAME output
            //       and change this position in code below 
            int i;
            for (i = 5; i < nLineLen; i++)
            {
                if (szLineBuff[i] == ']')
                    break;
            }

            memcpy(szBuff, szLineBuff + 5, i - 5);
            szBuff[i - 5] = '\0';

            plehData->nBitrateValue[plehData->nCount] = atoi(szBuff);

            plehData->nCountAsterisk[plehData->nCount] = 0;
            plehData->nCountPercent[plehData->nCount] = 0;

            // count asterisk and percent chars
            // NOTE: after ']' is one char break 
            //       i = position of ']'
            //       i + 1 = break char ' '
            //       i + 2 = start of Asterisk and Percent chars
            for (int j = i + 2; j < nLineLen; j++)
            {
                if (szLineBuff[j] == '*')
                    plehData->nCountAsterisk[plehData->nCount]++;

                if (szLineBuff[j] == '%')
                    plehData->nCountPercent[plehData->nCount]++;
            }

            // NOTE: for heavy DEBUG output uncomment TRACE(...) lines
            /*
            TRACE("%s\n", szLineBuff);
            TRACE("%02d: ", plehData->nCount);
            TRACE("%3d ", plehData->nBitrate[plehData->nCount]);
            TRACE("[%8d] ", plehData->nBitrateValue[plehData->nCount]);
            TRACE("(*=%d | ", plehData->nCountAsterisk[plehData->nCount]);
            TRACE("%%=%d)\n", plehData->nCountPercent[plehData->nCount]);
            */

            plehData->nCount++;
            if (plehData->nCount == HISTOGRAM_COLUMNS)
                plehData->nCount = 0;

            return 0;
        }
    }

    return -1;
}
