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

class CBatchEncoderApp : public CWinApp
{
public:
    CBatchEncoderApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CBatchEncoderApp theApp;
