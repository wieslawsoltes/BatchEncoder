// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "config\Item.h"
#include "config\ItemsList.h"

#define VALIDATE(value) if (!value) return false

namespace xml
{
    class XmlItems : public XmlDoc
    {
    public:
        XmlItems(XmlDocumnent &doc) : XmlDoc(doc)
        {
        }
        virtual ~XmlItems()
        {
        }
    public:
        bool GetPath(const XmlElement *element, config::CPath &m_Path)
        {
            VALIDATE(GetAttributeValue(element, "path", &m_Path.szPath));
            VALIDATE(GetAttributeValue(element, "size", &m_Path.szSize));
            return true;
        }
        void SetPath(XmlElement *element, config::CPath &m_Path)
        {
            SetAttributeValue(element, "path", m_Path.szPath);
            SetAttributeValue(element, "size", m_Path.szSize);
        }
        bool GetPaths(const XmlElement *parent, config::CPathsList &m_Paths)
        {
            auto element = parent->FirstChildElement("Path");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CPath path;
                    VALIDATE(this->GetPath(element, path));
                    m_Paths.Insert(path);
                }
                return true;
            }
            return false;
        }
        void SetPaths(XmlElement *parent, config::CPathsList &m_Paths)
        {
            int nPaths = m_Paths.Count();
            for (int i = 0; i < nPaths; i++)
            {
                config::CPath& path = m_Paths.Get(i);
                auto element = this->NewElement("Path");
                parent->LinkEndChild(element);
                this->SetPath(element, path);
            }
        }
    public:
        bool GetItem(const XmlElement *element, config::CItem &m_Item)
        {
            VALIDATE(GetAttributeValue(element, "id", &m_Item.nId));
            VALIDATE(GetAttributeValue(element, "size", &m_Item.szSize));
            VALIDATE(GetAttributeValue(element, "name", &m_Item.szName));
            VALIDATE(GetAttributeValue(element, "extension", &m_Item.szExtension));
            VALIDATE(GetAttributeValue(element, "format", &m_Item.szFormatId));
            VALIDATE(GetAttributeValue(element, "preset", &m_Item.nPreset));
            VALIDATE(GetAttributeValue(element, "options", &m_Item.szOptions));
            VALIDATE(GetAttributeValue(element, "checked", &m_Item.bChecked));
            VALIDATE(GetAttributeValue(element, "time", &m_Item.szTime));
            VALIDATE(GetAttributeValue(element, "status", &m_Item.szStatus));
            VALIDATE(this->GetPaths(element, m_Item.m_Paths));
            return true;
        }
        void SetItem(XmlElement *element, config::CItem &m_Item, int nId)
        {
            SetAttributeValue(element, "id", nId);
            SetAttributeValue(element, "size", m_Item.szSize);
            SetAttributeValue(element, "name", m_Item.szName);
            SetAttributeValue(element, "extension", m_Item.szExtension);
            SetAttributeValue(element, "format", m_Item.szFormatId);
            SetAttributeValue(element, "preset", m_Item.nPreset);
            SetAttributeValue(element, "options", m_Item.szOptions);
            SetAttributeValue(element, "checked", m_Item.bChecked);
            SetAttributeValue(element, "time", m_Item.szTime);
            SetAttributeValue(element, "status", m_Item.szStatus);
            this->SetPaths(element, m_Item.m_Paths);
        }
        bool GetItems(const XmlElement *parent, config::CItemsList &m_Items)
        {
            auto element = parent->FirstChildElement("Item");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CItem item;
                    VALIDATE(this->GetItem(element, item));
                    m_Items.Insert(item);
                }
                return true;
            }
            return false;
        }
        void SetItems(XmlElement *parent, config::CItemsList &m_Items)
        {
            int nItems = m_Items.Count();
            for (int i = 0; i < nItems; i++)
            {
                config::CItem& item = m_Items.Get(i);
                auto element = this->NewElement("Item");
                parent->LinkEndChild(element);
                this->SetItem(element, item, i);
            }
        }
    public:
        bool GetItem(config::CItem &m_Item)
        {
            auto element = this->FirstChildElement("Item");
            if (element != nullptr)
            {
                VALIDATE(this->GetItem(element, m_Item));
                return true;
            }
            return false;
        }
        void SetItem(config::CItem &m_Item)
        {
            auto element = this->NewElement("Item");
            this->LinkEndChild(element);
            this->SetItem(element, m_Item, -1);
        }
        bool GetItems(config::CItemsList &m_Items)
        {
            auto element = this->FirstChildElement("Items");
            if (element != nullptr)
            {
                VALIDATE(this->GetItems(element, m_Items));
                return true;
            }
            return false;
        }
        void SetItems(config::CItemsList &m_Items)
        {
            auto element = this->NewElement("Items");
            this->LinkEndChild(element);
            this->SetItems(element, m_Items);
        }
    };
}
