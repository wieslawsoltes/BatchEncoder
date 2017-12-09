// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
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
    void GetFormat(const XmlElement *element, CFormat &m_Format)
    {
        GetAttributeValue(element, "id", &m_Format.szId);
        GetAttributeValue(element, "name", &m_Format.szName);
        GetAttributeValue(element, "template", &m_Format.szTemplate);
        GetAttributeValue(element, "input", &m_Format.bPipeInput);
        GetAttributeValue(element, "output", &m_Format.bPipeOutput);
        GetAttributeValue(element, "function", &m_Format.szFunction);
        GetAttributeValue(element, "path", &m_Format.szPath);
        GetAttributeValue(element, "success", &m_Format.nExitCodeSuccess);
        GetAttributeValue(element, "type", &m_Format.nType);
        GetAttributeValue(element, "formats", &m_Format.szInputExtensions);
        GetAttributeValue(element, "extension", &m_Format.szOutputExtension);
        GetAttributeValue(element, "default", &m_Format.nDefaultPreset);

        auto parent = element->FirstChildElement("Presets");
        if (parent != nullptr)
        {
            this->GetPresets(parent, m_Format.m_Presets);
        }
    }
    void SetFormat(XmlElement *element, CFormat &m_Format)
    {
        SetAttributeValue(element, "id", m_Format.szId);
        SetAttributeValue(element, "name", m_Format.szName);
        SetAttributeValue(element, "template", m_Format.szTemplate);
        SetAttributeValue(element, "input", m_Format.bPipeInput);
        SetAttributeValue(element, "output", m_Format.bPipeOutput);
        SetAttributeValue(element, "function", m_Format.szFunction);
        SetAttributeValue(element, "path", m_Format.szPath);
        SetAttributeValue(element, "success", m_Format.nExitCodeSuccess);
        SetAttributeValue(element, "type", m_Format.nType);
        SetAttributeValue(element, "formats", m_Format.szInputExtensions);
        SetAttributeValue(element, "extension", m_Format.szOutputExtension);
        SetAttributeValue(element, "default", m_Format.nDefaultPreset);

        auto parent = this->NewElement("Presets");
        element->LinkEndChild(parent);
        this->SetPresets(parent, m_Format.m_Presets);
    }
    void GetFormats(const XmlElement *parent, CFormatsList &m_Formats)
    {
        auto element = parent->FirstChildElement("Format");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CFormat format;
                this->GetFormat(element, format);
                m_Formats.Insert(format);
            }
        }
    }
    void SetFormats(XmlElement *parent, CFormatsList &m_Formats)
    {
        int nFormats = m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            CFormat& format = m_Formats.Get(i);
            auto element = this->NewElement("Format");
            parent->LinkEndChild(element);
            this->SetFormat(element, format);
        }
    }
public:
    void GetFormat(CFormat &m_Format)
    {
        auto element = this->FirstChildElement("Format");
        if (element != nullptr)
        {
            this->GetFormat(element, m_Format);
        }
    }
    void SetFormat(CFormat &m_Format)
    {
        auto element = this->NewElement("Format");
        this->LinkEndChild(element);
        this->SetFormat(element, m_Format);
    }
    void GetFormats(CFormatsList &m_Formats)
    {
        auto element = this->FirstChildElement("Formats");
        if (element != nullptr)
        {
            this->GetFormats(element, m_Formats);
        }
    }
    void SetFormats(CFormatsList &m_Formats)
    {
        auto element = this->NewElement("Formats");
        this->LinkEndChild(element);
        this->SetFormats(element, m_Formats);
    }
};
