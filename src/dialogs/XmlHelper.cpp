// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "XmlHelper.h"

bool CXmlHelper::LoadOptions(XmlDocumnent &doc, COptions &options)
{
    XmlOptions xmlOptions(doc);
    xmlOptions.GetOptions(options);
    return true;
}

bool CXmlHelper::LoadOptions(CString szFileXml, COptions &options)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadOptions(doc, options);
    }
    return false;
}

bool CXmlHelper::SaveOptions(CString szFileXml, COptions &options)
{
    XmlDocumnent doc;
    XmlOptions xmlOptions(doc);
    xmlOptions.Create();
    xmlOptions.SetOptions(options);
    return xmlOptions.Save(szFileXml);
}

bool CXmlHelper::LoadItems(XmlDocumnent &doc, CItemsList &items)
{
    XmlItems xmlItems(doc);
    xmlItems.GetItems(items);
    return true;
}

bool CXmlHelper::LoadItems(CString szFileXml, CItemsList &items)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadItems(doc, items);
    }
    return false;
}

bool CXmlHelper::SaveItems(CString szFileXml, CItemsList &items)
{
    XmlDocumnent doc;
    XmlItems xmlItems(doc);
    xmlItems.Create();
    xmlItems.SetItems(items);
    return xmlItems.Save(szFileXml);
}

bool CXmlHelper::LoadLanguage(XmlDocumnent &doc, CLanguage &language)
{
    XmlLanguages xmlLanguages(doc);
    xmlLanguages.GetLanguage(language);
    return true;
}

bool CXmlHelper::LoadLanguage(CString szFileXml, CLanguage &language)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadLanguage(doc, language);
    }
    return false;
}

bool CXmlHelper::LoadPresets(XmlDocumnent &doc, CPresetsList &presets)
{
    XmlPresets xmlPresets(doc);
    xmlPresets.GetPresets(presets);
    return true;
}

bool CXmlHelper::LoadPresets(CString szFileXml, CPresetsList &presets)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadPresets(doc, presets);
    }
    return false;
}

bool CXmlHelper::SavePresets(CString szFileXml, CPresetsList &presets)
{
    XmlDocumnent doc;
    XmlPresets xmlPresets(doc);
    xmlPresets.Create();
    xmlPresets.SetPresets(presets;
    return xmlPresets.Save(szFileXml);
}

bool CXmlHelper::LoadFormat(XmlDocumnent &doc, CFormat &format)
{
    XmlFormats xmlFormats(doc);
    xmlFormats.GetFormat(format);
    return true;
}

bool CXmlHelper::LoadFormat(CString szFileXml, CFormat &format)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadFormat(doc, format);
    }
    return false;
}

bool CXmlHelper::SaveFormat(CString szFileXml, CFormat &format)
{
    XmlDocumnent doc;
    XmlFormats xmlFormats(doc);
    xmlFormats.Create();
    xmlFormats.SetFormat(format);
    return xmlFormats.Save(szFileXml);
}

bool CXmlHelper::LoadFormats(XmlDocumnent &doc, CFormatsList &formats)
{
    XmlFormats xmlFormats(doc);
    xmlFormats.GetFormats(formats);
    return true;
}

bool CXmlHelper::LoadFormats(CString szFileXml, CFormatsList &formats)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadFormats(doc, formats);
    }
    return false;
}

bool CXmlHelper::SaveFormats(CString szFileXml, CFormatsList &formats)
{
    XmlDocumnent doc;
    XmlFormats xmlFormats(doc);
    xmlFormats.Create();
    xmlFormats.SetFormats(formats);
    return xmlFormats.Save(szFileXml);
}

bool CXmlHelper::LoadTool(XmlDocumnent &doc, CTool &tool)
{
    XmlTools xmlTools(doc);
    xmlTools.GetTool(tool);
    return true;
}

bool CXmlHelper::LoadTool(CString szFileXml, CTool &tool)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadTool(doc, tool);
    }
    return false;
}

bool CXmlHelper::SaveTool(CString szFileXml, CTool &tool)
{
    XmlDocumnent doc;
    XmlTools xmlTools(doc);
    xmlTools.Create();
    xmlTools.SetTool(tool);
    return xmlTools.Save(szFileXml);
}

bool CXmlHelper::LoadTools(XmlDocumnent &doc, CToolsList &tools)
{
    XmlTools xmlTools(doc);
    xmlTools.GetTools(tools);
    return true;
}

bool CXmlHelper::LoadTools(CString szFileXml, CToolsList &tools)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadTools(doc, tools);
    }
    return false;
}

bool CXmlHelper::SaveTools(CString szFileXml, CToolsList &tools)
{
    XmlDocumnent doc;
    XmlTools xmlTools(doc);
    xmlTools.Create();
    xmlTools.SetTools(tools);
    return xmlTools.Save(szFileXml);
}
