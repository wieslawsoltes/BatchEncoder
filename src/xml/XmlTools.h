// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
#include "configuration\Tool.h"
#include "configuration\ToolsList.h"

class XmlTools : public XmlDoc
{
public:
    XmlTools()
    {
    }
    virtual ~XmlTools()
    {
    }
protected:
    void GetTool(XmlElement *element, CTool &m_Tool)
    {
        GetAttributeValue(element, "name", &m_Tool.szName);
        GetAttributeValue(element, "platform", &m_Tool.szPlatform);
        GetAttributeValue(element, "formats", &m_Tool.szFormats);
        GetAttributeValue(element, "url", &m_Tool.szUrl);
        GetAttributeValue(element, "file", &m_Tool.szFile);
        GetAttributeValue(element, "extract", &m_Tool.szExtract);
        GetAttributeValue(element, "path", &m_Tool.szPath);
    }
    void SetTool(XmlElement *element, CTool &m_Tool)
    {
        SetAttributeValue(element, "name", m_Tool.szName);
        SetAttributeValue(element, "platform", m_Tool.szPlatform);
        SetAttributeValue(element, "formats", m_Tool.szFormats);
        SetAttributeValue(element, "url", m_Tool.szUrl);
        SetAttributeValue(element, "file", m_Tool.szFile);
        SetAttributeValue(element, "extract", m_Tool.szExtract);
        SetAttributeValue(element, "path", m_Tool.szPath);
    }
    void GetTools(XmlElement *parent, CToolsList &m_Tools)
    {
        auto element = parent->FirstChildElement("Tool");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CTool tool;
                this->GetTool(element, tool);
                m_Tools.Insert(tool);
            }
        }
    }
    void SetTools(XmlElement *parent, CToolsList &m_Tools)
    {
        int nTools = m_Tools.Count();
        for (int i = 0; i < nTools; i++)
        {
            CTool& tool = m_Tools.Get(i);
            auto element = this->NewElement("Tool");
            parent->LinkEndChild(element);
            this->SetTool(element, tool);
        }
    }
public:
    void GetTool(CTool &m_Tool)
    {
        auto element = this->FirstChildElement("Tool");
        if (element != nullptr)
        {
            this->GetTool(element, m_Tool);
        }
    }
    void SetTool(CTool &m_Tool)
    {
        auto element = this->NewElement("Tool");
        this->LinkEndChild(element);
        this->SetTool(element, m_Tool);
    }
    void GetTools(CToolsList &m_Tools)
    {
        auto element = this->FirstChildElement("Tools");
        if (element != nullptr)
        {
            this->GetTools(element, m_Tools);
        }
    }
    void SetTools(CToolsList &m_Tools)
    {
        auto element = this->NewElement("Tools");
        this->LinkEndChild(element);
        this->SetTools(element, m_Tools);
    }
};
