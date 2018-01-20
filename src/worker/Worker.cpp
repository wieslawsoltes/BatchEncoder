// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "Strings.h"
#include "utilities\OutputPath.h"
#include "utilities\TimeCount.h"
#include "utilities\Utilities.h"
#include "FileToPipeWriter.h"
#include "PipeToFileWriter.h"
#include "LuaOutputParser.h"
#include "DebugOutputParser.h"
#include "Worker.h"

//#define PIPES_STDERR_DEBUG

bool CWorker::OutputLoop(IWorkerContext* pWorkerContext, CFileContext &context, CPipe &Stderr, IOutputParser &parser)
{
    const int nBuffSize = 4096;
    char szReadBuff[nBuffSize];
    char szLineBuff[nBuffSize];
    DWORD dwReadBytes = 0L;
    BOOL bRes = FALSE;
    bool bLineStart = false;
    bool bLineEnd = false;
    bool bRunning = true;
    int nLineLen = 0;

    if (parser.Init(pWorkerContext, &context) == false)
    {
        return false; // ERROR
    }

    // initialize buffers
    std::memset(szReadBuff, 0, sizeof(szReadBuff));
    std::memset(szLineBuff, 0, sizeof(szLineBuff));

    do
    {
        std::memset(szReadBuff, 0, sizeof(szReadBuff));
        bRes = ::ReadFile(Stderr.hRead, szReadBuff, 100, &dwReadBytes, 0);
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
                    pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00110003, pszProgresssLoop[2]));
                    pWorkerContext->Callback(context.nItemId, -1, true, true);
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
                    bRunning = parser.Parse(szLineBuff);
                    std::memset(szLineBuff, 0, sizeof(szLineBuff));
                    if ((pWorkerContext->bRunning == false) || (bRunning == false))
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

    return true;
}

bool CWorker::ConvertFileUsingConsole(IWorkerContext* pWorkerContext, CFileContext &context)
{
    if ((context.bUseReadPipes == true) || (context.bUseWritePipes == true))
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120001, pszConvertConsole[0]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    CProcess process;
    CPipe Stderr(true);
    CTimeCount timer;

    // create pipes for stderr
    if (Stderr.Create() == false)
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120002, pszConvertConsole[1]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    // duplicate stderr read pipe handle to prevent child process from closing the pipe
    if (Stderr.DuplicateRead() == false)
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120003, pszConvertConsole[2]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    // connect pipes to process
    process.ConnectStdInput(nullptr);
    process.ConnectStdOutput(Stderr.hWrite);
    process.ConnectStdError(Stderr.hWrite);

    timer.Start();
    if (process.Start(context.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        Stderr.CloseRead();
        Stderr.CloseWrite();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00120004, pszConvertConsole[3]), ::GetLastError());

        pWorkerContext->Status(context.nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handle
    Stderr.CloseWrite();

    // console progress loop
    CLuaOutputParser parser;
    if (OutputLoop(pWorkerContext, context, Stderr, parser) == false)
    {
        timer.Stop();
        Stderr.CloseRead();
        process.Stop(false, context.pFormat->nExitCodeSuccess);
        return false;
    }

    timer.Stop();
    Stderr.CloseRead();
    if (process.Stop(parser.nProgress == 100, context.pFormat->nExitCodeSuccess) == false)
        parser.nProgress = -1;

    if (parser.nProgress != 100)
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00120005, pszConvertConsole[4]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(context.nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x00120006, pszConvertConsole[5]));
        pWorkerContext->Callback(context.nItemId, 100, true, false);
        return true;
    }
}

bool CWorker::ConvertFileUsingPipes(IWorkerContext* pWorkerContext, CFileContext &context)
{
    if ((context.bUseReadPipes == false) && (context.bUseWritePipes == false))
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130001, pszConvertPipes[0]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    CProcess process;
    CPipe Stdin(true);
    CPipe Stdout(true);
#ifdef PIPES_STDERR_DEBUG
    CPipe Stderr(true);
#endif
    CFileToPipeWriter readContext;
    CPipeToFileWriter writeContext;
    CThread readThread;
    CThread writeThread;
#ifdef PIPES_STDERR_DEBUG
    CThread outputThread;
#endif
    int nProgress = 0;
    CTimeCount timer;

    if (context.bUseReadPipes == true)
    {
        // create pipes for stdin
        if (Stdin.Create() == false)
        {
            pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130002, pszConvertPipes[1]));
            pWorkerContext->Callback(context.nItemId, -1, true, true);
            return false;
        }

        // set stdin write pipe inherit flag
        if (Stdin.InheritWrite() == false)
        {
            pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130003, pszConvertPipes[2]));
            pWorkerContext->Callback(context.nItemId, -1, true, true);
            return false;
        }
    }

    if (context.bUseWritePipes == true)
    {
        // create pipes for stdout
        if (Stdout.Create() == false)
        {
            if (context.bUseReadPipes == true)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();
            }

            pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130004, pszConvertPipes[3]));
            pWorkerContext->Callback(context.nItemId, -1, true, true);
            return false;
        }

        // set stdout read pipe inherit flag
        if (Stdout.InheritRead() == false)
        {
            if (context.bUseReadPipes == true)
            {
                Stdin.CloseRead();
                Stdin.CloseWrite();
            }

            Stdout.CloseRead();
            Stdout.CloseWrite();

            pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130005, pszConvertPipes[4]));
            pWorkerContext->Callback(context.nItemId, -1, true, true);
            return false;
        }
    }

#ifdef PIPES_STDERR_DEBUG
    // create pipes for stderr
    if (Stderr.Create() == false)
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x0013000C, pszConvertPipes[11]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    // set stderr read pipe inherit flag
    if (Stderr.InheritRead() == false)
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x0013000D, pszConvertPipes[12]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }
#endif

    // connect pipes to process
    if ((context.bUseReadPipes == true) && (context.bUseWritePipes == false))
    {
        process.ConnectStdInput(Stdin.hRead);
        process.ConnectStdOutput(GetStdHandle(STD_OUTPUT_HANDLE));
#ifdef PIPES_STDERR_DEBUG
        process.ConnectStdError(Stderr.hWrite);
#else
        process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
#endif
    }
    else if ((context.bUseReadPipes == false) && (context.bUseWritePipes == true))
    {
        process.ConnectStdInput(GetStdHandle(STD_INPUT_HANDLE));
        process.ConnectStdOutput(Stdout.hWrite);
#ifdef PIPES_STDERR_DEBUG
        process.ConnectStdError(Stderr.hWrite);
#else
        process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
#endif
    }
    else if ((context.bUseReadPipes == true) && (context.bUseWritePipes == true))
    {
        process.ConnectStdInput(Stdin.hRead);
        process.ConnectStdOutput(Stdout.hWrite);
#ifdef PIPES_STDERR_DEBUG
        process.ConnectStdError(Stderr.hWrite);
#else
        process.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));
#endif
    }

    timer.Start();
    if (process.Start(context.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        if (context.bUseReadPipes == true)
        {
            Stdin.CloseRead();
            Stdin.CloseWrite();
        }

        if (context.bUseWritePipes == true)
        {
            Stdout.CloseRead();
            Stdout.CloseWrite();
        }

#ifdef PIPES_STDERR_DEBUG
        Stderr.CloseRead();
        Stderr.CloseWrite();
#endif

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(context.nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handles
    if (context.bUseReadPipes == true)
        Stdin.CloseRead();

    if (context.bUseWritePipes == true)
        Stdout.CloseWrite();

#ifdef PIPES_STDERR_DEBUG
    Stderr.CloseWrite();

    // create output thread
    CDebugOutputParser debug;
    if (outputThread.Start([this, pWorkerContext, &context, &Stderr, &debug]() { this->OutputLoop(pWorkerContext, context, Stderr, debug); }) == false)
    {
        timer.Stop();

        process.Stop(false, context.pFormat->nExitCodeSuccess);

        Stdin.CloseWrite();
        Stderr.CloseRead();

        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x0013000E, pszConvertPipes[13]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }
#endif

    // create read thread
    if (context.bUseReadPipes == true)
    {
        readContext.bError = false;
        readContext.bFinished = false;
        readContext.szFileName = context.szInputFile;
        readContext.nIndex = context.nItemId;

        if (readThread.Start([this, pWorkerContext, &readContext, &Stdin]() { readContext.ReadLoop(pWorkerContext, Stdin); }) == false)
        {
            timer.Stop();

            process.Stop(false, context.pFormat->nExitCodeSuccess);

#ifdef PIPES_STDERR_DEBUG
            outputThread.Terminate();
            outputThread.Close();
#endif
            Stdin.CloseWrite();

#ifdef PIPES_STDERR_DEBUG
            Stderr.CloseRead();
#endif

            pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130007, pszConvertPipes[6]));
            pWorkerContext->Callback(context.nItemId, -1, true, true);
            return false;
        }

        // wait for read thread to finish
        if (context.bUseWritePipes == false)
        {
            readThread.Wait();

            // NOTE: Handle is closed in ReadThread.
            //Stdin.CloseWrite();

            // close read thread handle
            readThread.Close();

            // check for result from read thread
            if ((readContext.bError == false) && (readContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;
        }
    }

    // create write thread
    if (context.bUseWritePipes == true)
    {
        writeContext.bError = false;
        writeContext.bFinished = false;
        writeContext.szFileName = context.szOutputFile;
        writeContext.nIndex = context.nItemId;

        if (writeThread.Start([this, pWorkerContext, &writeContext, &Stdout]() { writeContext.WriteLoop(pWorkerContext, Stdout); }) == false)
        {
            timer.Stop();

            process.Stop(false, context.pFormat->nExitCodeSuccess);

            Stdout.CloseRead();

            pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130008, pszConvertPipes[7]));
            pWorkerContext->Callback(context.nItemId, -1, true, true);
            return false;
        }

        if (context.bUseReadPipes == true)
        {
            // wait for read thread to finish
            readThread.Wait();

            // NOTE: Handle is closed in ReadThread.
            //Stdin.CloseWrite();

            // close read thread handle
            readThread.Close();

            if ((readContext.bError == true) || (readContext.bFinished == false))
            {
                // close write thread handle
                Stdout.CloseRead();

                // read thread failed so terminate write thread
                writeThread.Terminate();
                writeThread.Close();
            }
            else
            {
                // wait for write thread to finish
                writeThread.Wait();

                // close write thread handle
                Stdout.CloseRead();

                // close write thread handle
                writeThread.Close();
            }

            // check for result from read thread
            if ((readContext.bError == false) && (readContext.bFinished == true)
                && (writeContext.bError == false) && (writeContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;
        }
        else
        {
            // wait for write thread to finish
            writeThread.Wait();

            // close write thread handle
            Stdout.CloseRead();

            // close write thread handle
            writeThread.Close();

            // check for result from write thread
            if ((writeContext.bError == false) && (writeContext.bFinished == true))
                nProgress = 100;
            else
                nProgress = -1;
        }
    }

#ifdef PIPES_STDERR_DEBUG
    // wait for output thread to finish
    outputThread.Wait();
    outputThread.Close();
#endif

    timer.Stop();
#ifdef PIPES_STDERR_DEBUG
    Stderr.CloseRead();
#endif
    if (process.Stop(nProgress == 100, context.pFormat->nExitCodeSuccess) == false)
        nProgress = -1;

    if (nProgress != 100)
    {
        pWorkerContext->Status(context.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130009, pszConvertPipes[8]));
        pWorkerContext->Callback(context.nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(context.nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x0013000B, pszConvertPipes[10]));
        pWorkerContext->Callback(context.nItemId, 100, true, false);
        return true;
    }
}

bool CWorker::ConvertFileUsingOnlyPipes(IWorkerContext* pWorkerContext, CFileContext &decoderContext, CFileContext &encoderContext)
{
    CProcess decoderProcess;
    CProcess encoderProcess;
    CPipe Stdin(true);
    CPipe Stdout(true);
    CPipe Bridge(true);
    CFileToPipeWriter readContext;
    CPipeToFileWriter writeContext;
    CThread readThread;
    CThread writeThread;
    int nProgress = 0;
    CTimeCount timer;

    // create pipes for stdin
    if (Stdin.Create() == false)
    {
        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130002, pszConvertPipes[1]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // set stdin write pipe inherit flag
    if (Stdin.InheritWrite() == false)
    {
        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130003, pszConvertPipes[2]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // create pipes for stdout
    if (Stdout.Create() == false)
    {
        Stdin.CloseRead();
        Stdin.CloseWrite();

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130004, pszConvertPipes[3]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // set stdout read pipe inherit flag
    if (Stdout.InheritRead() == false)
    {
        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130005, pszConvertPipes[4]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // create pipes for processes bridge
    if (Bridge.Create() == false)
    {
        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x0013000A, pszConvertPipes[9]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // connect pipes to decoder process
    decoderProcess.ConnectStdInput(Stdin.hRead);
    decoderProcess.ConnectStdOutput(Bridge.hWrite);
    decoderProcess.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));

    // connect pipes to encoder process
    encoderProcess.ConnectStdInput(Bridge.hRead);
    encoderProcess.ConnectStdOutput(Stdout.hWrite);
    encoderProcess.ConnectStdError(GetStdHandle(STD_ERROR_HANDLE));

    timer.Start();

    // create decoder process
    if (decoderProcess.Start(decoderContext.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        Bridge.CloseRead();
        Bridge.CloseWrite();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // create encoder process
    if (encoderProcess.Start(encoderContext.pszCommandLine, pWorkerContext->pConfig->m_Options.bHideConsoleWindow) == false)
    {
        timer.Stop();

        decoderProcess.Stop(false, decoderContext.pFormat->nExitCodeSuccess);

        Stdin.CloseRead();
        Stdin.CloseWrite();

        Stdout.CloseRead();
        Stdout.CloseWrite();

        Bridge.CloseRead();
        Bridge.CloseWrite();

        CString szStatus;
        szStatus.Format(pWorkerContext->GetString(0x00130006, pszConvertPipes[5]), ::GetLastError());

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, szStatus);
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // close unused pipe handles
    Stdin.CloseRead();
    Stdout.CloseWrite();
    Bridge.CloseWrite();
    Bridge.CloseRead();

    // create read thread
    readContext.bError = false;
    readContext.bFinished = false;
    readContext.szFileName = decoderContext.szInputFile;
    readContext.nIndex = decoderContext.nItemId;

    if (readThread.Start([this, pWorkerContext, &readContext, &Stdin]() { readContext.ReadLoop(pWorkerContext, Stdin); }) == false)
    {
        timer.Stop();

        Stdin.CloseWrite();

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130007, pszConvertPipes[6]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // create write thread
    writeContext.bError = false;
    writeContext.bFinished = false;
    writeContext.szFileName = encoderContext.szOutputFile;
    writeContext.nIndex = encoderContext.nItemId;

    if (writeThread.Start([this, pWorkerContext, &writeContext, &Stdout]() { writeContext.WriteLoop(pWorkerContext, Stdout); }) == false)
    {
        timer.Stop();

        Stdout.CloseRead();

        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130008, pszConvertPipes[7]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }

    // wait for read thread to finish after write thread finished
    readThread.Wait();

    // NOTE: Handle is closed in ReadThread.
    //Stdin.CloseWrite();

    // close read thread handle
    readThread.Close();

    if ((readContext.bError == true) || (readContext.bFinished == false))
    {
        Stdout.CloseRead();

        // read thread failed so terminate write thread
        writeThread.Terminate();
        writeThread.Close();
    }
    else
    {
        Stdout.CloseRead();

        // wait for write thread to finish
        writeThread.Wait();

        // close write thread handle
        writeThread.Close();
    }

    // check for result from read and write thread
    if ((readContext.bError == false) && (readContext.bFinished == true)
        && (writeContext.bError == false) && (writeContext.bFinished == true))
        nProgress = 100;
    else
        nProgress = -1;

    timer.Stop();

    if (decoderProcess.Stop(nProgress == 100, decoderContext.pFormat->nExitCodeSuccess) == false)
        nProgress = -1;

    if (encoderProcess.Stop(nProgress == 100, encoderContext.pFormat->nExitCodeSuccess) == false)
        nProgress = -1;

    if (nProgress != 100)
    {
        pWorkerContext->Status(decoderContext.nItemId, pszDefaulTime, pWorkerContext->GetString(0x00130009, pszConvertPipes[8]));
        pWorkerContext->Callback(decoderContext.nItemId, -1, true, true);
        return false;
    }
    else
    {
        pWorkerContext->Status(decoderContext.nItemId, timer.Format(timer.ElapsedTime(), 1), pWorkerContext->GetString(0x0013000B, pszConvertPipes[10]));
        pWorkerContext->Callback(decoderContext.nItemId, 100, true, false);
        return true;
    }
}

bool CWorker::ConvertItem(IWorkerContext* pWorkerContext, CItem& item, CSynchronize &syncDir)
{
    CFormat *pEncFormat = nullptr;
    CFormat *pDecFormat = nullptr;
    CString szEncInputFile;
    CString szEncOutputFile;
    CString szDecInputFile;
    CString szDecOutputFile;
    COutputPath m_Output;

    // prepare encoder
    CPath& path = item.m_Paths.Get(0);

    szEncInputFile = path.szPath;
    if (::FileExists(szEncInputFile) == false)
    {
        pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140001, pszConvertItem[0]));
        return false;
    }

    int nEncoder = pWorkerContext->pConfig->m_Formats.GetFormatById(item.szFormatId);
    if (nEncoder == -1)
    {
        pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140002, pszConvertItem[1]));
        return false;
    }

    pEncFormat = &pWorkerContext->pConfig->m_Formats.Get(nEncoder);

    if (item.nPreset >= pEncFormat->m_Presets.Count())
    {
        pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140003, pszConvertItem[2]));
        return false;
    }

    bool bIsValidEncoderInput = pEncFormat->IsValidInputExtension(::GetFileExtension(szEncInputFile));

    m_Output.Validate(pWorkerContext->pConfig->m_Options.szOutputPath);
    szEncOutputFile = m_Output.CreateFilePath(
        pWorkerContext->pConfig->m_Options.szOutputPath,
        szEncInputFile,
        item.szName,
        pEncFormat->szOutputExtension);

    if (pWorkerContext->pConfig->m_Options.bOverwriteExistingFiles == false)
    {
        if (::FileExists(szEncOutputFile) == true)
        {
            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140010, pszConvertItem[15]));
            return false;
        }
    }

    // create output path
    if (syncDir.Wait() == true)
    {
        if (m_Output.CreateOutputPath(szEncOutputFile) == false)
        {
            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000F, pszConvertItem[14]));
            return false;
        }
    }
    else
    {
        pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000F, pszConvertItem[14]));
        return false;
    }

    if (syncDir.Release() == false)
    {
        pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000F, pszConvertItem[14]));
        return false;
    }

    ::SetCurrentDirectory(m_App.szSettingsPath);

    // prepare decoder
    if (bIsValidEncoderInput == false)
    {
        int nDecoder = pWorkerContext->pConfig->m_Formats.GetDecoderByExtensionAndFormat(item.szExtension, pEncFormat);
        if (nDecoder == -1)
        {
            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140004, pszConvertItem[3]));
            return false;
        }

        pDecFormat = &pWorkerContext->pConfig->m_Formats.Get(nDecoder);

        if (pDecFormat->nDefaultPreset >= pDecFormat->m_Presets.Count())
        {
            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140005, pszConvertItem[4]));
            return false;
        }

        bool bIsValidDecoderOutput = pEncFormat->IsValidInputExtension(pDecFormat->szOutputExtension);
        if (bIsValidDecoderOutput == false)
        {
            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140006, pszConvertItem[5]));
            return false;
        }

        szDecInputFile = szEncInputFile;
        szDecOutputFile.Format(_T("%s%s.%s"),
            ::GetFilePath(szEncOutputFile),
            GenerateUuidString(),
            CString(pDecFormat->szOutputExtension).MakeLower());
    }

    CFileContext decoderContext;
    if (bIsValidEncoderInput == false)
    {
        decoderContext.Create(
            pDecFormat,
            pDecFormat->nDefaultPreset,
            item.nId,
            szDecInputFile,
            szDecOutputFile,
            pDecFormat->bPipeInput,
            pDecFormat->bPipeOutput,
            _T(""));
    }

    CFileContext encoderContext;
    encoderContext.Create(
        pEncFormat,
        item.nPreset,
        item.nId,
        bIsValidEncoderInput == true ? szEncInputFile : szDecOutputFile,
        szEncOutputFile,
        pEncFormat->bPipeInput,
        pEncFormat->bPipeOutput,
        item.szOptions);

    if (bIsValidEncoderInput == false
        && decoderContext.bUseReadPipes == true
        && decoderContext.bUseWritePipes == true
        && encoderContext.bUseReadPipes == true
        && encoderContext.bUseWritePipes == true)
    {
        // trans-code
        try
        {
            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000C, pszConvertItem[11]));

            item.ResetProgress();

            bool bResult = ConvertFileUsingOnlyPipes(pWorkerContext, decoderContext, encoderContext);
            if (bResult == true)
            {
                // check if output file exists
                /*
                if (::FileExists(szEncOutputFile) == false)
                {
                    if (bIsValidEncoderInput == false)
                    {
                        if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szEncInputFile);
                    }

                    pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000D, pszConvertItem[12]));
                    return false;
                }
                */

                if (pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                    ::DeleteFile(szEncInputFile);

                return true;
            }
            else
            {
                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile);

                return false;
            }
        }
        catch (...)
        {
            if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szEncOutputFile);

            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000E, pszConvertItem[13]));
            pWorkerContext->Callback(item.nId, -1, true, true);
        }
    }
    else
    {
        // decode
        if (bIsValidEncoderInput == false)
        {
            try
            {
                pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140007, pszConvertItem[6]));

                item.ResetProgress();

                bool bResult = false;
                if ((decoderContext.bUseReadPipes == false) && (decoderContext.bUseWritePipes == false))
                    bResult = ConvertFileUsingConsole(pWorkerContext, decoderContext);
                else
                    bResult = ConvertFileUsingPipes(pWorkerContext, decoderContext);
                if (bResult == false)
                {
                    if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                        ::DeleteFile(szDecOutputFile);

                    return false;
                }

                if (::FileExists(szDecOutputFile) == false)
                {
                    pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140008, pszConvertItem[7]));
                    return false;
                }
            }
            catch (...)
            {
                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile);

                pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x00140009, pszConvertItem[8]));
                pWorkerContext->Callback(item.nId, -1, true, true);
            }
        }

        if (pWorkerContext->bRunning == false)
            return false;

        // encode
        try
        {
            if (pEncFormat->nType == 0)
                pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000A, pszConvertItem[9]));
            else if (pEncFormat->nType == 1)
                pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000B, pszConvertItem[10]));

            item.ResetProgress();

            bool bResult = false;
            if ((encoderContext.bUseReadPipes == false) && (encoderContext.bUseWritePipes == false))
                bResult = ConvertFileUsingConsole(pWorkerContext, encoderContext);
            else
                bResult = ConvertFileUsingPipes(pWorkerContext, encoderContext);
            if (bResult == true)
            {
                // check if output file exists
                /*
                if (::FileExists(szEncOutputFile) == false)
                {
                    if (bIsValidEncoderInput == false)
                    {
                        if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szEncInputFile);
                    }

                    pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000D, pszConvertItem[12]));
                    return false;
                }
                */

                if (bIsValidEncoderInput == false)
                    ::DeleteFile(szDecOutputFile);

                if (pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                    ::DeleteFile(szEncInputFile);

                return true;
            }
            else
            {
                if (bIsValidEncoderInput == false)
                    ::DeleteFile(szDecOutputFile);

                if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szEncOutputFile);

                return false;
            }
        }
        catch (...)
        {
            if (bIsValidEncoderInput == false)
                ::DeleteFile(szDecOutputFile);

            if (pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szEncOutputFile);

            pWorkerContext->Status(item.nId, pszDefaulTime, pWorkerContext->GetString(0x0014000E, pszConvertItem[13]));
            pWorkerContext->Callback(item.nId, -1, true, true);
        }
    }

    return false;
}

bool CWorker::ConvertLoop(IWorkerContext* pWorkerContext, std::queue<CItem> &queue, CSynchronize &sync, CSynchronize &syncDir)
{
    while (TRUE)
    {
        try
        {
            if (sync.Wait() == true)
            {
                if (!queue.empty())
                {
                    CItem item = queue.front();
                    queue.pop();

                    if (sync.Release() == false)
                        return false;

                    if (pWorkerContext->bRunning == false)
                        return false;

                    pWorkerContext->Next(item.nId);
                    if (ConvertItem(pWorkerContext, item, syncDir) == true)
                    {
                        pWorkerContext->nDoneWithoutError++;
                    }
                    else
                    {
                        if (pWorkerContext->pConfig->m_Options.bStopOnErrors == true)
                            return false;
                    }

                    if (pWorkerContext->bRunning == false)
                        return false; 
                }
                else
                {
                    if (sync.Release() == false)
                        return false;

                    return true;
                }
            }
            else
            {
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

void CWorker::Convert(IWorkerContext* pWorkerContext)
{
    int nItems = pWorkerContext->pConfig->m_Items.Count();

    pWorkerContext->nTotalFiles = 0;
    pWorkerContext->nProcessedFiles = 0;
    pWorkerContext->nDoneWithoutError = 0;
    pWorkerContext->nErrors = 0;
    pWorkerContext->nLastItemId = -1;

    std::queue<CItem> queue;
    CSynchronize sync;
    CSynchronize syncDir;

    for (int i = 0; i < nItems; i++)
    {
        CItem& item = pWorkerContext->pConfig->m_Items.Get(i);
        if (item.bChecked == true)
        {
            item.ResetProgress();
            queue.push(item);
            pWorkerContext->nTotalFiles++;
        }
        else
        {
            item.nProgress = 100;
            item.nPreviousProgress = 100;
        }
    }

    pWorkerContext->nThreadCount = pWorkerContext->pConfig->m_Options.nThreadCount;
    if (pWorkerContext->nThreadCount < 1)
    {
        LogicalProcessorInformation info;
        if (GetLogicalProcessorInformation(&info) == 0)
            pWorkerContext->nThreadCount = info.processorCoreCount;
        else
            pWorkerContext->nThreadCount = 1;
    }

    pWorkerContext->Init();

    // single-threaded
    if (pWorkerContext->nThreadCount == 1)
    {
        ConvertLoop(pWorkerContext, queue, sync, syncDir);
    }

    // multi-threaded
    if (pWorkerContext->nThreadCount > 1)
    {
        auto threads = std::make_unique<CThread[]>(pWorkerContext->nThreadCount);
        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
        {
            auto entry = [this, pWorkerContext, &queue, &sync, &syncDir]() 
            { 
                this->ConvertLoop(pWorkerContext, queue, sync, syncDir); 
            };
            if (threads[i].Start(entry, true) == false)
                break;

            if (threads[i].Resume() == false)
                break;
        }

        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
            threads[i].Wait();

        for (int i = 0; i < pWorkerContext->nThreadCount; i++)
            threads[i].Close();
    }

    pWorkerContext->Done();
    pWorkerContext->bDone = true;

    this->m_Thread.Close();
}
