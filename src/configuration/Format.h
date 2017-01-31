// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include "PresetsList.h"

class CFormat
{
public:
    const CString szExtensionsTokens = _T(",");
public:
    CString szId;
    CString szName;
    CString szTemplate;
    bool bPipeInput;
    bool bPipeOutput;
    CString szFunction;
    CString szPath;
    int nType; // 0 - Encoder, 1 - Decoder
    CString szInputExtensions;
    CString szOutputExtension;
    int nDefaultPreset;
    CPresetsList m_Presets;
public:
    CFormat()
    {
    }
    CFormat(const CFormat &other)
    {
        Copy(other);
    }
    CFormat& operator=(const CFormat &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CFormat()
    {
    }
public:
    void Copy(const CFormat &other)
    {
        this->szId = other.szId;
        this->szName = other.szName;
        this->szTemplate = other.szTemplate;
        this->bPipeInput = other.bPipeInput;
        this->bPipeOutput = other.bPipeOutput;
        this->szFunction = other.szFunction;
        this->szPath = other.szPath;
        this->nType = other.nType;
        this->szInputExtensions = other.szInputExtensions;
        this->szOutputExtension = other.szOutputExtension;
        this->nDefaultPreset = other.nDefaultPreset;
        this->m_Presets = other.m_Presets;
    }
public:
    bool IsValidInputExtension(CString szExt)
    {
        int nTokenPos = 0;
        CString strToken = this->szInputExtensions.Tokenize(szExtensionsTokens, nTokenPos);
        while (!strToken.IsEmpty())
        {
            if (strToken.CompareNoCase(szExt) == 0)
                return true;

            strToken = this->szInputExtensions.Tokenize(szExtensionsTokens, nTokenPos);
        }
        return false;
    }
};
