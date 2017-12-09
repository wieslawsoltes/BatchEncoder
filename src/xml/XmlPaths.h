// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "configuration\Path.h"
#include "configuration\PathsList.h"

class XmlPaths : public XmlDoc
{
public:
    XmlPaths(XmlDocumnent &doc) : XmlDoc(doc)
    {
    }
    virtual ~XmlPaths()
    {
    }
public:
    void GetPath(const XmlElement *element, CPath &m_Path)
    {
        GetAttributeValue(element, "path", &m_Path.szPath);
        GetAttributeValue(element, "size", &m_Path.szSize);
    }
    void SetPath(XmlElement *element, CPath &m_Path)
    {
        SetAttributeValue(element, "path", m_Path.szPath);
        SetAttributeValue(element, "size", m_Path.szSize);
    }
    void GetPaths(const XmlElement *parent, CPathsList &m_Paths)
    {
        auto element = parent->FirstChildElement("Path");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CPath path;
                this->GetPath(element, path);
                m_Paths.Insert(path);
            }
        }
    }
    void SetPaths(XmlElement *parent, CPathsList &m_Paths)
    {
        int nPaths = m_Paths.Count();
        for (int i = 0; i < nPaths; i++)
        {
            CPath& path = m_Paths.Get(i);
            auto element = this->NewElement("Path");
            parent->LinkEndChild(element);
            this->SetPath(element, path);
        }
    }
public:
    void GetPath(CPath &m_Path)
    {
        auto element = this->FirstChildElement("Path");
        if (element != nullptr)
        {
            this->GetPath(element, m_Path);
        }
    }
    void SetPath(CPath &m_Path)
    {
        auto element = this->NewElement("Path");
        this->LinkEndChild(element);
        this->SetPath(element, m_Path);
    }
    void GetPaths(CPathsList &m_Paths)
    {
        auto element = this->FirstChildElement("Paths");
        if (element != nullptr)
        {
            this->GetPaths(element, m_Paths);
        }
    }
    void SetPaths(CPathsList &m_Paths)
    {
        auto element = this->NewElement("Paths");
        this->LinkEndChild(element);
        this->SetPaths(element, m_Paths);
    }
};
