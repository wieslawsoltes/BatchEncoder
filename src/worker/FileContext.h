// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "utilities\Utilities.h"

class CFileContext
{
public:
    CWorkerContext* pWorkerContext;
    CFormat *pFormat;
    int nPreset;
    int nItemId;
    CString szInputFile;
    CString szOutputFile;
    TCHAR pszCommandLine[65536];
    bool bUseReadPipes;
    bool bUseWritePipes;
public:
    CFileContext() { }
    virtual ~CFileContext() { }
public:
    void Init(CWorkerContext *pWorkerContext, CFormat *pFormat, int nPreset, int nItemId, CString szInputFile, CString szOutputFile, bool bUseReadPipes, bool bUseWritePipes)
    {
        CPreset& preset = pFormat->m_Presets.Get(nPreset);

        this->pWorkerContext = pWorkerContext;
        this->pFormat = pFormat;
        this->nPreset = nPreset;
        this->nItemId = nItemId;
        this->szInputFile = szInputFile;
        this->szOutputFile = szOutputFile;
        this->bUseReadPipes = bUseReadPipes;
        this->bUseWritePipes = bUseWritePipes;

        CString szCommandLine = pFormat->szTemplate;

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$EXE"), _T("\"$EXE\""));
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$EXE"), pFormat->szPath);
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OPTIONS"), preset.szOptions);

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$INFILE"), _T("\"$INFILE\""));
        if (bUseReadPipes == true)
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$INFILE"), _T("-"));
        else
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$INFILE"), szInputFile);

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTFILE"), _T("\"$OUTFILE\""));
        if (bUseWritePipes == true)
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTFILE"), _T("-"));
        else
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTFILE"), szOutputFile);

        CString szOutputPath = ::GetFilePath(szOutputFile);
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTPATH"), _T("\"$OUTPATH\""));
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTPATH"), szOutputPath);

        ZeroMemory(this->pszCommandLine, sizeof(this->pszCommandLine));
        lstrcpy(this->pszCommandLine, szCommandLine.GetBuffer(szCommandLine.GetLength()));
    }
};
