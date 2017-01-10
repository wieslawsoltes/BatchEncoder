// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

// get LAME histogram data from console for VBR mode

// NOTE: init LAME_ENC_HISTOGRAM struct with zeros, sample code:
//
//  LAME_ENC_HISTOGRAM lehData;
//  memset(&lehData, 0x00, sizeof(LAME_ENC_HISTOGRAM));
//  int nRet = GetHistogram_LameEnc(szLineBuff, nLineLen, &lehData);
//  if(nRet == 0) { ... } // TRUE
//  else { ... } // FALSE

// number of LAME histogram columns
#define HISTOGRAM_COLUMNS 14

typedef struct TLAME_ENC_HISTOGRAM
{
    int nCount;
    int nBitrate[HISTOGRAM_COLUMNS];
    int nBitrateValue[HISTOGRAM_COLUMNS];
    int nCountAsterisk[HISTOGRAM_COLUMNS];
    int nCountPercent[HISTOGRAM_COLUMNS];
} LAME_ENC_HISTOGRAM, *PLAME_ENC_HISTOGRAM;

int GetHistogram_LameEnc(char *szLineBuff, int nLineLen, PLAME_ENC_HISTOGRAM plehData);
