// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "xml\XmlBase.h"

typedef tinyxml2::XMLElement XmlElement;

class XmlDoc : public XmlBase
{
public:
    XmlDoc()
    {
    }
    virtual ~XmlDoc()
    {
    }
protected:
    void GetAttributeValue(XmlElement *element, const char *name, CString *value)
    {
        const char *pszResult = element->Attribute(name);
        if (pszResult != nullptr)
        {
            (*value) = ToCString(pszResult);
        }
    }
    void GetAttributeValue(XmlElement *element, const char *name, int *value)
    {
        const char *pszResult = element->Attribute(name);
        if (pszResult != nullptr)
        {
            (*value) = ToInt(pszResult);
        }
    }
    void GetAttributeValue(XmlElement *element, const char *name, bool *value)
    {
        const char *pszResult = element->Attribute(name);
        if (pszResult != nullptr)
        {
            (*value) = ToBool(pszResult);
        }
    }
protected:
    void SetAttributeValue(XmlElement *element, const char *name, CString& value)
    {
        element->SetAttribute(name, CUtf8String(value).m_Result);
    }
    void SetAttributeValue(XmlElement *element, const char *name, int &value)
    {
        element->SetAttribute(name, CUtf8String(ToCString(value)).m_Result);
    }
    void SetAttributeValue(XmlElement *element, const char *name, bool &value)
    {
        element->SetAttribute(name, CUtf8String(ToCString(value)).m_Result);
    }
protected:
    void GetChildValue(XmlElement *parent, const char *name, CString *value)
    {
        auto element = parent->FirstChildElement(name);
        if (element != nullptr)
        {
            (*value) = ToCString(element->GetText());
        }
    }
    void GetChildValue(XmlElement *parent, const char *name, int *value)
    {
        auto element = parent->FirstChildElement(name);
        if (element != nullptr)
        {
            (*value) = ToInt(element->GetText());
        }
    }
    void GetChildValue(XmlElement *parent, const char *name, bool *value)
    {
        auto element = parent->FirstChildElement(name);
        if (element != nullptr)
        {
            (*value) = ToBool(element->GetText());
        }
    }
protected:
    void SetChildValue(XmlElement *parent, const char *name, CString& value)
    {
        auto element = this->NewElement(name);
        element->LinkEndChild(this->NewText(CUtf8String(value).m_Result));
        parent->LinkEndChild(element);
    }
    void SetChildValue(XmlElement *parent, const char *name, int &value)
    {
        auto element = this->NewElement(name);
        element->LinkEndChild(this->NewText(CUtf8String(ToCString(value)).m_Result));
        parent->LinkEndChild(element);
    }
    void SetChildValue(XmlElement *parent, const char *name, bool &value)
    {
        auto element = this->NewElement(name);
        element->LinkEndChild(this->NewText(CUtf8String(ToCString(value)).m_Result));
        parent->LinkEndChild(element);
    }
public:
    void Create()
    {
        this->LinkEndChild(this->NewDeclaration(m_Utf8DocumentDeclaration));
    }
};
