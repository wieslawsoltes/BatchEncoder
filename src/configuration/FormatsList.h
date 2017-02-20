// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include "ListT.h"
#include "Format.h"

class CFormatsList : public CListT<CFormat>
{
public:
    int GetFormatById(CString szFormatId)
    {
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (szFormatId.CompareNoCase(format.szId) == 0)
                return i;
        }
        return -1;
    }
    int GetDecoderByExtension(CString szExt)
    {
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 1 && format.IsValidInputExtension(szExt) == true)
            {
                return i;
            }
        }
        return -1;
    }
    int GetDecoderByExtensionAndFormat(CString szExt, CFormat *pEncoderFormat)
    {
        int nFormats = this->GetSize();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = this->GetData(i);
            if (format.nType == 1 && format.IsValidInputExtension(szExt) == true)
            {
                bool bIsValidEncoderInput = pEncoderFormat->IsValidInputExtension(format.szOutputExtension);
                if (bIsValidEncoderInput == true)
                    return i;
            }
        }
        return -1;
    }
};
