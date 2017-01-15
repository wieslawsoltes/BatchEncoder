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

    // option: SelectedPresets
    pOptionElem = pOptionsElem->FirstChildElement("SelectedPresets");
    if (pOptionElem)
    {
        const char *tmpBuff = pOptionElem->GetText();
        m_Options.szSelectedPresets = GetConfigString(tmpBuff);
    }
    else
    {
        m_Options.szSelectedPresets = _T("");
    }

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

    // option: SelectedPresets
    pOptionElem = this->NewElement("SelectedPresets");
    pOptionElem->LinkEndChild(this->NewText(szBuffUtf8.Create(m_Options.szSelectedPresets)));
    pOptionsElem->LinkEndChild(pOptionElem);
    szBuffUtf8.Clear();

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
