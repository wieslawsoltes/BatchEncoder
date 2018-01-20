// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "configuration\Preset.h"
#include "configuration\PresetsList.h"

#define VALIDATE(value) if (!value) return false

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
    bool GetPreset(const XmlElement *element, CPreset &m_Preset)
    {
        VALIDATE(GetAttributeValue(element, "name", &m_Preset.szName));
        VALIDATE(GetAttributeValue(element, "options", &m_Preset.szOptions));
        return true;
    }
    void SetPreset(XmlElement *element, CPreset &m_Preset)
    {
        SetAttributeValue(element, "name", m_Preset.szName);
        SetAttributeValue(element, "options", m_Preset.szOptions);
    }
    bool GetPresets(const XmlElement *parent, CPresetsList &m_Presets)
    {
        auto element = parent->FirstChildElement("Preset");
        if (element != nullptr)
        {
            for (element; element; element = element->NextSiblingElement())
            {
                CPreset preset;
                VALIDATE(this->GetPreset(element, preset));
                m_Presets.Insert(preset);
            }
            return true;
        }
        return false;
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
    bool GetPreset(CPreset &m_Preset)
    {
        auto element = this->FirstChildElement("Preset");
        if (element != nullptr)
        {
            VALIDATE(this->GetPreset(element, m_Preset));
            return true;
        }
        return false;
    }
    void SetPreset(CPreset &m_Preset)
    {
        auto element = this->NewElement("Preset");
        this->LinkEndChild(element);
        this->SetPreset(element, m_Preset);
    }
    bool GetPresets(CPresetsList &m_Presets)
    {
        auto element = this->FirstChildElement("Presets");
        if (element != nullptr)
        {
            VALIDATE(this->GetPresets(element, m_Presets));
            return true;
        }
        return false;
    }
    void SetPresets(CPresetsList &m_Presets)
    {
        auto element = this->NewElement("Presets");
        this->LinkEndChild(element);
        this->SetPresets(element, m_Presets);
    }
};
