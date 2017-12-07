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
    void GetLanguage(tinyxml2::XMLElement *pLanguageElem, CLanguage &m_Language)
    {
        GetAttributeValue(pLanguageElem, "id", &m_Language.szId);
        GetAttributeValue(pLanguageElem, "original", &m_Language.szOriginalName);
        GetAttributeValue(pLanguageElem, "translated", &m_Language.szTranslatedName);

        tinyxml2::XMLElement *pStringElem = pLanguageElem->FirstChildElement("String");
        if (pStringElem != NULL)
        {
            for (pStringElem; pStringElem; pStringElem = pStringElem->NextSiblingElement())
            {
                CString szKey;
                CString szValue;

                GetAttributeValue(pStringElem, "key", &szKey);
                GetAttributeValue(pStringElem, "value", &szValue);

                int nKey;
                _stscanf(szKey, _T("%x"), &nKey);
                m_Language.m_Strings.Insert(nKey, szValue);
            }
        }
    }
    void SetLanguage(tinyxml2::XMLElement *pLanguageElem, CLanguage &m_Language)
    {
        SetAttributeValue(pLanguageElem, "id", m_Language.szId);
        SetAttributeValue(pLanguageElem, "original", m_Language.szOriginalName);
        SetAttributeValue(pLanguageElem, "translated", m_Language.szTranslatedName);

        POSITION pos = m_Language.m_Strings.m_Map.GetStartPosition();
        while (pos != NULL)
        {
            CString rValue;
            CString szKey;
            int nKey;
            m_Language.m_Strings.m_Map.GetNextAssoc(pos, nKey, rValue);
            szKey.Format(_T("%X"), nKey);

            tinyxml2::XMLElement *pStringElem = this->NewElement("String");
            pLanguageElem->LinkEndChild(pStringElem);

            SetAttributeValue(pStringElem, "", szKey);
            SetAttributeValue(pStringElem, "", rValue);
        }
    }
    void GetLanguages(tinyxml2::XMLElement *pLanguagesElem, CLanguagesList &m_Languages)
    {
        tinyxml2::XMLElement *pLanguageElem = pLanguagesElem->FirstChildElement("Language");
        if (pLanguageElem != NULL)
        {
            for (pLanguageElem; pLanguageElem; pLanguageElem = pLanguageElem->NextSiblingElement())
            {
                CLanguage language;
                this->GetLanguage(pLanguageElem, language);
                m_Languages.Insert(language);
            }
        }
    }
    void SetLanguages(tinyxml2::XMLElement *pLanguagesElem, CLanguagesList &m_Languages)
    {
        int nLanguages = m_Languages.Count();
        for (int i = 0; i < nLanguages; i++)
        {
            CLanguage& language = m_Languages.Get(i);
            tinyxml2::XMLElement *pLanguageElem = this->NewElement("Language");
            pLanguagesElem->LinkEndChild(pLanguageElem);
            this->SetLanguage(pLanguageElem, language);
        }
    }
public:
    void GetLanguage(CLanguage &m_Language)
    {
        tinyxml2::XMLElement *pLanguageElem = this->FirstChildElement("Language");
        if (pLanguageElem != NULL)
        {
            this->GetLanguage(pLanguageElem, m_Language);
        }
    }
    void SetLanguage(CLanguage &m_Language)
    {
        tinyxml2::XMLElement *pLanguageElem = this->NewElement("Language");
        this->LinkEndChild(pLanguageElem);
        this->SetLanguage(pLanguageElem, m_Language);
    }
    void GetLanguages(CLanguagesList &m_Languages)
    {
        tinyxml2::XMLElement *pLanguagesElem = this->FirstChildElement("Languages");
        if (pLanguagesElem != NULL)
        {
            this->GetLanguages(pLanguagesElem, m_Languages);
        }
    }
    void SetLanguages(CLanguagesList &m_Languages)
    {
        tinyxml2::XMLElement *pLanguagesElem = this->NewElement("Languages");
        this->LinkEndChild(pLanguagesElem);
        this->SetLanguages(pLanguagesElem, m_Languages);
    }
};
