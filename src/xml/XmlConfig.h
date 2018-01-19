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

class CXmlConfig
{
public:
    static bool LoadOptions(XmlDocumnent &doc, COptions &options);
    static bool LoadOptions(CString szFileXml, COptions &options);
    static bool SaveOptions(CString szFileXml, COptions &options);
public:
    static bool LoadItem(XmlDocumnent &doc, CItem &item);
    static bool LoadItem(CString szFileXml, CItem &item);
    static bool SaveItem(CString szFileXml, CItem &item);
public:
    static bool LoadItems(XmlDocumnent &doc, CItemsList &items);
    static bool LoadItems(CString szFileXml, CItemsList &items);
    static bool SaveItems(CString szFileXml, CItemsList &items);
public:
    static bool LoadLanguage(XmlDocumnent &doc, CLanguage &language);
    static bool LoadLanguage(CString szFileXml, CLanguage &language);
    static bool SaveLanguage(CString szFileXml, CLanguage &language);
public:
    static bool LoadLanguages(XmlDocumnent &doc, CLanguagesList &languages);
    static bool LoadLanguages(CString szFileXml, CLanguagesList &languages);
    static bool SaveLanguages(CString szFileXml, CLanguagesList &languages);
public:
    static bool LoadPreset(XmlDocumnent &doc, CPreset &preset);
    static bool LoadPreset(CString szFileXml, CPreset &preset);
    static bool SavePreset(CString szFileXml, CPreset &preset);
public:
    static bool LoadPresets(XmlDocumnent &doc, CPresetsList &presets);
    static bool LoadPresets(CString szFileXml, CPresetsList &presets);
    static bool SavePresets(CString szFileXml, CPresetsList &presets);
public:
    static bool LoadFormat(XmlDocumnent &doc, CFormat &format);
    static bool LoadFormat(CString szFileXml, CFormat &format);
    static bool SaveFormat(CString szFileXml, CFormat &format);
public:
    static bool LoadFormats(XmlDocumnent &doc, CFormatsList &formats);
    static bool LoadFormats(CString szFileXml, CFormatsList &formats);
    static bool SaveFormats(CString szFileXml, CFormatsList &formats);
public:
    static bool LoadTool(XmlDocumnent &doc, CTool &tool);
    static bool LoadTool(CString szFileXml, CTool &tool);
    static bool SaveTool(CString szFileXml, CTool &tool);
public:
    static bool LoadTools(XmlDocumnent &doc, CToolsList &tools);
    static bool LoadTools(CString szFileXml, CToolsList &tools);
    static bool SaveTools(CString szFileXml, CToolsList &tools);
};
