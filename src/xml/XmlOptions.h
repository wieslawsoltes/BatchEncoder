// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlBase.h"
#include "configuration\Options.h"

class XmlOptions : public XmlBase
{
public:
    XmlOptions()
    {
    }
    virtual ~XmlOptions()
    {
    }
protected:
    void GetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
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

        pOptionElem = pOptionsElem->FirstChildElement("EnsureItemIsVisible");
        if (pOptionElem)
            m_Options.bEnsureItemIsVisible = ToBool(pOptionElem->GetText());

        pOptionElem = pOptionsElem->FirstChildElement("ValidateInputFiles");
        if (pOptionElem)
            m_Options.bValidateInputFiles = ToBool(pOptionElem->GetText());

        pOptionElem = pOptionsElem->FirstChildElement("OverwriteExistingFiles");
        if (pOptionElem)
            m_Options.bOverwriteExistingFiles = ToBool(pOptionElem->GetText());

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
    void SetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
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

        pOptionElem = this->NewElement("EnsureItemIsVisible");
        pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bEnsureItemIsVisible)).m_Result));
        pOptionsElem->LinkEndChild(pOptionElem);

        pOptionElem = this->NewElement("ValidateInputFiles");
        pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bValidateInputFiles)).m_Result));
        pOptionsElem->LinkEndChild(pOptionElem);

        pOptionElem = this->NewElement("OverwriteExistingFiles");
        pOptionElem->LinkEndChild(this->NewText(CUtf8String(ToCString(m_Options.bOverwriteExistingFiles)).m_Result));
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
public:
    void GetOptions(COptions &m_Options)
    {
        tinyxml2::XMLElement *pOptionsElem = this->FirstChildElement("Options");
        if (pOptionsElem != NULL)
            this->GetOptions(pOptionsElem, m_Options);
    }
    void SetOptions(COptions &m_Options)
    {
        tinyxml2::XMLDeclaration* decl = this->NewDeclaration(m_Utf8DocumentDeclaration);
        this->LinkEndChild(decl);

        tinyxml2::XMLElement *pOptionsElem = this->NewElement("Options");
        this->LinkEndChild(pOptionsElem);

        this->SetOptions(pOptionsElem, m_Options);
    }
};
