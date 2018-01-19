// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "configuration\Path.h"
#include "configuration\PathsList.h"

#define VALIDATE(value) if (!value) return false

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
    bool GetPath(const XmlElement *element, CPath &m_Path)
    {
        VALIDATE(GetAttributeValue(element, "path", &m_Path.szPath));
        VALIDATE(GetAttributeValue(element, "size", &m_Path.szSize));
        return true;
    }
    void SetPath(XmlElement *element, CPath &m_Path)
    {
        SetAttributeValue(element, "path", m_Path.szPath);
        SetAttributeValue(element, "size", m_Path.szSize);
    }
    bool GetPaths(const XmlElement *parent, CPathsList &m_Paths)
    {
        auto element = parent->FirstChildElement("Path");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CPath path;
                VALIDATE(this->GetPath(element, path));
                m_Paths.Insert(path);
            }
            return true;
        }
        return false;
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
    bool GetPath(CPath &m_Path)
    {
        auto element = this->FirstChildElement("Path");
        if (element != nullptr)
        {
            VALIDATE(this->GetPath(element, m_Path));
            return true;
        }
        return false;
    }
    void SetPath(CPath &m_Path)
    {
        auto element = this->NewElement("Path");
        this->LinkEndChild(element);
        this->SetPath(element, m_Path);
    }
    bool GetPaths(CPathsList &m_Paths)
    {
        auto element = this->FirstChildElement("Paths");
        if (element != nullptr)
        {
            VALIDATE(this->GetPaths(element, m_Paths));
            return true;
        }
        return false;
    }
    void SetPaths(CPathsList &m_Paths)
    {
        auto element = this->NewElement("Paths");
        this->LinkEndChild(element);
        this->SetPaths(element, m_Paths);
    }
};
