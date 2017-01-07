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
#include "UnicodeUtf8.h"
#include "Utf8String.h"
#include "BatchEncoderDlg.h"
#include "LameHistogram.h"
#include "WorkThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ULONGLONG GetFileSize64(HANDLE hFile)
{
   ULARGE_INTEGER liSize;

   liSize.LowPart = ::GetFileSize(hFile, &liSize.HighPart);
   if(liSize.LowPart == (DWORD) -1)
   {
	  if(::GetLastError() != NO_ERROR)
		 return(0);
   }

	return liSize.QuadPart;
}

DWORD WINAPI ReadThread(LPVOID lpParam)
{
    // NOTE: 4096 bytes is default buffer for pipes (on XP SP2)

    PREAD_DATA pReadData = (PREAD_DATA) lpParam;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE pReadBuff[4096];
    BOOL bRes = FALSE;
    DWORD dwReadBytes = 0;
    DWORD dwWriteBytes = 0;
    ULONGLONG nTotalBytesRead = 0;
    ULONGLONG nFileSize = 0;
    int nProgress = -1;
    bool bRunning = true;

    pReadData->bError = false;
    pReadData->bFinished = false;

    // open existing source file with read-only flag
    hFile = ::CreateFile(pReadData->szFileName, 
        GENERIC_READ, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        0, 
        NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        pReadData->bError = true;
        pReadData->bFinished = true;
        return(1);
    }

    // get the filesize
    nFileSize = GetFileSize64(hFile);

    // nothing todo with 0 bytes file
    if(nFileSize == 0)
    {
        pReadData->bError = true;
        pReadData->bFinished = true;
        ::CloseHandle(hFile);
        return(1);
    }

    // read/write loop
    do
    {
        // read data from source file
        bRes = ::ReadFile(hFile, pReadBuff, 4096, &dwReadBytes, 0);
        if((bRes == FALSE) || (dwReadBytes == 0))
            break;

        // NOTE: Sleep(0) solves problem writing to pipe error (MPPDEC)
        ::Sleep(0);

        // LastError(_T("ReadThread:ReadFile"));

        // write data to write pipe
        bRes = ::WriteFile(pReadData->hPipe, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        // LastError(_T("ReadThread:WriteFile"));

        // NOTE: this is not used
        // ::FlushFileBuffers(pReadData->wPipe);

        // count read/write bytes
        nTotalBytesRead += dwReadBytes;

        nProgress = (int) ((nTotalBytesRead * 100) / nFileSize);

        bRunning = pReadData->pDlg->WorkerCallback(nProgress, false);
        if(bRunning == false)
            break;
    }
    while(bRes != FALSE);

    // clean up memory
    ::CloseHandle(hFile);

    // check if all data was processed
    if(nTotalBytesRead != nFileSize)
    {
        pReadData->bError = true;
        pReadData->bFinished = true;
        return(1);
    }

    pReadData->bError = false;
    pReadData->bFinished = true;
    return(0);
}

DWORD WINAPI WriteThread(LPVOID lpParam)
{
    // NOTE: 4096 bytes is default buffer for pipes (on XP SP2)

    PREAD_DATA pReadData = (PREAD_DATA) lpParam;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE pReadBuff[4096];
    BOOL bRes = FALSE;
    DWORD dwReadBytes = 0;
    DWORD dwWriteBytes = 0;
    ULONGLONG nTotalBytesWrite = 0;
    ULONGLONG nFileSize = 0;
    int nProgress = -1;

    pReadData->bError = false;
    pReadData->bFinished = false;

    // open existing source file with readpnly flag
    hFile = ::CreateFile(pReadData->szFileName, 
        GENERIC_READ | GENERIC_WRITE, 
        0, 
        NULL, 
        CREATE_ALWAYS, 
        0, 
        NULL);
    if(hFile == INVALID_HANDLE_VALUE)
    {
        pReadData->bError = true;
        pReadData->bFinished = true;
        return(1);
    }

    // read/write loop
    do
    {
        // read data from source pipe
        bRes = ::ReadFile(pReadData->hPipe, pReadBuff, 4096, &dwReadBytes, 0);
        if((bRes == FALSE) || (dwReadBytes == 0))
            break;

        // LastError(_T("WriteThread:ReadFile"));

        // write data to file
        bRes = ::WriteFile(hFile, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        // LastError(_T("WriteThread:WriteFile"));

        // NOTE: this is not used
        // ::FlushFileBuffers(pReadData->wPipe);

        // count read/write bytes
        nTotalBytesWrite += dwReadBytes;

        // handle user Stop
        if(pReadData->pDlg->bRunning == false)
            break;
    }
    while(bRes != FALSE);

    // clean up memory
    ::CloseHandle(hFile);

    pReadData->bError = false;
    pReadData->bFinished = true;
    return(0);
}

bool ConvertFile(CBatchEncoderDlg *pDlg, 
                 CString szInputFile, 
                 CString szOutputFile,
                 TCHAR *szCommandLine, 
                 int nIndex, 
                 bool bDecode, 
                 int nTool,
                 bool bUseReadPipes,
                 bool bUseWritePipes)
{
    // TODO:
    // if there is no input pipes and output pipes are enabled
    // we can not get progress status for out ProgressBars
    // so for encoder/decoder mode treat this as an error
    // and for transcoding thread this as decoder to encoder piping

    // TODO: handle bUseWritePipes flag mostly like bUseReadPipes

    // NOTE: bUseReadPipes
    // if true use pipes to read source file and send the data to console stdin
    // if false create full commandline and read from stdout/stderr convertion progress

    // log cosnsole text output
    bool bLogConsoleOutput = false;

    CMenu *mainMenu = pDlg->GetMenu();
    UINT nState = mainMenu->GetMenuState(ID_OPTIONS_LOGCONSOLEOUTPUT, MF_BYCOMMAND);
    if(nState & MF_CHECKED)
        bLogConsoleOutput = true;

    // set the correct security attributes
    SECURITY_ATTRIBUTES secattr; 
    ZeroMemory(&secattr, sizeof(secattr));
    secattr.nLength = sizeof(secattr);
    secattr.bInheritHandle = TRUE;

    HANDLE rOutErrPipe = INVALID_HANDLE_VALUE;
    HANDLE wOutErrPipe = INVALID_HANDLE_VALUE;
    HANDLE rInPipe = INVALID_HANDLE_VALUE;
    HANDLE wInPipe = INVALID_HANDLE_VALUE;
    HANDLE rOutPipe = INVALID_HANDLE_VALUE;
    HANDLE wOutPipe = INVALID_HANDLE_VALUE;

    if((bUseReadPipes == false) && (bUseWritePipes == false))
    {
        // create pipes for stdout and stderr
        BOOL bRet = FALSE;
        bRet = ::CreatePipe(&rOutErrPipe, &wOutErrPipe, &secattr, 0);
        if(bRet == FALSE)
        {
            pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
            return false;
        }
    }
    else
    {
        if(bUseReadPipes == true)
        {
            // create pipes for stdin
            BOOL bRet = FALSE;
            bRet = ::CreatePipe(&rInPipe, &wInPipe, &secattr, 0);
            if(bRet == FALSE)
            {
                pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
                return false;
            }
        }

        if(bUseWritePipes == true)
        {
            // create pipes for stdout
            BOOL bRet = FALSE;
            bRet = ::CreatePipe(&rOutPipe, &wOutPipe, &secattr, 0);
            if(bRet == FALSE)
            {
                if(bUseReadPipes == true)
                {
                    ::CloseHandle(rInPipe);
                    ::CloseHandle(wInPipe);
                }

                pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
                return false;
            }
        }
    }

    // this pipes are not inherited by child process
    // SetHandleInformation used only in NT systems
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&osvi);
    if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        if(bUseReadPipes == true)
        {
            BOOL bRet = FALSE;
            bRet = ::SetHandleInformation(wInPipe, HANDLE_FLAG_INHERIT, 0);
            if(bRet == FALSE)
            {
                ::CloseHandle(rInPipe);
                ::CloseHandle(wInPipe);

                pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
                return false;
            }
        }

        if(bUseWritePipes == true)
        {
            BOOL bRet = FALSE;
            bRet = ::SetHandleInformation(rOutPipe, HANDLE_FLAG_INHERIT, 0);
            if(bRet == FALSE)
            {
                if(bUseWritePipes == true)
                {
                    ::CloseHandle(rInPipe);
                    ::CloseHandle(wInPipe);
                }

                ::CloseHandle(rOutPipe);
                ::CloseHandle(wOutPipe);

                pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
                return false;
            }
        }
    }

    // NOTE: DuplicateHandle prevents child process to close pipe
    if((bUseReadPipes == false) && (bUseWritePipes == false))
    {
        // this could be useful when reading from stderr
        if(::DuplicateHandle(::GetCurrentProcess(), 
            rOutErrPipe, 
            ::GetCurrentProcess(),
            &rOutErrPipe, 
            0, 
            TRUE, 
            DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS) == FALSE)
        {
            ::CloseHandle(rOutErrPipe);
            ::CloseHandle(wOutErrPipe);

            pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
            return false;
        }
    }

    STARTUPINFO sInfo; 
    ZeroMemory(&sInfo, sizeof(sInfo));

    PROCESS_INFORMATION pInfo; 
    ZeroMemory(&pInfo, sizeof(pInfo));

    sInfo.cb = sizeof(sInfo);
    sInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    // TODO: 
    // when read pipes are disabled and write pipes enabled
    // we maybe can get stderr progress from console
    // this was tested not tested with commandline tools

    if((bUseReadPipes == false) && (bUseWritePipes == false))
    {
        sInfo.hStdInput = NULL;
        sInfo.hStdOutput = wOutErrPipe; 
        sInfo.hStdError = wOutErrPipe;
    }
    else
    {
        if((bUseReadPipes == true) && (bUseWritePipes == false))
        {
            sInfo.hStdInput = rInPipe;
            sInfo.hStdOutput = NULL;
            sInfo.hStdError = NULL;
        }
        else if((bUseReadPipes == false) && (bUseWritePipes == true))
        {
            sInfo.hStdInput = NULL;
            sInfo.hStdOutput = wOutPipe;
            sInfo.hStdError = NULL;
        }
        else if((bUseReadPipes == true) && (bUseWritePipes == true))
        {
            sInfo.hStdInput = rInPipe;
            sInfo.hStdOutput = wOutPipe;
            sInfo.hStdError = NULL;
        }
    }

    int nProgress = 0;
    CTimeCount countTime;

    // init convertion time counter
    countTime.InitCounter();

    // and start it right now
    countTime.StartCounter();

    if(pDlg->bForceConsoleWindow == true)
    {
        // in this mode all encoders/decoders are forced 
        // to run in native system console windows
        // all pipes and progress settings are omitted
        int nRet;

        _flushall();
        nRet = ::_tsystem(NULL);
        if((nRet == 0) && (errno == ENOENT))
        {
            // the command interpreter is not found
            nProgress = -1;
        }
        else
        {
            // remove path to tool and leave only raw tool name
            // note that this is complicated method and in future
            // we need to simplify creation of command-line for system
            // or use CreateProcess here instead of using _tsystem
            CString szSysCommandLine = szCommandLine;
            CString szToolName;
            CString szToolPath;
            CString szReplace;

            int nStart = szSysCommandLine.Find('"', 0);
            int nEnd = szSysCommandLine.Find('"', nStart + 1);

            szToolPath = szSysCommandLine;
            szToolPath.Truncate(nEnd);
            szReplace = szToolPath + _T("\"");
            szToolPath.TrimLeft(_T("\""));
            szToolName = ::GetFileName(szToolPath);
            szToolPath = szToolPath.TrimRight(szToolName);
            szToolName.Replace(_T("\""), _T(""));
            szSysCommandLine.Replace(szReplace, szToolName);

            // set tool name to current dir because using only tool .exe name
            ::SetCurrentDirectory(szToolPath);

            // use system(...) stdlib.h/process.h function
            _flushall();
            nRet = ::_tsystem(szSysCommandLine); // szCommandLine
            if(nRet == -1)
            {
                nProgress = -1;
            }
            else
            {
                nProgress = 100;
            }
        }
    }
    else
    {
        // NOTE: lpCurrentDirectory param may be useful when app is loading some dll's
        BOOL bRet = FALSE;
        bRet = ::CreateProcess(0, 
            szCommandLine,
            0,
            0,
            TRUE,
            NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW /* | CREATE_SUSPENDED */,
            0,
            0,
            &sInfo,
            &pInfo);
        if(bRet == FALSE)
        {
            countTime.StopCounter();

            ::CloseHandle(rOutErrPipe);
            ::CloseHandle(wOutErrPipe);

            if(bUseReadPipes == true)
            {
                ::CloseHandle(rInPipe);
                ::CloseHandle(wInPipe);
            }

            if(bUseWritePipes == true)
            {
                ::CloseHandle(rOutPipe);
                ::CloseHandle(wOutPipe);
            }

            pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
            return false;
        }

        // NOTE: save current priority for parent process/thread
        // DWORD dwPriorityClass = ::GetPriorityClass(::GetCurrentProcess());
        // int nPriority = ::GetThreadPriority(::GetCurrentThread());

        // NOTE: set priority for parent process/thread
        // ::SetPriorityClass(::GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
        // ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_NORMAL);

        // set priority for child process/thread
        ::SetPriorityClass(pInfo.hProcess, g_dwProcessPriority[pDlg->nProcessPriorityIndex]);
        ::SetThreadPriority(pInfo.hThread, g_nThreadPriority[pDlg->nThreadPriorityIndex]);

        // close unused pipes handles
        if((bUseReadPipes == false) && (bUseWritePipes == false))
        {
            ::CloseHandle(wOutErrPipe);
        }
        else
        {
            if(bUseReadPipes == true)
                ::CloseHandle(rInPipe);

            if(bUseWritePipes == true)
                ::CloseHandle(wOutPipe);
        }

        bool bWriteSuccess = false;
        READ_DATA rd;
        READ_DATA wd;
        DWORD dwReadThreadID = 0L;
        DWORD dwWriteThreadID = 0L;
        HANDLE hReadThread = NULL;
        HANDLE hWriteThread = NULL;

        if(bUseReadPipes == true)
        {
            // NOTE: get pipe buffer size
            // DWORD dwOutBufferSize, dwInBufferSize, dwMaxInstances;
            // ::GetNamedPipeInfo(wInPipe, NULL, &dwOutBufferSize, &dwInBufferSize, &dwMaxInstances);

            rd.bError = false;
            rd.bFinished = false;
            rd.pDlg = pDlg;
            rd.szFileName = szInputFile;
            rd.hPipe = wInPipe;

            dwReadThreadID = 0L;
            hReadThread = ::CreateThread(NULL,
                0,
                ReadThread,
                (LPVOID) &rd,
                /* CREATE_SUSPENDED */ 0,
                &dwReadThreadID);
            if(hReadThread == NULL)
            {
                countTime.StopCounter();

                ::CloseHandle(wInPipe);

                pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
                return false;
            }

            // ::ResumeThread(pInfo.hThread);
            // ::ResumeThread(hReadThread);

            if(bUseWritePipes == false)
            {
                ::WaitForSingleObject(hReadThread, INFINITE);

                ::CloseHandle(wInPipe);

                // check for result from read thread
                if((rd.bError == false) && (rd.bFinished == true))
                    nProgress = 100;
                else
                    nProgress = -1;

                // close read thread handle
                ::CloseHandle(hReadThread);
            }
        }

        // NOTE:
        // write pipes may cause serious problems
        // because seeking on written file is impossible
        // this means that file headers can not be written

        // NOTE: write pipes are designed for transcoding only

        if(bUseWritePipes == true)
        {
            // NOTE: get pipe buffer size
            // DWORD dwOutBufferSize, dwInBufferSize, dwMaxInstances;
            // ::GetNamedPipeInfo(rInPipe, NULL, &dwOutBufferSize, &dwInBufferSize, &dwMaxInstances);

            wd.bError = false;
            wd.bFinished = false;
            wd.pDlg = pDlg;
            wd.szFileName = szOutputFile;
            wd.hPipe = rOutPipe;

            dwWriteThreadID = 0L;
            hWriteThread = ::CreateThread(NULL,
                0,
                WriteThread,
                (LPVOID) &wd,
                /* CREATE_SUSPENDED */ 0,
                &dwWriteThreadID);
            if(hWriteThread == NULL)
            {
                countTime.StopCounter();

                ::CloseHandle(rOutPipe);

                pDlg->WorkerCallback(-1, true, true, 0.0, nIndex);
                return false;
            }

            // ::ResumeThread(pInfo.hThread);
            // ::ResumeThread(hWriteThread);

            ::WaitForSingleObject(hWriteThread, INFINITE);

            ::CloseHandle(rOutPipe);

            // check for result from read thread
            if((wd.bError == false) && (wd.bFinished == true))
            {
                bWriteSuccess = true;
                if(bUseReadPipes == false)
                    nProgress = 100;
            }
            else
            {
                bWriteSuccess = false;
                if(bUseReadPipes == false)
                    nProgress = -1;
            }

            // close read thread handle
            ::CloseHandle(hWriteThread);
        }

        if((bUseReadPipes == true) && (bUseWritePipes == true))
        {
            ::WaitForSingleObject(hReadThread, INFINITE);

            ::CloseHandle(wInPipe);

            // check for result from read thread
            if((rd.bError == false) && (rd.bFinished == true) && (bWriteSuccess == true))
                nProgress = 100;
            else
                nProgress = -1;

            // close read thread handle
            ::CloseHandle(hReadThread);
        }

        if((bUseReadPipes == false) && (bUseWritePipes == false))
        {
            // ::ResumeThread(pInfo.hThread);

            // NOTE: 
            // is there a need to check console output codepage
            // all apps should be using system codepage
            // or they are using UNICODE output?
            char szReadBuff[512];
            char szLineBuff[512];
            DWORD dwReadBytes = 0L;
            BOOL bRes = FALSE;
            bool bLineStart;
            bool bLineEnd;
            bool bRunning = true;
            int nLineLen = 0;

            // init buffers
            ZeroMemory(szReadBuff, sizeof(szReadBuff));
            ZeroMemory(szLineBuff, sizeof(szLineBuff));

            // NOTE: what we get first start of line or end of line?
            bLineStart = false;
            bLineEnd = false; 

            // LAME histogram data
            LAME_ENC_HISTOGRAM lehData;
            memset(&lehData, 0x00, sizeof(LAME_ENC_HISTOGRAM));

            // create logfile
            CFile fp;
            bool bHaveLogFile = false;

            if(bLogConsoleOutput == true)
            {
                ::UpdatePath();
                if(fp.Open(pDlg->szLogFileName, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate) == TRUE)
                {
                    bHaveLogFile = true;

                    if(fp.GetLength() == (ULONGLONG) 0)
                    {
                        const unsigned char szHeaderUnicode[2] = { 0xFFU, 0xFEU };
                        const unsigned char szHeaderUtf8[3] = { 0xEFU, 0xBBU, 0xBFU };

                        switch(pDlg->nLogEncoding)
                        {
                        case 1: // UNICODE
                            fp.Write(szHeaderUnicode, 2);
                            fp.Flush();
                            break;
                        case 2: // UTF-8
                            fp.Write(szHeaderUtf8, 3);
                            fp.Flush();
                            break;
                        };
                    }

                    fp.SeekToEnd();
                }
            }

            // NOTE: in case we used histogram set flag to erase it after finished
            bool bEraseHistogram = false;

            // NOTE: preload all progress dll's before conversion process

            // load progress function
            typedef int (*lpfnGetProgress)(char *szLineBuff, int nLineLen);

            lpfnGetProgress pProgressProc;
            HMODULE hDll = ::LoadLibrary(pDlg->szFormatFunction[nTool]);
            if(hDll != NULL)
            {
                // _T("GetProgress") == pDlg->szFormatFunction[nTool]

                // NOTE: the GetProcAddress function has only ANSI version
                pProgressProc = (lpfnGetProgress) ::GetProcAddress(hDll, "GetProgress");
                if(pProgressProc == NULL)
                    return false; // ERROR

                // TODO: when failed to load dll we need to do more clean-up
            }

            // main loop
            do
            {
                ZeroMemory(szReadBuff, sizeof(szReadBuff));

                // check if there is something to read, only for debug
                // DWORD dwAvailable;
                // ::PeekNamedPipe(rOutErrPipe, NULL, 0, NULL, &dwAvailable, NULL);

                // NOTE: this is not used
                // ::FlushFileBuffers(rOutErrPipe);

                bRes = ::ReadFile(rOutErrPipe, szReadBuff, 100, &dwReadBytes, 0);
                if(bRes == FALSE || dwReadBytes == 0)
                    break;

                // terminate readed data by '\0'
                szReadBuff[dwReadBytes] = '\0';

                for(int i = 0; i < (int) dwReadBytes; i++)
                {
                    // NOTE: processed escape chars ( \r \n \t \b )
                    if(szReadBuff[i] == '\r') // '\r'
                    {
                        if((bLineStart == true) && (bLineEnd == false))
                        {
                            bLineEnd = true;
                            bLineStart = false;
                            szLineBuff[nLineLen] = '\0';
                        }
                    }
                    else if(szReadBuff[i] == '\n') // '\n'
                    {
                        // do nothing
                    }
                    else if(szReadBuff[i] == '\t') // '\t'
                    {
                        // do nothing
                    }
                    else if(szReadBuff[i] == '\b') // '\b'
                    {
                        // do nothing in most situations

                        // NOTE: special case for wavpack and wvunpack
                        if(((bDecode == false) && (nTool == TOOL_ID_ENC_WAVPACK)) ||
                            ((bDecode == true) && (nTool == TOOL_ID_DEC_WVUNPACK)))
                        {
                            // NOTE: same code as if(szReadBuff[i] == '\r')
                            if((bLineStart == true) && (bLineEnd == false))
                            {
                                bLineEnd = true;
                                bLineStart = false;
                                szLineBuff[nLineLen] = '\0';
                            }
                        }
                    }
                    else if(bLineEnd == false)
                    {
                        bLineStart = true; // for sure we have start
                        nLineLen++;
                        szLineBuff[nLineLen - 1] = szReadBuff[i];
                    }

                    // now we have correct full line of text
                    if((bLineEnd == true) && (bLineStart == false))
                    {
                        // don't include empty lines
                        if(strlen(szLineBuff) > 0)
                        {
                            if(bDecode == false)
                            {
                                // NOTE: histogram works only if pipes are disabled

                                // get LameEnc histogram data and update Histogram control
                                if(nTool == TOOL_ID_ENC_LAME)
                                {
                                    // NOTE: create dll for histogram function and add option to main config file
                                    int nRet = GetHistogram_LameEnc(szLineBuff, nLineLen, &lehData);
                                    if(nRet == 0)
                                    {
                                        if(pDlg->bIsHistogramVisible == true)
                                        {
                                            pDlg->HistogramCallback(&lehData);
                                            bEraseHistogram = true;
                                        }
                                    }
                                }

                                int nRet = (pProgressProc)(szLineBuff, nLineLen);
                                if(nRet != -1)
                                    nProgress = nRet;
                            }
                            else
                            {
                                int nRet = (pProgressProc)(szLineBuff, nLineLen);
                                if(nRet != -1)
                                    nProgress = nRet;
                            }

                            // log all console output for error checking
                            if(bLogConsoleOutput == true)
                            {
                                if((bHaveLogFile == true) 
                                    && (fp.m_hFile != NULL) 
                                    && (strlen(szLineBuff) > (size_t) 0))
                                {
                                    switch(pDlg->nLogEncoding)
                                    {
                                    case 0: // ANSI, default string is ANSI
                                        {
                                            fp.Write(szLineBuff, (UINT) strlen(szLineBuff));
                                            fp.Write("\r\n", 2);
                                            fp.Flush();
                                        }
                                        break;
                                    case 1: // UNICODE
                                        {
                                            wchar_t *pszBuffUnicode;
                                            UnicodeEncode(szLineBuff, &pszBuffUnicode);
                                            fp.Write(pszBuffUnicode, (UINT) wcslen(pszBuffUnicode) * 2);
                                            fp.Write(L"\r\n", 4);
                                            fp.Flush();
                                            free(pszBuffUnicode);
                                        }
                                        break;
                                    case 2: // UTF-8
                                        {
                                            char *pszBuffUtf8;
                                            Utf8Encode(szLineBuff, &pszBuffUtf8);
                                            fp.Write(pszBuffUtf8, (UINT) strlen(pszBuffUtf8));
                                            fp.Write("\r\n", 2);
                                            fp.Flush();
                                            free(pszBuffUtf8);
                                        }
                                        break;
                                    default:
                                        break;
                                    };
                                }
                            }

                            ZeroMemory(szLineBuff, sizeof(szLineBuff));

                            bRunning = pDlg->WorkerCallback(nProgress, false);
                            if(bRunning == false)
                                break;
                        }

                        // reset line counter
                        nLineLen = 0;

                        // now find next start
                        bLineStart = true;
                        bLineEnd = false; 
                    }
                }

                if(bRunning == false)
                    break;
            }
            while(bRes);

            // free memory by unloading unused dll
            if(hDll != NULL)
                ::FreeLibrary(hDll);

            // erase histogram window
            if(bEraseHistogram == true)
                pDlg->m_Histogram.Erase(true);

            // close logfile
            if((bLogConsoleOutput == true) && (bHaveLogFile = true))
            {
                fp.Close();
                bHaveLogFile = false;
            }

            ::CloseHandle(rOutErrPipe);
        }
    }

    // finished, stop convertion time counter
    countTime.StopCounter();

    if(pDlg->bForceConsoleWindow == false)
    {
        // terminate console process if there was error
        HANDLE hProc;
        hProc = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, pInfo.dwProcessId); // PROCESS_TERMINATE
        if(hProc != NULL)
        {
            if(nProgress == 100)
            {
                // when properly finishing we need to wait
                // for process to terminate = 5 seconds max
                // here we are doing this right and clean
                DWORD dwRet = ::WaitForSingleObject(hProc, 2000);
                switch(dwRet)
                {
                case WAIT_FAILED: 
                    // failed, propobly process has finished, but error could be to
                    ::TerminateProcess(hProc, 0);
                    break;
                case WAIT_ABANDONED: 
                    // skip, only for mutex objects
                    break;
                case WAIT_OBJECT_0: 
                    // skip, object is signaled, process has terminated
                    break;
                case WAIT_TIMEOUT: 
                    // time-out interval elapsed
                    // object's state is nonsignaled
                    // used when user had pressed stop button
                    ::TerminateProcess(hProc, 0);
                    break;
                };
            }
            else
            {
                // don't wait for process to terminate becose
                // this code is only executed when user pressed Stop
                ::TerminateProcess(hProc, 0);

                // wait for process to terminate = 5 seconds max
                // this is becose process must release file handles
                // and we need to delete it if there was an error
                ::WaitForSingleObject(hProc, 5000);
            }

            // release process handle
            ::CloseHandle(hProc);
        }

        ::CloseHandle(pInfo.hProcess);
        ::CloseHandle(pInfo.hThread);
    }

    // NOTE: restore parent priority for process/thread
    // ::SetPriorityClass(::GetCurrentProcess(), dwPriorityClass);
    // ::SetThreadPriority(::GetCurrentThread(), nPriority);

    pDlg->WorkerCallback(nProgress, true, false, countTime.GetTime(), nIndex);
    if(nProgress == 100)
        return true;
    else
        return false;
}

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    // NOTE:
    // input and output filenames are UNICODE but
    // we can convert it to OS current codepage using CharToOem
    // this should work if UNICODE chars are in codepage of OS

    ::UpdatePath();

    // get handle of main dialog
    CBatchEncoderDlg *pDlg = (CBatchEncoderDlg *) lpParam;
    if(pDlg == NULL)
        return (DWORD) (-1);

    TCHAR szCommandLine[(64 * 1024)];
    ZeroMemory(szCommandLine, sizeof(szCommandLine));

    // check for delete flag
    bool bDeleteAfterConverion = false;
    if(pDlg->GetMenu()->GetMenuState(ID_OPTIONS_DELETESOURCEFILEWHENDONE, MF_BYCOMMAND) == MF_CHECKED)
        bDeleteAfterConverion = true;
    else
        bDeleteAfterConverion = false;

    // check for output path
    CString szOutPath;
    bool bOutPath;
    pDlg->m_EdtOutPath.GetWindowText(szOutPath);
    if(pDlg->m_ChkOutPath.GetCheck() == BST_CHECKED)
        bOutPath = true;
    else
        bOutPath = false;

    // get number of files in ListView
    int nFiles = pDlg->m_LstInputFiles.GetItemCount();

    // get number of checked files in ListView
    int nTotalFiles = 0;
    int nProcessedFiles = 0;
    int nDoneWithoutError = 0;
    int nErrors = 0;
    for(int i = 0; i < nFiles; i++)
    {
        if(pDlg->m_LstInputFiles.GetCheck(i) == TRUE)
            nTotalFiles++;
    }

    CTimeCount countTime;
    countTime.InitCounter();
    countTime.StartCounter();

    // process all checked files
    for(int i = 0; i < nFiles; i++)
    {
        // get next file name and check if we need to encode/decode/transcode
        if(pDlg->m_LstInputFiles.GetCheck(i) == TRUE)
        {
            // update statusbar conversion status
            nProcessedFiles++;

            nErrors = (nProcessedFiles - 1) - nDoneWithoutError;
            CString szText;
            szText.Format(_T("Processing file %d of %d (%d Done, %d %s)"), 
                nProcessedFiles, 
                nTotalFiles,
                nDoneWithoutError,
                nErrors,
                ((nErrors == 0) || (nErrors > 1)) ? _T("Errors") : _T("Error"));

            VERIFY(pDlg->m_StatusBar.SetText(szText, 1, 0));

            // reset progress bar on start of next file
            pDlg->m_FileProgress.SetPos(0);

            // scroll list to ensure the item is visible
            pDlg->m_LstInputFiles.EnsureVisible(i, FALSE);

            // TODO: when transcoding on decode pass sum the decode+encode passes as one
            //       in the total progressbar calculations

            // processing modes:
            // 0 - encoding - input is WAV file and we only have to encode
            // 1 - decoding - we only have to decode input file to WAV
            // 2 - transcoding - we need to decode input file to encoder stdin stream
            int nProcessingMode = -1;

            // get input file format
            int nIntputFormat = pDlg->m_FileList.GetItemInFormat(i);

            // get output file format
            int nOutputFormat = pDlg->m_FileList.GetItemOutFormat(i);

            // get output preset for selected format
            int nPreset = pDlg->m_FileList.GetItemOutPreset(i);

            // get full file path
            CString szInputFile = pDlg->m_FileList.GetItemFilePath(i);
            
            // output path is same as input file path
            if(bOutPath == false)
            {
                szOutPath = szInputFile;
                CString szToRemove = pDlg->m_FileList.GetFileName(szInputFile);
                int nNewLenght = szOutPath.GetLength() - szToRemove.GetLength();
                szOutPath.Truncate(nNewLenght);
            }

            // setup decoder steps:
            // 1. set encoder exe path
            // 2. set encoder options string
            CString szDecoderExePath;
            CString szDecoderOptions;

            szDecoderExePath = pDlg->GetDecoderExe(nIntputFormat);
            szDecoderOptions = pDlg->GetDecoderOpt(nIntputFormat, -1);

            // get only output filename
            CString szName = pDlg->m_FileList.GetItemFileName(i);

            // setup encoder steps:
            // 1. add extension to output filename
            // 2. set encoder exe path
            // 3. set encoder options string
            CString szEncoderExePath;
            CString szEncoderOptions;
           
            szEncoderExePath = pDlg->GetEncoderExe(nOutputFormat);
            szEncoderOptions = pDlg->GetEncoderOpt(nOutputFormat, nPreset);
            szName = szName + _T(".") + pDlg->m_FileList.GetItemOutExt(i).MakeLower();

            // set full path for output file
            CString szOutputFile;

            if(szOutPath.GetLength() >= 1)
            {
                if(szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/' )
                    szOutputFile = szOutPath + szName;
                else
                    szOutputFile = szOutPath + _T("\\") + szName;
            }
            else
            {
                szOutputFile = szName;
            }

            // set proper processing mode
            nProcessingMode = 1;
            if(nIntputFormat == 0)
                nProcessingMode = 0;

            if(nProcessingMode == 1)
                nProcessingMode = 2;

            // TODO: use CopyOneFile(...) in case 1
            // [1] Input is WAV, [Output is WAV], No Resmapling          = Copy Input File (using SSRC without options)
            // [2] Input is WAV, [Output is WAV], Resmapling             = Encode Input File (using SSRC)
            // [3] Input need decoding, [Output is WAV], No Resampling   = Decode Input File (using Decoder)
            // [4] Input need decoding, [Output is WAV], Resampling      = Decode and Encode (using Decoder and SSRC)
            if(nOutputFormat == 0)
            {
                bool bNeedResampling = (szEncoderOptions.GetLength() > 0) ? true : false;

                // case 1
                if((nIntputFormat == 0) && (bNeedResampling == false))
                    nProcessingMode = 0;

                // case 2
                if((nIntputFormat == 0) && (bNeedResampling == true))
                    nProcessingMode = 0;

                // case 3
                if((nIntputFormat > 0) && (bNeedResampling == false))
                    nProcessingMode = 1;

                // case 4
                if((nIntputFormat > 0) && (bNeedResampling == true))
                    nProcessingMode = 2;
            }

            // build proper commandline depending on processing mode:
            CString csExecute;            
            bool bDecode = false;
            int nTool = -1;
            bool bUseInPipesEnc = false;
            bool bUseOutPipesEnc = false;
            bool bUseInPipesDec = false;
            bool bUseOutPipesDec = false;

            CString szOrgInputFile = szInputFile;
            CString szOrgOutputFile = szOutputFile;

            // decode
            if((nProcessingMode == 1) || (nProcessingMode == 2))
            {
                if(pDlg->bForceConsoleWindow == false)
                {
                    // configure decoder input and output pipes
                    bUseInPipesDec = pDlg->bFormatInput[(NUM_OUTPUT_EXT + nIntputFormat - 1)];
                    bUseOutPipesDec = pDlg->bFormatOutput[(NUM_OUTPUT_EXT + nIntputFormat - 1)];
                }

                // input file is stdin
                if(bUseInPipesDec == true)
                    szInputFile = _T("-");

                // output file is stdout
                if(bUseOutPipesDec == true)
                    szOutputFile = _T("-");

                // TODO:
                // bUseOutPipes == true then handle szOutputFile same as input file
                if(nProcessingMode == 2)
                    szOutputFile = szOutputFile + _T(".wav");

                // TODO: validate the command-line template

                // build full command line for decoder (DECODER-EXE + OPTIONS + INFILE + OUTFILE) 
                // this is basic model, some of encoder may have different commandline structure
                csExecute = pDlg->szFormatTemplate[(NUM_OUTPUT_EXT + nIntputFormat - 1)];
                csExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
                csExecute.Replace(_T("$EXE"), szDecoderExePath);
                csExecute.Replace(_T("$OPTIONS"), szDecoderOptions);
                csExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
                csExecute.Replace(_T("$INFILE"), szInputFile);
                csExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
                csExecute.Replace(_T("$OUTFILE"), szOutputFile);

                bDecode = true;
                nTool = (NUM_OUTPUT_EXT + nIntputFormat - 1);

                lstrcpy(szCommandLine, csExecute.GetBuffer(csExecute.GetLength()));

                pDlg->m_LstInputFiles.SetItemText(i, 5, _T("--:--"));
                pDlg->m_LstInputFiles.SetItemText(i, 6, _T("Decoding..."));

                // update (init) conversion status data
                pDlg->m_CnvStatus.SetCurrentInfo(nProcessedFiles, 
                    nTotalFiles, 
                    nDoneWithoutError, 
                    pDlg->m_FileList.GetFileName(szOrgInputFile), 
                    pDlg->m_FileList.GetFileName(szOrgOutputFile), 
                    szOutPath);

                pDlg->m_CnvStatus.Draw(0);

                // TODO: when decoding in nProcessingMode == 2 don't show time stats
                if(::ConvertFile(pDlg, 
                    szOrgInputFile,
                    szOrgOutputFile,
                    szCommandLine, 
                    i, 
                    bDecode,  
                    nTool, 
                    bUseInPipesDec,
                    bUseOutPipesDec) == true)
                {
                    if(nProcessingMode == 1)
                        nDoneWithoutError++;

                    if(bDeleteAfterConverion == true)
                        ::DeleteFile(szOrgInputFile);

                    if(nProcessingMode == 1)
                        pDlg->m_LstInputFiles.SetCheck(i, FALSE);
                }
                else
                {
                    // delete output file on error
                    if(pDlg->bDeleteOnError == true)
                        ::DeleteFile(szOutputFile);

                    if(pDlg->bRunning == false)
                        break;

                    // stop conversion process on error
                    if(pDlg->bStopOnErrors == true)
                        break;

                    // in processing mode 2 we are skipping to next file
                    // no encoding is done when decoding failed
                    continue;
                }
            }

            if(pDlg->bRunning == false)
                break;

            if(pDlg->bForceConsoleWindow == false)
            {
                // configure encoder input and output pipes
                bUseInPipesEnc = pDlg->bFormatInput[nOutputFormat];
                bUseOutPipesEnc = pDlg->bFormatOutput[nOutputFormat];
            }

            if(nProcessingMode == 0)
            {
                // input file is stdin
                if(bUseInPipesEnc == true)
                    szInputFile = _T("-");

                // output file is stdout
                if(bUseOutPipesEnc == true)
                    szOutputFile = _T("-");
            }

            if(nProcessingMode == 2)
            {
                // input filename is decoded output filename
                szInputFile = szOutputFile;
                szOrgInputFile = szOutputFile;

                // restore output filename
                szOutputFile = szOrgOutputFile;

                // input file is stdin
                if(bUseInPipesEnc == true)
                    szInputFile = _T("-");

                // output file is stdout
                if(bUseOutPipesEnc == true)
                    szOutputFile = _T("-");
            }

            // encode
            if((nProcessingMode == 0) || (nProcessingMode == 2))
            {
                // TODO: validate the command-line template

                // build full command line for encoder (ENCODER-EXE + OPTIONS + INFILE + OUTFILE)
                // this is basic model, some of encoder may have different commandline structure
                csExecute = pDlg->szFormatTemplate[nOutputFormat];
                csExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
                csExecute.Replace(_T("$EXE"), szEncoderExePath);
                csExecute.Replace(_T("$OPTIONS"), szEncoderOptions);
                csExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
                csExecute.Replace(_T("$INFILE"), szInputFile);
                csExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
                csExecute.Replace(_T("$OUTFILE"), szOutputFile);

                bDecode = false;
                nTool = nOutputFormat;

                lstrcpy(szCommandLine, csExecute.GetBuffer(csExecute.GetLength()));

                pDlg->m_LstInputFiles.SetItemText(i, 5, _T("--:--"));
                pDlg->m_LstInputFiles.SetItemText(i, 6, _T("Encoding..."));

                // update (init) conversion status data
                pDlg->m_CnvStatus.SetCurrentInfo(nProcessedFiles, 
                    nTotalFiles, 
                    nDoneWithoutError, 
                    pDlg->m_FileList.GetFileName(szOrgInputFile), 
                    pDlg->m_FileList.GetFileName(szOrgOutputFile), 
                    szOutPath);

                pDlg->m_CnvStatus.Draw(0);

                if(::ConvertFile(pDlg, 
                    szOrgInputFile,
                    szOrgOutputFile,
                    szCommandLine, 
                    i, 
                    bDecode,  
                    nTool, 
                    bUseInPipesEnc,
                    bUseOutPipesEnc) == true)
                {
                    nDoneWithoutError++;

                    if(bDeleteAfterConverion == true)
                        ::DeleteFile(szOrgInputFile);

                    pDlg->m_LstInputFiles.SetCheck(i, FALSE);
                }
                else
                {
                    // delete output file on error
                    if(pDlg->bDeleteOnError == true)
                        ::DeleteFile(szOutputFile);

                    // stop conversion process on error
                    if(pDlg->bStopOnErrors == true)
                        break;
                }

                // delete temporary file
                if(nProcessingMode == 2)
                    ::DeleteFile(szOrgInputFile);
            }

            if(pDlg->bRunning == false)
                break;
        }
    }

    countTime.StopCounter();

    // show total time
    if(nProcessedFiles > 0)
    {
        CString szText;
        szText.Format(_T("Done in %s"), ::FormatTime(countTime.GetTime(), 3));
        pDlg->m_StatusBar.SetText(szText, 1, 0);
    }
    else
    {
        pDlg->m_StatusBar.SetText(_T(""), 1, 0);
    }

    // restore user interface to default state
    if(pDlg->bShowTrayIcon == true)
        pDlg->EnableTrayIcon(true, true);

    pDlg->m_BtnConvert.SetWindowText(_T("Conve&rt"));
    pDlg->GetMenu()->ModifyMenu(ID_ACTION_CONVERT, MF_BYCOMMAND, ID_ACTION_CONVERT, _T("Conve&rt\tF9"));
    pDlg->m_FileProgress.SetPos(0);
    pDlg->bRunning = false;
    pDlg->EnableUserInterface(TRUE);

    // now we shutdown the system
    if(pDlg->GetMenu()->GetMenuState(ID_OPTIONS_SHUTDOWN_WHEN_FINISHED, MF_BYCOMMAND) == MF_CHECKED)
    {
        // NOTE: before shutdown we are doing OnClose() stuff

        // save current settings to file
        if(pDlg->GetMenu()->GetMenuState(ID_OPTIONS_DO_NOT_SAVE, MF_BYCOMMAND) != MF_CHECKED)
            pDlg->SaveSettings();

        // disable tray icon
        pDlg->EnableTrayIcon(false);

        ::DoTheShutdown();
    }

    // close this worker thread handle
    return ::CloseHandle(pDlg->hThread);
}
