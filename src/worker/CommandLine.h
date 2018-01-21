// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <cstring>
#include "utilities\Utilities.h"

class CCommandLine
{
public:
    config::CFormat * pFormat;
    int nPreset;
    int nItemId;
    CString szInputFile;
    CString szOutputFile;
    CString szOutputPath;
    bool bUseReadPipes;
    bool bUseWritePipes;
    CString szOptions;
    TCHAR pszCommandLine[65536];
public:
    CCommandLine() { }
    virtual ~CCommandLine() { }
public:
    void Build(config::CFormat *pFormat, int nPreset, int nItemId, CString szInputFile, CString szOutputFile, bool bUseReadPipes, bool bUseWritePipes, CString szAdditionalOptions)
    {
        config::CPreset& preset = pFormat->m_Presets.Get(nPreset);

        this->pFormat = pFormat;
        this->nPreset = nPreset;
        this->nItemId = nItemId;
        this->szInputFile = szInputFile;
        this->szOutputFile = szOutputFile;
        this->bUseReadPipes = bUseReadPipes;
        this->bUseWritePipes = bUseWritePipes;

        if (szAdditionalOptions.GetLength() > 0)
            this->szOptions = preset.szOptions + _T(" ") + szAdditionalOptions;
        else
            this->szOptions = preset.szOptions;

        CString szCommandLine = pFormat->szTemplate;

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$EXE"), _T("\"$EXE\""));
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$EXE"), pFormat->szPath);
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OPTIONS"), this->szOptions);

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$INFILE"), _T("\"$INFILE\""));
        if (bUseReadPipes == true)
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$INFILE"), _T("-"));
        else
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$INFILE"), this->szInputFile);

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTFILE"), _T("\"$OUTFILE\""));
        if (bUseWritePipes == true)
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTFILE"), _T("-"));
        else
            szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTFILE"), this->szOutputFile);

        this->szOutputPath = ::GetFilePath(this->szOutputFile);

        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTPATH"), _T("\"$OUTPATH\""));
        szCommandLine = ReplaceNoCase(szCommandLine, _T("$OUTPATH"), this->szOutputPath);

        std::memset(this->pszCommandLine, 0, sizeof(this->pszCommandLine));
        lstrcpy(this->pszCommandLine, szCommandLine.GetBuffer(szCommandLine.GetLength()));
    }
};
