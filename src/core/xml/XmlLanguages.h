// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "XmlDoc.h"
#include "utilities\Language.h"
#include "utilities\LanguagesList.h"
#include "utilities\StringHelper.h"

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

                    int nKey = util::StringHelper::ToIntFromHex(szKey);
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
                int nKey = item.first;
                std::wstring szKey = util::StringHelper::ToWStringHex(nKey);

                auto element = this->NewElement("String");
                parent->LinkEndChild(element);

                SetAttributeValue(element, "", szKey);
                SetAttributeValue(element, "", item.second);
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
    };
}
