// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "StringHelper.h"
#include "PresetsList.h"

namespace config
{
    class CFormat
    {
    public:
        const wchar_t token = ',';
    public:
        std::wstring szId;
        std::wstring szName;
        std::wstring szTemplate;
        bool bPipeInput;
        bool bPipeOutput;
        std::wstring szFunction;
        std::wstring szPath;
        int nExitCodeSuccess;
        int nType; // 0 - Encoder, 1 - Decoder
        std::wstring szInputExtensions;
        std::wstring szOutputExtension;
        int nDefaultPreset;
        CPresetsList m_Presets;
    public:
        bool IsValidInputExtension(std::wstring& szExt)
        {
            return StringHelper::Contains(this->szInputExtensions, szExt, token);
        }
    };
}
