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
    class XmlConfig
    {
    public:
        static std::string GetRootName(const std::wstring& szFileXml, XmlDocumnent &doc)
        {
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                const char *name = XmlDoc::GetRootName(doc);
                if (name != nullptr)
                    return std::string(name);
            }
            return std::string();
        }
    public:
        static bool LoadOptions(XmlDocumnent &doc, config::COptions &options)
        {
            XmlOptions xml(doc);
            return xml.GetOptions(options);
        }
        static bool LoadOptions(const std::wstring& szFileXml, config::COptions &options)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadOptions(doc, options);
            return false;
        }
        static bool SaveOptions(const std::wstring& szFileXml, config::COptions &options)
        {
            XmlDocumnent doc;
            XmlOptions xml(doc);
            xml.Create();
            xml.SetOptions(options);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadItem(XmlDocumnent &doc, config::CItem &item)
        {
            XmlItems xml(doc);
            return xml.GetItem(item);
        }
        static bool LoadItem(const std::wstring& szFileXml, config::CItem &item)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadItem(doc, item);
            return false;
        }
        static bool SaveItem(const std::wstring& szFileXml, config::CItem &item)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItem(item);
            return xml.Save(szFileXml);
        }
        static bool LoadItems(XmlDocumnent &doc, config::CItemsList &items)
        {
            XmlItems xml(doc);
            return xml.GetItems(items);
        }
        static bool LoadItems(const std::wstring& szFileXml, config::CItemsList &items)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadItems(doc, items);
            return false;
        }
        static bool SaveItems(const std::wstring& szFileXml, config::CItemsList &items)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItems(items);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadLanguage(XmlDocumnent &doc, lang::CLanguage &language)
        {
            XmlLanguages xml(doc);
            return xml.GetLanguage(language);
        }
        static bool LoadLanguage(const std::wstring& szFileXml, lang::CLanguage &language)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadLanguage(doc, language);
            return false;
        }
        static bool SaveLanguage(const std::wstring& szFileXml, lang::CLanguage &language)
        {
            XmlDocumnent doc;
            XmlLanguages xml(doc);
            xml.Create();
            xml.SetLanguage(language);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadPreset(XmlDocumnent &doc, config::CPreset &preset)
        {
            XmlPresets xml(doc);
            return xml.GetPreset(preset);
        }
        static bool LoadPreset(const std::wstring& szFileXml, config::CPreset &preset)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadPreset(doc, preset);
            return false;
        }
        static bool SavePreset(const std::wstring& szFileXml, config::CPreset &preset)
        {
            XmlDocumnent doc;
            XmlPresets xml(doc);
            xml.Create();
            xml.SetPreset(preset);
            return xml.Save(szFileXml);
        }
        static bool LoadPresets(XmlDocumnent &doc, config::CPresetsList &presets)
        {
            XmlPresets xml(doc);
            return xml.GetPresets(presets);
        }
        static bool LoadPresets(const std::wstring& szFileXml, config::CPresetsList &presets)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadPresets(doc, presets);
            return false;
        }
        static bool SavePresets(const std::wstring& szFileXml, config::CPresetsList &presets)
        {
            XmlDocumnent doc;
            XmlPresets xml(doc);
            xml.Create();
            xml.SetPresets(presets);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadFormat(XmlDocumnent &doc, config::CFormat &format)
        {
            XmlFormats xml(doc);
            return xml.GetFormat(format);
        }
        static bool LoadFormat(const std::wstring& szFileXml, config::CFormat &format)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadFormat(doc, format);
            return false;
        }
        static bool SaveFormat(const std::wstring& szFileXml, config::CFormat &format)
        {
            XmlDocumnent doc;
            XmlFormats xml(doc);
            xml.Create();
            xml.SetFormat(format);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadTool(XmlDocumnent &doc, config::CTool &tool)
        {
            XmlTools xml(doc);
            return xml.GetTool(tool);
        }
        static bool LoadTool(const std::wstring& szFileXml, config::CTool &tool)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadTool(doc, tool);
            return false;
        }
        static bool SaveTool(const std::wstring& szFileXml, config::CTool &tool)
        {
            XmlDocumnent doc;
            XmlTools xml(doc);
            xml.Create();
            xml.SetTool(tool);
            return xml.Save(szFileXml);
        }
    };
}
