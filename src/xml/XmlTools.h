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
    void GetTool(tinyxml2::XMLElement *pToolElem, CTool &m_Tool)
    {
        GetAttributeValue(pToolElem, "name", &m_Tool.szName);
        GetAttributeValue(pToolElem, "platform", &m_Tool.szPlatform);
        GetAttributeValue(pToolElem, "formats", &m_Tool.szFormats);
        GetAttributeValue(pToolElem, "url", &m_Tool.szUrl);
        GetAttributeValue(pToolElem, "file", &m_Tool.szFile);
        GetAttributeValue(pToolElem, "extract", &m_Tool.szExtract);
        GetAttributeValue(pToolElem, "path", &m_Tool.szPath);
    }
    void SetTool(tinyxml2::XMLElement *pToolElem, CTool &m_Tool)
    {
        SetAttributeValue(pToolElem, "name", m_Tool.szName);
        SetAttributeValue(pToolElem, "platform", m_Tool.szPlatform);
        SetAttributeValue(pToolElem, "formats", m_Tool.szFormats);
        SetAttributeValue(pToolElem, "url", m_Tool.szUrl);
        SetAttributeValue(pToolElem, "file", m_Tool.szFile);
        SetAttributeValue(pToolElem, "extract", m_Tool.szExtract);
        SetAttributeValue(pToolElem, "path", m_Tool.szPath);
    }
    void GetTools(tinyxml2::XMLElement *pToolsElem, CToolsList &m_Tools)
    {
        tinyxml2::XMLElement *pToolElem = pToolsElem->FirstChildElement("Tool");
        if (pToolElem != NULL)
        {
            for (pToolElem; pToolElem; pToolElem = pToolElem->NextSiblingElement())
            {
                CTool tool;
                this->GetTool(pToolElem, tool);
                m_Tools.Insert(tool);
            }
        }
    }
    void SetTools(tinyxml2::XMLElement *pToolsElem, CToolsList &m_Tools)
    {
        int nTools = m_Tools.Count();
        for (int i = 0; i < nTools; i++)
        {
            CTool& tool = m_Tools.Get(i);
            tinyxml2::XMLElement *pToolElem = this->NewElement("Tool");
            pToolsElem->LinkEndChild(pToolElem);
            this->SetTool(pToolElem, tool);
        }
    }
public:
    void GetTool(CTool &m_Tool)
    {
        tinyxml2::XMLElement *pToolElem = this->FirstChildElement("Tool");
        if (pToolElem != NULL)
        {
            this->GetTool(pToolElem, m_Tool);
        }
    }
    void SetTool(CTool &m_Tool)
    {
        tinyxml2::XMLElement *pToolElem = this->NewElement("Tool");
        this->LinkEndChild(pToolElem);
        this->SetTool(pToolElem, m_Tool);
    }
    void GetTools(CToolsList &m_Tools)
    {
        tinyxml2::XMLElement *pToolsElem = this->FirstChildElement("Tools");
        if (pToolsElem != NULL)
        {
            this->GetTools(pToolsElem, m_Tools);
        }
    }
    void SetTools(CToolsList &m_Tools)
    {
        tinyxml2::XMLElement *pToolsElem = this->NewElement("Tools");
        this->LinkEndChild(pToolsElem);
        this->SetTools(pToolsElem, m_Tools);
    }
};
