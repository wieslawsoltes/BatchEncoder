// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "XmlHelper.h"

bool CXmlHelper::LoadOptions(XmlDocumnent &doc, COptions &options)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadOptions(doc);
    }
    return false;
}

bool CXmlHelper::LoadOptions(CString szFileXml, COptions &options)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadOptions(doc);
    }
    return false;
}

bool CXmlHelper::SaveOptions(CString szFileXml, COptions &options)
{
    this->GetOptions();

    XmlDocumnent doc;
    XmlOptions xmlOptions(doc);
    xmlOptions.Create();
    xmlOptions.SetOptions(this->m_Config.m_Options);
    return xmlOptions.Save(szFileXml);
}

bool CXmlHelper::LoadItems(XmlDocumnent &doc, CItemsList &items)
{
    XmlItems xmlItems(doc);

    this->m_LstInputItems.DeleteAllItems();
    this->m_Config.m_Items.RemoveAll();

    xmlItems.GetItems(this->m_Config.m_Items);

    this->SetItems();
    this->UpdateStatusBar();

    return true;
}

bool CXmlHelper::LoadItems(CString szFileXml, CItemsList &items)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadItems(doc);
    }
    return false;
}

bool CXmlHelper::SaveItems(CString szFileXml, CItemsList &items)
{
    this->GetItems();

    XmlDocumnent doc;
    XmlItems xmlItems(doc);
    xmlItems.Create();
    xmlItems.SetItems(this->m_Config.m_Items);
    return xmlItems.Save(szFileXml);
}

bool CXmlHelper::LoadLanguage(XmlDocumnent &doc, CLanguage &language)
{
    XmlLanguages xmlLanguages(doc);

    // insert to languages list
    CLanguage language;
    xmlLanguages.GetLanguage(language);

    this->m_Config.m_Language.m_Languages.Insert(language);

    // insert to languages menu
    CMenu *m_hMenu = this->GetMenu();
    CMenu *m_hLangMenu = m_hMenu->GetSubMenu(4);
    int nLanguages = m_Config.m_Language.m_Languages.Count();

    CString szBuff;
    szBuff.Format(_T("%s (%s)"), language.szOriginalName, language.szTranslatedName);

    UINT nLangID = ID_LANGUAGE_MIN + nLanguages - 1;
    m_hLangMenu->AppendMenu(MF_STRING, nLangID, szBuff);
    m_hLangMenu->CheckMenuItem(nLangID, MF_UNCHECKED);

    return true;
}

bool CXmlHelper::LoadLanguage(CString szFileXml, CLanguage &language)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadLanguage(doc);
    }
    return false;
}

bool CXmlHelper::LoadPresets(XmlDocumnent &doc, CPresetsList &presets)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);

        XmlPresets xmlPresets(doc);

        format.m_Presets.RemoveAll();
        xmlPresets.GetPresets(format.m_Presets);

        this->UpdatePresetComboBox();

        return true;
    }
    return false;
}

bool CXmlHelper::LoadPresets(CString szFileXml, CPresetsList &presets)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadPresets(doc);
    }
    return false;
}

bool CXmlHelper::SavePresets(CString szFileXml, CPresetsList &presets)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);

        XmlDocumnent doc;
        XmlPresets xmlPresets(doc);
        xmlPresets.Create();
        xmlPresets.SetPresets(format.m_Presets);
        return xmlPresets.Save(szFileXml);
    }
    return false;
}

bool CXmlHelper::LoadFormat(XmlDocumnent &doc, CFormat &format)
{
    XmlFormats xmlFormats(doc);

    CFormat format;
    xmlFormats.GetFormat(format);
    m_Config.m_Formats.Insert(format);

    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();
    return true;
}

bool CXmlHelper::LoadFormat(CString szFileXml, CFormat &format)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadFormat(doc);
    }
    return false;
}

bool CXmlHelper::SaveFormat(CString szFileXml, CFormat &format)
{
    int nFormat = this->m_CmbFormat.GetCurSel();
    if (nFormat != -1)
    {
        CFormat& format = m_Config.m_Formats.Get(nFormat);

        XmlDocumnent doc;
        XmlFormats xmlFormats(doc);
        xmlFormats.Create();
        xmlFormats.SetFormat(format);
        return xmlFormats.Save(szFileXml);
    }
    return false;
}

bool CXmlHelper::LoadFormats(XmlDocumnent &doc, CFormatsList &formats)
{
    XmlFormats xmlFormats(doc);

    this->m_Config.m_Formats.RemoveAll();

    xmlFormats.GetFormats(this->m_Config.m_Formats);

    this->UpdateFormatComboBox();
    this->UpdatePresetComboBox();

    return true;
}

bool CXmlHelper::LoadFormats(CString szFileXml, CFormatsList &formats)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadFormats(doc);
    }
    return false;
}

bool CXmlHelper::SaveFormats(CString szFileXml, CFormatsList &formats)
{
    XmlDocumnent doc;
    XmlFormats xmlFormats(doc);
    xmlFormats.Create();
    xmlFormats.SetFormats(this->m_Config.m_Formats);
    return xmlFormats.Save(szFileXml);
}

bool CXmlHelper::LoadTool(XmlDocumnent &doc, CTool &tool)
{
    XmlTools xmlTools(doc);

    CTool tool;
    xmlTools.GetTool(tool);
    m_Config.m_Tools.Insert(tool);

    return true;
}

bool CXmlHelper::LoadTool(CString szFileXml, CTool &tool)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadTool(doc);
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

    this->m_Config.m_Tools.RemoveAll();

    xmlTools.GetTools(this->m_Config.m_Tools);

    return true;
}

bool CXmlHelper::LoadTools(CString szFileXml, CToolsList &tools)
{
    XmlDocumnent doc;
    if (XmlDoc::Open(szFileXml, doc) == true)
    {
        return LoadTools(doc);
    }
    return false;
}

bool CXmlHelper::SaveTools(CString szFileXml, CToolsList &tools)
{
    XmlDocumnent doc;
    XmlTools xmlTools(doc);
    xmlTools.Create();
    xmlTools.SetTools(this->m_Config.m_Tools);
    return xmlTools.Save(szFileXml);
}
