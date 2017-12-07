// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
#include "configuration\Preset.h"
#include "configuration\PresetsList.h"

class XmlPresets : public XmlDoc
{
public:
    XmlPresets()
    {
    }
    virtual ~XmlPresets()
    {
    }
protected:
    void GetPreset(tinyxml2::XMLElement *pPresetElem, CPreset &m_Preset)
    {
        GetAttributeValue(pPresetElem, "name", &m_Preset.szName);
        GetAttributeValue(pPresetElem, "options", &m_Preset.szOptions);
    }
    void SetPreset(tinyxml2::XMLElement *pPresetElem, CPreset &m_Preset)
    {
        SetAttributeValue(pPresetElem, "name", m_Preset.szName);
        SetAttributeValue(pPresetElem, "options", m_Preset.szOptions);
    }
    void GetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
    {
        tinyxml2::XMLElement *pPresetElem = pPresetsElem->FirstChildElement("Preset");
        if (pPresetElem != NULL)
        {
            for (pPresetElem; pPresetElem; pPresetElem = pPresetElem->NextSiblingElement())
            {
                CPreset preset;
                this->GetPreset(pPresetElem, preset);
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
            this->SetPreset(pPresetElem, preset);
            pPresetsElem->LinkEndChild(pPresetElem);
        }
    }
public:
    void GetPreset(CPreset &m_Preset)
    {
        tinyxml2::XMLElement *pPresetElem = this->FirstChildElement("Preset");
        if (pPresetElem != NULL)
        {
            this->GetPreset(pPresetElem, m_Preset);
        }
    }
    void SetPreset(CPreset &m_Preset)
    {
        tinyxml2::XMLElement *pPresetElem = this->NewElement("Preset");
        this->LinkEndChild(pPresetElem);
        this->SetPreset(pPresetElem, m_Preset);
    }
    void GetPresets(CPresetsList &m_Presets)
    {
        tinyxml2::XMLElement *pPresetsElem = this->FirstChildElement("Presets");
        if (pPresetsElem != NULL)
        {
            this->GetPresets(pPresetsElem, m_Presets);
        }
    }
    void SetPresets(CPresetsList &m_Presets)
    {
        tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
        this->LinkEndChild(pPresetsElem);
        this->SetPresets(pPresetsElem, m_Presets);
    }
};
