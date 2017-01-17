// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include "PresetsList.h"

class CFormat
{
public:
    CString szId;
    CString szName;
    CString szTemplate;
    bool bInput;
    bool bOutput;
    CString szFunction;
    CString szPath;
    int nType; // 0 - Encoder, 1 - Decoder
    CString szExtension;
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
        this->bInput = other.bInput;
        this->bOutput = other.bOutput;
        this->szFunction = other.szFunction;
        this->szPath = other.szPath;
        this->nType = other.nType;
        this->szExtension = other.szExtension;
        this->nDefaultPreset = other.nDefaultPreset;
        this->m_Presets = other.m_Presets;
    }
};
