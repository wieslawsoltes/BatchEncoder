//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
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

/* $Id: LameHistogram.h 437 2008-03-24 12:15:45Z wisodev $ */

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
