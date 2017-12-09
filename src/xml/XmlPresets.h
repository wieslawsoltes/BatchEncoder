// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "configuration\Preset.h"
#include "configuration\PresetsList.h"

class XmlPresets : public XmlDoc
{
public:
    XmlPresets(XmlDocumnent &doc) : XmlDoc(doc)
    {
    }
    virtual ~XmlPresets()
    {
    }
public:
    void GetPreset(const XmlElement *element, CPreset &m_Preset)
    {
        GetAttributeValue(element, "name", &m_Preset.szName);
        GetAttributeValue(element, "options", &m_Preset.szOptions);
    }
    void SetPreset(XmlElement *element, CPreset &m_Preset)
    {
        SetAttributeValue(element, "name", m_Preset.szName);
        SetAttributeValue(element, "options", m_Preset.szOptions);
    }
    void GetPresets(const XmlElement *parent, CPresetsList &m_Presets)
    {
        auto element = parent->FirstChildElement("Preset");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CPreset preset;
                this->GetPreset(element, preset);
                m_Presets.Insert(preset);
            }
        }
    }
    void SetPresets(XmlElement *parent, CPresetsList &m_Presets)
    {
        int nPresets = m_Presets.Count();
        for (int i = 0; i < nPresets; i++)
        {
            CPreset& preset = m_Presets.Get(i);
            auto element = this->NewElement("Preset");
            this->SetPreset(element, preset);
            parent->LinkEndChild(element);
        }
    }
public:
    void GetPreset(CPreset &m_Preset)
    {
        auto element = this->FirstChildElement("Preset");
        if (element != nullptr)
        {
            this->GetPreset(element, m_Preset);
        }
    }
    void SetPreset(CPreset &m_Preset)
    {
        auto element = this->NewElement("Preset");
        this->LinkEndChild(element);
        this->SetPreset(element, m_Preset);
    }
    void GetPresets(CPresetsList &m_Presets)
    {
        auto element = this->FirstChildElement("Presets");
        if (element != nullptr)
        {
            this->GetPresets(element, m_Presets);
        }
    }
    void SetPresets(CPresetsList &m_Presets)
    {
        auto element = this->NewElement("Presets");
        this->LinkEndChild(element);
        this->SetPresets(element, m_Presets);
    }
};
