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

/* $Id: BatchEncoder.h 437 2008-03-24 12:15:45Z wisodev $ */

#pragma once

#ifndef __AFXWIN_H__
    #error "Include 'StdAfx.h' before including this file for PCH"
#endif // __AFXWIN_H__

#include "..\res\resource.h"

// main program command-line configuration flags
typedef struct TBATCHENCODER_OPTIONS
{
    bool bHaveMainConfig;
    bool bHaveCurrentPreset;
    bool bHaveCurrentFormat;
    bool bHaveFileList;
    bool bHavePresets;
    bool bHaveFormats;
    bool bHaveStartConversion;
    bool bHaveStopConversion;
    bool bHaveSingleInstance;
    bool bHaveMinimize;
    bool bHaveProgressType;
    bool bHaveShutdown;
    bool bHaveStopOnError;
    bool bHaveDeleteOnError;
    bool bHaveTrayIcon;
    bool bHaveSaveSettings;
    bool bHavePriorityThread;
    bool bHavePriorityProcess;
    bool bHavePriorityThreadTool;
    bool bHavePriorityProcessTool;
    bool bHaveLogConsoleOutput;
    bool bHaveLogFile;
    bool bHaveLogFileFormat;
    bool bHaveRecurseDirs;
    bool bHaveSaveFiles;
    bool bHaveBatchFile;
    bool bHaveOutputPath;
} BATCHENCODER_OPTIONS, *PBATCHENCODER_OPTIONS;

// NOTE: undefine to enable memory leaks checking in debug builds
// #define MEMORY_LEAK_CHECKS

/*
    // NOTE: use macros placed below for memory leaks detection:
    LEAK_INIT
    // NOTE: add code here
    LEAK_CHECK(_T("FunctionName"))
    // NOTE: usage is allowed only once per function
*/

#ifdef MEMORY_LEAK_CHECKS
#ifdef _DEBUG
    #define LEAK_INIT \
        AfxEnableMemoryTracking(TRUE); \
        CMemoryState oldMemState, newMemState, diffMemState; \
        oldMemState.Checkpoint();
    #define LEAK_CHECK(x) \
        newMemState.Checkpoint(); \
        if(diffMemState.Difference(oldMemState, newMemState)) \
        { \
            TRACE(_T("Memory leaked: %s\n"), x); \
            diffMemState.DumpStatistics(); \
        }
#else
    #define LEAK_INIT
    #define LEAK_CHECK(x)
#endif // _DEBUG
#endif // MEMORY_LEAK_CHECKS

class CBatchEncoderApp : public CWinApp
{
public:
    CBatchEncoderApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CBatchEncoderApp theApp;
