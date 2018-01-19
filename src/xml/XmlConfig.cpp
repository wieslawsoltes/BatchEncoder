// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "XmlConfig.h"

bool CXmlConfig::LoadOptions(XmlDocumnent &doc, COptions &options)
{
    XmlOptions xmlOptions(doc);
    xmlOptions.GetOptions(options);
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
    XmlOptions xmlOptions(doc);
    xmlOptions.Create();
    xmlOptions.SetOptions(options);
    return xmlOptions.Save(szFileXml);
}

bool CXmlConfig::LoadItems(XmlDocumnent &doc, CItemsList &items)
{
    XmlItems xmlItems(doc);
    xmlItems.GetItems(items);
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
    XmlItems xmlItems(doc);
    xmlItems.Create();
    xmlItems.SetItems(items);
    return xmlItems.Save(szFileXml);
}

bool CXmlConfig::LoadLanguage(XmlDocumnent &doc, CLanguage &language)
{
    XmlLanguages xmlLanguages(doc);
    xmlLanguages.GetLanguage(language);
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

bool CXmlConfig::LoadPresets(XmlDocumnent &doc, CPresetsList &presets)
{
    XmlPresets xmlPresets(doc);
    xmlPresets.GetPresets(presets);
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
    XmlPresets xmlPresets(doc);
    xmlPresets.Create();
    xmlPresets.SetPresets(presets;
    return xmlPresets.Save(szFileXml);
}

bool CXmlConfig::LoadFormat(XmlDocumnent &doc, CFormat &format)
{
    XmlFormats xmlFormats(doc);
    xmlFormats.GetFormat(format);
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
    XmlFormats xmlFormats(doc);
    xmlFormats.Create();
    xmlFormats.SetFormat(format);
    return xmlFormats.Save(szFileXml);
}

bool CXmlConfig::LoadFormats(XmlDocumnent &doc, CFormatsList &formats)
{
    XmlFormats xmlFormats(doc);
    xmlFormats.GetFormats(formats);
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
    XmlFormats xmlFormats(doc);
    xmlFormats.Create();
    xmlFormats.SetFormats(formats);
    return xmlFormats.Save(szFileXml);
}

bool CXmlConfig::LoadTool(XmlDocumnent &doc, CTool &tool)
{
    XmlTools xmlTools(doc);
    xmlTools.GetTool(tool);
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
    XmlTools xmlTools(doc);
    xmlTools.Create();
    xmlTools.SetTool(tool);
    return xmlTools.Save(szFileXml);
}

bool CXmlConfig::LoadTools(XmlDocumnent &doc, CToolsList &tools)
{
    XmlTools xmlTools(doc);
    xmlTools.GetTools(tools);
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
    XmlTools xmlTools(doc);
    xmlTools.Create();
    xmlTools.SetTools(tools);
    return xmlTools.Save(szFileXml);
}
