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

#ifdef _UNICODE
CString XmlConfiguration::ToCString(const char *pszUtf8)
{
    if (pszUtf8 == NULL)
        return _T("");
    if (strlen(pszUtf8) == 0)
        return _T("");
    wchar_t *pszUnicode = MakeUnicodeString((unsigned char *)pszUtf8);
    CString szBuff = pszUnicode;
    free(pszUnicode);
    return szBuff;
}
#else
CString XmlConfiguration::ToCString(const char *pszUtf8)
{
    if (pszUtf8 == NULL)
        return _T("");
    if (strlen(pszUtf8) == 0)
        return _T("");
    char *pszAnsi;
    Utf8Decode(pszUtf8, &pszAnsi);
    CString szBuff = pszAnsi;
    free(pszAnsi);
    return szBuff;
}
#endif

bool XmlConfiguration::ToBool(const char *pszUtf8)
{
    return ToCString(pszUtf8).CompareNoCase(m_True) == 0;
}

int XmlConfiguration::ToInt(const char *pszUtf8)
{
    return _tstoi(ToCString(pszUtf8));
}

CString XmlConfiguration::ToCString(const int nValue)
{
    CString rValue;
    rValue.Format(_T("%d\0"), nValue);
    return rValue;
}

CString XmlConfiguration::ToCString(const bool bValue)
{
    CString rValue;
    rValue.Format(_T("%s\0"), bValue ? m_True : m_False);
    return rValue;
}

void XmlConfiguration::GetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionElem;

    pOptionElem = pOptionsElem->FirstChildElement("SelectedLanguage");
    if (pOptionElem)
        m_Options.szSelectedLanguage = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("SelectedFormat");
    if (pOptionElem)
        m_Options.nSelectedFormat = ToInt(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("OutputPath");
    if (pOptionElem)
        m_Options.szOutputPath = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("OutputPathChecked");
    if (pOptionElem)
        m_Options.bOutputPathChecked = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("DeleteSourceFiles");
    if (pOptionElem)
        m_Options.bDeleteSourceFiles = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("RecurseChecked");
    if (pOptionElem)
        m_Options.bRecurseChecked = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("ShutdownWhenFinished");
    if (pOptionElem)
        m_Options.bShutdownWhenFinished = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("DoNotSaveConfiguration");
    if (pOptionElem)
        m_Options.bDoNotSaveConfiguration = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("DeleteOnErrors");
    if (pOptionElem)
        m_Options.bDeleteOnErrors = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("StopOnErrors");
    if (pOptionElem)
        m_Options.bStopOnErrors = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("HideConsoleWindow");
    if (pOptionElem)
        m_Options.bHideConsoleWindow = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("TryToFindDecoder");
    if (pOptionElem)
        m_Options.bTryToFindDecoder = ToBool(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("ThreadCount");
    if (pOptionElem)
        m_Options.nThreadCount = ToInt(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("MainWindowResize");
    if (pOptionElem)
        m_Options.szMainWindowResize = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("FileListColumns");
    if (pOptionElem)
        m_Options.szFileListColumns = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("PresetsDialogResize");
    if (pOptionElem)
        m_Options.szPresetsDialogResize = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("PresetsListColumns");
    if (pOptionElem)
        m_Options.szPresetsListColumns = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("FormatsDialogResize");
    if (pOptionElem)
        m_Options.szFormatsDialogResize = ToCString(pOptionElem->GetText());

    pOptionElem = pOptionsElem->FirstChildElement("FormatsListColumns");
    if (pOptionElem)
        m_Options.szFormatsListColumns = ToCString(pOptionElem->GetText());
}

void XmlConfiguration::SetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionElem;

    pOptionElem = this->NewElement("SelectedLanguage");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szSelectedLanguage).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("SelectedFormat");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.nSelectedFormat)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("OutputPath");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szOutputPath).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("OutputPathChecked");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bOutputPathChecked)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("DeleteSourceFiles");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bDeleteSourceFiles)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("RecurseChecked");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bRecurseChecked)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("ShutdownWhenFinished");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bShutdownWhenFinished)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("DoNotSaveConfiguration");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bDoNotSaveConfiguration)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("DeleteOnErrors");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bDeleteOnErrors)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("StopOnErrors");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bStopOnErrors)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("HideConsoleWindow");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bHideConsoleWindow)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("TryToFindDecoder");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bTryToFindDecoder)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("ThreadCount");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.nThreadCount)).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("MainWindowResize");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szMainWindowResize).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("FileListColumns");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szFileListColumns).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("PresetsDialogResize");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szPresetsDialogResize).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("PresetsListColumns");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szPresetsListColumns).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("FormatsDialogResize");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szFormatsDialogResize).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);

    pOptionElem = this->NewElement("FormatsListColumns");
    pOptionElem->LinkEndChild(this->NewText(CUtf8String(m_Options.szFormatsListColumns).m_Result));
    pOptionsElem->LinkEndChild(pOptionElem);
}

void XmlConfiguration::GetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
{
    tinyxml2::XMLElement *pPresetElem = pPresetsElem->FirstChildElement("Preset");
    for (pPresetElem; pPresetElem; pPresetElem = pPresetElem->NextSiblingElement())
    {
        const char *pszName = pPresetElem->Attribute("name");
        const char *pszOptions = pPresetElem->Attribute("options");
        if (pszName != NULL && pszOptions != NULL)
        {
            CPreset preset;
            preset.szName = ToCString(pszName);
            preset.szOptions = ToCString(pszOptions);
            m_Presets.InsertNode(preset);
        }
    }
}

void XmlConfiguration::SetPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList &m_Presets)
{
    tinyxml2::XMLElement *pPresetElem;

    int nPresets = m_Presets.GetSize();
    for (int i = 0; i < nPresets; i++)
    {
        CPreset& preset = m_Presets.GetData(i);
        pPresetElem = this->NewElement("Preset");
        pPresetElem->SetAttribute("name", CUtf8String(preset.szName).m_Result);
        pPresetElem->SetAttribute("options", CUtf8String(preset.szOptions).m_Result);
        pPresetsElem->LinkEndChild(pPresetElem);
    }
}

void XmlConfiguration::GetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
{
    const char *pszId = pFormatElem->Attribute("id");
    if (pszId != NULL)
        m_Format.szId = ToCString(pszId);

    const char *pszName = pFormatElem->Attribute("name");
    if (pszName != NULL)
        m_Format.szName = ToCString(pszName);

    const char *pszTemplate = pFormatElem->Attribute("template");
    if (pszTemplate != NULL)
        m_Format.szTemplate = ToCString(pszTemplate);

    const char *pszPipesInput = pFormatElem->Attribute("input");
    if (pszPipesInput != NULL)
        m_Format.bPipeInput = ToBool(pszPipesInput);

    const char *pszPipesOutput = pFormatElem->Attribute("output");
    if (pszPipesOutput != NULL)
        m_Format.bPipeOutput = ToBool(pszPipesOutput);

    const char *pszFunction = pFormatElem->Attribute("function");
    if (pszFunction != NULL)
        m_Format.szFunction = ToCString(pszFunction);

    const char *pszPath = pFormatElem->Attribute("path");
    if (pszPath != NULL)
        m_Format.szPath = ToCString(pszPath);

    const char *pszType = pFormatElem->Attribute("type");
    if (pszType != NULL)
        m_Format.nType = ToInt(pszType);

    const char *pszFormats = pFormatElem->Attribute("formats");
    if (pszFormats != NULL)
        m_Format.szInputExtensions = ToCString(pszFormats);

    const char *pszExtension = pFormatElem->Attribute("extension");
    if (pszExtension != NULL)
        m_Format.szOutputExtension = ToCString(pszExtension);

    const char *pszDefaultPreset = pFormatElem->Attribute("default");
    if (pszDefaultPreset != NULL)
        m_Format.nDefaultPreset = ToInt(pszDefaultPreset);

    tinyxml2::XMLElement *pPresetsElem = pFormatElem->FirstChildElement("Presets");
    this->GetPresets(pPresetsElem, m_Format.m_Presets);
}

void XmlConfiguration::SetFormat(tinyxml2::XMLElement *pFormatElem, CFormat &m_Format)
{
    pFormatElem->SetAttribute("id", CUtf8String(m_Format.szId).m_Result);
    pFormatElem->SetAttribute("name", CUtf8String(m_Format.szName).m_Result);
    pFormatElem->SetAttribute("template", CUtf8String(m_Format.szTemplate).m_Result);
    pFormatElem->SetAttribute("input", CUtf8String(ToCString(m_Format.bPipeInput)).m_Result);
    pFormatElem->SetAttribute("output", CUtf8String(ToCString(m_Format.bPipeOutput)).m_Result);
    pFormatElem->SetAttribute("function", CUtf8String(m_Format.szFunction).m_Result);
    pFormatElem->SetAttribute("path", CUtf8String(m_Format.szPath).m_Result);
    pFormatElem->SetAttribute("type", CUtf8String(ToCString(m_Format.nType)).m_Result);
    pFormatElem->SetAttribute("formats", CUtf8String(m_Format.szInputExtensions).m_Result);
    pFormatElem->SetAttribute("extension", CUtf8String(m_Format.szOutputExtension).m_Result);
    pFormatElem->SetAttribute("default", CUtf8String(ToCString(m_Format.nDefaultPreset)).m_Result);

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
            item.nId = _tstoi(ToCString(pszId));

        const char *pszPath = pItemElem->Attribute("path");
        if (pszPath != NULL)
            item.szPath = ToCString(pszPath);

        const char *pszSize = pItemElem->Attribute("size");
        if (pszSize != NULL)
            item.szSize = ToCString(pszSize);

        const char *pszName = pItemElem->Attribute("name");
        if (pszName != NULL)
            item.szName = ToCString(pszName);

        const char *pszExtension = pItemElem->Attribute("extension");
        if (pszExtension != NULL)
            item.szExtension = ToCString(pszExtension);

        const char *pszFormatId = pItemElem->Attribute("format");
        if (pszFormatId != NULL)
            item.szFormatId = ToCString(pszFormatId);

        const char *pszPreset = pItemElem->Attribute("preset");
        if (pszPreset != NULL)
            item.nPreset = ToInt(pszPreset);

        const char *pszChecked = pItemElem->Attribute("checked");
        if (pszChecked != NULL)
            item.bChecked = ToBool(pszChecked);

        const char *pszTime = pItemElem->Attribute("time");
        if (pszTime != NULL)
            item.szTime = ToCString(pszTime);

        const char *pszStatus = pItemElem->Attribute("status");
        if (pszStatus != NULL)
            item.szStatus = ToCString(pszStatus);

        m_Items.InsertNode(item);
    }
}

void XmlConfiguration::SetItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
{
    tinyxml2::XMLElement *pItemElem;
    int nItems = m_Items.GetSize();
    for (int i = 0; i < nItems; i++)
    {
        CItem& item = m_Items.GetData(i);
        pItemElem = this->NewElement("Item");
        pItemElem->SetAttribute("id", CUtf8String(ToCString(i)).m_Result);
        pItemElem->SetAttribute("path", CUtf8String(item.szPath).m_Result);
        pItemElem->SetAttribute("size", CUtf8String(item.szSize).m_Result);
        pItemElem->SetAttribute("name", CUtf8String(item.szName).m_Result);
        pItemElem->SetAttribute("extension", CUtf8String(item.szExtension).m_Result);
        pItemElem->SetAttribute("format", CUtf8String(item.szFormatId).m_Result);
        pItemElem->SetAttribute("preset", CUtf8String(ToCString(item.nPreset)).m_Result);
        pItemElem->SetAttribute("checked", CUtf8String(ToCString(item.bChecked)).m_Result);
        pItemElem->SetAttribute("time", CUtf8String(item.szTime).m_Result);
        pItemElem->SetAttribute("status", CUtf8String(item.szStatus).m_Result);
        pItemsElem->LinkEndChild(pItemElem);
    }
}

void XmlConfiguration::GetLanguage(tinyxml2::XMLElement *pLanguageElem, CLanguage &m_Language)
{
    const char *pszId = pLanguageElem->Attribute("id");
    if (pszId != NULL)
        m_Language.szId = ToCString(pszId);

    const char *pszOriginal = pLanguageElem->Attribute("original");
    if (pszOriginal != NULL)
        m_Language.szOriginalName = ToCString(pszOriginal);

    const char *pszTranslated = pLanguageElem->Attribute("translated");
    if (pszTranslated != NULL)
        m_Language.szTranslatedName = ToCString(pszTranslated);

    tinyxml2::XMLElement *pStringElem = pLanguageElem->FirstChildElement("String");
    for (pStringElem; pStringElem; pStringElem = pStringElem->NextSiblingElement())
    {
        const char *pszKey = pStringElem->Attribute("key");
        const char *pszValue = pStringElem->Attribute("value");
        if (pszKey != NULL && pszValue != NULL)
        {
            int nKey;
            CString szValue;
            _stscanf(ToCString(pszKey), _T("%x"), &nKey);
            szValue = ToCString(pszValue);
            m_Language.m_Strings.InsertNode(nKey, szValue);
        }
    }
}

void XmlConfiguration::SetLanguage(tinyxml2::XMLElement *pLanguageElem, CLanguage &m_Language)
{
    pLanguageElem->SetAttribute("id", CUtf8String(m_Language.szId).m_Result);
    pLanguageElem->SetAttribute("original", CUtf8String(m_Language.szOriginalName).m_Result);
    pLanguageElem->SetAttribute("translated", CUtf8String(m_Language.szTranslatedName).m_Result);

    tinyxml2::XMLElement *pStringElem;
    POSITION pos = m_Language.m_Strings.m_Map.GetStartPosition();
    while (pos != NULL)
    {
        CString rValue;
        CString szKey;
        int nKey;
        m_Language.m_Strings.m_Map.GetNextAssoc(pos, nKey, rValue);
        szKey.Format(_T("%X"), nKey);
        pStringElem = this->NewElement("String");
        pStringElem->SetAttribute("key", CUtf8String(szKey).m_Result);
        pStringElem->SetAttribute("value", CUtf8String(rValue).m_Result);
        pLanguageElem->LinkEndChild(pStringElem);
    }
}

void XmlConfiguration::GetLanguages(tinyxml2::XMLElement *pLanguagesElem, CLanguagesList &m_Languages)
{
    tinyxml2::XMLElement *pLanguageElem = pLanguagesElem->FirstChildElement("Language");
    for (pLanguageElem; pLanguageElem; pLanguageElem = pLanguageElem->NextSiblingElement())
    {
        CLanguage language;
        this->GetLanguage(pLanguageElem, language);
        m_Languages.InsertNode(language);
    }
}

void XmlConfiguration::SetLanguages(tinyxml2::XMLElement *pLanguagesElem, CLanguagesList &m_Languages)
{
    int nLanguages = m_Languages.GetSize();
    for (int i = 0; i < nLanguages; i++)
    {
        CLanguage& language = m_Languages.GetData(i);
        tinyxml2::XMLElement *pLanguageElem = this->NewElement("Language");
        this->SetLanguage(pLanguageElem, language);
        pLanguagesElem->LinkEndChild(pLanguageElem);
    }
}

void XmlConfiguration::GetOptions(COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionsElem = this->FirstChildElement("Options");
    if (pOptionsElem != NULL)
        this->GetOptions(pOptionsElem, m_Options);
}

void XmlConfiguration::SetOptions(COptions &m_Options)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pOptionsElem = this->NewElement("Options");
    this->LinkEndChild(pOptionsElem);

    this->SetOptions(pOptionsElem, m_Options);
}

void XmlConfiguration::GetPresets(CPresetsList &m_Presets)
{
    tinyxml2::XMLElement *pPresetsElem = this->FirstChildElement("Presets");
    if (pPresetsElem != NULL)
        this->GetPresets(pPresetsElem, m_Presets);
}

void XmlConfiguration::SetPresets(CPresetsList &m_Presets)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
    this->LinkEndChild(pPresetsElem);

    this->SetPresets(pPresetsElem, m_Presets);
}

void XmlConfiguration::GetFormat(CFormat &m_Format)
{
    tinyxml2::XMLElement *pFormatElem = this->FirstChildElement("Format");
    if (pFormatElem != NULL)
        this->GetFormat(pFormatElem, m_Format);
}

void XmlConfiguration::SetFormat(CFormat &m_Format)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pFormatElem = this->NewElement("Format");
    this->LinkEndChild(pFormatElem);

    this->SetFormat(pFormatElem, m_Format);
}

void XmlConfiguration::GetFormats(CFormatsList &m_Formats)
{
    tinyxml2::XMLElement *pFormatsElem = this->FirstChildElement("Formats");
    if (pFormatsElem != NULL)
        this->GetFormats(pFormatsElem, m_Formats);
}

void XmlConfiguration::SetFormats(CFormatsList &m_Formats)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pFormatsElem = this->NewElement("Formats");
    this->LinkEndChild(pFormatsElem);

    this->SetFormats(pFormatsElem, m_Formats);
}

void XmlConfiguration::GetItems(CItemsList &m_Items)
{
    tinyxml2::XMLElement *pItemsElem = this->FirstChildElement("Items");
    if (pItemsElem != NULL)
        this->GetItems(pItemsElem, m_Items);
}

void XmlConfiguration::SetItems(CItemsList &m_Items)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pItemsElem = this->NewElement("Items");
    this->LinkEndChild(pItemsElem);

    this->SetItems(pItemsElem, m_Items);
}

void XmlConfiguration::GetLanguage(CLanguage &m_Language)
{
    tinyxml2::XMLElement *pLanguageElem = this->FirstChildElement("Language");
    if (pLanguageElem != NULL)
        this->GetLanguage(pLanguageElem, m_Language);
}

void XmlConfiguration::SetLanguage(CLanguage &m_Language)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pLanguageElem = this->NewElement("Language");
    this->LinkEndChild(pLanguageElem);

    this->SetLanguage(pLanguageElem, m_Language);
}

void XmlConfiguration::GetLanguages(CLanguagesList &m_Languages)
{
    tinyxml2::XMLElement *pLanguagesElem = this->FirstChildElement("Languages");
    if (pLanguagesElem != NULL)
        this->GetLanguages(pLanguagesElem, m_Languages);
}

void XmlConfiguration::SetLanguages(CLanguagesList &m_Languages)
{
    tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
    this->LinkEndChild(decl);

    tinyxml2::XMLElement *pLanguagesElem = this->NewElement("Languages");
    this->LinkEndChild(pLanguagesElem);

    this->SetLanguages(pLanguagesElem, m_Languages);
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
