// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "XmlConfig.h"

namespace xml
{
    CString CXmlConfig::GetRootName(CString szFileXml, XmlDocumnent &doc)
    {
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            const char *name = XmlDoc::GetRootName(doc);
            if (name != nullptr)
                return CString(name);
        }
        return CString();
    }

    bool CXmlConfig::LoadOptions(XmlDocumnent &doc, config::COptions &options)
    {
        XmlOptions xml(doc);
        return xml.GetOptions(options);
    }

    bool CXmlConfig::LoadOptions(CString szFileXml, config::COptions &options)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadOptions(doc, options);
        }
        return false;
    }

    bool CXmlConfig::SaveOptions(CString szFileXml, config::COptions &options)
    {
        XmlDocumnent doc;
        XmlOptions xml(doc);
        xml.Create();
        xml.SetOptions(options);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadItem(XmlDocumnent &doc, config::CItem &item)
    {
        XmlItems xml(doc);
        return xml.GetItem(item);
    }

    bool CXmlConfig::LoadItem(CString szFileXml, config::CItem &item)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadItem(doc, item);
        }
        return false;
    }

    bool CXmlConfig::SaveItem(CString szFileXml, config::CItem &item)
    {
        XmlDocumnent doc;
        XmlItems xml(doc);
        xml.Create();
        xml.SetItem(item);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadItems(XmlDocumnent &doc, config::CItemsList &items)
    {
        XmlItems xml(doc);
        return xml.GetItems(items);
    }

    bool CXmlConfig::LoadItems(CString szFileXml, config::CItemsList &items)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadItems(doc, items);
        }
        return false;
    }

    bool CXmlConfig::SaveItems(CString szFileXml, config::CItemsList &items)
    {
        XmlDocumnent doc;
        XmlItems xml(doc);
        xml.Create();
        xml.SetItems(items);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadLanguage(XmlDocumnent &doc, lang::CLanguage &language)
    {
        XmlLanguages xml(doc);
        return xml.GetLanguage(language);
    }

    bool CXmlConfig::LoadLanguage(CString szFileXml, lang::CLanguage &language)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadLanguage(doc, language);
        }
        return false;
    }

    bool CXmlConfig::SaveLanguage(CString szFileXml, lang::CLanguage &language)
    {
        XmlDocumnent doc;
        XmlLanguages xml(doc);
        xml.Create();
        xml.SetLanguage(language);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadLanguages(XmlDocumnent &doc, lang::CLanguagesList &languages)
    {
        XmlLanguages xml(doc);
        return xml.GetLanguages(languages);
    }

    bool CXmlConfig::LoadLanguages(CString szFileXml, lang::CLanguagesList &languages)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadLanguages(doc, languages);
        }
        return false;
    }

    bool CXmlConfig::SaveLanguages(CString szFileXml, lang::CLanguagesList &languages)
    {
        XmlDocumnent doc;
        XmlLanguages xml(doc);
        xml.Create();
        xml.SetLanguages(languages);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadPreset(XmlDocumnent &doc, config::CPreset &preset)
    {
        XmlPresets xml(doc);
        return xml.GetPreset(preset);
    }

    bool CXmlConfig::LoadPreset(CString szFileXml, config::CPreset &preset)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadPreset(doc, preset);
        }
        return false;
    }

    bool CXmlConfig::SavePreset(CString szFileXml, config::CPreset &preset)
    {
        XmlDocumnent doc;
        XmlPresets xml(doc);
        xml.Create();
        xml.SetPreset(preset);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadPresets(XmlDocumnent &doc, config::CPresetsList &presets)
    {
        XmlPresets xml(doc);
        return xml.GetPresets(presets);
    }

    bool CXmlConfig::LoadPresets(CString szFileXml, config::CPresetsList &presets)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadPresets(doc, presets);
        }
        return false;
    }

    bool CXmlConfig::SavePresets(CString szFileXml, config::CPresetsList &presets)
    {
        XmlDocumnent doc;
        XmlPresets xml(doc);
        xml.Create();
        xml.SetPresets(presets);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadFormat(XmlDocumnent &doc, config::CFormat &format)
    {
        XmlFormats xml(doc);
        return xml.GetFormat(format);
    }

    bool CXmlConfig::LoadFormat(CString szFileXml, config::CFormat &format)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadFormat(doc, format);
        }
        return false;
    }

    bool CXmlConfig::SaveFormat(CString szFileXml, config::CFormat &format)
    {
        XmlDocumnent doc;
        XmlFormats xml(doc);
        xml.Create();
        xml.SetFormat(format);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadFormats(XmlDocumnent &doc, config::CFormatsList &formats)
    {
        XmlFormats xml(doc);
        return xml.GetFormats(formats);
    }

    bool CXmlConfig::LoadFormats(CString szFileXml, config::CFormatsList &formats)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadFormats(doc, formats);
        }
        return false;
    }

    bool CXmlConfig::SaveFormats(CString szFileXml, config::CFormatsList &formats)
    {
        XmlDocumnent doc;
        XmlFormats xml(doc);
        xml.Create();
        xml.SetFormats(formats);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadTool(XmlDocumnent &doc, config::CTool &tool)
    {
        XmlTools xml(doc);
        return xml.GetTool(tool);
    }

    bool CXmlConfig::LoadTool(CString szFileXml, config::CTool &tool)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadTool(doc, tool);
        }
        return false;
    }

    bool CXmlConfig::SaveTool(CString szFileXml, config::CTool &tool)
    {
        XmlDocumnent doc;
        XmlTools xml(doc);
        xml.Create();
        xml.SetTool(tool);
        return xml.Save(szFileXml);
    }

    bool CXmlConfig::LoadTools(XmlDocumnent &doc, config::CToolsList &tools)
    {
        XmlTools xml(doc);
        return xml.GetTools(tools);
    }

    bool CXmlConfig::LoadTools(CString szFileXml, config::CToolsList &tools)
    {
        XmlDocumnent doc;
        if (XmlDoc::Open(szFileXml, doc) == true)
        {
            return LoadTools(doc, tools);
        }
        return false;
    }

    bool CXmlConfig::SaveTools(CString szFileXml, config::CToolsList &tools)
    {
        XmlDocumnent doc;
        XmlTools xml(doc);
        xml.Create();
        xml.SetTools(tools);
        return xml.Save(szFileXml);
    }
}
