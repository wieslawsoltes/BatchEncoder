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
    bool LoadOptions(XmlDocumnent &doc);
    bool LoadOptions(CString szFileXml);
    bool SaveOptions(CString szFileXml);
public:
    bool LoadItems(XmlDocumnent &doc);
    bool LoadItems(CString szFileXml);
    bool SaveItems(CString szFileXml);
public:
    bool LoadLanguage(XmlDocumnent &doc);
    bool LoadLanguage(CString szFileXml);
public:
    bool LoadPresets(XmlDocumnent &doc);
    bool LoadPresets(CString szFileXml);
    bool SavePresets(CString szFileXml);
public:
    bool LoadFormat(XmlDocumnent &doc);
    bool LoadFormat(CString szFileXml);
    bool SaveFormat(CString szFileXml);
public:
    bool LoadFormats(XmlDocumnent &doc);
    bool LoadFormats(CString szFileXml);
    bool SaveFormats(CString szFileXml);
public:
    bool LoadTool(XmlDocumnent &doc);
    bool LoadTool(CString szFileXml);
public:
    bool LoadTools(XmlDocumnent &doc);
    bool LoadTools(CString szFileXml);
    bool SaveTools(CString szFileXml);
};
