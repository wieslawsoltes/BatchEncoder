// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\tinyxml2.h" // https://github.com/leethomason/tinyxml2
#include "Configuration.h"

#define UTF8_DOCUMENT_DECLARATION "xml version=\"1.0\" encoding=\"UTF-8\""

class CXMLDocumentW : public tinyxml2::XMLDocument
{
public:
    CXMLDocumentW();
    virtual ~CXMLDocumentW();
public:
    void LoadOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options);
    void SaveOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options);
public:
    void LoadPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets);
    void SavePresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets);
public:
    void LoadFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats);
    void SaveFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats);
public:
    void LoadItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items);
    void SaveItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items);
public:
    bool LoadFileW(CString szFileName);
    bool SaveFileW(CString szFileName);
};
