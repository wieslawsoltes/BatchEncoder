// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>

namespace config
{
    class CTool
    {
    private:
        static const wchar_t token = ',';
    public:
        std::wstring szName;
        std::wstring szPlatform;
        std::wstring szFormats;
        std::wstring szUrl;
        std::wstring szFile;
        std::wstring szExtract;
        std::wstring szPath;
        std::wstring szStatus;
    public:
        bool IsValidFormat(const std::wstring& szFormat)
        {
            return util::StringHelper::ContainsNoCase(this->szFormats, szFormat, token);
        }
    };
}
