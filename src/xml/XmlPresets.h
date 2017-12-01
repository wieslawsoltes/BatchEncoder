// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlBase.h"
#include "configuration\Preset.h"
#include "configuration\PresetsList.h"

class XmlPresets : public XmlBase
{
public:
    XmlPresets()
    {
    }
    virtual ~XmlPresets()
    {
    }
protected:
    void GetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
    {
        tinyxml2::XMLElement *pPresetElem = pPresetsElem->FirstChildElement("Preset");
        for (pPresetElem; pPresetElem; pPresetElem = pPresetElem->NextSiblingElement())
        {
            const char *pszName = pPresetElem->Attribute("name");
            const char *pszOptions = pPresetElem->Attribute("options");
            if (pszName != NULL && pszOptions != NULL)
            {
                CPreset preset;
                preset.szName = ToCString(pszName);
                preset.szOptions = ToCString(pszOptions);
                m_Presets.Insert(preset);
            }
        }
    }
    void SetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
    {
        tinyxml2::XMLElement *pPresetElem;

        int nPresets = m_Presets.Count();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset& preset = m_Presets.Get(i);
            pPresetElem = this->NewElement("Preset");
            pPresetElem->SetAttribute("name", CUtf8String(preset.szName).m_Result);
            pPresetElem->SetAttribute("options", CUtf8String(preset.szOptions).m_Result);
            pPresetsElem->LinkEndChild(pPresetElem);
        }
    }
public:
    void GetPresets(CPresetsList &m_Presets)
    {
        tinyxml2::XMLElement *pPresetsElem = this->FirstChildElement("Presets");
        if (pPresetsElem != NULL)
            this->GetPresets(pPresetsElem, m_Presets);
    }
    void SetPresets(CPresetsList &m_Presets)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
        this->LinkEndChild(pPresetsElem);

        this->SetPresets(pPresetsElem, m_Presets);
    }
};
