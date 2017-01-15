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
    bool LoadFileW(CString szFileName);
    bool SaveFileW(CString szFileName);
};
