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

            // reset progress bar on start of next file
            pDlg->m_FileProgress.SetPos(0);

            // scroll list to ensure the item is visible
            pDlg->m_LstInputItems.EnsureVisible(i, FALSE);

            ItemContext context;

            context.pDlg = pDlg;
            context.item = &item;

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

    timeCount.StopCounter();

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

    pDlg->FinishConvert();

    return ::CloseHandle(pDlg->hThread);
}
