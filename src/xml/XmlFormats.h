// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
#include "XmlPresets.h"
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
        GetAttributeValue(pFormatElem, "id", &m_Format.szId);
        GetAttributeValue(pFormatElem, "name", &m_Format.szName);
        GetAttributeValue(pFormatElem, "template", &m_Format.szTemplate);
        GetAttributeValue(pFormatElem, "input", &m_Format.bPipeInput);
        GetAttributeValue(pFormatElem, "output", &m_Format.bPipeOutput);
        GetAttributeValue(pFormatElem, "function", &m_Format.szFunction);
        GetAttributeValue(pFormatElem, "path", &m_Format.szPath);
        GetAttributeValue(pFormatElem, "success", &m_Format.nExitCodeSuccess);
        GetAttributeValue(pFormatElem, "type", &m_Format.nType);
        GetAttributeValue(pFormatElem, "formats", &m_Format.szInputExtensions);
        GetAttributeValue(pFormatElem, "extension", &m_Format.szOutputExtension);
        GetAttributeValue(pFormatElem, "default", &m_Format.nDefaultPreset);

        tinyxml2::XMLElement *pPresetsElem = pFormatElem->FirstChildElement("Presets");
        if (pPresetsElem != NULL)
        {
            this->GetPresets(pPresetsElem, m_Format.m_Presets);
        }
    }
    void SetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
    {
        SetAttributeValue(pFormatElem, "id", m_Format.szId);
        SetAttributeValue(pFormatElem, "name", m_Format.szName);
        SetAttributeValue(pFormatElem, "template", m_Format.szTemplate);
        SetAttributeValue(pFormatElem, "input", m_Format.bPipeInput);
        SetAttributeValue(pFormatElem, "output", m_Format.bPipeOutput);
        SetAttributeValue(pFormatElem, "function", m_Format.szFunction);
        SetAttributeValue(pFormatElem, "path", m_Format.szPath);
        SetAttributeValue(pFormatElem, "success", m_Format.nExitCodeSuccess);
        SetAttributeValue(pFormatElem, "type", m_Format.nType);
        SetAttributeValue(pFormatElem, "formats", m_Format.szInputExtensions);
        SetAttributeValue(pFormatElem, "extension", m_Format.szOutputExtension);
        SetAttributeValue(pFormatElem, "default", m_Format.nDefaultPreset);

        tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
        pFormatElem->LinkEndChild(pPresetsElem);
        this->SetPresets(pPresetsElem, m_Format.m_Presets);
    }
    void GetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
    {
        tinyxml2::XMLElement *pFormatElem = pFormatsElem->FirstChildElement("Format");
        if (pFormatElem != NULL)
        {
            for (pFormatElem; pFormatElem; pFormatElem = pFormatElem->NextSiblingElement())
            {
                CFormat format;
                this->GetFormat(pFormatElem, format);
                m_Formats.Insert(format);
            }
        }
    }
    void SetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
    {
        int nFormats = m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = m_Formats.Get(i);
            tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
            pFormatsElem->LinkEndChild(pFormatElem);
            this->SetFormat(pFormatElem, format);
        }
    }
public:
    void GetFormat(CFormat &m_Format)
    {
        tinyxml2::XMLElement *pFormatElem = this->FirstChildElement("Format");
        if (pFormatElem != NULL)
        {
            this->GetFormat(pFormatElem, m_Format);
        }
    }
    void SetFormat(CFormat &m_Format)
    {
        tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
        this->LinkEndChild(pFormatElem);
        this->SetFormat(pFormatElem, m_Format);
    }
    void GetFormats(CFormatsList &m_Formats)
    {
        tinyxml2::XMLElement *pFormatsElem = this->FirstChildElement("Formats");
        if (pFormatsElem != NULL)
        {
            this->GetFormats(pFormatsElem, m_Formats);
        }
    }
    void SetFormats(CFormatsList &m_Formats)
    {
        tinyxml2::XMLElement *pFormatsElem = this->NewElement("Formats");
        this->LinkEndChild(pFormatsElem);
        this->SetFormats(pFormatsElem, m_Formats);
    }
};
