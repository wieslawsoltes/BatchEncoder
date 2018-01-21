// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "XmlPresets.h"
#include "configuration\Format.h"
#include "configuration\FormatsList.h"

#define VALIDATE(value) if (!value) return false

class XmlFormats : public XmlDoc
{
public:
    XmlFormats(XmlDocumnent &doc) : XmlDoc(doc)
    {
    }
    virtual ~XmlFormats()
    {
    }
public:
    bool GetFormat(const XmlElement *element, config::CFormat &m_Format)
    {
        VALIDATE(GetAttributeValue(element, "id", &m_Format.szId));
        VALIDATE(GetAttributeValue(element, "name", &m_Format.szName));
        VALIDATE(GetAttributeValue(element, "template", &m_Format.szTemplate));
        VALIDATE(GetAttributeValue(element, "input", &m_Format.bPipeInput));
        VALIDATE(GetAttributeValue(element, "output", &m_Format.bPipeOutput));
        VALIDATE(GetAttributeValue(element, "function", &m_Format.szFunction));
        VALIDATE(GetAttributeValue(element, "path", &m_Format.szPath));
        VALIDATE(GetAttributeValue(element, "success", &m_Format.nExitCodeSuccess));
        VALIDATE(GetAttributeValue(element, "type", &m_Format.nType));
        VALIDATE(GetAttributeValue(element, "formats", &m_Format.szInputExtensions));
        VALIDATE(GetAttributeValue(element, "extension", &m_Format.szOutputExtension));
        VALIDATE(GetAttributeValue(element, "default", &m_Format.nDefaultPreset));

        auto parent = element->FirstChildElement("Presets");
        if (parent != nullptr)
        {
            VALIDATE(XmlPresets(m_Document).GetPresets(parent, m_Format.m_Presets));
            return true;
        }
        return false;
    }
    void SetFormat(XmlElement *element, config::CFormat &m_Format)
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
        XmlPresets(m_Document).SetPresets(parent, m_Format.m_Presets);
    }
    bool GetFormats(const XmlElement *parent, config::CFormatsList &m_Formats)
    {
        auto element = parent->FirstChildElement("Format");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                config::CFormat format;
                VALIDATE(this->GetFormat(element, format));
                m_Formats.Insert(format);
            }
            return true;
        }
        return false;
    }
    void SetFormats(XmlElement *parent, config::CFormatsList &m_Formats)
    {
        int nFormats = m_Formats.Count();
        for (int i = 0; i < nFormats; i++)
        {
            config::CFormat& format = m_Formats.Get(i);
            auto element = this->NewElement("Format");
            parent->LinkEndChild(element);
            this->SetFormat(element, format);
        }
    }
public:
    bool GetFormat(config::CFormat &m_Format)
    {
        auto element = this->FirstChildElement("Format");
        if (element != nullptr)
        {
            VALIDATE(this->GetFormat(element, m_Format));
            return true;
        }
        return false;
    }
    void SetFormat(config::CFormat &m_Format)
    {
        auto element = this->NewElement("Format");
        this->LinkEndChild(element);
        this->SetFormat(element, m_Format);
    }
    bool GetFormats(config::CFormatsList &m_Formats)
    {
        auto element = this->FirstChildElement("Formats");
        if (element != nullptr)
        {
            VALIDATE(this->GetFormats(element, m_Formats));
            return true;
        }
        return false;
    }
    void SetFormats(config::CFormatsList &m_Formats)
    {
        auto element = this->NewElement("Formats");
        this->LinkEndChild(element);
        this->SetFormats(element, m_Formats);
    }
};
