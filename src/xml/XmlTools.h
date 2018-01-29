// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "configuration\Tool.h"
#include "configuration\ToolsList.h"

#define VALIDATE(value) if (!value) return false

namespace xml
{
    class XmlTools : public XmlDoc
    {
    public:
        XmlTools(XmlDocumnent &doc) : XmlDoc(doc)
        {
        }
        virtual ~XmlTools()
        {
        }
    public:
        bool GetTool(const XmlElement *element, config::CTool &m_Tool)
        {
            VALIDATE(GetAttributeValue(element, "name", &m_Tool.szName));
            VALIDATE(GetAttributeValue(element, "platform", &m_Tool.szPlatform));
            VALIDATE(GetAttributeValue(element, "formats", &m_Tool.szFormats));
            VALIDATE(GetAttributeValue(element, "priority", &m_Tool.nPriority));
            VALIDATE(GetAttributeValue(element, "url", &m_Tool.szUrl));
            VALIDATE(GetAttributeValue(element, "file", &m_Tool.szFile));
            VALIDATE(GetAttributeValue(element, "extract", &m_Tool.szExtract));
            VALIDATE(GetAttributeValue(element, "path", &m_Tool.szPath));
            return true;
        }
        void SetTool(XmlElement *element, config::CTool &m_Tool)
        {
            SetAttributeValue(element, "name", m_Tool.szName);
            SetAttributeValue(element, "platform", m_Tool.szPlatform);
            SetAttributeValue(element, "formats", m_Tool.szFormats);
            SetAttributeValue(element, "priority", m_Tool.nPriority);
            SetAttributeValue(element, "url", m_Tool.szUrl);
            SetAttributeValue(element, "file", m_Tool.szFile);
            SetAttributeValue(element, "extract", m_Tool.szExtract);
            SetAttributeValue(element, "path", m_Tool.szPath);
        }
    public:
        bool GetTool(config::CTool &m_Tool)
        {
            auto element = this->FirstChildElement("Tool");
            if (element != nullptr)
            {
                VALIDATE(this->GetTool(element, m_Tool));
                return true;
            }
            return false;
        }
        void SetTool(config::CTool &m_Tool)
        {
            auto element = this->NewElement("Tool");
            this->LinkEndChild(element);
            this->SetTool(element, m_Tool);
        }
    };
}
