// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "utilities\UnicodeUtf8.h"
#include "utilities\Utf8String.h"
#include "XmlConfiguration.h"

XmlConfiguration::XmlConfiguration()
{

}

XmlConfiguration::~XmlConfiguration()
{

}

CString XmlConfiguration::ToCString(const char *pszUtf8)
{
    if (pszUtf8 == NULL)
        return _T("");

    if (strlen(pszUtf8) == 0)
        return _T("");

    CString szBuff;
#ifdef _UNICODE
    // UTF-8 to UNICODE
    wchar_t *pszUnicode;
    pszUnicode = MakeUnicodeString((unsigned char *)pszUtf8);
    szBuff = pszUnicode;
    free(pszUnicode);
#else
    // UTF-8 to ANSI
    char *pszAnsi;
    Utf8Decode(pszUtf8, &pszAnsi);
    szBuff = pszAnsi;
    free(pszAnsi);
#endif
    return szBuff;
}

void XmlConfiguration::GetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionElem;

    // option: SelectedFormat
    pOptionElem = pOptionsElem->FirstChildElement("SelectedFormat");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.nSelectedFormat = _tstoi(ToCString(tmpBuff));
    }
    else
    {
        m_Options.nSelectedFormat = 0;
    }

    // option: OutputPath
    pOptionElem = pOptionsElem->FirstChildElement("OutputPath");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szOutputPath = ToCString(tmpBuff);
    }
    else
    {
        m_Options.szOutputPath = _T("");
    }

    // option: OutputPathChecked
    pOptionElem = pOptionsElem->FirstChildElement("OutputPathChecked");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bOutputPathChecked = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bOutputPathChecked = false;
    }

    // option: DeleteSourceFiles
    pOptionElem = pOptionsElem->FirstChildElement("DeleteSourceFiles");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bDeleteSourceFiles = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bDeleteSourceFiles = false;
    }

    // option: RecurseChecked
    pOptionElem = pOptionsElem->FirstChildElement("RecurseChecked");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bRecurseChecked = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bRecurseChecked = false;
    }

    // option: ShutdownWhenFinished
    pOptionElem = pOptionsElem->FirstChildElement("ShutdownWhenFinished");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bShutdownWhenFinished = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bShutdownWhenFinished = false;
    }

    // option: DoNotSaveConfiguration
    pOptionElem = pOptionsElem->FirstChildElement("DoNotSaveConfiguration");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bDoNotSaveConfiguration = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bDoNotSaveConfiguration = false;
    }

    // option: MainWindowResize
    pOptionElem = pOptionsElem->FirstChildElement("MainWindowResize");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szMainWindowResize = ToCString(tmpBuff);
    }

    // option: FileListColumns
    pOptionElem = pOptionsElem->FirstChildElement("FileListColumns");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szFileListColumns = ToCString(tmpBuff);
    }

    // option: PresetsDialogResize
    pOptionElem = pOptionsElem->FirstChildElement("PresetsDialogResize");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szPresetsDialogResize = ToCString(tmpBuff);
    }

    // option: PresetsListColumns
    pOptionElem = pOptionsElem->FirstChildElement("PresetsListColumns");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szPresetsListColumns = ToCString(tmpBuff);
    }

    // option: FormatsDialogResize
    pOptionElem = pOptionsElem->FirstChildElement("FormatsDialogResize");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szFormatsDialogResize = ToCString(tmpBuff);
    }

    // option: FormatsListColumns
    pOptionElem = pOptionsElem->FirstChildElement("FormatsListColumns");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szFormatsListColumns = ToCString(tmpBuff);
    }

    // option: DeleteOnErrors
    pOptionElem = pOptionsElem->FirstChildElement("DeleteOnErrors");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bDeleteOnErrors = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bDeleteOnErrors = false;
    }

    // option: StopOnErrors
    pOptionElem = pOptionsElem->FirstChildElement("StopOnErrors");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bStopOnErrors = ToCString(tmpBuff).CompareNoCase(_T("true")) == 0;
    }
    else
    {
        m_Options.bStopOnErrors = false;
    }

    // option: ThreadCount
    pOptionElem = pOptionsElem->FirstChildElement("ThreadCount");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.nThreadCount = _tstoi(ToCString(tmpBuff));
    }
    else
    {
        m_Options.nThreadCount = 1;
    }
}

void XmlConfiguration::SetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionElem;
    CUtf8String szBuffUtf8;

    // option: SelectedFormat
    CString szSelectedFormat;
    szSelectedFormat.Format(_T("%d\0"), m_Options.nSelectedFormat);

    pOptionElem = this->NewElement("SelectedFormat");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(szSelectedFormat)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: OutputPath
    pOptionElem = this->NewElement("OutputPath");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szOutputPath)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: OutputPathChecked
    pOptionElem = this->NewElement("OutputPathChecked");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bOutputPathChecked ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: DeleteSourceFiles
    pOptionElem = this->NewElement("DeleteSourceFiles");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bDeleteSourceFiles ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: RecurseChecked
    pOptionElem = this->NewElement("RecurseChecked");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bRecurseChecked ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: ShutdownWhenFinished
    pOptionElem = this->NewElement("ShutdownWhenFinished");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bShutdownWhenFinished ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: DoNotSaveConfiguration
    pOptionElem = this->NewElement("DoNotSaveConfiguration");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bDoNotSaveConfiguration ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: MainWindowResize
    pOptionElem = this->NewElement("MainWindowResize");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szMainWindowResize)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: FileListColumns
    pOptionElem = this->NewElement("FileListColumns");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szFileListColumns)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: PresetsDialogResize
    pOptionElem = this->NewElement("PresetsDialogResize");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szPresetsDialogResize)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: PresetsListColumns
    pOptionElem = this->NewElement("PresetsListColumns");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szPresetsListColumns)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: FormatsDialogResize
    pOptionElem = this->NewElement("FormatsDialogResize");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szFormatsDialogResize)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: FormatsListColumns
    pOptionElem = this->NewElement("FormatsListColumns");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szFormatsListColumns)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: DeleteOnErrors
    pOptionElem = this->NewElement("DeleteOnErrors");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bDeleteOnErrors ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: StopOnErrors
    pOptionElem = this->NewElement("StopOnErrors");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bStopOnErrors ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: nThreadCount
    CString szThreadCount;
    szThreadCount.Format(_T("%d\0"), m_Options.nThreadCount);

    pOptionElem = this->NewElement("ThreadCount");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(szThreadCount)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    pOptionsElem->LinkEndChild(pOptionElem);
}

void XmlConfiguration::GetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
{
    tinyxml2::XMLElement *pPresetElem = pPresetsElem->FirstChildElement("Preset");
    for (pPresetElem; pPresetElem; pPresetElem = pPresetElem->NextSiblingElement())
    {
        CPreset preset;

        const char *pszName = pPresetElem->Attribute("name");
        if (pszName != NULL)
        {
            preset.szName = ToCString(pszName);
        }

        const char *pszOptions = pPresetElem->Attribute("options");
        if (pszOptions != NULL)
        {
            preset.szOptions = ToCString(pszOptions);
        }

        m_Presets.InsertNode(preset);
    }
}

void XmlConfiguration::SetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
{
    CUtf8String szBuffUtf8;
    tinyxml2::XMLElement *pPresetElem;

    int nPresets = m_Presets.GetSize();
    for (int i = 0; i < nPresets; i++)
    {
        CPreset& preset = m_Presets.GetData(i);

        pPresetElem = this->NewElement("Preset");

        pPresetElem->SetAttribute("name", szBuffUtf8.Create(preset.szName));
        szBuffUtf8.Clear();

        pPresetElem->SetAttribute("options", szBuffUtf8.Create(preset.szOptions));
        szBuffUtf8.Clear();

        pPresetsElem->LinkEndChild(pPresetElem);
    }
}

void XmlConfiguration::GetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
{
    const char *pszId = pFormatElem->Attribute("id");
    if (pszId != NULL)
    {
        m_Format.szId = ToCString(pszId);
    }

    const char *pszName = pFormatElem->Attribute("name");
    if (pszName != NULL)
    {
        m_Format.szName = ToCString(pszName);
    }

    const char *pszTemplate = pFormatElem->Attribute("template");
    if (pszTemplate != NULL)
    {
        m_Format.szTemplate = ToCString(pszTemplate);
    }

    const char *pszPipesInput = pFormatElem->Attribute("input");
    if (pszPipesInput != NULL)
    {
        m_Format.bPipeInput = ToCString(pszPipesInput).CompareNoCase(_T("true")) == 0;
    }

    const char *pszPipesOutput = pFormatElem->Attribute("output");
    if (pszPipesOutput != NULL)
    {
        m_Format.bPipeOutput = ToCString(pszPipesOutput).CompareNoCase(_T("true")) == 0;
    }

    const char *pszFunction = pFormatElem->Attribute("function");
    if (pszFunction != NULL)
    {
        m_Format.szFunction = ToCString(pszFunction);
    }

    const char *pszPath = pFormatElem->Attribute("path");
    if (pszPath != NULL)
    {
        m_Format.szPath = ToCString(pszPath);
    }

    const char *pszType = pFormatElem->Attribute("type");
    if (pszType != NULL)
    {
        m_Format.nType = _tstoi(ToCString(pszType));
    }

    const char *pszFormats = pFormatElem->Attribute("formats");
    if (pszFormats != NULL)
    {
        m_Format.szInputExtensions = ToCString(pszFormats);
    }

    const char *pszExtension = pFormatElem->Attribute("extension");
    if (pszExtension != NULL)
    {
        m_Format.szOutputExtension = ToCString(pszExtension);
    }

    const char *pszDefaultPreset = pFormatElem->Attribute("default");
    if (pszDefaultPreset != NULL)
    {
        m_Format.nDefaultPreset = _tstoi(ToCString(pszDefaultPreset));
    }

    tinyxml2::XMLElement *pPresetsElem = pFormatElem->FirstChildElement("Presets");
    this->GetPresets(pPresetsElem, m_Format.m_Presets);
}

void XmlConfiguration::SetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
{
    CUtf8String szBuffUtf8;

    pFormatElem->SetAttribute("id", szBuffUtf8.Create(m_Format.szId));
    szBuffUtf8.Clear();

    pFormatElem->SetAttribute("name", szBuffUtf8.Create(m_Format.szName));
    szBuffUtf8.Clear();

    pFormatElem->SetAttribute("template", szBuffUtf8.Create(m_Format.szTemplate));
    szBuffUtf8.Clear();

    pFormatElem->SetAttribute("input", (m_Format.bPipeInput) ? "true" : "false");
    pFormatElem->SetAttribute("output", (m_Format.bPipeOutput) ? "true" : "false");

    pFormatElem->SetAttribute("function", szBuffUtf8.Create(m_Format.szFunction));
    szBuffUtf8.Clear();

    pFormatElem->SetAttribute("path", szBuffUtf8.Create(m_Format.szPath));
    szBuffUtf8.Clear();

    CString szType;
    szType.Format(_T("%d\0"), m_Format.nType);
    pFormatElem->SetAttribute("type", szBuffUtf8.Create(szType));
    szBuffUtf8.Clear();

    pFormatElem->SetAttribute("formats", szBuffUtf8.Create(m_Format.szInputExtensions));
    szBuffUtf8.Clear();

    pFormatElem->SetAttribute("extension", szBuffUtf8.Create(m_Format.szOutputExtension));
    szBuffUtf8.Clear();

    CString szDefaultPreset;
    szDefaultPreset.Format(_T("%d\0"), m_Format.nDefaultPreset);
    pFormatElem->SetAttribute("default", szBuffUtf8.Create(szDefaultPreset));
    szBuffUtf8.Clear();

    tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
    pFormatElem->LinkEndChild(pPresetsElem);
    this->SetPresets(pPresetsElem, m_Format.m_Presets);
}

void XmlConfiguration::GetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
{
    tinyxml2::XMLElement *pFormatElem = pFormatsElem->FirstChildElement("Format");
    for (pFormatElem; pFormatElem; pFormatElem = pFormatElem->NextSiblingElement())
    {
        CFormat format;
        this->GetFormat(pFormatElem, format);
        m_Formats.InsertNode(format);
    }
}

void XmlConfiguration::SetFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
{
    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);
        tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
        this->SetFormat(pFormatElem, format);
        pFormatsElem->LinkEndChild(pFormatElem);
    }
}

void XmlConfiguration::GetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
{
    tinyxml2::XMLElement *pItemElem = pItemsElem->FirstChildElement("Item");
    for (pItemElem; pItemElem; pItemElem = pItemElem->NextSiblingElement())
    {
        CItem item;

        const char *pszId = pItemElem->Attribute("id");
        if (pszId != NULL)
        {
            item.nId = _tstoi(ToCString(pszId));
        }

        const char *pszPath = pItemElem->Attribute("path");
        if (pszPath != NULL)
        {
            item.szPath = ToCString(pszPath);
        }

        const char *pszSize = pItemElem->Attribute("size");
        if (pszSize != NULL)
        {
            item.szSize = ToCString(pszSize);
        }

        const char *pszName = pItemElem->Attribute("name");
        if (pszName != NULL)
        {
            item.szName = ToCString(pszName);
        }

        const char *pszExtension = pItemElem->Attribute("extension");
        if (pszExtension != NULL)
        {
            item.szExtension = ToCString(pszExtension);
        }

        const char *pszFormatId = pItemElem->Attribute("format");
        if (pszFormatId != NULL)
        {
            item.szFormatId = ToCString(pszFormatId);
        }

        const char *pszPreset = pItemElem->Attribute("preset");
        if (pszPreset != NULL)
        {
            item.nPreset = _tstoi(ToCString(pszPreset));
        }

        const char *pszChecked = pItemElem->Attribute("checked");
        if (pszChecked != NULL)
        {
            item.bChecked = ToCString(pszChecked).CompareNoCase(_T("true")) == 0;
        }

        const char *pszTime = pItemElem->Attribute("time");
        if (pszTime != NULL)
        {
            item.szTime = ToCString(pszTime);
        }

        const char *pszStatus = pItemElem->Attribute("status");
        if (pszStatus != NULL)
        {
            item.szStatus = ToCString(pszStatus);
        }

        m_Items.InsertNode(item);
    }
}

void XmlConfiguration::SetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
{
    CUtf8String szBuffUtf8;
    tinyxml2::XMLElement *pItemElem;

    int nItems = m_Items.GetSize();
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = m_Items.GetData(i);

        pItemElem = this->NewElement("Item");

        CString szId;
        szId.Format(_T("%d\0"), i);
        pItemElem->SetAttribute("id", szBuffUtf8.Create(szId));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("path", szBuffUtf8.Create(item.szPath));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("size", szBuffUtf8.Create(item.szSize));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("name", szBuffUtf8.Create(item.szName));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("extension", szBuffUtf8.Create(item.szExtension));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("format", szBuffUtf8.Create(item.szFormatId));
        szBuffUtf8.Clear();

        CString szPreset;
        szPreset.Format(_T("%d\0"), item.nPreset);
        pItemElem->SetAttribute("preset", szBuffUtf8.Create(szPreset));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("checked", (item.bChecked) ? "true" : "false");

        pItemElem->SetAttribute("time", szBuffUtf8.Create(item.szTime));
        szBuffUtf8.Clear();

        pItemElem->SetAttribute("status", szBuffUtf8.Create(item.szStatus));
        szBuffUtf8.Clear();

        pItemsElem->LinkEndChild(pItemElem);
    }
}

void XmlConfiguration::GetOptions(COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionsElem = this->FirstChildElement("Options");
    if (pOptionsElem != NULL)
    {
        this->GetOptions(pOptionsElem, m_Options);
    }
}

void XmlConfiguration::SetOptions(COptions &m_Options)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pOptionsElem = this->NewElement("Options");
    this->LinkEndChild(pOptionsElem);

    this->SetOptions(pOptionsElem, m_Options);
}

void XmlConfiguration::GetPresets(CPresetsList &m_Presets)
{
    tinyxml2::XMLElement *pPresetsElem = this->FirstChildElement("Presets");
    if (pPresetsElem != NULL)
    {
        this->GetPresets(pPresetsElem, m_Presets);
    }
}

void XmlConfiguration::SetPresets(CPresetsList &m_Presets)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
    this->LinkEndChild(pPresetsElem);

    this->SetPresets(pPresetsElem, m_Presets);
}

void XmlConfiguration::GetFormat(CFormat &m_Format)
{
    tinyxml2::XMLElement *pFormatElem = this->FirstChildElement("Format");
    if (pFormatElem != NULL)
    {
        this->GetFormat(pFormatElem, m_Format);
    }
}

void XmlConfiguration::SetFormat(CFormat &m_Format)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
    this->LinkEndChild(pFormatElem);

    this->SetFormat(pFormatElem, m_Format);
}

void XmlConfiguration::GetFormats(CFormatsList &m_Formats)
{
    tinyxml2::XMLElement *pFormatsElem = this->FirstChildElement("Formats");
    if (pFormatsElem != NULL)
    {
        this->GetFormats(pFormatsElem, m_Formats);
    }
}

void XmlConfiguration::SetFormats(CFormatsList &m_Formats)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pFormatsElem = this->NewElement("Formats");
    this->LinkEndChild(pFormatsElem);

    this->SetFormats(pFormatsElem, m_Formats);
}

void XmlConfiguration::GetItems(CItemsList &m_Items)
{
    tinyxml2::XMLElement *pItemsElem = this->FirstChildElement("Items");
    if (pItemsElem != NULL)
    {
        this->GetItems(pItemsElem, m_Items);
    }
}

void XmlConfiguration::SetItems(CItemsList &m_Items)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(UTF8_DOCUMENT_DECLARATION);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pItemsElem = this->NewElement("Items");
    this->LinkEndChild(pItemsElem);

    this->SetItems(pItemsElem, m_Items);
}

bool XmlConfiguration::Open(CString szFileName)
{
    CStdioFile fp;
    if (fp.Open(szFileName, CFile::modeRead | CFile::typeBinary) == TRUE)
    {
        tinyxml2::XMLError result = this->LoadFile(fp.m_pStream);
        fp.Close();
        return result == tinyxml2::XMLError::XML_SUCCESS;
    }
    return false;
}

bool XmlConfiguration::Save(CString szFileName)
{
    CStdioFile fp;
    if (fp.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText) == TRUE)
    {
        const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
        const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
        const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

        fputc(TIXML_UTF_LEAD_0, fp.m_pStream);
        fputc(TIXML_UTF_LEAD_1, fp.m_pStream);
        fputc(TIXML_UTF_LEAD_2, fp.m_pStream);

        tinyxml2::XMLPrinter printer(fp.m_pStream);
        this->Print(&printer);

        fp.Close();
        return true;
    }
    return false;
}
