// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "xml\XmlBase.h"
#include "configuration\Tool.h"
#include "configuration\ToolsList.h"

class XmlTools : public XmlBase
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
        const char *pszName = pToolElem->Attribute("name");
        if (pszName != NULL)
            m_Tool.szName = ToCString(pszName);

        const char *pszPlatform = pToolElem->Attribute("platform");
        if (pszPlatform != NULL)
            m_Tool.szPlatform = ToCString(pszPlatform);

        const char *pszFormats = pToolElem->Attribute("formats");
        if (pszFormats != NULL)
            m_Tool.szFormats = ToCString(pszFormats);

        const char *pszUrl = pToolElem->Attribute("url");
        if (pszUrl != NULL)
            m_Tool.szUrl = ToCString(pszUrl);

        const char *pszFile = pToolElem->Attribute("file");
        if (pszFile != NULL)
            m_Tool.szFile = ToCString(pszFile);

        const char *pszExtract = pToolElem->Attribute("extract");
        if (pszExtract != NULL)
            m_Tool.szExtract = ToCString(pszExtract);

        const char *pszPath = pToolElem->Attribute("path");
        if (pszPath != NULL)
            m_Tool.szPath = ToCString(pszPath);
    }
    void SetTool(tinyxml2::XMLElement *pToolElem, CTool &m_Tool)
    {
        pToolElem->SetAttribute("name", CUtf8String(m_Tool.szName).m_Result);
        pToolElem->SetAttribute("platform", CUtf8String(m_Tool.szName).m_Result);
        pToolElem->SetAttribute("formats", CUtf8String(m_Tool.szName).m_Result);
        pToolElem->SetAttribute("url", CUtf8String(m_Tool.szName).m_Result);
        pToolElem->SetAttribute("file", CUtf8String(m_Tool.szName).m_Result);
        pToolElem->SetAttribute("extract", CUtf8String(m_Tool.szName).m_Result);
        pToolElem->SetAttribute("path", CUtf8String(m_Tool.szName).m_Result);
    }
    void GetTools(tinyxml2::XMLElement *pToolsElem, CToolsList &m_Tools)
    {
        tinyxml2::XMLElement *pToolElem = pToolsElem->FirstChildElement("Tool");
        for (pToolElem; pToolElem; pToolElem = pToolElem->NextSiblingElement())
        {
            CTool tool;
            this->GetTool(pToolElem, tool);
            m_Tools.Insert(tool);
        }
    }
    void SetTools(tinyxml2::XMLElement *pToolsElem, CToolsList &m_Tools)
    {
        int nTools = m_Tools.Count();
        for (int i = 0; i < nTools; i++)
        {
            CTool& tool = m_Tools.Get(i);
            tinyxml2::XMLElement *pToolElem = this->NewElement("Tool");
            this->SetTool(pToolElem, tool);
            pToolsElem->LinkEndChild(pToolElem);
        }
    }
public:
    void GetTool(CTool &m_Tool)
    {
        tinyxml2::XMLElement *pToolElem = this->FirstChildElement("Tool");
        if (pToolElem != NULL)
            this->GetTool(pToolElem, m_Tool);
    }
    void SetTool(CTool &m_Tool)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pToolElem = this->NewElement("Tool");
        this->LinkEndChild(pToolElem);

        this->SetTool(pToolElem, m_Tool);
    }
    void GetTools(CToolsList &m_Tools)
    {
        tinyxml2::XMLElement *pToolsElem = this->FirstChildElement("Tools");
        if (pToolsElem != NULL)
            this->GetTools(pToolsElem, m_Tools);
    }
    void SetTools(CToolsList &m_Tools)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pToolsElem = this->NewElement("Tools");
        this->LinkEndChild(pToolsElem);

        this->SetTools(pToolsElem, m_Tools);
    }
};
