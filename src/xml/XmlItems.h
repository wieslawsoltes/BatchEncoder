// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlBase.h"
#include "configuration\Item.h"
#include "configuration\ItemsList.h"

class XmlItems : public XmlBase
{
public:
    XmlItems()
    {
    }
    virtual ~XmlItems()
    {
    }
protected:
    void GetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
    {
        tinyxml2::XMLElement *pItemElem = pItemsElem->FirstChildElement("Item");
        for (pItemElem; pItemElem; pItemElem = pItemElem->NextSiblingElement())
        {
            CItem item;

            const char *pszId = pItemElem->Attribute("id");
            if (pszId != NULL)
                item.nId = _tstoi(ToCString(pszId));

            const char *pszPath = pItemElem->Attribute("path");
            if (pszPath != NULL)
                item.szPath = ToCString(pszPath);

            const char *pszSize = pItemElem->Attribute("size");
            if (pszSize != NULL)
                item.szSize = ToCString(pszSize);

            const char *pszName = pItemElem->Attribute("name");
            if (pszName != NULL)
                item.szName = ToCString(pszName);

            const char *pszExtension = pItemElem->Attribute("extension");
            if (pszExtension != NULL)
                item.szExtension = ToCString(pszExtension);

            const char *pszFormatId = pItemElem->Attribute("format");
            if (pszFormatId != NULL)
                item.szFormatId = ToCString(pszFormatId);

            const char *pszPreset = pItemElem->Attribute("preset");
            if (pszPreset != NULL)
                item.nPreset = ToInt(pszPreset);

            const char *pszChecked = pItemElem->Attribute("checked");
            if (pszChecked != NULL)
                item.bChecked = ToBool(pszChecked);

            const char *pszTime = pItemElem->Attribute("time");
            if (pszTime != NULL)
                item.szTime = ToCString(pszTime);

            const char *pszStatus = pItemElem->Attribute("status");
            if (pszStatus != NULL)
                item.szStatus = ToCString(pszStatus);

            m_Items.Insert(item);
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
            pItemElem->SetAttribute("id", CUtf8String(ToCString(i)).m_Result);
            pItemElem->SetAttribute("path", CUtf8String(item.szPath).m_Result);
            pItemElem->SetAttribute("size", CUtf8String(item.szSize).m_Result);
            pItemElem->SetAttribute("name", CUtf8String(item.szName).m_Result);
            pItemElem->SetAttribute("extension", CUtf8String(item.szExtension).m_Result);
            pItemElem->SetAttribute("format", CUtf8String(item.szFormatId).m_Result);
            pItemElem->SetAttribute("preset", CUtf8String(ToCString(item.nPreset)).m_Result);
            pItemElem->SetAttribute("checked", CUtf8String(ToCString(item.bChecked)).m_Result);
            pItemElem->SetAttribute("time", CUtf8String(item.szTime).m_Result);
            pItemElem->SetAttribute("status", CUtf8String(item.szStatus).m_Result);
            pItemsElem->LinkEndChild(pItemElem);
        }
    }
public:
    void GetItems(CItemsList &m_Items)
    {
        tinyxml2::XMLElement *pItemsElem = this->FirstChildElement("Items");
        if (pItemsElem != NULL)
            this->GetItems(pItemsElem, m_Items);
    }
    void SetItems(CItemsList &m_Items)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pItemsElem = this->NewElement("Items");
        this->LinkEndChild(pItemsElem);

        this->SetItems(pItemsElem, m_Items);
    }
};
