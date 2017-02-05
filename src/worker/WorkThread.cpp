// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\Strings.h"
#include "..\utilities\TimeCount.h"
#include "..\utilities\Utilities.h"
#include "..\Configuration.h"
#include "WorkThread.h"

bool ProgresssLoop(CFileContext* pContext, CProcessContext &processContext, int &nProgress)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;
    const int nBuffSize = 4096;
    char szReadBuff[nBuffSize];
    char szLineBuff[nBuffSize];
    DWORD dwReadBytes = 0L;
    BOOL bRes = FALSE;
    bool bLineStart = false;
    bool bLineEnd = false;
    bool bRunning = true;
    int nLineLen = 0;
    int nPreviousProgress = 0;

    // load progress function
    HMODULE hDll = ::LoadLibrary(pContext->szFunction);
    if (hDll == NULL)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110001, pszProgresssLoop[0]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false; // ERROR
    }

    GetProgressFunc *pGetProgress = (GetProgressFunc*) ::GetProcAddress(hDll, "GetProgress");
    if (pGetProgress == NULL)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110002, pszProgresssLoop[1]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false; // ERROR
    }

    // initialize buffers
    ZeroMemory(szReadBuff, sizeof(szReadBuff));
    ZeroMemory(szLineBuff, sizeof(szLineBuff));

    do
    {
        ZeroMemory(szReadBuff, sizeof(szReadBuff));
        bRes = ::ReadFile(processContext.hReadPipeStderr, szReadBuff, 100, &dwReadBytes, 0);
        if (bRes == FALSE || dwReadBytes == 0)
            break;

        // terminate read data by '\0'
        szReadBuff[dwReadBytes] = '\0';

        for (int i = 0; i < (int)dwReadBytes; i++)
        {
            // processed escape chars ( \r \n \t \b )
            if (szReadBuff[i] == '\r') // '\r'
            {
                if ((bLineStart == true) && (bLineEnd == false))
                {
                    bLineEnd = true;
                    bLineStart = false;
                    szLineBuff[nLineLen] = '\0';
                }
            }
            else if (szReadBuff[i] == '\n') // '\n'
            {
                // do nothing
            }
            else if (szReadBuff[i] == '\t') // '\t'
            {
                // do nothing
            }
            else if (szReadBuff[i] == '\b') // '\b'
            {
                // do nothing (most of the tools)
                if ((bLineStart == true) && (bLineEnd == false))
                {
                    bLineEnd = true;
                    bLineStart = false;
                    szLineBuff[nLineLen] = '\0';
                }
            }
            else if (bLineEnd == false)
            {
                bLineStart = true; // we have start
                nLineLen++;
                if (nLineLen > nBuffSize)
                {
                    pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110003, pszProgresssLoop[2]));
                    pWorkerContext->Callback(pContext->nItemId, -1, true, true);
                    return false;
                }

                szLineBuff[nLineLen - 1] = szReadBuff[i];
            }

            // now we have correct full line of text
            if ((bLineEnd == true) && (bLineStart == false))
            {
                // don't include empty lines
                if (strlen(szLineBuff) > 0)
                {
                    int nRet = (pGetProgress)(szLineBuff, nLineLen);
                    if (nRet != -1)
                        nProgress = nRet;

                    ZeroMemory(szLineBuff, sizeof(szLineBuff));

                    if (nProgress != nPreviousProgress)
                    {
                        bRunning = pWorkerContext->Callback(pContext->nItemId, nProgress, false);
                        nPreviousProgress = nProgress;
                    }

                    if (bRunning == false)
                        break;
                }

                // reset line counter
                nLineLen = 0;

                // find next start
                bLineStart = true;
                bLineEnd = false;
            }
        }

        if (bRunning == false)
            break;
    } while (bRes);

    if (hDll != NULL)
        ::FreeLibrary(hDll);

    return true;
}

bool ConvertFileUsingConsole(CFileContext* pContext)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;

    if ((pContext->bUseReadPipes == true) || (pContext->bUseWritePipes == true))
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120001, pszConvertConsole[0]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    CProcessContext processContext;
    int nProgress = 0;
    CTimeCount timer;

    // create pipes for stderr
    if (processContext.CreateStderrPipe() == false)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120002, pszConvertConsole[1]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // duplicate stderr read pipe handle to prevent child process from closing the pipe
    if (processContext.DuplicateStderrReadPipe() == false)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120003, pszConvertConsole[2]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // connect pipes to process
    processContext.ConnectStdInput(NULL);
    processContext.ConnectStdOutput(processContext.hWritePipeStderr);
    processContext.ConnectStdError(processContext.hWritePipeStderr);

    timer.Start();
    if (processContext.Start(pContext->szCommandLine) == false)
    {
        timer.Stop();

        processContext.CloseStderrReadPipe();
        processContext.CloseStderrWritePipe();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00120004, pszConvertConsole[3]), ::GetLastError());

        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handle
    processContext.CloseStderrWritePipe();

    // console progress loop
    if (ProgresssLoop(pContext, processContext, nProgress) == false)
    {
        timer.Stop();
        processContext.CloseStderrReadPipe();
        processContext.Stop(false);
        return false;
    }

    timer.Stop();
    processContext.CloseStderrReadPipe();
    processContext.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120005, pszConvertConsole[4]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(pContext->nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x00120006, pszConvertConsole[5]));
        pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}

bool ReadLoop(CPipeContext* pContext)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE pReadBuff[4096];
    BOOL bRes = FALSE;
    DWORD dwReadBytes = 0;
    DWORD dwWriteBytes = 0;
    ULONGLONG nTotalBytesRead = 0;
    ULONGLONG nFileSize = 0;
    int nProgress = -1;
    int nPreviousProgress = -1;
    bool bRunning = true;

    pContext->bError = false;
    pContext->bFinished = false;

    // open existing source file with read-only flag
    hFile = ::CreateFile(pContext->szFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return false;
    }

    nFileSize = ::GetFileSize64(hFile);
    if (nFileSize == 0)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        ::CloseHandle(hFile);
        return false;
    }

    // read/write loop
    do
    {
        // read data from source file
        bRes = ::ReadFile(hFile, pReadBuff, 4096, &dwReadBytes, 0);
        if ((bRes == FALSE) || (dwReadBytes == 0))
            break;

        // NOTE: Sleep(0) solves problem writing to pipe errors
        ::Sleep(0);

        // write data to write pipe
        bRes = ::WriteFile(pContext->hPipe, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        // count read/write bytes
        nTotalBytesRead += dwReadBytes;
        nProgress = (int)((nTotalBytesRead * 100) / nFileSize);

        if (nProgress != nPreviousProgress)
        {
            bRunning = pContext->pWorkerContext->Callback(pContext->nIndex, nProgress, false);
            nPreviousProgress = nProgress;
        }

        if (bRunning == false)
            break;
    } while (bRes != FALSE);

    // clean up memory
    ::CloseHandle(hFile);

    // close write pipe to allow write thread terminate reading
    ::CloseHandle(pContext->hPipe);

    // check if all data was processed
    if (nTotalBytesRead != nFileSize)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return false;
    }

    pContext->bError = false;
    pContext->bFinished = true;
    return true;
}

bool WriteLoop(CPipeContext* pContext)
{
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE pReadBuff[4096];
    BOOL bRes = FALSE;
    DWORD dwReadBytes = 0;
    DWORD dwWriteBytes = 0;
    ULONGLONG nTotalBytesWrite = 0;
    ULONGLONG nFileSize = 0;
    int nProgress = -1;

    pContext->bError = false;
    pContext->bFinished = false;

    // open existing source file with read-only flag
    hFile = ::CreateFile(pContext->szFileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        0,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        pContext->bError = true;
        pContext->bFinished = true;
        return false;
    }

    // read/write loop
    do
    {
        // read data from source pipe
        bRes = ::ReadFile(pContext->hPipe, pReadBuff, 4096, &dwReadBytes, 0);
        if ((bRes == FALSE) || (dwReadBytes == 0))
            break;

        // write data to file
        bRes = ::WriteFile(hFile, pReadBuff, dwReadBytes, &dwWriteBytes, 0);
        if ((bRes == FALSE) || (dwWriteBytes == 0) || (dwReadBytes != dwWriteBytes))
            break;

        // count read/write bytes
        nTotalBytesWrite += dwReadBytes;

        // handle user Stop
        if (pContext->pWorkerContext->bRunning == false)
            break;
    } while (bRes != FALSE);

    // clean up memory
    ::CloseHandle(hFile);

    pContext->bError = false;
    pContext->bFinished = true;
    return true;
}

DWORD WINAPI ReadThread(LPVOID lpParam)
{
    CPipeContext* pContext = (CPipeContext*)lpParam;
    if (pContext != NULL)
    {
        if (ReadLoop(pContext) == true)
            return TRUE;
    }
    return FALSE;
}

DWORD WINAPI WriteThread(LPVOID lpParam)
{
    CPipeContext* pContext = (CPipeContext*)lpParam;
    if (pContext != NULL)
    {
        if (WriteLoop(pContext) == true)
            return TRUE;
    }
    return FALSE;
}

bool ConvertFileUsingPipes(CFileContext* pContext)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;

    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130001, pszConvertPipes[0]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    CProcessContext processContext;
    CPipeContext readContext;
    CPipeContext writeContext;
    DWORD dwReadThreadID = 0L;
    DWORD dwWriteThreadID = 0L;
    HANDLE hReadThread = NULL;
    HANDLE hWriteThread = NULL;
    bool bWriteThreadResult = false;
    int nProgress = 0;
    CTimeCount timer;

    if (pContext->bUseReadPipes == true)
    {
        // create pipes for stdin
        if (processContext.CreateStdinPipe() == false)
        {
            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130002, pszConvertPipes[1]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // set stdin write pipe inherit flag
        if (processContext.InheritStdinWritePipe() == false)
        {
            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130003, pszConvertPipes[2]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    if (pContext->bUseWritePipes == true)
    {
        // create pipes for stdout
        if (processContext.CreateStdoutPipe() == false)
        {
            if (pContext->bUseReadPipes == true)
            {
                processContext.CloseStdinReadPipe();
                processContext.CloseStdinWritePipe();
            }

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130004, pszConvertPipes[3]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // set stdout read pipe inherit flag
        if (processContext.InheritStdoutReadPipe() == false)
        {
            if (pContext->bUseReadPipes == true)
            {
                processContext.CloseStdinReadPipe();
                processContext.CloseStdinWritePipe();
            }

            processContext.CloseStdoutReadPipe();
            processContext.CloseStdoutWritePipe();

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130005, pszConvertPipes[4]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }
    }

    // connect pipes to process
    if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == false))
    {
        processContext.ConnectStdInput(processContext.hReadPipeStdin);
        processContext.ConnectStdOutput(NULL);
        processContext.ConnectStdError(NULL);
    }
    else if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == true))
    {
        processContext.ConnectStdInput(NULL);
        processContext.ConnectStdOutput(processContext.hWritePipeStdout);
        processContext.ConnectStdError(NULL);
    }
    else if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
    {
        processContext.ConnectStdInput(processContext.hReadPipeStdin);
        processContext.ConnectStdOutput(processContext.hWritePipeStdout);
        processContext.ConnectStdError(NULL);
    }

    timer.Start();
    if (processContext.Start(pContext->szCommandLine) == false)
    {
        timer.Stop();

        if (pContext->bUseReadPipes == true)
        {
            processContext.CloseStdinReadPipe();
            processContext.CloseStdinWritePipe();
        }

        if (pContext->bUseWritePipes == true)
        {
            processContext.CloseStdoutReadPipe();
            processContext.CloseStdoutWritePipe();
        }

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handles
    if (pContext->bUseReadPipes == true)
        processContext.CloseStdinReadPipe();

    if (pContext->bUseWritePipes == true)
        processContext.CloseStdoutWritePipe();

    // create read thread
    if (pContext->bUseReadPipes == true)
    {
        readContext.bError = false;
        readContext.bFinished = false;
        readContext.pWorkerContext = pWorkerContext;
        readContext.szFileName = pContext->szInputFile;
        readContext.hPipe = processContext.hWritePipeStdin;
        readContext.nIndex = pContext->nItemId;

        dwReadThreadID = 0L;
        hReadThread = ::CreateThread(NULL, 0, ReadThread, (LPVOID)&readContext, 0, &dwReadThreadID);
        if (hReadThread == NULL)
        {
            timer.Stop();

            processContext.CloseStdinWritePipe();

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130007, pszConvertPipes[6]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // wait for read thread to finish
        if (pContext->bUseWritePipes == false)
        {
            ::WaitForSingleObject(hReadThread, INFINITE);

            // NOTE: Handle is closed in ReadThread.
            //processContext.CloseStdinWritePipe();

            // check for result from read thread
            if ((readContext.bError == false) && (readContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;

            // close read thread handle
            ::CloseHandle(hReadThread);
        }
    }

    // create write thread
    if (pContext->bUseWritePipes == true)
    {
        writeContext.bError = false;
        writeContext.bFinished = false;
        writeContext.pWorkerContext = pWorkerContext;
        writeContext.szFileName = pContext->szOutputFile;
        writeContext.hPipe = processContext.hReadPipeStdout;
        writeContext.nIndex = pContext->nItemId;

        dwWriteThreadID = 0L;
        hWriteThread = ::CreateThread(NULL, 0, WriteThread, (LPVOID)&writeContext, 0, &dwWriteThreadID);
        if (hWriteThread == NULL)
        {
            timer.Stop();

            processContext.CloseStdoutReadPipe();

            pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130008, pszConvertPipes[7]));
            pWorkerContext->Callback(pContext->nItemId, -1, true, true);
            return false;
        }

        // wait for write thread to finish
        ::WaitForSingleObject(hWriteThread, INFINITE);

        processContext.CloseStdoutReadPipe();

        // check for result from read thread
        if ((writeContext.bError == false) && (writeContext.bFinished == true))
        {
            bWriteThreadResult = true;
            if (pContext->bUseReadPipes == false)
                nProgress = 100;
        }
        else
        {
            bWriteThreadResult = false;
            if (pContext->bUseReadPipes == false)
                nProgress = -1;
        }

        // close read thread handle
        ::CloseHandle(hWriteThread);
    }

    // wait for read thread to finish after write thread finished
    if ((pContext->bUseReadPipes == true) && (pContext->bUseWritePipes == true))
    {
        ::WaitForSingleObject(hReadThread, INFINITE);

        // NOTE: Handle is closed in ReadThread.
        //processContext.CloseStdinWritePipe();

        // check for result from read thread
        if ((readContext.bError == false) && (readContext.bFinished == true) && (bWriteThreadResult == true))
            nProgress = 100;
        else
            nProgress = -1;

        // close read thread handle
        ::CloseHandle(hReadThread);
    }

    timer.Stop();
    processContext.Stop(nProgress == 100);

    if (nProgress != 100)
    {
        pWorkerContext->Status(pContext->nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130009, pszConvertPipes[8]));
        pWorkerContext->Callback(pContext->nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(pContext->nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x0013000A, pszConvertPipes[9]));
        pWorkerContext->Callback(pContext->nItemId, 100, true, false);
        return true;
    }
}

bool ConvertFile(CFileContext* pContext)
{
    if ((pContext->bUseReadPipes == false) && (pContext->bUseWritePipes == false))
        return ConvertFileUsingConsole(pContext);
    else
        return ConvertFileUsingPipes(pContext);
}

bool FileExists(CString szPath)
{
    WIN32_FIND_DATA w32FileData;
    ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = ::FindFirstFile(szPath, &w32FileData);
    bool bInvalidHandle = hFind == INVALID_HANDLE_VALUE;
    ::FindClose(hFind);
    return bInvalidHandle == false;
}

bool ConvertItem(CItemContext* pContext, ConvertFileFunc *pConvertFile)
{
    CWorkerContext *pWorkerContext = pContext->pWorkerContext;

    // input file
    CString szInputFile = pContext->item->szPath;

    // validate input file
    if (FileExists(szInputFile) == false)
    {
        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140001, pszConvertItem[0]));
        return false;
    }

    // output path
    CString szOutPath;
    if (pWorkerContext->pConfig->m_Options.bOutputPathChecked == true)
    {
        szOutPath = pWorkerContext->pConfig->m_Options.szOutputPath;
    }
    else
    {
        CString szInputFileName = ::GetFileName(szInputFile);
        szOutPath = szInputFile;
        szOutPath.Truncate(szOutPath.GetLength() - szInputFileName.GetLength());
    }

    // find encoder
    int nEncoder = pWorkerContext->pConfig->m_Formats.GetFormatById(pContext->item->szFormatId);
    if (nEncoder == -1)
    {
        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140002, pszConvertItem[1]));
        return false;
    }

    CFormat& encoderFormat = pWorkerContext->pConfig->m_Formats.GetData(nEncoder);

    // validate encoder preset
    if (pContext->item->nPreset >= encoderFormat.m_Presets.GetSize())
    {
        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140003, pszConvertItem[2]));
        return false;
    }

    // validate input extension
    CString szInputFileExt = ::GetFileExtension(szInputFile).MakeUpper();
    bool bIsValidEncoderInput = encoderFormat.IsValidInputExtension(szInputFileExt);

    // processing mode
    Mode nProcessingMode = Mode::None;
    if (bIsValidEncoderInput)
        nProcessingMode = Mode::Encode;
    else
        nProcessingMode = Mode::Transcode;

    // output path
    CString szEncoderExtension = encoderFormat.szOutputExtension;
    CString szName = pContext->item->szName + _T(".") + szEncoderExtension.MakeLower();
    CString szOutputFile;
    if (szOutPath.GetLength() >= 1)
    {
        if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
            szOutputFile = szOutPath + szName;
        else
            szOutputFile = szOutPath + _T("\\") + szName;
    }
    else
    {
        szOutputFile = szName;
    }

    CString szOrgInputFile = szInputFile;
    CString szOrgOutputFile = szOutputFile;

    if (nProcessingMode == Mode::Transcode)
    {
        // find decoder
        int nDecoder = pWorkerContext->pConfig->m_Formats.GetDecoderByExtension(pContext->item->szExtension);
        if (nDecoder == -1)
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140004, pszConvertItem[3]));
            return false;
        }

        CFormat& decoderFormat = pWorkerContext->pConfig->m_Formats.GetData(nDecoder);

        // validate decoder preset
        if (decoderFormat.nDefaultPreset >= decoderFormat.m_Presets.GetSize())
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140005, pszConvertItem[4]));
            return false;
        }

        // validate decoder output extension
        bIsValidEncoderInput = encoderFormat.IsValidInputExtension(decoderFormat.szOutputExtension);
        if (bIsValidEncoderInput == false)
        {
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140006, pszConvertItem[5]));
            return false;
        }

        CString szDecoderExtension = decoderFormat.szOutputExtension;
        szOutputFile = szOutputFile + +_T(".") + szDecoderExtension.MakeLower();

        pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140007, pszConvertItem[6]));
        try
        {
            CFileContext context(
                pWorkerContext,
                decoderFormat,
                decoderFormat.nDefaultPreset,
                pContext->item->nId,
                szInputFile,
                szOutputFile);
            if ((pConvertFile)(&context) == false)
            {
                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szOutputFile);

                return false;
            }

            // validate decoded file
            if (FileExists(szOutputFile) == false)
            {
                pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140008, pszConvertItem[7]));
                return false;
            }
        }
        catch (...)
        {
            if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szOutputFile);

            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x00140009, pszConvertItem[8]));
            pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    if (pWorkerContext->bRunning == false)
        return false;

    if (nProcessingMode == Mode::Transcode)
    {
        // decoder output file as encoder input file
        szInputFile = szOutputFile;

        // original encoder output file
        szOutputFile = szOrgOutputFile;
    }

    if ((nProcessingMode == Mode::Encode) || (nProcessingMode == Mode::Transcode))
    {
        if (encoderFormat.nType == 0)
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000A, pszConvertItem[9]));
        else if (encoderFormat.nType == 1)
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000B, pszConvertItem[10]));
        else
            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000C, pszConvertItem[11]));

        try
        {
            CFileContext context(
                pWorkerContext,
                encoderFormat,
                pContext->item->nPreset,
                pContext->item->nId,
                szInputFile,
                szOutputFile);
            if ((pConvertFile)(&context) == true)
            {
                // validate encoded file
                if (FileExists(szOutputFile) == false)
                {
                    if (nProcessingMode == Mode::Transcode)
                    {
                        if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szInputFile);
                    }

                    pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000D, pszConvertItem[12]));
                    return false;
                }

                if (nProcessingMode == Mode::Transcode)
                    ::DeleteFile(szInputFile);

                if (pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                    ::DeleteFile(szOrgInputFile);

                return true;
            }
            else
            {
                if (nProcessingMode == Mode::Transcode)
                    ::DeleteFile(szInputFile);

                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szOutputFile);

                return false;
            }
        }
        catch (...)
        {
            if (nProcessingMode == Mode::Transcode)
                ::DeleteFile(szInputFile);

            if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szOutputFile);

            pWorkerContext->Status(pContext->item->nId, pszDefaulTime, pWorkerContext->GetString(0x0014000E, pszConvertItem[13]));
            pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    return false;
}

bool ConvertLoop(CWorkerContext* pWorkerContext, ConvertFileFunc *pConvertFile)
{
    while (!pWorkerContext->pQueue->IsEmpty())
    {
        try
        {
            CItemContext* pContext = NULL;
            DWORD dwWaitResult = ::WaitForSingleObject(pWorkerContext->hMutex, INFINITE);
            switch (dwWaitResult)
            {
            case WAIT_OBJECT_0:
            {
                pContext = (CItemContext*)pWorkerContext->pQueue->RemoveHead();
                if (!::ReleaseMutex(pWorkerContext->hMutex))
                    return false;
            }
            break;
            case WAIT_ABANDONED:
                return false;
            }

            if (pContext != NULL)
            {
                pContext->pWorkerContext->Next(pContext->item->nId);
                if (ConvertItem(pContext, pConvertFile) == true)
                {
                    pContext->pWorkerContext->nDoneWithoutError++;
                }
                else
                {
                    if (pContext->pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                        return false;
                }

                if (pContext->pWorkerContext->bRunning == false)
                    return false;
            }
        }
        catch (...)
        {
            return false;
        }
    }
    return true;
}

DWORD WINAPI ConvertThread(LPVOID lpParam)
{
    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    if (pWorkerContext != NULL)
    {
        if (ConvertLoop(pWorkerContext, &ConvertFile) == true)
            return TRUE;
    }
    return FALSE;
}

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    CWorkerContext* pWorkerContext = (CWorkerContext*)lpParam;
    if (pWorkerContext == NULL)
        return (DWORD)(-1);

    int nItems = pWorkerContext->pConfig->m_Items.GetSize();
    CItemContext *pItemsContext = new CItemContext[nItems];

    pWorkerContext->nTotalFiles = 0;
    pWorkerContext->nProcessedFiles = 0;
    pWorkerContext->nDoneWithoutError = 0;
    pWorkerContext->nErrors = 0;
    pWorkerContext->pQueue = new CObList();
    pWorkerContext->nProgess = new int[nItems];
    pWorkerContext->nPreviousProgess = new int[nItems];
    pWorkerContext->nLastItemId = -1;

    for (int i = 0; i < nItems; i++)
    {
        CItem& item = pWorkerContext->pConfig->m_Items.GetData(i);
        if (item.bChecked == true)
        {
            pWorkerContext->nTotalFiles++;

            pWorkerContext->nProgess[i] = 0;
            pWorkerContext->nPreviousProgess[i] = 0;

            pItemsContext[i].pWorkerContext = pWorkerContext;
            pItemsContext[i].item = &item;
            pWorkerContext->pQueue->AddTail(&pItemsContext[i]);
        }
        else
        {
            pWorkerContext->nProgess[i] = 100;
            pWorkerContext->nPreviousProgess[i] = 100;
        }
    }

    pWorkerContext->nThreadCount = pWorkerContext->pConfig->m_Options.nThreadCount;
    if (pWorkerContext->nThreadCount < 1)
    {
        // auto-detect number of available threads
        LogicalProcessorInformation info;
        if (GetLogicalProcessorInformation(&info) == 0)
            pWorkerContext->nThreadCount = info.processorCoreCount;
        else
            pWorkerContext->nThreadCount = 1;
    }

    pWorkerContext->hMutex = ::CreateMutex(NULL, FALSE, NULL);
    pWorkerContext->hConvertThread = new HANDLE[pWorkerContext->nThreadCount];
    pWorkerContext->dwConvertThreadID = new DWORD[pWorkerContext->nThreadCount];

    pWorkerContext->Init();

    // single-threaded
    if (pWorkerContext->nThreadCount == 1)
    {
        ConvertLoop(pWorkerContext, &ConvertFile);
    }

    // multi-threaded
    if (pWorkerContext->nThreadCount > 1)
    {
        // create worker threads
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
        {
            pWorkerContext->dwConvertThreadID[i] = i;
            pWorkerContext->hConvertThread[i] = ::CreateThread(NULL, 0, ConvertThread, pWorkerContext, CREATE_SUSPENDED, &pWorkerContext->dwConvertThreadID[i]);
            if (pWorkerContext->hConvertThread[i] == NULL)
                break;
            ::ResumeThread(pWorkerContext->hConvertThread[i]);
        }

        // wait for all workers to finish
        ::WaitForMultipleObjects(pWorkerContext->nThreadCount, pWorkerContext->hConvertThread, TRUE, INFINITE);

        // close convert thread handles
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
            ::CloseHandle(pWorkerContext->hConvertThread[i]);
    }

    delete pWorkerContext->hConvertThread;
    delete pWorkerContext->dwConvertThreadID;
    delete pWorkerContext->pQueue;
    delete pWorkerContext->nProgess;
    delete pWorkerContext->nPreviousProgess;
    delete[] pItemsContext;

    ::CloseHandle(pWorkerContext->hMutex);

    pWorkerContext->Done();
    pWorkerContext->bDone = true;

    return ::CloseHandle(pWorkerContext->hThread);
}
