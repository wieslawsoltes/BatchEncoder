// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
#include "configuration\Path.h"
#include "configuration\PathsList.h"

class XmlPaths : public XmlDoc
{
public:
    XmlPaths()
    {
    }
    virtual ~XmlPaths()
    {
    }
protected:
    void GetPath(tinyxml2::XMLElement *pPathElem, CPath &m_Path)
    {
        GetAttributeValue(pPathElem, "path", &m_Path.szPath);
        GetAttributeValue(pPathElem, "size", &m_Path.szSize);
    }
    void SetPath(tinyxml2::XMLElement *pPathElem, CPath &m_Path)
    {
        SetAttributeValue(pPathElem, "path", m_Path.szPath);
        SetAttributeValue(pPathElem, "size", m_Path.szSize);
    }
    void GetPaths(tinyxml2::XMLElement *pPathsElem, CPathsList &m_Paths)
    {
        tinyxml2::XMLElement *pPathElem = pPathsElem->FirstChildElement("Path");
        if (pPathElem != NULL)
        {
            for (pPathElem; pPathElem; pPathElem = pPathElem->NextSiblingElement())
            {
                CPath path;
                this->GetPath(pPathElem, path);
                m_Paths.Insert(path);
            }
        }
    }
    void SetPaths(tinyxml2::XMLElement *pPathsElem, CPathsList &m_Paths)
    {
        tinyxml2::XMLElement *pPathElem;

        int nPaths = m_Paths.Count();
        for (int i = 0; i < nPaths; i++)
        {
            CPath& path = m_Paths.Get(i);
            pPathElem = this->NewElement("Path");
            pPathsElem->LinkEndChild(pPathElem);
            this->SetPath(pPathElem, path);
        }
    }
public:
    void GetPath(CPath &m_Path)
    {
        tinyxml2::XMLElement *pPathElem = this->FirstChildElement("Path");
        if (pPathElem != NULL)
        {
            this->GetPath(pPathElem, m_Path);
        }
    }
    void SetPath(CPath &m_Path)
    {
        tinyxml2::XMLElement *pPathElem = this->NewElement("Path");
        this->LinkEndChild(pPathElem);
        this->SetPath(pPathElem, m_Path);
    }
    void GetPaths(CPathsList &m_Paths)
    {
        tinyxml2::XMLElement *pPathsElem = this->FirstChildElement("Paths");
        if (pPathsElem != NULL)
        {
            this->GetPaths(pPathsElem, m_Paths);
        }
    }
    void SetPaths(CPathsList &m_Paths)
    {
        tinyxml2::XMLElement *pPathsElem = this->NewElement("Paths");
        this->LinkEndChild(pPathsElem);
        this->SetPaths(pPathsElem, m_Paths);
    }
};
