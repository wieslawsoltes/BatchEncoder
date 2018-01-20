// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "language\Language.h"
#include "language\LanguagesList.h"

#define VALIDATE(value) if (!value) return false

class XmlLanguages : public XmlDoc
{
public:
    XmlLanguages(XmlDocumnent &doc) : XmlDoc(doc)
    {
    }
    virtual ~XmlLanguages()
    {
    }
public:
    bool GetLanguage(const XmlElement *parent, CLanguage &m_Language)
    {
        VALIDATE(GetAttributeValue(parent, "id", &m_Language.szId));
        VALIDATE(GetAttributeValue(parent, "original", &m_Language.szOriginalName));
        VALIDATE(GetAttributeValue(parent, "translated", &m_Language.szTranslatedName));

        auto element = parent->FirstChildElement("String");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CString szKey;
                CString szValue;

                VALIDATE(GetAttributeValue(element, "key", &szKey));
                VALIDATE(GetAttributeValue(element, "value", &szValue));

                int nKey;
                _stscanf(szKey, _T("%x"), &nKey);
                m_Language.m_Strings.Insert(nKey, szValue);
            }
            return true;
        }
        return false;
    }
    void SetLanguage(XmlElement *parent, CLanguage &m_Language)
    {
        SetAttributeValue(parent, "id", m_Language.szId);
        SetAttributeValue(parent, "original", m_Language.szOriginalName);
        SetAttributeValue(parent, "translated", m_Language.szTranslatedName);

        POSITION pos = m_Language.m_Strings.m_Map.GetStartPosition();
        while (pos != nullptr)
        {
            CString rValue;
            CString szKey;
            int nKey;
            m_Language.m_Strings.m_Map.GetNextAssoc(pos, nKey, rValue);
            szKey.Format(_T("%X"), nKey);

            auto element = this->NewElement("String");
            parent->LinkEndChild(element);

            SetAttributeValue(element, "", szKey);
            SetAttributeValue(element, "", rValue);
        }
    }
    bool GetLanguages(const XmlElement *parent, CLanguagesList &m_Languages)
    {
        auto element = parent->FirstChildElement("Language");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CLanguage language;
                VALIDATE(this->GetLanguage(element, language));
                m_Languages.Insert(language);
            }
            return true;
        }
        return false;
    }
    void SetLanguages(XmlElement *parent, CLanguagesList &m_Languages)
    {
        int nLanguages = m_Languages.Count();
        for (int i = 0; i < nLanguages; i++)
        {
            CLanguage& language = m_Languages.Get(i);
            auto element = this->NewElement("Language");
            parent->LinkEndChild(element);
            this->SetLanguage(element, language);
        }
    }
public:
    bool GetLanguage(CLanguage &m_Language)
    {
        auto element = this->FirstChildElement("Language");
        if (element != nullptr)
        {
            VALIDATE(this->GetLanguage(element, m_Language));
            return true;
        }
        return false;
    }
    void SetLanguage(CLanguage &m_Language)
    {
        auto element = this->NewElement("Language");
        this->LinkEndChild(element);
        this->SetLanguage(element, m_Language);
    }
    bool GetLanguages(CLanguagesList &m_Languages)
    {
        auto element = this->FirstChildElement("Languages");
        if (element != nullptr)
        {
            VALIDATE(this->GetLanguages(element, m_Languages));
            return true;
        }
        return false;
    }
    void SetLanguages(CLanguagesList &m_Languages)
    {
        auto element = this->NewElement("Languages");
        this->LinkEndChild(element);
        this->SetLanguages(element, m_Languages);
    }
};
