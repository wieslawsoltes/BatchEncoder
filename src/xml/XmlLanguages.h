// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlBase.h"
#include "language\Language.h"
#include "language\LanguagesList.h"

class XmlLanguages : public XmlBase
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
        const char *pszId = pLanguageElem->Attribute("id");
        if (pszId != NULL)
            m_Language.szId = ToCString(pszId);

        const char *pszOriginal = pLanguageElem->Attribute("original");
        if (pszOriginal != NULL)
            m_Language.szOriginalName = ToCString(pszOriginal);

        const char *pszTranslated = pLanguageElem->Attribute("translated");
        if (pszTranslated != NULL)
            m_Language.szTranslatedName = ToCString(pszTranslated);

        tinyxml2::XMLElement *pStringElem = pLanguageElem->FirstChildElement("String");
        for (pStringElem; pStringElem; pStringElem = pStringElem->NextSiblingElement())
        {
            const char *pszKey = pStringElem->Attribute("key");
            const char *pszValue = pStringElem->Attribute("value");
            if (pszKey != NULL && pszValue != NULL)
            {
                int nKey;
                CString szValue;
                _stscanf(ToCString(pszKey), _T("%x"), &nKey);
                szValue = ToCString(pszValue);
                m_Language.m_Strings.Insert(nKey, szValue);
            }
        }
    }
    void SetLanguage(tinyxml2::XMLElement *pLanguageElem, CLanguage &m_Language)
    {
        pLanguageElem->SetAttribute("id", CUtf8String(m_Language.szId).m_Result);
        pLanguageElem->SetAttribute("original", CUtf8String(m_Language.szOriginalName).m_Result);
        pLanguageElem->SetAttribute("translated", CUtf8String(m_Language.szTranslatedName).m_Result);

        tinyxml2::XMLElement *pStringElem;
        POSITION pos = m_Language.m_Strings.m_Map.GetStartPosition();
        while (pos != NULL)
        {
            CString rValue;
            CString szKey;
            int nKey;
            m_Language.m_Strings.m_Map.GetNextAssoc(pos, nKey, rValue);
            szKey.Format(_T("%X"), nKey);
            pStringElem = this->NewElement("String");
            pStringElem->SetAttribute("key", CUtf8String(szKey).m_Result);
            pStringElem->SetAttribute("value", CUtf8String(rValue).m_Result);
            pLanguageElem->LinkEndChild(pStringElem);
        }
    }
    void GetLanguages(tinyxml2::XMLElement *pLanguagesElem, CLanguagesList &m_Languages)
    {
        tinyxml2::XMLElement *pLanguageElem = pLanguagesElem->FirstChildElement("Language");
        for (pLanguageElem; pLanguageElem; pLanguageElem = pLanguageElem->NextSiblingElement())
        {
            CLanguage language;
            this->GetLanguage(pLanguageElem, language);
            m_Languages.Insert(language);
        }
    }
    void SetLanguages(tinyxml2::XMLElement *pLanguagesElem, CLanguagesList &m_Languages)
    {
        int nLanguages = m_Languages.Count();
        for (int i = 0; i < nLanguages; i++)
        {
            CLanguage& language = m_Languages.Get(i);
            tinyxml2::XMLElement *pLanguageElem = this->NewElement("Language");
            this->SetLanguage(pLanguageElem, language);
            pLanguagesElem->LinkEndChild(pLanguageElem);
        }
    }
public:
    void GetLanguage(CLanguage &m_Language)
    {
        tinyxml2::XMLElement *pLanguageElem = this->FirstChildElement("Language");
        if (pLanguageElem != NULL)
            this->GetLanguage(pLanguageElem, m_Language);
    }
    void SetLanguage(CLanguage &m_Language)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pLanguageElem = this->NewElement("Language");
        this->LinkEndChild(pLanguageElem);

        this->SetLanguage(pLanguageElem, m_Language);
    }
    void GetLanguages(CLanguagesList &m_Languages)
    {
        tinyxml2::XMLElement *pLanguagesElem = this->FirstChildElement("Languages");
        if (pLanguagesElem != NULL)
            this->GetLanguages(pLanguagesElem, m_Languages);
    }
    void SetLanguages(CLanguagesList &m_Languages)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pLanguagesElem = this->NewElement("Languages");
        this->LinkEndChild(pLanguagesElem);

        this->SetLanguages(pLanguagesElem, m_Languages);
    }
};
