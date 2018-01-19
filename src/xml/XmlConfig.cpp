// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "XmlConfig.h"

CString CXmlConfig::GetRootName(CString szFileXml)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return CString(XmlDoc::GetRootName(doc));
    }
    return nullptr;
}

bool CXmlConfig::LoadOptions(XmlDocumnent &doc, COptions &options)
{
    XmlOptions xml(doc);
    xml.GetOptions(options);
    return true;
}

bool CXmlConfig::LoadOptions(CString szFileXml, COptions &options)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadOptions(doc, options);
    }
    return false;
}

bool CXmlConfig::SaveOptions(CString szFileXml, COptions &options)
{
    XmlDocumnent doc;
    XmlOptions xml(doc);
    xml.Create();
    xml.SetOptions(options);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadItem(XmlDocumnent &doc, CItem &item)
{
    XmlItems xml(doc);
    xml.GetItem(item);
    return true;
}

bool CXmlConfig::LoadItem(CString szFileXml, CItem &item)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadItem(doc, item);
    }
    return false;
}

bool CXmlConfig::SaveItem(CString szFileXml, CItem &item)
{
    XmlDocumnent doc;
    XmlItems xml(doc);
    xml.Create();
    xml.SetItem(item);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadItems(XmlDocumnent &doc, CItemsList &items)
{
    XmlItems xml(doc);
    xml.GetItems(items);
    return true;
}

bool CXmlConfig::LoadItems(CString szFileXml, CItemsList &items)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadItems(doc, items);
    }
    return false;
}

bool CXmlConfig::SaveItems(CString szFileXml, CItemsList &items)
{
    XmlDocumnent doc;
    XmlItems xml(doc);
    xml.Create();
    xml.SetItems(items);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadLanguage(XmlDocumnent &doc, CLanguage &language)
{
    XmlLanguages xml(doc);
    xml.GetLanguage(language);
    return true;
}

bool CXmlConfig::LoadLanguage(CString szFileXml, CLanguage &language)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadLanguage(doc, language);
    }
    return false;
}

bool CXmlConfig::SaveLanguage(CString szFileXml, CLanguage &language)
{
    XmlDocumnent doc;
    XmlLanguages xml(doc);
    xml.Create();
    xml.SetLanguage(language);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadLanguages(XmlDocumnent &doc, CLanguagesList &languages)
{
    XmlLanguages xml(doc);
    xml.GetLanguages(languages);
    return true;
}

bool CXmlConfig::LoadLanguages(CString szFileXml, CLanguagesList &languages)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadLanguages(doc, languages);
    }
    return false;
}

bool CXmlConfig::SaveLanguages(CString szFileXml, CLanguagesList &languages)
{
    XmlDocumnent doc;
    XmlLanguages xml(doc);
    xml.Create();
    xml.SetLanguages(languages);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadPreset(XmlDocumnent &doc, CPreset &preset)
{
    XmlPresets xml(doc);
    xml.GetPreset(preset);
    return true;
}

bool CXmlConfig::LoadPreset(CString szFileXml, CPreset &preset)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadPreset(doc, preset);
    }
    return false;
}

bool CXmlConfig::SavePreset(CString szFileXml, CPreset &preset)
{
    XmlDocumnent doc;
    XmlPresets xml(doc);
    xml.Create();
    xml.SetPreset(preset);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadPresets(XmlDocumnent &doc, CPresetsList &presets)
{
    XmlPresets xml(doc);
    xml.GetPresets(presets);
    return true;
}

bool CXmlConfig::LoadPresets(CString szFileXml, CPresetsList &presets)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadPresets(doc, presets);
    }
    return false;
}

bool CXmlConfig::SavePresets(CString szFileXml, CPresetsList &presets)
{
    XmlDocumnent doc;
    XmlPresets xml(doc);
    xml.Create();
    xml.SetPresets(presets);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadFormat(XmlDocumnent &doc, CFormat &format)
{
    XmlFormats xml(doc);
    xml.GetFormat(format);
    return true;
}

bool CXmlConfig::LoadFormat(CString szFileXml, CFormat &format)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadFormat(doc, format);
    }
    return false;
}

bool CXmlConfig::SaveFormat(CString szFileXml, CFormat &format)
{
    XmlDocumnent doc;
    XmlFormats xml(doc);
    xml.Create();
    xml.SetFormat(format);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadFormats(XmlDocumnent &doc, CFormatsList &formats)
{
    XmlFormats xml(doc);
    xml.GetFormats(formats);
    return true;
}

bool CXmlConfig::LoadFormats(CString szFileXml, CFormatsList &formats)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadFormats(doc, formats);
    }
    return false;
}

bool CXmlConfig::SaveFormats(CString szFileXml, CFormatsList &formats)
{
    XmlDocumnent doc;
    XmlFormats xml(doc);
    xml.Create();
    xml.SetFormats(formats);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadTool(XmlDocumnent &doc, CTool &tool)
{
    XmlTools xml(doc);
    xml.GetTool(tool);
    return true;
}

bool CXmlConfig::LoadTool(CString szFileXml, CTool &tool)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadTool(doc, tool);
    }
    return false;
}

bool CXmlConfig::SaveTool(CString szFileXml, CTool &tool)
{
    XmlDocumnent doc;
    XmlTools xml(doc);
    xml.Create();
    xml.SetTool(tool);
    return xml.Save(szFileXml);
}

bool CXmlConfig::LoadTools(XmlDocumnent &doc, CToolsList &tools)
{
    XmlTools xml(doc);
    xml.GetTools(tools);
    return true;
}

bool CXmlConfig::LoadTools(CString szFileXml, CToolsList &tools)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadTools(doc, tools);
    }
    return false;
}

bool CXmlConfig::SaveTools(CString szFileXml, CToolsList &tools)
{
    XmlDocumnent doc;
    XmlTools xml(doc);
    xml.Create();
    xml.SetTools(tools);
    return xml.Save(szFileXml);
}
