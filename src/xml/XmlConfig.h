// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "XmlOptions.h"
#include "XmlItems.h"
#include "XmlLanguages.h"
#include "XmlPresets.h"
#include "XmlFormats.h"
#include "XmlTools.h"

namespace xml
{
    class CXmlConfig
    {
    public:
        static CString GetRootName(CString szFileXml, XmlDocumnent &doc);
    public:
        static bool LoadOptions(XmlDocumnent &doc, config::COptions &options);
        static bool LoadOptions(CString szFileXml, config::COptions &options);
        static bool SaveOptions(CString szFileXml, config::COptions &options);
    public:
        static bool LoadItem(XmlDocumnent &doc, config::CItem &item);
        static bool LoadItem(CString szFileXml, config::CItem &item);
        static bool SaveItem(CString szFileXml, config::CItem &item);
    public:
        static bool LoadItems(XmlDocumnent &doc, config::CItemsList &items);
        static bool LoadItems(CString szFileXml, config::CItemsList &items);
        static bool SaveItems(CString szFileXml, config::CItemsList &items);
    public:
        static bool LoadLanguage(XmlDocumnent &doc, lang::CLanguage &language);
        static bool LoadLanguage(CString szFileXml, lang::CLanguage &language);
        static bool SaveLanguage(CString szFileXml, lang::CLanguage &language);
    public:
        static bool LoadLanguages(XmlDocumnent &doc, lang::CLanguagesList &languages);
        static bool LoadLanguages(CString szFileXml, lang::CLanguagesList &languages);
        static bool SaveLanguages(CString szFileXml, lang::CLanguagesList &languages);
    public:
        static bool LoadPreset(XmlDocumnent &doc, config::CPreset &preset);
        static bool LoadPreset(CString szFileXml, config::CPreset &preset);
        static bool SavePreset(CString szFileXml, config::CPreset &preset);
    public:
        static bool LoadPresets(XmlDocumnent &doc, config::CPresetsList &presets);
        static bool LoadPresets(CString szFileXml, config::CPresetsList &presets);
        static bool SavePresets(CString szFileXml, config::CPresetsList &presets);
    public:
        static bool LoadFormat(XmlDocumnent &doc, config::CFormat &format);
        static bool LoadFormat(CString szFileXml, config::CFormat &format);
        static bool SaveFormat(CString szFileXml, config::CFormat &format);
    public:
        static bool LoadFormats(XmlDocumnent &doc, config::CFormatsList &formats);
        static bool LoadFormats(CString szFileXml, config::CFormatsList &formats);
        static bool SaveFormats(CString szFileXml, config::CFormatsList &formats);
    public:
        static bool LoadTool(XmlDocumnent &doc, config::CTool &tool);
        static bool LoadTool(CString szFileXml, config::CTool &tool);
        static bool SaveTool(CString szFileXml, config::CTool &tool);
    public:
        static bool LoadTools(XmlDocumnent &doc, config::CToolsList &tools);
        static bool LoadTools(CString szFileXml, config::CToolsList &tools);
        static bool SaveTools(CString szFileXml, config::CToolsList &tools);
    };
}
