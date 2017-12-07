// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "XmlDoc.h"
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
    void GetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
    {
        GetChildValue(pOptionsElem, "SelectedLanguage", &m_Options.szSelectedLanguage);
        GetChildValue(pOptionsElem, "SelectedFormat", &m_Options.nSelectedFormat);
        GetChildValue(pOptionsElem, "OutputPath", &m_Options.szOutputPath);
        GetChildValue(pOptionsElem, "DeleteSourceFiles", &m_Options.bDeleteSourceFiles);
        GetChildValue(pOptionsElem, "RecurseChecked", &m_Options.bRecurseChecked);
        GetChildValue(pOptionsElem, "ShutdownWhenFinished", &m_Options.bShutdownWhenFinished);
        GetChildValue(pOptionsElem, "DoNotSaveConfiguration", &m_Options.bDoNotSaveConfiguration);
        GetChildValue(pOptionsElem, "DeleteOnErrors", &m_Options.bDeleteOnErrors);
        GetChildValue(pOptionsElem, "StopOnErrors", &m_Options.bStopOnErrors);
        GetChildValue(pOptionsElem, "HideConsoleWindow", &m_Options.bHideConsoleWindow);
        GetChildValue(pOptionsElem, "TryToFindDecoder", &m_Options.bTryToFindDecoder);
        GetChildValue(pOptionsElem, "EnsureItemIsVisible", &m_Options.bEnsureItemIsVisible);
        GetChildValue(pOptionsElem, "ValidateInputFiles", &m_Options.bValidateInputFiles);
        GetChildValue(pOptionsElem, "OverwriteExistingFiles", &m_Options.bOverwriteExistingFiles);
        GetChildValue(pOptionsElem, "ThreadCount", &m_Options.nThreadCount);
        GetChildValue(pOptionsElem, "MainWindowResize", &m_Options.szMainWindowResize);
        GetChildValue(pOptionsElem, "FileListColumns", &m_Options.szFileListColumns);
        GetChildValue(pOptionsElem, "PresetsDialogResize", &m_Options.szPresetsDialogResize);
        GetChildValue(pOptionsElem, "PresetsListColumns", &m_Options.szPresetsListColumns);
        GetChildValue(pOptionsElem, "FormatsDialogResize", &m_Options.szFormatsDialogResize);
        GetChildValue(pOptionsElem, "FormatsListColumns", &m_Options.szFormatsListColumns);
        GetChildValue(pOptionsElem, "ToolsDialogResize", &m_Options.szToolsDialogResize);
        GetChildValue(pOptionsElem, "ToolsListColumns", &m_Options.szToolsListColumns);
    }
    void SetOptions(tinyxml2::XMLElement *pOptionsElem, COptions &m_Options)
    {
        SetChildValue(pOptionsElem, "SelectedLanguage", m_Options.szSelectedLanguage);
        SetChildValue(pOptionsElem, "SelectedFormat", m_Options.nSelectedFormat);
        SetChildValue(pOptionsElem, "OutputPath", m_Options.szOutputPath);
        SetChildValue(pOptionsElem, "DeleteSourceFiles", m_Options.bDeleteSourceFiles);
        SetChildValue(pOptionsElem, "RecurseChecked", m_Options.bRecurseChecked);
        SetChildValue(pOptionsElem, "ShutdownWhenFinished", m_Options.bShutdownWhenFinished);
        SetChildValue(pOptionsElem, "DoNotSaveConfiguration", m_Options.bDoNotSaveConfiguration);
        SetChildValue(pOptionsElem, "DeleteOnErrors", m_Options.bDeleteOnErrors);
        SetChildValue(pOptionsElem, "StopOnErrors", m_Options.bStopOnErrors);
        SetChildValue(pOptionsElem, "HideConsoleWindow", m_Options.bHideConsoleWindow);
        SetChildValue(pOptionsElem, "TryToFindDecoder", m_Options.bTryToFindDecoder);
        SetChildValue(pOptionsElem, "EnsureItemIsVisible", m_Options.bEnsureItemIsVisible);
        SetChildValue(pOptionsElem, "ValidateInputFiles", m_Options.bValidateInputFiles);
        SetChildValue(pOptionsElem, "OverwriteExistingFiles", m_Options.bOverwriteExistingFiles);
        SetChildValue(pOptionsElem, "ThreadCount", m_Options.nThreadCount);
        SetChildValue(pOptionsElem, "MainWindowResize", m_Options.szMainWindowResize);
        SetChildValue(pOptionsElem, "FileListColumns", m_Options.szFileListColumns);
        SetChildValue(pOptionsElem, "PresetsDialogResize", m_Options.szPresetsDialogResize);
        SetChildValue(pOptionsElem, "PresetsListColumns", m_Options.szPresetsListColumns);
        SetChildValue(pOptionsElem, "FormatsDialogResize", m_Options.szFormatsDialogResize);
        SetChildValue(pOptionsElem, "FormatsListColumns", m_Options.szFormatsListColumns);
        SetChildValue(pOptionsElem, "ToolsDialogResize", m_Options.szToolsDialogResize);
        SetChildValue(pOptionsElem, "ToolsListColumns", m_Options.szToolsListColumns);
    }
public:
    void GetOptions(COptions &m_Options)
    {
        tinyxml2::XMLElement *pOptionsElem = this->FirstChildElement("Options");
        if (pOptionsElem != NULL)
        {
            this->GetOptions(pOptionsElem, m_Options);
        }
    }
    void SetOptions(COptions &m_Options)
    {
        tinyxml2::XMLElement *pOptionsElem = this->NewElement("Options");
        this->LinkEndChild(pOptionsElem);
        this->SetOptions(pOptionsElem, m_Options);
    }
};
