// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <string.h>
#include "tinyxml2\tinyxml2.h" // https://github.com/leethomason/tinyxml2
#include "utilities\Utf8String.h"

namespace xml
{
    typedef tinyxml2::XMLNode XmlNode;
    typedef tinyxml2::XMLElement XmlElement;
    typedef tinyxml2::XMLDocument XmlDocumnent;

    #define VALIDATE(value) if (!value) return false

    class XmlDoc
    {
    protected:
        XmlDocumnent & m_Document;
    public:
        XmlDoc(XmlDocumnent &doc) : m_Document(doc)
        {
        }
        virtual ~XmlDoc()
        {
        }
    public:
        static const char* GetRootName(XmlDocumnent &doc)
        {
            auto root = doc.RootElement();
            if (root != nullptr)
                return root->Name();
            return nullptr;
        }
    public:
        static void Create(XmlDocumnent & doc)
        {
            doc.LinkEndChild(doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
        }
        static bool Open(const std::wstring& szFileName, XmlDocumnent & doc)
        {
            CStdioFile fp;
            if (fp.Open(szFileName.c_str(), CFile::modeRead | CFile::typeBinary) == TRUE)
            {
                auto result = doc.LoadFile(fp.m_pStream);
                fp.Close();
                return result == tinyxml2::XMLError::XML_SUCCESS;
            }
            return false;
        }
        static bool Save(const std::wstring& szFileName, XmlDocumnent & doc)
        {
            CStdioFile fp;
            if (fp.Open(szFileName.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::typeText) == TRUE)
            {
                fputc(0xefU, fp.m_pStream);
                fputc(0xbbU, fp.m_pStream);
                fputc(0xbfU, fp.m_pStream);
                tinyxml2::XMLPrinter printer(fp.m_pStream);
                doc.Print(&printer);
                fp.Close();
                return true;
            }
            return false;
        }
    public:
        const std::wstring m_True = _T("true");
        const std::wstring m_False = _T("false");
    public:
        const std::wstring ToString(const char *pszUtf8)
        {
            if (pszUtf8 == nullptr)
                return std::wstring();
            if (strlen(pszUtf8) == 0)
                return std::wstring();
            return util::CUtf8String::ToUnicode(pszUtf8);
        }
        int ToInt(const char *pszUtf8)
        {
            return _wtoi(ToString(pszUtf8).c_str());
        }
        unsigned __int64 ToUInt64(const char *pszUtf8)
        {
           return _wcstoui64(ToString(pszUtf8).c_str(), nullptr, 10);
        }
        bool ToBool(const char *pszUtf8)
        {
            return _wcsicmp(ToString(pszUtf8).c_str(), m_True.c_str()) == 0;
        }
        std::wstring ToString(const int nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring ToString(const unsigned __int64 nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring ToString(const bool bValue)
        {
            return bValue ? m_True : m_False;
        }
    public:
        bool GetAttributeValue(const XmlElement *element, const char *name, std::wstring *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToString(pszResult).c_str();
                return true;
            }
            return false;
        }
        bool GetAttributeValue(const XmlElement *element, const char *name, int *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToInt(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValue(const XmlElement *element, const char *name, unsigned __int64 *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToUInt64(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValue(const XmlElement *element, const char *name, bool *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToBool(pszResult);
                return true;
            }
            return false;
        }
    public:
        void SetAttributeValue(XmlElement *element, const char *name, const std::wstring& value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(value).c_str());
        }
        void SetAttributeValue(XmlElement *element, const char *name, const int &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(ToString(value)).c_str());
        }
        void SetAttributeValue(XmlElement *element, const char *name, const unsigned __int64 &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(ToString(value)).c_str());
        }
        void SetAttributeValue(XmlElement *element, const char *name, const bool &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(ToString(value)).c_str());
        }
    public:
        bool GetChildValue(const XmlElement *parent, const char *name, std::wstring *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToString(element->GetText()).c_str();
                return true;
            }
            return false;
        }
        bool GetChildValue(const XmlElement *parent, const char *name, int *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToInt(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValue(const XmlElement *parent, const char *name, unsigned __int64 *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToUInt64(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValue(const XmlElement *parent, const char *name, bool *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToBool(element->GetText());
                return true;
            }
            return false;
        }
    public:
        void SetChildValue(XmlElement *parent, const char *name, const std::wstring& value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(value).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValue(XmlElement *parent, const char *name, const int &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(ToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValue(XmlElement *parent, const char *name, const unsigned __int64 &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(ToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValue(XmlElement *parent, const char *name, const bool &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(ToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
    public:
        XmlElement * NewElement(const char *name)
        {
            return m_Document.NewElement(name);
        }
        XmlElement* FirstChildElement(const char *name)
        {
            return m_Document.FirstChildElement(name);
        }
        XmlNode* LinkEndChild(XmlNode* node)
        {
            return m_Document.LinkEndChild(node);
        }
    public:
        void Create()
        {
            XmlDoc::Create(m_Document);
        }
        bool Open(const std::wstring& szFileName)
        {
            return XmlDoc::Open(szFileName, m_Document);
        }
        bool Save(const std::wstring& szFileName)
        {
            return XmlDoc::Save(szFileName, m_Document);
        }
    };
}
