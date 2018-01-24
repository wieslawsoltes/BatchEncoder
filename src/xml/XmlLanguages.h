// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "xml\XmlDoc.h"
#include "language\Language.h"
#include "language\LanguagesList.h"
#include "utilities\StringHelper.h"

#define VALIDATE(value) if (!value) return false

namespace xml
{
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
        bool GetLanguage(const XmlElement *parent, lang::CLanguage &m_Language)
        {
            VALIDATE(GetAttributeValue(parent, "id", &m_Language.szId));
            VALIDATE(GetAttributeValue(parent, "original", &m_Language.szOriginalName));
            VALIDATE(GetAttributeValue(parent, "translated", &m_Language.szTranslatedName));

            auto element = parent->FirstChildElement("String");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    std::wstring szKey;
                    std::wstring szValue;

                    VALIDATE(GetAttributeValue(element, "key", &szKey));
                    VALIDATE(GetAttributeValue(element, "value", &szValue));

                    int nKey = std::stoi(szKey, nullptr, 16);
                    m_Language.m_Strings.Insert(nKey, szValue);
                }
                return true;
            }
            return false;
        }
        void SetLanguage(XmlElement *parent, lang::CLanguage &m_Language)
        {
            SetAttributeValue(parent, "id", m_Language.szId);
            SetAttributeValue(parent, "original", m_Language.szOriginalName);
            SetAttributeValue(parent, "translated", m_Language.szTranslatedName);

            for (auto& item : m_Language.m_Strings.m_Map)
            {
                std::wstring szKey = util::StringHelper::ToHex(item.first);

                auto element = this->NewElement("String");
                parent->LinkEndChild(element);

                SetAttributeValue(element, "", szKey);
                SetAttributeValue(element, "", item.second);
            }
        }
        bool GetLanguages(const XmlElement *parent, lang::CLanguagesList &m_Languages)
        {
            auto element = parent->FirstChildElement("Language");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    lang::CLanguage language;
                    VALIDATE(this->GetLanguage(element, language));
                    m_Languages.Insert(language);
                }
                return true;
            }
            return false;
        }
        void SetLanguages(XmlElement *parent, lang::CLanguagesList &m_Languages)
        {
            int nLanguages = m_Languages.Count();
            for (int i = 0; i < nLanguages; i++)
            {
                lang::CLanguage& language = m_Languages.Get(i);
                auto element = this->NewElement("Language");
                parent->LinkEndChild(element);
                this->SetLanguage(element, language);
            }
        }
    public:
        bool GetLanguage(lang::CLanguage &m_Language)
        {
            auto element = this->FirstChildElement("Language");
            if (element != nullptr)
            {
                VALIDATE(this->GetLanguage(element, m_Language));
                return true;
            }
            return false;
        }
        void SetLanguage(lang::CLanguage &m_Language)
        {
            auto element = this->NewElement("Language");
            this->LinkEndChild(element);
            this->SetLanguage(element, m_Language);
        }
        bool GetLanguages(lang::CLanguagesList &m_Languages)
        {
            auto element = this->FirstChildElement("Languages");
            if (element != nullptr)
            {
                VALIDATE(this->GetLanguages(element, m_Languages));
                return true;
            }
            return false;
        }
        void SetLanguages(lang::CLanguagesList &m_Languages)
        {
            auto element = this->NewElement("Languages");
            this->LinkEndChild(element);
            this->SetLanguages(element, m_Languages);
        }
    };
}
