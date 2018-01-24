// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "XmlOptions.h"
#include "XmlItems.h"
#include "XmlLanguages.h"
#include "XmlPresets.h"
#include "XmlFormats.h"
#include "XmlTools.h"

namespace xml
{
    class CXmlConfig
    {
    public:
        static std::string CXmlConfig::GetRootName(const std::wstring& szFileXml, XmlDocumnent &doc)
        {
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                const char *name = XmlDoc::GetRootName(doc);
                if (name != nullptr)
                    return std::string(name);
            }
            return std::string();
        }
        static bool CXmlConfig::LoadOptions(XmlDocumnent &doc, config::COptions &options)
        {
            XmlOptions xml(doc);
            return xml.GetOptions(options);
        }
        static bool CXmlConfig::LoadOptions(const std::wstring& szFileXml, config::COptions &options)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadOptions(doc, options);
            }
            return false;
        }
        static bool CXmlConfig::SaveOptions(const std::wstring& szFileXml, config::COptions &options)
        {
            XmlDocumnent doc;
            XmlOptions xml(doc);
            xml.Create();
            xml.SetOptions(options);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadItem(XmlDocumnent &doc, config::CItem &item)
        {
            XmlItems xml(doc);
            return xml.GetItem(item);
        }
        static bool CXmlConfig::LoadItem(const std::wstring& szFileXml, config::CItem &item)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadItem(doc, item);
            }
            return false;
        }
        static bool CXmlConfig::SaveItem(const std::wstring& szFileXml, config::CItem &item)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItem(item);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadItems(XmlDocumnent &doc, config::CItemsList &items)
        {
            XmlItems xml(doc);
            return xml.GetItems(items);
        }
        static bool CXmlConfig::LoadItems(const std::wstring& szFileXml, config::CItemsList &items)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadItems(doc, items);
            }
            return false;
        }
        static bool CXmlConfig::SaveItems(const std::wstring& szFileXml, config::CItemsList &items)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItems(items);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadLanguage(XmlDocumnent &doc, lang::CLanguage &language)
        {
            XmlLanguages xml(doc);
            return xml.GetLanguage(language);
        }
        static bool CXmlConfig::LoadLanguage(const std::wstring& szFileXml, lang::CLanguage &language)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadLanguage(doc, language);
            }
            return false;
        }
        static bool CXmlConfig::SaveLanguage(const std::wstring& szFileXml, lang::CLanguage &language)
        {
            XmlDocumnent doc;
            XmlLanguages xml(doc);
            xml.Create();
            xml.SetLanguage(language);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadLanguages(XmlDocumnent &doc, lang::CLanguagesList &languages)
        {
            XmlLanguages xml(doc);
            return xml.GetLanguages(languages);
        }
        static bool CXmlConfig::LoadLanguages(const std::wstring& szFileXml, lang::CLanguagesList &languages)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadLanguages(doc, languages);
            }
            return false;
        }
        static bool CXmlConfig::SaveLanguages(const std::wstring& szFileXml, lang::CLanguagesList &languages)
        {
            XmlDocumnent doc;
            XmlLanguages xml(doc);
            xml.Create();
            xml.SetLanguages(languages);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadPreset(XmlDocumnent &doc, config::CPreset &preset)
        {
            XmlPresets xml(doc);
            return xml.GetPreset(preset);
        }
        static bool CXmlConfig::LoadPreset(const std::wstring& szFileXml, config::CPreset &preset)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadPreset(doc, preset);
            }
            return false;
        }
        static bool CXmlConfig::SavePreset(const std::wstring& szFileXml, config::CPreset &preset)
        {
            XmlDocumnent doc;
            XmlPresets xml(doc);
            xml.Create();
            xml.SetPreset(preset);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadPresets(XmlDocumnent &doc, config::CPresetsList &presets)
        {
            XmlPresets xml(doc);
            return xml.GetPresets(presets);
        }
        static bool CXmlConfig::LoadPresets(const std::wstring& szFileXml, config::CPresetsList &presets)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadPresets(doc, presets);
            }
            return false;
        }
        static bool CXmlConfig::SavePresets(const std::wstring& szFileXml, config::CPresetsList &presets)
        {
            XmlDocumnent doc;
            XmlPresets xml(doc);
            xml.Create();
            xml.SetPresets(presets);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadFormat(XmlDocumnent &doc, config::CFormat &format)
        {
            XmlFormats xml(doc);
            return xml.GetFormat(format);
        }
        static bool CXmlConfig::LoadFormat(const std::wstring& szFileXml, config::CFormat &format)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadFormat(doc, format);
            }
            return false;
        }
        static bool CXmlConfig::SaveFormat(const std::wstring& szFileXml, config::CFormat &format)
        {
            XmlDocumnent doc;
            XmlFormats xml(doc);
            xml.Create();
            xml.SetFormat(format);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadFormats(XmlDocumnent &doc, config::CFormatsList &formats)
        {
            XmlFormats xml(doc);
            return xml.GetFormats(formats);
        }
        static bool CXmlConfig::LoadFormats(const std::wstring& szFileXml, config::CFormatsList &formats)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadFormats(doc, formats);
            }
            return false;
        }
        static bool CXmlConfig::SaveFormats(const std::wstring& szFileXml, config::CFormatsList &formats)
        {
            XmlDocumnent doc;
            XmlFormats xml(doc);
            xml.Create();
            xml.SetFormats(formats);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadTool(XmlDocumnent &doc, config::CTool &tool)
        {
            XmlTools xml(doc);
            return xml.GetTool(tool);
        }
        static bool CXmlConfig::LoadTool(const std::wstring& szFileXml, config::CTool &tool)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadTool(doc, tool);
            }
            return false;
        }
        static bool CXmlConfig::SaveTool(const std::wstring& szFileXml, config::CTool &tool)
        {
            XmlDocumnent doc;
            XmlTools xml(doc);
            xml.Create();
            xml.SetTool(tool);
            return xml.Save(szFileXml);
        }
        static bool CXmlConfig::LoadTools(XmlDocumnent &doc, config::CToolsList &tools)
        {
            XmlTools xml(doc);
            return xml.GetTools(tools);
        }
        static bool CXmlConfig::LoadTools(const std::wstring& szFileXml, config::CToolsList &tools)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                return LoadTools(doc, tools);
            }
            return false;
        }
        static bool CXmlConfig::SaveTools(const std::wstring& szFileXml, config::CToolsList &tools)
        {
            XmlDocumnent doc;
            XmlTools xml(doc);
            xml.Create();
            xml.SetTools(tools);
            return xml.Save(szFileXml);
        }
    };
}
