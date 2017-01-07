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

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "ResizeDialog.h"
#include "LListFiles.h"
#include "BatchEncoderDlg.h"
#include "PresetsDlg.h"
#include "FormatsDlg.h"
#include "AboutDlg.h"
#include "CopyFileDlg.h"
#include "CommandLine.h"
#include "OptionsParser.h"
#include "NewItemData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum CL_OPTIONS_CONST
{    
    CLOP_SETTINGS,
    CLOP_CURRENT_FORMAT,
    CLOP_CURRENT_PRESET,
    CLOP_LOAD_PRESETS,
    CLOP_LOAD_FORMATS,
    CLOP_START_CONVERSION,
    CLOP_STOP_CONVERSION,
    CLOP_SINGLE_INSTANCE,
    CLOP_MINIMIZE,
    CLOP_PROGRESS_TYPE,
    CLOP_SHUTDOWN,
    CLOP_STOP_ON_ERROR,
    CLOP_DELETE_ON_ERROR,
    CLOP_TRAY_ICON,
    CLOP_SAVE_SETTINGS,
    CLOP_PRIORITY_THREAD,
    CLOP_PRIORITY_PROCESS,
    CLOP_PRIORITY_THREAD_TOOL,
    CLOP_PRIORITY_PROCESS_TOOL,
    CLOP_LOG_CONSOLE_OUTPUT,
    CLOP_LOG_FILE,
    CLOP_LOG_FILE_FORMAT,
    CLOP_RECURSE_DIRS,
    CLOP_ADD_FILE,
    CLOP_ADD_DIR,
    CLOP_LOAD_LIST,
    CLOP_SAVE_LIST,
    CLOP_BATCH_FILE,
    CLOP_OUTPUT_PATH,
    CLOP_CONFIGURE_PRESETS,
    CLOP_CONFIGURE_FORMATS,
    CLOP_COPYFILE,
    CLOP_WEBSITE,
    CLOP_ABOUT,
    CLOP_HELP,
};

const COptionsParser::CL_OPTIONS clOptions[] = 
{
    { _T("settings"),                     CLOP_SETTINGS,                  1,  true  },
    { _T("current-format;scf"),           CLOP_CURRENT_FORMAT,            1,  true  },
    { _T("current-preset;scp"),           CLOP_CURRENT_PRESET,            1,  true  },
    { _T("load-presets;ldp"),             CLOP_LOAD_PRESETS,              1,  true  },
    { _T("load-formats;ldf"),             CLOP_LOAD_FORMATS,              1,  true  },
    { _T("start-conversion;stc"),         CLOP_START_CONVERSION,          0,  false },
    { _T("stop-conversion;spc"),          CLOP_STOP_CONVERSION,           0,  false },
    { _T("single-instance;i;si"),         CLOP_SINGLE_INSTANCE,           0,  false },
    { _T("minimize;m;min"),               CLOP_MINIMIZE,                  0,  false },
    { _T("progress-type;t;pt"),           CLOP_PROGRESS_TYPE,             1,  true  },
    { _T("shutdown;sd"),                  CLOP_SHUTDOWN,                  1,  false },
    { _T("stop-on-error;soe"),            CLOP_STOP_ON_ERROR,             1,  false },
    { _T("delete-on-error;doe"),          CLOP_DELETE_ON_ERROR,           1,  false },
    { _T("tray-icon;ti"),                 CLOP_TRAY_ICON,                 1,  false },
    { _T("save-settings;ss"),             CLOP_SAVE_SETTINGS,             1,  false },
    { _T("priority-thread;pt"),           CLOP_PRIORITY_THREAD,           1,  true  },
    { _T("priority-process;pp"),          CLOP_PRIORITY_PROCESS,          1,  true  },
    { _T("priority-thread-tool;ptt"),     CLOP_PRIORITY_THREAD_TOOL,      1,  true  },
    { _T("priority-process-tool;ppt"),    CLOP_PRIORITY_PROCESS_TOOL,     1,  true  },
    { _T("log-console-output;lco"),       CLOP_LOG_CONSOLE_OUTPUT,        1,  false },
    { _T("log-file;lf"),                  CLOP_LOG_FILE,                  1,  true  },
    { _T("log-file-format;lff"),          CLOP_LOG_FILE_FORMAT,           1,  true  },
    { _T("recurse-dirs;r;rd"),            CLOP_RECURSE_DIRS,              1,  false },
    { _T("add-file;f;af"),                CLOP_ADD_FILE,                  3,  true  },
    { _T("add-dir;d;ad"),                 CLOP_ADD_DIR,                   3,  true  },
    { _T("load-list;l;ll"),               CLOP_LOAD_LIST,                 1,  true  },
    { _T("save-list;s;sl"),               CLOP_SAVE_LIST,                 1,  true  },
    { _T("batch-file;b;bf"),              CLOP_BATCH_FILE,                1,  true  },
    { _T("output-path;o;op"),             CLOP_OUTPUT_PATH,               1,  true  },
    { _T("configure-presets;cp"),         CLOP_CONFIGURE_PRESETS,         1,  false },
    { _T("configure-formats;cf"),         CLOP_CONFIGURE_FORMATS,         1,  false },
    { _T("copyfile;c"),                   CLOP_COPYFILE,                  2,  true  },
    { _T("website;w"),                    CLOP_WEBSITE,                   0,  false },
    { _T("about;a"),                      CLOP_ABOUT,                     0,  false },
    { _T("help;h;?"),                     CLOP_HELP,                      0,  false },
    { NULL,                               0,                              0,  false }
};

BEGIN_MESSAGE_MAP(CBatchEncoderApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CBatchEncoderApp::CBatchEncoderApp()
{

}

CBatchEncoderApp theApp;

BOOL CBatchEncoderApp::InitInstance()
{
    // basic program initialization
    InitCommonControls();
    CWinApp::InitInstance();

    // main window object
    CBatchEncoderDlg dlg;

    // command-line help variables
    int nNumArgs = 1;
    LPTSTR *pArgv = NULL;
    LPTSTR lpCommandLine = NULL;
    volatile bool bExit = false;
    int nCountOptions = 0;
    BATCHENCODER_OPTIONS fo;
    COptionsParser op;

    // clean options flags
    ZeroMemory(&fo, sizeof(BATCHENCODER_OPTIONS));

    // get command line form system
    lpCommandLine = ::GetCommandLine();

    // get command line in *argv[] format
    if(lpCommandLine != NULL)
        pArgv = MyCommandLineToArgv(lpCommandLine, &nNumArgs);

    // if there is only one arg then free memory
    if((nNumArgs == 1) && (pArgv != NULL))
        ::GlobalFree(pArgv);

    // parse the command-line specified by user
    if(op.Init(nNumArgs, pArgv, clOptions) == true)
    {
        int nCurFormat = -1;
        int nCurPreset = -1;
        bool bRecurse = true;

        CString szParam;
        int nOptionId;
        while((nOptionId = op.Next()) != COptionsParser::NEXT_LAST_OPTION)
        {
            bExit = false;

            switch(nOptionId)
            {
            case COptionsParser::NEXT_INVALID_ARG:
                {
                    // show user the fatal error message
                    MessageBox(NULL, 
                        _T("Invalid command-line arguments!"), 
                        _T("ERROR"), 
                        MB_OK | MB_ICONERROR);

                    bExit = true;
                }
                break;
            case CLOP_SETTINGS:
                {
                    op.GetParam(szParam, 0);

                    // TODO:
                    // szMainConfigFile = szParam;

                    fo.bHaveMainConfig = true;
                }
                break;
            case CLOP_CURRENT_FORMAT:
                {
                    op.GetParam(szParam, 0);

                    if(CLListFiles::IsValidOutExtension(szParam) == true)
                        nCurFormat = CLListFiles::GetOutFormatIndex(szParam);
                    else
                        nCurFormat = stoi(szParam);

                    fo.bHaveCurrentFormat = true;
                }
                break;
            case CLOP_CURRENT_PRESET:
                {
                    op.GetParam(szParam, 0);
                    nCurPreset = stoi(szParam);

                    fo.bHaveCurrentPreset = true;
                }
                break;
            case CLOP_LOAD_PRESETS:
                {
                    // TODO:

                    fo.bHavePresets = true;
                }
                break;
            case CLOP_LOAD_FORMATS:
                {
                    // TODO:

                    fo.bHaveFormats = true;
                }
                break;
            case CLOP_START_CONVERSION:
                {
                    // TODO:

                    fo.bHaveStartConversion = true;
                }
                break;
            case CLOP_STOP_CONVERSION:
                {
                    // TODO:

                    fo.bHaveStopConversion = true;
                }
                break;
            case CLOP_SINGLE_INSTANCE:
                {
                    // TODO:

                    fo.bHaveSingleInstance = true;
                }
                break;
            case CLOP_MINIMIZE:
                {
                    // TODO:

                    fo.bHaveMinimize = true;
                }
                break;
            case CLOP_PROGRESS_TYPE:
                {
                    // TODO:

                    fo.bHaveProgressType = true;
                }
                break;
            case CLOP_SHUTDOWN:
                {
                    // TODO:

                    // NOTE: param is not required here
                    /*
                    if(op.GetParam(szParam, 0) == true)
                    {

                    {
                    else
                    {

                    }
                    */

                    fo.bHaveShutdown = true;
                }
                break;
            case CLOP_STOP_ON_ERROR:
                {
                    // TODO:

                    fo.bHaveStopOnError = true;
                }
                break;
            case CLOP_DELETE_ON_ERROR:
                {
                    // TODO:

                    fo.bHaveDeleteOnError = true;
                }
                break;
            case CLOP_TRAY_ICON:
                {
                    // TODO:

                    fo.bHaveTrayIcon = true;
                }
                break;
            case CLOP_SAVE_SETTINGS:
                {
                    // TODO:

                    fo.bHaveSaveSettings = true;
                }
                break;
            case CLOP_PRIORITY_THREAD:
                {
                    // TODO:

                    fo.bHavePriorityThread = true;
                }
                break;
            case CLOP_PRIORITY_PROCESS:
                {
                    // TODO:

                    fo.bHavePriorityProcess = true;
                }
                break;
            case CLOP_PRIORITY_THREAD_TOOL:
                {
                    // TODO:

                    fo.bHavePriorityThreadTool = true;
                }
                break;
            case CLOP_PRIORITY_PROCESS_TOOL:
                {
                    // TODO:

                    fo.bHavePriorityProcessTool = true;
                }
                break;
            case CLOP_LOG_CONSOLE_OUTPUT:
                {
                    // TODO:

                    fo.bHaveLogConsoleOutput = true;
                }
                break;
            case CLOP_LOG_FILE:
                {
                    // TODO:

                    fo.bHaveLogFile = true;
                }
                break;
            case CLOP_LOG_FILE_FORMAT:
                {
                    // TODO:

                    fo.bHaveLogFileFormat = true;
                }
                break;
            case CLOP_RECURSE_DIRS:
                {
                    op.GetParam(szParam, 0);

                    if(szParam.CompareNoCase(_T("on")) == 0)
                        bRecurse = true;
                    else if(szParam.CompareNoCase(_T("off")) == 0)
                        bRecurse = false;
                    else
                        bRecurse = true; // default: on

                    fo.bHaveRecurseDirs = true;
                }
                break;
            case CLOP_ADD_FILE:
                {
                    int nFormat;
                    int nPreset;
                    CString szBuff;

                    op.GetParam(szParam, 0);
                    szBuff = szParam;

                    op.GetParam(szParam, 1);

                    if(CLListFiles::IsValidOutExtension(szParam) == true)
                    {
                        nFormat = CLListFiles::GetOutFormatIndex(szParam);
                    }
                    else
                    {
                        if(szParam.CompareNoCase(_T("X")) == 0)
                            nFormat = -1;
                        else
                            nFormat = stoi(szParam);

                        if((fo.bHaveCurrentFormat == true) && (nFormat == -1))
                            nFormat = nCurFormat;
                    }

                    op.GetParam(szParam, 2);
                    if(szParam.CompareNoCase(_T("X")) == 0)
                        nPreset = -1;
                    else
                        nPreset = stoi(szParam);

                    if((fo.bHaveCurrentPreset == true) && (nPreset == -1))
                        nPreset = nCurPreset;

                    NewItemData nid;
                    ::InitNewItemData(nid);

                    nid.nAction = 0;
                    nid.szFileName = szBuff;
                    nid.nItem = -1;
                    nid.szName = _T("");
                    nid.szOutExt = ::g_szAllOutExt[nFormat];
                    nid.nPreset = nPreset;

                    dlg.InsertToList(nid);

                    fo.bHaveFileList = true;
                }
                break;
            case CLOP_ADD_DIR:
                {
                    int nFormat;
                    int nPreset;
                    CString szBuff;

                    op.GetParam(szParam, 0);
                    szBuff = szParam;

                    op.GetParam(szParam, 1);

                    if(CLListFiles::IsValidOutExtension(szParam) == true)
                    {
                        nFormat = CLListFiles::GetOutFormatIndex(szParam);
                    }
                    else
                    {
                        if(szParam.CompareNoCase(_T("X")) == 0)
                            nFormat = -1;
                        else
                            nFormat = stoi(szParam);

                        if((fo.bHaveCurrentFormat == true) && (nFormat == -1))
                            nFormat = nCurFormat;
                    }

                    op.GetParam(szParam, 2);
                    if(szParam.CompareNoCase(_T("X")) == 0)
                        nPreset = -1;
                    else
                        nPreset = stoi(szParam);

                    if((fo.bHaveCurrentPreset == true) && (nPreset == -1))
                        nPreset = nCurPreset;

                    dlg.SearchFolderForFiles(szBuff, 
                        bRecurse, 
                        ::g_szAllOutExt[nFormat], 
                        nPreset);

                    fo.bHaveFileList = true;
                }
                break;
            case CLOP_LOAD_LIST:
                {
                    op.GetParam(szParam, 0);

                    dlg.LoadList(szParam, false);

                    fo.bHaveFileList = true;
                }
                break;
            case CLOP_SAVE_LIST:
                {
                    if(fo.bHaveFileList == true)
                    {
                        op.GetParam(szParam, 0);

                        dlg.SaveList(szParam, false);

                        fo.bHaveSaveFiles = true;
                    }

                    bExit = true;
                }
                break;
            case CLOP_BATCH_FILE:
                {
                    if(fo.bHaveFileList == true)
                    {
                        op.GetParam(szParam, 0);

                        dlg.CreateBatchFile(szParam, false);

                        fo.bHaveBatchFile = true;
                    }

                    bExit = true;
                }
                break;
            case CLOP_OUTPUT_PATH:
                {
                    // TODO:
                }
                break;
            case CLOP_CONFIGURE_PRESETS:
                {
                    CPresetsDlg dlg;

                    dlg.bStandAlone = true;
                    m_pMainWnd = &dlg;

                    if(op.GetParam(szParam, 0) == true)
                        dlg.szStandAloneFile = szParam;

                    dlg.DoModal();

                    bExit = true;
                }
                break;
            case CLOP_CONFIGURE_FORMATS:
                {
                    CFormatsDlg dlg;

                    // TODO:
                    // dlg.bStandAlone = true;
                    m_pMainWnd = &dlg;

                    // TODO:
                    // if(op.GetParam(szParam, 0) == true)
                    //     dlg.szStandAloneFile = szParam;

                    dlg.DoModal();

                    bExit = true;
                }
                break;
            case CLOP_COPYFILE:
                {
                    CCopyFileDlg dlg;

                    op.GetParam(szParam, 0);
                    dlg.szInFile = szParam;

                    op.GetParam(szParam, 1);
                    dlg.szOutFile = szParam;

                    dlg.bNoFiles = false;
                    m_pMainWnd = &dlg;
                    dlg.DoModal();

                    bExit = true;
                }
                break;
            case CLOP_WEBSITE:
                {
                    ::LaunchAndWait(MAIN_APP_WEBSITE, _T(""), FALSE);

                    bExit = true;
                }
                break;
            case CLOP_ABOUT:
                {
                    CAboutDlg dlg;
                    m_pMainWnd = &dlg;
                    dlg.DoModal();

                    bExit = true;
                }
                break;
            case CLOP_HELP:
                {
                    ::UpdatePath();
                    ::LaunchAndWait(MAIN_APP_HELP, _T(""), FALSE);

                    bExit = true;
                }
                break;
            default:
                {
                    // unknown option Id
                    bExit = true;
                }
                break;
            };

            if(bExit == true)
            {
                // free used command-line memory
                ::GlobalFree(pArgv);

                return FALSE;
            }

            nCountOptions++;
        };

        // free used command-line memory
        ::GlobalFree(pArgv);
    }

    // show main program dialog window
    m_pMainWnd = &dlg;

    dlg.m_pFo = &fo;
    dlg.DoModal();

    // terminate program execution
    return FALSE;
}
