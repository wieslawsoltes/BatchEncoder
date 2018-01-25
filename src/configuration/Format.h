// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\StringHelper.h"
#include "PresetsList.h"

namespace config
{
    class CFormat
    {
    private:
        static const wchar_t token = ',';
    public:
        std::wstring szId;
        std::wstring szName;
        std::wstring szTemplate;
        bool bPipeInput;
        bool bPipeOutput;
        std::wstring szFunction;
        std::wstring szPath;
        int nExitCodeSuccess;
        int nType;
        std::wstring szInputExtensions;
        std::wstring szOutputExtension;
        int nDefaultPreset;
        CPresetsList m_Presets;
    public:
        bool IsValidInputExtension(const std::wstring& szExt)
        {
            return util::StringHelper::ContainsNoCase(this->szInputExtensions, szExt, token);
        }
    };
}
