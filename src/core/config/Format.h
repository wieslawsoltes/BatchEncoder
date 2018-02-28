// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "utilities\StringHelper.h"
#include "Preset.h"

namespace config
{
    enum class FormatType : int
    {
        Encoder = 0,
        Decoder = 1
    };

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
        FormatType nType;
        int nPriority;
        std::wstring szInputExtensions;
        std::wstring szOutputExtension;
        size_t nDefaultPreset;
        std::vector<CPreset> m_Presets;
    public:
        static int ToInt(const FormatType value)
        {
            return static_cast<int>(value);
        }
        static FormatType FromInt(int value)
        {
            return static_cast<FormatType>(value);
        }
    public:
        bool IsValidInputExtension(const std::wstring& szExt)
        {
            return util::StringHelper::ContainsNoCase(this->szInputExtensions, szExt, token);
        }
    public:
        static size_t GetFormatById(std::vector<CFormat>& formats, const std::wstring& szFormatId)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                CFormat& format = formats[i];
                if (util::StringHelper::CompareNoCase(szFormatId, format.szId))
                    return i;
            }
            return -1;
        }
        static size_t GetDecoderByExtension(std::vector<CFormat>& formats, const std::wstring& szExt)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                CFormat& format = formats[i];
                if (format.nType == FormatType::Decoder && format.IsValidInputExtension(szExt) == true)
                    return i;
            }
            return -1;
        }
        static size_t GetDecoderByExtensionAndFormat(std::vector<CFormat>& formats, const std::wstring& szExt, CFormat *pEncoderFormat)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                CFormat& format = formats[i];
                if (format.nType == FormatType::Decoder && format.IsValidInputExtension(szExt) == true)
                {
                    bool bIsValidEncoderInput = pEncoderFormat->IsValidInputExtension(format.szOutputExtension);
                    if (bIsValidEncoderInput == true)
                        return i;
                }
            }
            return -1;
        }
        static bool IsValidInputExtension(std::vector<CFormat>& formats, const std::wstring& szExt)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                CFormat& format = formats[i];
                if (format.IsValidInputExtension(szExt) == true)
                    return true;
            }
            return false;
        }
        
    public:
        static bool CompareName(const CFormat& a, const CFormat& b)
        {
            return a.szName < b.szName;
        };
        static bool CompareType(const CFormat& a, const CFormat& b)
        {
            return a.nType < b.nType;
        };
        static bool ComparePriority(const CFormat& a, const CFormat& b)
        {
            return a.nPriority < b.nPriority;
        };
        static void Sort(std::vector<CFormat>& formats)
        {
            std::stable_sort(formats.begin(), formats.end(), &CompareName);
            std::stable_sort(formats.begin(), formats.end(), &ComparePriority);
            std::stable_sort(formats.begin(), formats.end(), &CompareType);
        }
    };
}
