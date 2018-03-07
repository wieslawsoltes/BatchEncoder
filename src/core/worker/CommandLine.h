// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\String.h"
#include "utilities\FileSystem.h"

namespace worker
{
    class CCommandLine
    {
    public:
        config::CFormat& format;
        size_t nPreset;
        int nItemId;
        std::wstring szInputFile;
        std::wstring szOutputFile;
        std::wstring szOutputPath;
        bool bUseReadPipes;
        bool bUseWritePipes;
        std::wstring szOptions;
        std::wstring szCommandLine;
    public:
        CCommandLine(
            util::IFileSystem* fs,
            config::CFormat& format,
            size_t nPreset,
            int nItemId,
            const std::wstring& szInputFile,
            const std::wstring& szOutputFile,
            const std::wstring& szAdditionalOptions) : format(format)
        {
            this->nPreset = nPreset;
            this->nItemId = nItemId;
            this->szInputFile = szInputFile;
            this->szOutputFile = szOutputFile;
            this->bUseReadPipes = format.bPipeInput;
            this->bUseWritePipes = format.bPipeOutput;

            this->szOptions = format.m_Presets[nPreset].szOptions;
            if (szAdditionalOptions.length() > 0)
                this->szOptions += L" " + szAdditionalOptions;

            this->szCommandLine = format.szTemplate;

            util::string::ReplaceNoCase(this->szCommandLine, L"$EXE", L"\"$EXE\"");
            util::string::ReplaceNoCase(this->szCommandLine, L"$EXE", format.szPath);
            util::string::ReplaceNoCase(this->szCommandLine, L"$OPTIONS", this->szOptions);

            util::string::ReplaceNoCase(this->szCommandLine, L"$INFILE", L"\"$INFILE\"");
            if (bUseReadPipes == true)
                util::string::ReplaceNoCase(this->szCommandLine, L"$INFILE", L"-");
            else
                util::string::ReplaceNoCase(this->szCommandLine, L"$INFILE", this->szInputFile);

            util::string::ReplaceNoCase(this->szCommandLine, L"$OUTFILE", L"\"$OUTFILE\"");
            if (bUseWritePipes == true)
                util::string::ReplaceNoCase(this->szCommandLine, L"$OUTFILE", L"-");
            else
                util::string::ReplaceNoCase(this->szCommandLine, L"$OUTFILE", this->szOutputFile);

            this->szOutputPath = fs->GetFilePath(this->szOutputFile);

            util::string::ReplaceNoCase(this->szCommandLine, L"$OUTPATH", L"\"$OUTPATH\"");
            util::string::ReplaceNoCase(this->szCommandLine, L"$OUTPATH", this->szOutputPath);
        }
    };
}
