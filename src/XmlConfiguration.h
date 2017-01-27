// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\tinyxml2.h" // https://github.com/leethomason/tinyxml2
#include "Configuration.h"

#define UTF8_DOCUMENT_DECLARATION "xml version=\"1.0\" encoding=\"UTF-8\""

class XmlConfiguration : public tinyxml2::XMLDocument
{
public:
    XmlConfiguration();
    virtual ~XmlConfiguration();
private:
    CString ToCString(const char *pszUtf8);
public:
    void GetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options);
    void SetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options);
    void GetOptions(COptions &m_Options);
    void SetOptions(COptions &m_Options);
    void GetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets);
    void SetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets);
    void GetPresets(CPresetsList &m_Presets);
    void SetPresets(CPresetsList &m_Presets);
    void GetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format);
    void SetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format);
    void GetFormat(CFormat &m_Format);
    void SetFormat(CFormat &m_Format);
    void GetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats);
    void SetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats);
    void GetFormats(CFormatsList &m_Formats);
    void SetFormats(CFormatsList &m_Formats);
    void GetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items);
    void SetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items);
    void GetItems(CItemsList &m_Items);
    void SetItems(CItemsList &m_Items);
    bool Open(CString szFileName);
    bool Save(CString szFileName);
};
