// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef __AFXWIN_H__
#error "Include 'StdAfx.h' before including this file for PCH"
#endif // __AFXWIN_H__

#include "res\resource.h"

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
