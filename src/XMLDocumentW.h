// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// TinyXml2 https://github.com/leethomason/tinyxml2

#pragma once

#include "xml\tinyxml2.h"

class CXMLDocumentW : public tinyxml2::XMLDocument
{
public:
    CXMLDocumentW();
    virtual ~CXMLDocumentW();
public:
    bool LoadFileW(CString szFileName);
    bool SaveFileW(CString szFileName);
};
