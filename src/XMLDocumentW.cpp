// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "utilities\Utilities.h"
#include "utilities\Utf8String.h"
#include "XMLDocumentW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CXMLDocumentW::CXMLDocumentW()
{

}

CXMLDocumentW::~CXMLDocumentW()
{

}

void CXMLDocumentW::LoadOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
{
    tinyxml2::XMLElement *pOptionElem;

    // option: SelectedFormat
    pOptionElem = pOptionsElem->FirstChildElement("SelectedFormat");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.nSelectedFormat = stoi(GetConfigString(tmpBuff));
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
        m_Options.szOutputPath = GetConfigString(tmpBuff);
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
        m_Options.bOutputPathChecked = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bOutputPathChecked = false;
    }

    // option: LogConsoleOutput
    pOptionElem = pOptionsElem->FirstChildElement("LogConsoleOutput");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bLogConsoleOutput = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bLogConsoleOutput = false;
    }

    // option: DeleteSourceFiles
    pOptionElem = pOptionsElem->FirstChildElement("DeleteSourceFiles");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bDeleteSourceFiles = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bDeleteSourceFiles = false;
    }

    // option: StayOnTop
    pOptionElem = pOptionsElem->FirstChildElement("StayOnTop");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bStayOnTop = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bStayOnTop = false;
    }

    // option: RecurseChecked
    pOptionElem = pOptionsElem->FirstChildElement("RecurseChecked");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bRecurseChecked = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bRecurseChecked = false;
    }

    // option: MainWindowResize
    pOptionElem = pOptionsElem->FirstChildElement("MainWindowResize");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szMainWindowResize = GetConfigString(tmpBuff);
    }

    // option: FileListColumns
    pOptionElem = pOptionsElem->FirstChildElement("FileListColumns");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szFileListColumns = GetConfigString(tmpBuff);
    }

    // option: ShowGridLines
    pOptionElem = pOptionsElem->FirstChildElement("ShowGridLines");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bShowGridLines = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bShowGridLines = false;
    }

    // option: ShowTrayIcon
    pOptionElem = pOptionsElem->FirstChildElement("ShowTrayIcon");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bShowTrayIcon = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bShowTrayIcon = false;
    }

    // option: DoNotSaveConfiguration
    pOptionElem = pOptionsElem->FirstChildElement("DoNotSaveConfiguration");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bDoNotSaveConfiguration = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bDoNotSaveConfiguration = false;
    }

    // option: PresetsDialogResize
    pOptionElem = pOptionsElem->FirstChildElement("PresetsDialogResize");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szPresetsDialogResize = GetConfigString(tmpBuff);
    }

    // option: PresetsListColumns
    pOptionElem = pOptionsElem->FirstChildElement("PresetsListColumns");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szPresetsListColumns = GetConfigString(tmpBuff);
    }

    // option: FormatsDialogResize
    pOptionElem = pOptionsElem->FirstChildElement("FormatsDialogResize");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szFormatsDialogResize = GetConfigString(tmpBuff);
    }

    // option: FormatsListColumns
    pOptionElem = pOptionsElem->FirstChildElement("FormatsListColumns");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szFormatsListColumns = GetConfigString(tmpBuff);
    }

    // option: DeleteOnError
    pOptionElem = pOptionsElem->FirstChildElement("DeleteOnError");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bDeleteOnError = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bDeleteOnError = false;
    }

    // option: StopOnErrors
    pOptionElem = pOptionsElem->FirstChildElement("StopOnErrors");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bStopOnErrors = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bStopOnErrors = false;
    }

    // option: LogFileName
    pOptionElem = pOptionsElem->FirstChildElement("LogFileName");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szLogFileName = GetConfigString(tmpBuff);
    }
    else
    {
        m_Options.szLogFileName = MAIN_APP_LOG;
    }

    // option: LogFileEncoding
    pOptionElem = pOptionsElem->FirstChildElement("LogFileEncoding");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.nLogEncoding = stoi(GetConfigString(tmpBuff));
    }
    else
    {
        m_Options.nLogEncoding = 2;
    }

    // option: ForceConsoleWindow
    pOptionElem = pOptionsElem->FirstChildElement("ForceConsoleWindow");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.bForceConsoleWindow = GetConfigString(tmpBuff).Compare(_T("true")) == 0;
    }
    else
    {
        m_Options.bForceConsoleWindow = false;
    }
}

void CXMLDocumentW::SaveOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
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

    // option: LogConsoleOutput
    pOptionElem = this->NewElement("LogConsoleOutput");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bLogConsoleOutput ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: DeleteSourceFiles
    pOptionElem = this->NewElement("DeleteSourceFiles");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bDeleteSourceFiles ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: StayOnTop
    pOptionElem = this->NewElement("StayOnTop");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bStayOnTop ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: RecurseChecked
    pOptionElem = this->NewElement("RecurseChecked");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bRecurseChecked ? _T("true") : _T("false"))));
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

    // option: ShowGridLines
    pOptionElem = this->NewElement("ShowGridLines");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bShowGridLines ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: ShowTrayIcon
    pOptionElem = this->NewElement("ShowTrayIcon");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bShowTrayIcon ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: DoNotSaveConfiguration
    pOptionElem = this->NewElement("DoNotSaveConfiguration");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bDoNotSaveConfiguration ? _T("true") : _T("false"))));
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

    // option: DeleteOnError
    pOptionElem = this->NewElement("DeleteOnError");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bDeleteOnError ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: StopOnErrors
    pOptionElem = this->NewElement("StopOnErrors");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bStopOnErrors ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: LogFileName
    pOptionElem = this->NewElement("LogFileName");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szLogFileName)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: LogFileEncoding
    CString szLogEncoding;
    szLogEncoding.Format(_T("%d\0"), m_Options.nLogEncoding);

    pOptionElem = this->NewElement("LogFileEncoding");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(szLogEncoding)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    // option: ForceConsoleWindow
    pOptionElem = this->NewElement("ForceConsoleWindow");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.bForceConsoleWindow ? _T("true") : _T("false"))));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

    pOptionsElem->LinkEndChild(pOptionElem);
}

void CXMLDocumentW::LoadPresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList *m_pPresets)
{
    tinyxml2::XMLElement *pPresetElem = pPresetsElem->FirstChildElement("Preset");
    for (pPresetElem; pPresetElem; pPresetElem = pPresetElem->NextSiblingElement())
    {
        CPreset preset;

        const char *pszName = pPresetElem->Attribute("name");
        if (pszName != NULL)
        {
            preset.szName = GetConfigString(pszName);
        }

        const char *pszOptions = pPresetElem->Attribute("options");
        if (pszOptions != NULL)
        {
            preset.szOptions = GetConfigString(pszOptions);
        }

        m_pPresets->InsertNode(preset);
    }
}

void CXMLDocumentW::SavePresets(tinyxml2::XMLElement *pPresetsElem, CPresetsList *m_pPresets)
{
    CUtf8String szBuffUtf8;
    tinyxml2::XMLElement *pPresetElem;

    int nPresets = m_pPresets->GetSize();
    for (int i = 0; i < nPresets; i++)
    {
        CPreset& preset = m_pPresets->GetData(i);

        pPresetElem = this->NewElement("Preset");

        pPresetElem->SetAttribute("name", szBuffUtf8.Create(preset.szName));
        szBuffUtf8.Clear();

        pPresetElem->SetAttribute("options", szBuffUtf8.Create(preset.szOptions));
        szBuffUtf8.Clear();

        pPresetsElem->LinkEndChild(pPresetElem);
    }
}

void CXMLDocumentW::LoadFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
{
    tinyxml2::XMLElement *pFormatElem = pFormatsElem->FirstChildElement("Format");
    for (pFormatElem; pFormatElem; pFormatElem = pFormatElem->NextSiblingElement())
    {
        CFormat format;
        CPresetsList m_Presets;
        format.m_pPresets = &m_Presets;
        
        const char *pszId = pFormatElem->Attribute("id");
        if (pszId != NULL)
        {
            format.szId = GetConfigString(pszId);
        }

        const char *pszName = pFormatElem->Attribute("name");
        if (pszName != NULL)
        {
            format.szName = GetConfigString(pszName);
        }

        const char *pszTemplate = pFormatElem->Attribute("template");
        if (pszTemplate != NULL)
        {
            format.szTemplate = GetConfigString(pszTemplate);
        }

        const char *pszPipesInput = pFormatElem->Attribute("input");
        if (pszPipesInput != NULL)
        {
            format.bInput = GetConfigString(pszPipesInput).CompareNoCase(_T("true")) == 0;
        }

        const char *pszPipesOutput = pFormatElem->Attribute("output");
        if (pszPipesOutput != NULL)
        {
            format.bOutput = GetConfigString(pszPipesOutput).CompareNoCase(_T("true")) == 0;
        }

        const char *pszFunction = pFormatElem->Attribute("function");
        if (pszFunction != NULL)
        {
            format.szFunction = GetConfigString(pszFunction);
        }

        const char *pszPath = pFormatElem->Attribute("path");
        if (pszPath != NULL)
        {
            format.szPath = GetConfigString(pszPath);
        }

        const char *pszType = pFormatElem->Attribute("type");
        if (pszType != NULL)
        {
            format.nType = stoi(GetConfigString(pszType));
        }

        const char *pszExtension = pFormatElem->Attribute("extension");
        if (pszExtension != NULL)
        {
            format.szExtension = GetConfigString(pszExtension);
        }

        const char *pszDefaultPreset = pFormatElem->Attribute("default");
        if (pszDefaultPreset != NULL)
        {
            format.nDefaultPreset = stoi(GetConfigString(pszDefaultPreset));
        }

        tinyxml2::XMLElement *pPresetsElem = pFormatElem->FirstChildElement("Presets");
        this->LoadPresets(pPresetsElem, format.m_pPresets);

        m_Formats.InsertNode(format);
    }
}

void CXMLDocumentW::SaveFormats(tinyxml2::XMLElement *pFormatsElem, CFormatsList &m_Formats)
{
    CUtf8String szBuffUtf8;
    tinyxml2::XMLElement *pFormatElem;

    int nFormats = m_Formats.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CFormat& format = m_Formats.GetData(i);

        pFormatElem = this->NewElement("Format");

        pFormatElem->SetAttribute("id", szBuffUtf8.Create(format.szId));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("name", szBuffUtf8.Create(format.szName));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("template", szBuffUtf8.Create(format.szTemplate));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("input", (format.bInput) ? "true" : "false");
        pFormatElem->SetAttribute("output", (format.bOutput) ? "true" : "false");

        pFormatElem->SetAttribute("function", szBuffUtf8.Create(format.szFunction));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("path", szBuffUtf8.Create(format.szPath));
        szBuffUtf8.Clear();

        CString szType;
        szType.Format(_T("%d\0"), format.nType);
        pFormatElem->SetAttribute("type", szBuffUtf8.Create(szType));
        szBuffUtf8.Clear();

        pFormatElem->SetAttribute("extension", szBuffUtf8.Create(format.szExtension));
        szBuffUtf8.Clear();

        CString szDefaultPreset;
        szDefaultPreset.Format(_T("%d\0"), format.nDefaultPreset);
        pFormatElem->SetAttribute("default", szBuffUtf8.Create(szDefaultPreset));
        szBuffUtf8.Clear();

        tinyxml2::XMLElement *pPresetsElem = this->NewElement("Presets");
        pFormatElem->LinkEndChild(pPresetsElem);
        this->SavePresets(pPresetsElem, format.m_pPresets);

        pFormatsElem->LinkEndChild(pFormatElem);
    }
}

void CXMLDocumentW::LoadItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
{
    tinyxml2::XMLElement *pItemElem = pItemsElem->FirstChildElement("Item");
    for (pItemElem; pItemElem; pItemElem = pItemElem->NextSiblingElement())
    {
        CItem item;

        const char *pszPath = pItemElem->Attribute("path");
        if (pszPath != NULL)
        {
            item.szPath = GetConfigString(pszPath);
        }

        const char *pszSize = pItemElem->Attribute("size");
        if (pszSize != NULL)
        {
            item.szSize = GetConfigString(pszSize);
        }

        const char *pszName = pItemElem->Attribute("name");
        if (pszName != NULL)
        {
            item.szName = GetConfigString(pszName);
        }

        const char *pszExtension = pItemElem->Attribute("extension");
        if (pszExtension != NULL)
        {
            item.szExtension = GetConfigString(pszExtension);
        }

        const char *pszFormatId = pItemElem->Attribute("format");
        if (pszFormatId != NULL)
        {
            item.szFormatId = GetConfigString(pszFormatId);
        }

        const char *pszPreset = pItemElem->Attribute("preset");
        if (pszPreset != NULL)
        {
            item.nPreset = stoi(GetConfigString(pszPreset));
        }

        const char *pszChecked = pItemElem->Attribute("checked");
        if (pszChecked != NULL)
        {
            item.bChecked = GetConfigString(pszChecked).CompareNoCase(_T("true")) == 0;
        }

        const char *pszTime = pItemElem->Attribute("time");
        if (pszTime != NULL)
        {
            item.szTime = GetConfigString(pszTime);
        }

        const char *pszStatus = pItemElem->Attribute("status");
        if (pszStatus != NULL)
        {
            item.szStatus = GetConfigString(pszStatus);
        }

        m_Items.InsertNode(item);
    }
}

void CXMLDocumentW::SaveItems(tinyxml2::XMLElement *pItemsElem, CItemsList &m_Items)
{
    CUtf8String szBuffUtf8;
    tinyxml2::XMLElement *pItemElem;

    int nFormats = m_Items.GetSize();
    for (int i = 0; i < nFormats; i++)
    {
        CItem& item = m_Items.GetData(i);

        pItemElem = this->NewElement("Item");

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

bool CXMLDocumentW::LoadFileW(CString szFileName)
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

bool CXMLDocumentW::SaveFileW(CString szFileName)
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
