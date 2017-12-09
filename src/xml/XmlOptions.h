// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "xml\XmlDoc.h"
#include "configuration\Options.h"

class XmlOptions : public XmlDoc
{
public:
    XmlOptions()
    {
    }
    virtual ~XmlOptions()
    {
    }
protected:
    void GetOptions(const XmlElement *element, COptions &m_Options)
    {
        GetChildValue(element, "SelectedLanguage", &m_Options.szSelectedLanguage);
        GetChildValue(element, "SelectedFormat", &m_Options.nSelectedFormat);
        GetChildValue(element, "OutputPath", &m_Options.szOutputPath);
        GetChildValue(element, "DeleteSourceFiles", &m_Options.bDeleteSourceFiles);
        GetChildValue(element, "RecurseChecked", &m_Options.bRecurseChecked);
        GetChildValue(element, "ShutdownWhenFinished", &m_Options.bShutdownWhenFinished);
        GetChildValue(element, "DoNotSaveConfiguration", &m_Options.bDoNotSaveConfiguration);
        GetChildValue(element, "DeleteOnErrors", &m_Options.bDeleteOnErrors);
        GetChildValue(element, "StopOnErrors", &m_Options.bStopOnErrors);
        GetChildValue(element, "HideConsoleWindow", &m_Options.bHideConsoleWindow);
        GetChildValue(element, "TryToFindDecoder", &m_Options.bTryToFindDecoder);
        GetChildValue(element, "EnsureItemIsVisible", &m_Options.bEnsureItemIsVisible);
        GetChildValue(element, "ValidateInputFiles", &m_Options.bValidateInputFiles);
        GetChildValue(element, "OverwriteExistingFiles", &m_Options.bOverwriteExistingFiles);
        GetChildValue(element, "ThreadCount", &m_Options.nThreadCount);
        GetChildValue(element, "MainWindowResize", &m_Options.szMainWindowResize);
        GetChildValue(element, "FileListColumns", &m_Options.szFileListColumns);
        GetChildValue(element, "PresetsDialogResize", &m_Options.szPresetsDialogResize);
        GetChildValue(element, "PresetsListColumns", &m_Options.szPresetsListColumns);
        GetChildValue(element, "FormatsDialogResize", &m_Options.szFormatsDialogResize);
        GetChildValue(element, "FormatsListColumns", &m_Options.szFormatsListColumns);
        GetChildValue(element, "ToolsDialogResize", &m_Options.szToolsDialogResize);
        GetChildValue(element, "ToolsListColumns", &m_Options.szToolsListColumns);
    }
    void SetOptions(XmlElement *element, COptions &m_Options)
    {
        SetChildValue(element, "SelectedLanguage", m_Options.szSelectedLanguage);
        SetChildValue(element, "SelectedFormat", m_Options.nSelectedFormat);
        SetChildValue(element, "OutputPath", m_Options.szOutputPath);
        SetChildValue(element, "DeleteSourceFiles", m_Options.bDeleteSourceFiles);
        SetChildValue(element, "RecurseChecked", m_Options.bRecurseChecked);
        SetChildValue(element, "ShutdownWhenFinished", m_Options.bShutdownWhenFinished);
        SetChildValue(element, "DoNotSaveConfiguration", m_Options.bDoNotSaveConfiguration);
        SetChildValue(element, "DeleteOnErrors", m_Options.bDeleteOnErrors);
        SetChildValue(element, "StopOnErrors", m_Options.bStopOnErrors);
        SetChildValue(element, "HideConsoleWindow", m_Options.bHideConsoleWindow);
        SetChildValue(element, "TryToFindDecoder", m_Options.bTryToFindDecoder);
        SetChildValue(element, "EnsureItemIsVisible", m_Options.bEnsureItemIsVisible);
        SetChildValue(element, "ValidateInputFiles", m_Options.bValidateInputFiles);
        SetChildValue(element, "OverwriteExistingFiles", m_Options.bOverwriteExistingFiles);
        SetChildValue(element, "ThreadCount", m_Options.nThreadCount);
        SetChildValue(element, "MainWindowResize", m_Options.szMainWindowResize);
        SetChildValue(element, "FileListColumns", m_Options.szFileListColumns);
        SetChildValue(element, "PresetsDialogResize", m_Options.szPresetsDialogResize);
        SetChildValue(element, "PresetsListColumns", m_Options.szPresetsListColumns);
        SetChildValue(element, "FormatsDialogResize", m_Options.szFormatsDialogResize);
        SetChildValue(element, "FormatsListColumns", m_Options.szFormatsListColumns);
        SetChildValue(element, "ToolsDialogResize", m_Options.szToolsDialogResize);
        SetChildValue(element, "ToolsListColumns", m_Options.szToolsListColumns);
    }
public:
    void GetOptions(COptions &m_Options)
    {
        auto element = this->FirstChildElement("Options");
        if (element != nullptr)
        {
            this->GetOptions(element, m_Options);
        }
    }
    void SetOptions(COptions &m_Options)
    {
        auto pOptionsElem = this->NewElement("Options");
        this->LinkEndChild(pOptionsElem);
        this->SetOptions(pOptionsElem, m_Options);
    }
};
