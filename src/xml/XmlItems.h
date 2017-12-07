// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
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
    void GetItem(tinyxml2::XMLElement *pItemElem, CItem &m_Item)
    {
        GetAttributeValue(pItemElem, "id", &m_Item.nId);
        GetAttributeValue(pItemElem, "size", &m_Item.szSize);
        GetAttributeValue(pItemElem, "name", &m_Item.szName);
        GetAttributeValue(pItemElem, "extension", &m_Item.szExtension);
        GetAttributeValue(pItemElem, "format", &m_Item.szFormatId);
        GetAttributeValue(pItemElem, "preset", &m_Item.nPreset);
        GetAttributeValue(pItemElem, "checked", &m_Item.bChecked);
        GetAttributeValue(pItemElem, "time", &m_Item.szTime);
        GetAttributeValue(pItemElem, "status", &m_Item.szStatus);

        tinyxml2::XMLElement *pPathsElem = pItemElem->FirstChildElement("Paths");
        if (pPathsElem != NULL)
        {
            this->GetPaths(pPathsElem, m_Item.m_Paths);
        }
    }
    void SetItem(tinyxml2::XMLElement *pItemElem, CItem &m_Item, int nId)
    {
        SetAttributeValue(pItemElem, "id", nId);
        SetAttributeValue(pItemElem, "size", m_Item.szSize);
        SetAttributeValue(pItemElem, "name", m_Item.szName);
        SetAttributeValue(pItemElem, "extension", m_Item.szExtension);
        SetAttributeValue(pItemElem, "format", m_Item.szFormatId);
        SetAttributeValue(pItemElem, "preset", m_Item.nPreset);
        SetAttributeValue(pItemElem, "checked", m_Item.bChecked);
        SetAttributeValue(pItemElem, "time", m_Item.szTime);
        SetAttributeValue(pItemElem, "status", m_Item.szStatus);

        tinyxml2::XMLElement *pPathsElem = this->NewElement("Paths");
        pItemElem->LinkEndChild(pPathsElem);
        this->SetPaths(pPathsElem, m_Item.m_Paths);
    }
    void GetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
    {
        tinyxml2::XMLElement *pItemElem = pItemsElem->FirstChildElement("Item");
        if (pItemElem != NULL)
        {
            for (pItemElem; pItemElem; pItemElem = pItemElem->NextSiblingElement())
            {
                CItem item;
                this->GetItem(pItemElem, item);
                m_Items.Insert(item);
            }
        }
    }
    void SetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
    {
        tinyxml2::XMLElement *pItemElem;
        int nItems = m_Items.Count();
        for (int i = 0; i < nItems; i++)
        {
            CItem& item = m_Items.Get(i);
            pItemElem = this->NewElement("Item");
            pItemsElem->LinkEndChild(pItemElem);
            this->SetItem(pItemElem, item, i);
        }
    }
public:
    void GetItem(CItem &m_Item)
    {
        tinyxml2::XMLElement *pItemElem = this->FirstChildElement("Item");
        if (pItemElem != NULL)
        {
            this->GetItem(pItemElem, m_Item);
        }
    }
    void SetItem(CItem &m_Item)
    {
        tinyxml2::XMLElement *pItemElem = this->NewElement("Item");
        this->LinkEndChild(pItemElem);
        this->SetItem(pItemElem, m_Item, -1);
    }
    void GetItems(CItemsList &m_Items)
    {
        tinyxml2::XMLElement *pItemsElem = this->FirstChildElement("Items");
        if (pItemsElem != NULL)
        {
            this->GetItems(pItemsElem, m_Items);
        }
    }
    void SetItems(CItemsList &m_Items)
    {
        tinyxml2::XMLElement *pItemsElem = this->NewElement("Items");
        this->LinkEndChild(pItemsElem);
        this->SetItems(pItemsElem, m_Items);
    }
};
