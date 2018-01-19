// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <afxcmn.h>
#include "xml\XmlDoc.h"
#include "xml\XmlOptions.h"
#include "xml\XmlItems.h"
#include "xml\XmlLanguages.h"
#include "xml\XmlPresets.h"
#include "xml\XmlFormats.h"
#include "xml\XmlTools.h"

class CXmlHelper
{
public:
    CXmlHelper();
    virtual ~CXmlHelper();
public:
    bool LoadOptions(XmlDocumnent &doc, COptions &options);
    bool LoadOptions(CString szFileXml, COptions &options);
    bool SaveOptions(CString szFileXml, COptions &options);
public:
    bool LoadItems(XmlDocumnent &doc, CItemsList &items);
    bool LoadItems(CString szFileXml, CItemsList &items);
    bool SaveItems(CString szFileXml, CItemsList &items);
public:
    bool LoadLanguage(XmlDocumnent &doc, CLanguage &language);
    bool LoadLanguage(CString szFileXml, CLanguage &language);
public:
    bool LoadPresets(XmlDocumnent &doc, CPresetsList &presets);
    bool LoadPresets(CString szFileXml, CPresetsList &presets);
    bool SavePresets(CString szFileXml, CPresetsList &presets);
public:
    bool LoadFormat(XmlDocumnent &doc, CFormat &format);
    bool LoadFormat(CString szFileXml, CFormat &format);
    bool SaveFormat(CString szFileXml, CFormat &format);
public:
    bool LoadFormats(XmlDocumnent &doc, CFormatsList &formats);
    bool LoadFormats(CString szFileXml, CFormatsList &formats);
    bool SaveFormats(CString szFileXml, CFormatsList &formats);
public:
    bool LoadTool(XmlDocumnent &doc, CTool &tool);
    bool LoadTool(CString szFileXml, CTool &tool);
    bool SaveTool(CString szFileXml, CTool &tool);
public:
    bool LoadTools(XmlDocumnent &doc, CToolsList &tools);
    bool LoadTools(CString szFileXml, CToolsList &tools);
    bool SaveTools(CString szFileXml, CToolsList &tools);
};
