// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "utilities\ListT.h"
#include "Format.h"

namespace config
{
    class CFormatsList : public util::CListT<CFormat>
    {
    public:
        int GetFormatById(const std::wstring& szFormatId)
        {
            int nFormats = this->Count();
            for (int i = 0; i < nFormats; i++)
            {
                CFormat& format = this->Get(i);
                if (StringHelper::CompareNoCase(szFormatId, format.szId)) 
                    return i;
            }
            return -1;
        }
        int GetDecoderByExtension(const std::wstring& szExt)
        {
            int nFormats = this->Count();
            for (int i = 0; i < nFormats; i++)
            {
                CFormat& format = this->Get(i);
                if (format.nType == 1 && format.IsValidInputExtension(szExt) == true)
                {
                    return i;
                }
            }
            return -1;
        }
        int GetDecoderByExtensionAndFormat(const std::wstring szExt, CFormat *pEncoderFormat)
        {
            int nFormats = this->Count();
            for (int i = 0; i < nFormats; i++)
            {
                CFormat& format = this->Get(i);
                if (format.nType == 1 && format.IsValidInputExtension(szExt) == true)
                {
                    bool bIsValidEncoderInput = pEncoderFormat->IsValidInputExtension(format.szOutputExtension);
                    if (bIsValidEncoderInput == true)
                        return i;
                }
            }
            return -1;
        }
        bool IsValidInputExtension(const std::wstring szExt)
        {
            int nFormats = this->Count();
            for (int i = 0; i < nFormats; i++)
            {
                CFormat& format = this->Get(i);
                if (format.IsValidInputExtension(szExt) == true)
                    return true;
            }
            return false;
        }
    };
}
