// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
#include "language\Language.h"
#include "language\LanguagesList.h"

class XmlLanguages : public XmlDoc
{
public:
    XmlLanguages()
    {
    }
    virtual ~XmlLanguages()
    {
    }
protected:
    void GetLanguage(XmlElement *parent, CLanguage &m_Language)
    {
        GetAttributeValue(parent, "id", &m_Language.szId);
        GetAttributeValue(parent, "original", &m_Language.szOriginalName);
        GetAttributeValue(parent, "translated", &m_Language.szTranslatedName);

        auto element = parent->FirstChildElement("String");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CString szKey;
                CString szValue;

                GetAttributeValue(element, "key", &szKey);
                GetAttributeValue(element, "value", &szValue);

                int nKey;
                _stscanf(szKey, _T("%x"), &nKey);
                m_Language.m_Strings.Insert(nKey, szValue);
            }
        }
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
    void GetLanguages(XmlElement *parent, CLanguagesList &m_Languages)
    {
        auto element = parent->FirstChildElement("Language");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CLanguage language;
                this->GetLanguage(element, language);
                m_Languages.Insert(language);
            }
        }
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
    void GetLanguage(CLanguage &m_Language)
    {
        auto element = this->FirstChildElement("Language");
        if (element != nullptr)
        {
            this->GetLanguage(element, m_Language);
        }
    }
    void SetLanguage(CLanguage &m_Language)
    {
        auto element = this->NewElement("Language");
        this->LinkEndChild(element);
        this->SetLanguage(element, m_Language);
    }
    void GetLanguages(CLanguagesList &m_Languages)
    {
        auto element = this->FirstChildElement("Languages");
        if (element != nullptr)
        {
            this->GetLanguages(element, m_Languages);
        }
    }
    void SetLanguages(CLanguagesList &m_Languages)
    {
        auto element = this->NewElement("Languages");
        this->LinkEndChild(element);
        this->SetLanguages(element, m_Languages);
    }
};
