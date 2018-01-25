// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"

namespace worker
{
    class CCommandLine
    {
    public:
        config::CFormat * pFormat;
        int nPreset;
        int nItemId;
        std::wstring szInputFile;
        std::wstring szOutputFile;
        std::wstring szOutputPath;
        bool bUseReadPipes;
        bool bUseWritePipes;
        std::wstring szOptions;
        std::wstring szCommandLine;
    public:
        void Build(config::CFormat *pFormat, int nPreset, int nItemId, const std::wstring& szInputFile, const std::wstring& szOutputFile, bool bUseReadPipes, bool bUseWritePipes, const std::wstring& szAdditionalOptions)
        {
            config::CPreset& preset = pFormat->m_Presets.Get(nPreset);

            this->pFormat = pFormat;
            this->nPreset = nPreset;
            this->nItemId = nItemId;
            this->szInputFile = szInputFile;
            this->szOutputFile = szOutputFile;
            this->bUseReadPipes = bUseReadPipes;
            this->bUseWritePipes = bUseWritePipes;

            if (szAdditionalOptions.length() > 0)
                this->szOptions = preset.szOptions + _T(" ") + szAdditionalOptions;
            else
                this->szOptions = preset.szOptions;

            szCommandLine = pFormat->szTemplate;

            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$EXE"), _T("\"$EXE\""));
            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$EXE"), pFormat->szPath);
            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$OPTIONS"), this->szOptions);

            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$INFILE"), _T("\"$INFILE\""));
            if (bUseReadPipes == true)
               util::StringHelper::ReplaceNoCase(szCommandLine, _T("$INFILE"), _T("-"));
            else
                util::StringHelper::ReplaceNoCase(szCommandLine, _T("$INFILE"), this->szInputFile);

            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$OUTFILE"), _T("\"$OUTFILE\""));
            if (bUseWritePipes == true)
                util::StringHelper::ReplaceNoCase(szCommandLine, _T("$OUTFILE"), _T("-"));
            else
                util::StringHelper::ReplaceNoCase(szCommandLine, _T("$OUTFILE"), this->szOutputFile);

            this->szOutputPath = util::GetFilePath(this->szOutputFile);

            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$OUTPATH"), _T("\"$OUTPATH\""));
            util::StringHelper::ReplaceNoCase(szCommandLine, _T("$OUTPATH"), this->szOutputPath);
        }
    };
}
