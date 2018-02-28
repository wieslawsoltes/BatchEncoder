// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "XmlDoc.h"
#include "XmlPresets.h"
#include "config\Format.h"
#include "config\FormatsList.h"

namespace xml
{
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

            int nType;
            VALIDATE(GetAttributeValue(element, "type", &nType));
            m_Format.nType = config::CFormat::FromInt(nType);

            VALIDATE(GetAttributeValue(element, "priority", &m_Format.nPriority));

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

            int nType = config::CFormat::ToInt(m_Format.nType);
            SetAttributeValue(element, "type", nType);

            SetAttributeValue(element, "priority", m_Format.nPriority);

            SetAttributeValue(element, "formats", m_Format.szInputExtensions);
            SetAttributeValue(element, "extension", m_Format.szOutputExtension);
            SetAttributeValue(element, "default", m_Format.nDefaultPreset);

            auto parent = this->NewElement("Presets");
            element->LinkEndChild(parent);
            XmlPresets(m_Document).SetPresets(parent, m_Format.m_Presets);
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
    };
}
