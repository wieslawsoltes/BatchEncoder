// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "XmlPaths.h"
#include "configuration\Item.h"
#include "configuration\ItemsList.h"

class XmlItems : public XmlPaths
{
public:
    XmlItems()
    {
    }
    virtual ~XmlItems()
    {
    }
protected:
    void GetItem(XmlElement *element, CItem &m_Item)
    {
        GetAttributeValue(element, "id", &m_Item.nId);
        GetAttributeValue(element, "size", &m_Item.szSize);
        GetAttributeValue(element, "name", &m_Item.szName);
        GetAttributeValue(element, "extension", &m_Item.szExtension);
        GetAttributeValue(element, "format", &m_Item.szFormatId);
        GetAttributeValue(element, "preset", &m_Item.nPreset);
        GetAttributeValue(element, "checked", &m_Item.bChecked);
        GetAttributeValue(element, "time", &m_Item.szTime);
        GetAttributeValue(element, "status", &m_Item.szStatus);

        auto parent = element->FirstChildElement("Paths");
        if (parent != nullptr)
        {
            this->GetPaths(parent, m_Item.m_Paths);
        }
    }
    void SetItem(XmlElement *element, CItem &m_Item, int nId)
    {
        SetAttributeValue(element, "id", nId);
        SetAttributeValue(element, "size", m_Item.szSize);
        SetAttributeValue(element, "name", m_Item.szName);
        SetAttributeValue(element, "extension", m_Item.szExtension);
        SetAttributeValue(element, "format", m_Item.szFormatId);
        SetAttributeValue(element, "preset", m_Item.nPreset);
        SetAttributeValue(element, "checked", m_Item.bChecked);
        SetAttributeValue(element, "time", m_Item.szTime);
        SetAttributeValue(element, "status", m_Item.szStatus);

        auto parent = this->NewElement("Paths");
        element->LinkEndChild(parent);
        this->SetPaths(parent, m_Item.m_Paths);
    }
    void GetItems(XmlElement *parent, CItemsList &m_Items)
    {
        auto element = parent->FirstChildElement("Item");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CItem item;
                this->GetItem(element, item);
                m_Items.Insert(item);
            }
        }
    }
    void SetItems(XmlElement *parent, CItemsList &m_Items)
    {
        int nItems = m_Items.Count();
        for (int i = 0; i < nItems; i++)
        {
            CItem& item = m_Items.Get(i);
            auto element = this->NewElement("Item");
            parent->LinkEndChild(element);
            this->SetItem(element, item, i);
        }
    }
public:
    void GetItem(CItem &m_Item)
    {
        auto element = this->FirstChildElement("Item");
        if (element != nullptr)
        {
            this->GetItem(element, m_Item);
        }
    }
    void SetItem(CItem &m_Item)
    {
        auto element = this->NewElement("Item");
        this->LinkEndChild(element);
        this->SetItem(element, m_Item, -1);
    }
    void GetItems(CItemsList &m_Items)
    {
        auto element = this->FirstChildElement("Items");
        if (element != nullptr)
        {
            this->GetItems(element, m_Items);
        }
    }
    void SetItems(CItemsList &m_Items)
    {
        auto element = this->NewElement("Items");
        this->LinkEndChild(element);
        this->SetItems(element, m_Items);
    }
};
