// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\dialogs\BatchEncoderDlg.h"
#include "WorkThread.h"

DWORD WINAPI WorkThread(LPVOID lpParam)
{
    ::UpdatePath();

    CBatchEncoderDlg *pDlg = (CBatchEncoderDlg *)lpParam;
    if (pDlg == NULL)
        return (DWORD)(-1);

    int nTotalFiles = 0;
    int nProcessedFiles = 0;
    int nDoneWithoutError = 0;
    int nErrors = 0;
    int nItems = pDlg->m_Config.m_Items.GetSize();

    for (int i = 0; i < nItems; i++)
    {
        if (pDlg->m_Config.m_Items.GetData(i).bChecked == TRUE)
            nTotalFiles++;
    }

    int nThreadCount = pDlg->m_Config.m_Options.nThreadCount;
    HANDLE* hConvertThread = new HANDLE[nThreadCount];
    DWORD* dwThreadID = new DWORD[nThreadCount];
    CObList queue;

    CTimeCount timeCount;
    timeCount.InitCounter();
    timeCount.StartCounter();

    for (int i = 0; i < nItems; i++)
    {
        // get next file name and check if we need to encode/decode/trans-code
        CItem& item = pDlg->m_Config.m_Items.GetData(i);
        if (item.bChecked == true)
        {
            // update status-bar conversion status
            if (nThreadCount == 1)
            {
                nProcessedFiles++;
                nErrors = (nProcessedFiles - 1) - nDoneWithoutError;
                CString szText;
                szText.Format(_T("Processing item %d of %d (%d Done, %d %s)"),
                    nProcessedFiles,
                    nTotalFiles,
                    nDoneWithoutError,
                    nErrors,
                    ((nErrors == 0) || (nErrors > 1)) ? _T("Errors") : _T("Error"));

                VERIFY(pDlg->m_StatusBar.SetText(szText, 1, 0));
            }

            // reset progress bar on start of next file
            pDlg->m_FileProgress.SetPos(0);

            // scroll list to ensure the item is visible
            pDlg->m_LstInputItems.EnsureVisible(i, FALSE);

            if (nThreadCount > 1)
            {
                // insert work item to queue
                ItemContext* context = new ItemContext(pDlg, &item);
                queue.AddTail(context);
            }
            else
            {
                ItemContext context(pDlg, &item);
                bool bSuccess = ConvertItem(&context);
                if (bSuccess == true)
                {
                    nDoneWithoutError++;
                }
                else
                {
                    // stop conversion process on error
                    if (pDlg->m_Config.m_Options.bStopOnErrors == true)
                        break;
                }

                if (pDlg->bRunning == false)
                    break;
            }
        }
    }

    if (nThreadCount > 1)
    {
        // create worker threads
        for (int i = 0; i < nThreadCount; i++)
        {
            dwThreadID[i] = i;
            hConvertThread[i] = ::CreateThread(NULL, 0, ConvertThread, &queue, CREATE_SUSPENDED, &dwThreadID[i]);
            if (hConvertThread[i] == NULL)
            {
                break;
            }
            ::ResumeThread(hConvertThread[i]);
        }

        // wait for all workers to finish
        ::WaitForMultipleObjects(nThreadCount, hConvertThread, TRUE, INFINITE);

        delete hConvertThread;
        delete dwThreadID;
    }

    timeCount.StopCounter();

    if (nThreadCount == 1)
    {
        if (nProcessedFiles > 0)
        {
            CString szText;
            szText.Format(_T("Done in %s"), ::FormatTime(timeCount.GetTime(), 3));
            pDlg->m_StatusBar.SetText(szText, 1, 0);
        }
        else
        {
            pDlg->m_StatusBar.SetText(_T(""), 1, 0);
        }
    }

    pDlg->FinishConvert();

    return ::CloseHandle(pDlg->hThread);
}
