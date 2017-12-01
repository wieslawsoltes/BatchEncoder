// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlBase.h"
#include "configuration\Format.h"
#include "configuration\FormatsList.h"

class XmlFormats : public XmlPresets
{
public:
    XmlFormats()
    {
    }
    virtual ~XmlFormats()
    {
    }
protected:
    void GetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
    {
        const char *pszId = pFormatElem->Attribute("id");
        if (pszId != NULL)
            m_Format.szId = ToCString(pszId);

        const char *pszName = pFormatElem->Attribute("name");
        if (pszName != NULL)
            m_Format.szName = ToCString(pszName);

        const char *pszTemplate = pFormatElem->Attribute("template");
        if (pszTemplate != NULL)
            m_Format.szTemplate = ToCString(pszTemplate);

        const char *pszPipesInput = pFormatElem->Attribute("input");
        if (pszPipesInput != NULL)
            m_Format.bPipeInput = ToBool(pszPipesInput);

        const char *pszPipesOutput = pFormatElem->Attribute("output");
        if (pszPipesOutput != NULL)
            m_Format.bPipeOutput = ToBool(pszPipesOutput);

        const char *pszFunction = pFormatElem->Attribute("function");
        if (pszFunction != NULL)
            m_Format.szFunction = ToCString(pszFunction);

        const char *pszPath = pFormatElem->Attribute("path");
        if (pszPath != NULL)
            m_Format.szPath = ToCString(pszPath);

        const char *pszExitCodeSuccess = pFormatElem->Attribute("success");
        if (pszExitCodeSuccess != NULL)
            m_Format.nExitCodeSuccess = ToInt(pszExitCodeSuccess);

        const char *pszType = pFormatElem->Attribute("type");
        if (pszType != NULL)
            m_Format.nType = ToInt(pszType);

        const char *pszFormats = pFormatElem->Attribute("formats");
        if (pszFormats != NULL)
            m_Format.szInputExtensions = ToCString(pszFormats);

        const char *pszExtension = pFormatElem->Attribute("extension");
        if (pszExtension != NULL)
            m_Format.szOutputExtension = ToCString(pszExtension);

        const char *pszDefaultPreset = pFormatElem->Attribute("default");
        if (pszDefaultPreset != NULL)
            m_Format.nDefaultPreset = ToInt(pszDefaultPreset);

        tinyxml2::XMLElement *pPresetsElem = pFormatElem->FirstChildElement("Presets");
        this->GetPresets(pPresetsElem, m_Format.m_Presets);
    }
    void SetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
    {
        pFormatElem->SetAttribute("id", CUtf8String(m_Format.szId).m_Result);
        pFormatElem->SetAttribute("name", CUtf8String(m_Format.szName).m_Result);
        pFormatElem->SetAttribute("template", CUtf8String(m_Format.szTemplate).m_Result);
        pFormatElem->SetAttribute("input", CUtf8String(ToCString(m_Format.bPipeInput)).m_Result);
        pFormatElem->SetAttribute("output", CUtf8String(ToCString(m_Format.bPipeOutput)).m_Result);
        pFormatElem->SetAttribute("function", CUtf8String(m_Format.szFunction).m_Result);
        pFormatElem->SetAttribute("path", CUtf8String(m_Format.szPath).m_Result);
        pFormatElem->SetAttribute("success", CUtf8String(ToCString(m_Format.nExitCodeSuccess)).m_Result);
        pFormatElem->SetAttribute("type", CUtf8String(ToCString(m_Format.nType)).m_Result);
        pFormatElem->SetAttribute("formats", CUtf8String(m_Format.szInputExtensions).m_Result);
        pFormatElem->SetAttribute("extension", CUtf8String(m_Format.szOutputExtension).m_Result);
        pFormatElem->SetAttribute("default", CUtf8String(ToCString(m_Format.nDefaultPreset)).m_Result);

        tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
        pFormatElem->LinkEndChild(pPresetsElem);
        this->SetPresets(pPresetsElem, m_Format.m_Presets);
    }
    void GetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
    {
        tinyxml2::XMLElement *pFormatElem = pFormatsElem->FirstChildElement("Format");
        for (pFormatElem; pFormatElem; pFormatElem = pFormatElem->NextSiblingElement())
        {
            CFormat format;
            this->GetFormat(pFormatElem, format);
            m_Formats.Insert(format);
        }
    }
    void SetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
    {
        int nFormats = m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = m_Formats.Get(i);
            tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
            this->SetFormat(pFormatElem, format);
            pFormatsElem->LinkEndChild(pFormatElem);
        }
    }
public:
    void GetFormat(CFormat &m_Format)
    {
        tinyxml2::XMLElement *pFormatElem = this->FirstChildElement("Format");
        if (pFormatElem != NULL)
            this->GetFormat(pFormatElem, m_Format);
    }
    void SetFormat(CFormat &m_Format)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
        this->LinkEndChild(pFormatElem);

        this->SetFormat(pFormatElem, m_Format);
    }
    void GetFormats(CFormatsList &m_Formats)
    {
        tinyxml2::XMLElement *pFormatsElem = this->FirstChildElement("Formats");
        if (pFormatsElem != NULL)
            this->GetFormats(pFormatsElem, m_Formats);
    }
    void SetFormats(CFormatsList &m_Formats)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pFormatsElem = this->NewElement("Formats");
        this->LinkEndChild(pFormatsElem);

        this->SetFormats(pFormatsElem, m_Formats);
    }
};
