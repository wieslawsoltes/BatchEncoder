// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "XmlDoc.h"
#include "config\Options.h"

namespace xml
{
    class XmlOptions : public XmlDoc
    {
    public:
        XmlOptions(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlOptions() { }
    public:
        bool GetOptions(const XmlElement *element, config::COptions &m_Options)
        {
            VALIDATE(GetChildValue(element, "SelectedLanguage", &m_Options.szSelectedLanguage));
            VALIDATE(GetChildValue(element, "SelectedFormat", &m_Options.nSelectedFormat));
            VALIDATE(GetChildValue(element, "OutputPath", &m_Options.szOutputPath));
            VALIDATE(GetChildValue(element, "DeleteSourceFiles", &m_Options.bDeleteSourceFiles));
            VALIDATE(GetChildValue(element, "RecurseChecked", &m_Options.bRecurseChecked));
            VALIDATE(GetChildValue(element, "ShutdownWhenFinished", &m_Options.bShutdownWhenFinished));
            VALIDATE(GetChildValue(element, "DoNotSaveConfiguration", &m_Options.bDoNotSaveConfiguration));
            VALIDATE(GetChildValue(element, "DeleteOnErrors", &m_Options.bDeleteOnErrors));
            VALIDATE(GetChildValue(element, "StopOnErrors", &m_Options.bStopOnErrors));
            VALIDATE(GetChildValue(element, "HideConsoleWindow", &m_Options.bHideConsoleWindow));
            VALIDATE(GetChildValue(element, "TryToFindDecoder", &m_Options.bTryToFindDecoder));
            VALIDATE(GetChildValue(element, "EnsureItemIsVisible", &m_Options.bEnsureItemIsVisible));
            VALIDATE(GetChildValue(element, "ValidateInputFiles", &m_Options.bValidateInputFiles));
            VALIDATE(GetChildValue(element, "OverwriteExistingFiles", &m_Options.bOverwriteExistingFiles));
            VALIDATE(GetChildValue(element, "TryToDownloadTools", &m_Options.bTryToDownloadTools));
            VALIDATE(GetChildValue(element, "ThreadCount", &m_Options.nThreadCount));
            VALIDATE(GetChildValue(element, "OutputBrowse", &m_Options.szOutputBrowse));
            VALIDATE(GetChildValue(element, "DirectoryBrowse", &m_Options.szDirectoryBrowse));
            VALIDATE(GetChildValue(element, "MainWindowResize", &m_Options.szMainWindowResize));
            VALIDATE(GetChildValue(element, "FileListColumns", &m_Options.szFileListColumns));
            VALIDATE(GetChildValue(element, "PresetsDialogResize", &m_Options.szPresetsDialogResize));
            VALIDATE(GetChildValue(element, "PresetsListColumns", &m_Options.szPresetsListColumns));
            VALIDATE(GetChildValue(element, "FormatsDialogResize", &m_Options.szFormatsDialogResize));
            VALIDATE(GetChildValue(element, "FormatsListColumns", &m_Options.szFormatsListColumns));
            VALIDATE(GetChildValue(element, "ToolsDialogResize", &m_Options.szToolsDialogResize));
            VALIDATE(GetChildValue(element, "ToolsListColumns", &m_Options.szToolsListColumns));
            return true;
        }
        void SetOptions(XmlElement *element, config::COptions &m_Options)
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
            SetChildValue(element, "TryToDownloadTools", m_Options.bTryToDownloadTools);
            SetChildValue(element, "ThreadCount", m_Options.nThreadCount);
            SetChildValue(element, "OutputBrowse", m_Options.szOutputBrowse);
            SetChildValue(element, "DirectoryBrowse", m_Options.szDirectoryBrowse);
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
        bool GetOptions(config::COptions &m_Options)
        {
            auto element = this->FirstChildElement("Options");
            if (element != nullptr)
            {
                VALIDATE(this->GetOptions(element, m_Options));
                return true;
            }
            return false;
        }
        void SetOptions(config::COptions &m_Options)
        {
            auto pOptionsElem = this->NewElement("Options");
            this->LinkEndChild(pOptionsElem);
            this->SetOptions(pOptionsElem, m_Options);
        }
    };
}
