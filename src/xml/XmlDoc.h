// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "xml\XmlBase.h"

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
    void GetAttributeValue(tinyxml2::XMLElement *pElem, const char *pszName, CString *pszValue)
    {
        const char *pszResult = pElem->Attribute(pszName);
        if (pszResult != NULL)
        {
            (*pszValue) = ToCString(pszResult);
        }
    }
    void GetAttributeValue(tinyxml2::XMLElement *pElem, const char *pszName, int *pnValue)
    {
        const char *pszResult = pElem->Attribute(pszName);
        if (pszResult != NULL)
        {
            (*pnValue) = ToInt(pszResult);
        }
    }
    void GetAttributeValue(tinyxml2::XMLElement *pElem, const char *pszName, bool *pbValue)
    {
        const char *pszResult = pElem->Attribute(pszName);
        if (pszResult != NULL)
        {
            (*pbValue) = ToBool(pszResult);
        }
    }
protected:
    void SetAttributeValue(tinyxml2::XMLElement *pElem, const char *pszName, CString& szValue)
    {
        pElem->SetAttribute(pszName, CUtf8String(szValue).m_Result);
    }
    void SetAttributeValue(tinyxml2::XMLElement *pElem, const char *pszName, int &nValue)
    {
        pElem->SetAttribute(pszName, CUtf8String(ToCString(nValue)).m_Result);
    }
    void SetAttributeValue(tinyxml2::XMLElement *pElem, const char *pszName, bool &bValue)
    {
        pElem->SetAttribute(pszName, CUtf8String(ToCString(bValue)).m_Result);
    }
protected:
    void GetChildValue(tinyxml2::XMLElement *pParent, const char *pszName, CString *pszValue)
    {
        tinyxml2::XMLElement *pOptionElem = pParent->FirstChildElement(pszName);
        if (pOptionElem != NULL)
        {
            (*pszValue) = ToCString(pOptionElem->GetText());
        }
    }
    void GetChildValue(tinyxml2::XMLElement *pParent, const char *pszName, int *pnValue)
    {
        tinyxml2::XMLElement *pOptionElem = pParent->FirstChildElement(pszName);
        if (pOptionElem != NULL)
        {
            (*pnValue) = ToInt(pOptionElem->GetText());
        }
    }
    void GetChildValue(tinyxml2::XMLElement *pParent, const char *pszName, bool *pbValue)
    {
        tinyxml2::XMLElement *pOptionElem = pParent->FirstChildElement(pszName);
        if (pOptionElem != NULL)
        {
            (*pbValue) = ToBool(pOptionElem->GetText());
        }
    }
protected:
    void SetChildValue(tinyxml2::XMLElement *pParent, const char *pszName, CString& szValue)
    {
        tinyxml2::XMLElement * pElem = this->NewElement(pszName);
        pElem->LinkEndChild(this->NewText(CUtf8String(szValue).m_Result));
        pParent->LinkEndChild(pElem);
    }
    void SetChildValue(tinyxml2::XMLElement *pParent, const char *pszName, int &nValue)
    {
        tinyxml2::XMLElement * pElem = this->NewElement(pszName);
        pElem->LinkEndChild(this->NewText(CUtf8String(ToCString(nValue)).m_Result));
        pParent->LinkEndChild(pElem);
    }
    void SetChildValue(tinyxml2::XMLElement *pParent, const char *pszName, bool &bValue)
    {
        tinyxml2::XMLElement * pElem = this->NewElement(pszName);
        pElem->LinkEndChild(this->NewText(CUtf8String(ToCString(bValue)).m_Result));
        pParent->LinkEndChild(pElem);
    }
public:
    void Create()
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);
    }
};
