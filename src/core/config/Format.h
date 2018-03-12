// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "utilities\String.h"
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
        FormatType nType;
        int nPriority;
        std::wstring szInputExtensions;
        std::wstring szOutputExtension;
    public:
        std::wstring szTemplate;
        bool bPipeInput;
        bool bPipeOutput;
        std::wstring szFunction;
        std::wstring szPath;
        int nExitCodeSuccess;
    public:
        size_t nDefaultPreset;
        std::vector<CPreset> m_Presets;
    public:
        static inline int ToInt(const FormatType value)
        {
            return static_cast<int>(value);
        }
        static inline FormatType FromInt(int value)
        {
            return static_cast<FormatType>(value);
        }
    public:
        static inline int CountUniqueIds(const std::vector<CFormat>& formats, const std::wstring& id)
        {
            auto predicate = [&id](const CFormat& format)
            {
                return util::string::CompareNoCase(format.szId, id);
            };
            return std::count_if(formats.begin(), formats.end(), predicate);
        }
        static inline int CountUniqueNames(const std::vector<CFormat>& formats, const std::wstring& name)
        {
            auto predicate = [&name](const CFormat& format)
            {
                return util::string::CompareNoCase(format.szName, name);
            };
            return std::count_if(formats.begin(), formats.end(), predicate);
        }
        static inline bool IsUniqueId(const std::vector<CFormat>& formats, const std::wstring& id)
        {
            return CountUniqueIds(formats, id) == 0;
        }
        static inline bool IsUniqueName(const std::vector<CFormat>& formats, const std::wstring& name)
        {
            return CountUniqueNames(formats, name) == 0;
        }
        static inline bool AreIdsUnique(const std::vector<CFormat>& formats)
        {
            for (const auto& format : formats)
            {
                if (CountUniqueIds(formats, format.szId) != 1)
                    return false;
            }
            return true;
        }
        static inline bool AreNamesUnique(const std::vector<CFormat>& formats)
        {
            for (const auto& format : formats)
            {
                if (CountUniqueNames(formats, format.szName) != 1)
                    return false;
            }
            return true;
        }
        static inline size_t GetFormatById(const std::vector<CFormat>& formats, const std::wstring& szFormatId)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                const CFormat& format = formats[i];
                if (util::string::CompareNoCase(szFormatId, format.szId))
                    return i;
            }
            return -1;
        }
        static inline bool IsValidInputExtension(const std::wstring& szInputExtensions, const std::wstring& szExt)
        {
            return util::string::ContainsNoCase(szInputExtensions, szExt, token);
        }
        static inline bool IsValidInputExtension(const std::vector<CFormat>& formats, const std::wstring& szExt)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                const CFormat& format = formats[i];
                if (IsValidInputExtension(format.szInputExtensions, szExt) == true)
                    return true;
            }
            return false;
        }
        static inline size_t GetDecoderByExtension(const std::vector<CFormat>& formats, const std::wstring& szExt)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                const CFormat& format = formats[i];
                if (format.nType == FormatType::Decoder && IsValidInputExtension(format.szInputExtensions, szExt) == true)
                    return i;
            }
            return -1;
        }
        static inline size_t GetDecoderByExtensionAndFormat(const std::vector<CFormat>& formats, const std::wstring& szExt, const CFormat& ef)
        {
            size_t nFormats = formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                const CFormat& format = formats[i];
                if (format.nType == FormatType::Decoder && IsValidInputExtension(format.szInputExtensions, szExt) == true)
                {
                    bool bIsValidEncoderInput = IsValidInputExtension(ef.szInputExtensions, format.szOutputExtension);
                    if (bIsValidEncoderInput == true)
                        return i;
                }
            }
            return -1;
        }
    public:
        static inline bool CompareName(const CFormat& a, const CFormat& b)
        {
            return a.szName < b.szName;
        };
        static inline bool CompareType(const CFormat& a, const CFormat& b)
        {
            return a.nType < b.nType;
        };
        static inline bool ComparePriority(const CFormat& a, const CFormat& b)
        {
            return a.nPriority > b.nPriority;
        };
        static inline void Sort(std::vector<CFormat>& formats)
        {
            std::stable_sort(formats.begin(), formats.end(), &CompareName);
            std::stable_sort(formats.begin(), formats.end(), &ComparePriority);
            std::stable_sort(formats.begin(), formats.end(), &CompareType);
        }
    };
}
