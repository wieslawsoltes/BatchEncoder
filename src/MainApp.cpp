// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "controls\MyDialogEx.h"
#include "dialogs\MainDlg.h"
#include "utilities\Utilities.h"

namespace app
{
    CMainApp m_App;

    BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
        ON_COMMAND(ID_HELP, CWinAppEx::OnHelp)
    END_MESSAGE_MAP()

    CMainApp::CMainApp()
    {
    }

    bool CMainApp::IsPortable()
    {
        std::wstring szPath = util::Utilities::GetExeFilePath() + L"BatchEncoder.portable";
        return ::PathFileExists(szPath.c_str()) == TRUE;
    }

    BOOL CMainApp::InitInstance()
    {
        INITCOMMONCONTROLSEX InitCtrls;
        InitCtrls.dwSize = sizeof(InitCtrls);
        InitCtrls.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&InitCtrls);

        CWinAppEx::InitInstance();
        AfxEnableControlContainer();
        InitShellManager();

        if (this->IsPortable())
        {
            this->szSettingsPath = util::Utilities::GetExeFilePath();

            this->szFormatsPath = util::Utilities::CombinePath(this->szSettingsPath, L"formats");
            this->szPresetsPath = util::Utilities::CombinePath(this->szSettingsPath, L"presets");
            this->szLanguagesPath = util::Utilities::CombinePath(this->szSettingsPath, L"lang");
            this->szProgressPath = util::Utilities::CombinePath(this->szSettingsPath, L"progress");
            this->szToolsPath = util::Utilities::CombinePath(this->szSettingsPath, L"tools");

            try
            {
                ::CreateDirectory(this->szFormatsPath.c_str(), NULL);
                ::CreateDirectory(this->szPresetsPath.c_str(), NULL);
                ::CreateDirectory(this->szLanguagesPath.c_str(), NULL);
                ::CreateDirectory(this->szProgressPath.c_str(), NULL);
                ::CreateDirectory(this->szToolsPath.c_str(), NULL);
            }
            catch (...) {}

            this->szOptionsFile = util::Utilities::CombinePath(this->szSettingsPath, L"Options.xml");
            this->szFormatsFile = util::Utilities::CombinePath(this->szSettingsPath, L"Formats.xml");
            this->szItemsFile = util::Utilities::CombinePath(this->szSettingsPath, L"Items.xml");
            this->szToolsFile = util::Utilities::CombinePath(this->szSettingsPath, L"Tools.xml");
        }
        else
        {
            std::wstring szConfigDir = L"BatchEncoder";

            this->szSettingsPath = util::Utilities::GetSettingsFilePath(L"", szConfigDir);

            this->szFormatsPath = util::Utilities::GetSettingsFilePath(L"", szConfigDir + L"\\formats");
            this->szPresetsPath = util::Utilities::GetSettingsFilePath(L"", szConfigDir + L"\\presets");
            this->szLanguagesPath = util::Utilities::GetSettingsFilePath(L"", szConfigDir + L"\\lang");
            this->szProgressPath = util::Utilities::GetSettingsFilePath(L"", szConfigDir + L"\\progress");
            this->szToolsPath = util::Utilities::GetSettingsFilePath(L"", szConfigDir + L"\\tools");

            try
            {
                ::CreateDirectory(szSettingsPath.c_str(), NULL);
                ::CreateDirectory(szFormatsPath.c_str(), NULL);
                ::CreateDirectory(szPresetsPath.c_str(), NULL);
                ::CreateDirectory(szLanguagesPath.c_str(), NULL);
                ::CreateDirectory(szProgressPath.c_str(), NULL);
                ::CreateDirectory(szToolsPath.c_str(), NULL);
            }
            catch (...) {}

            this->szOptionsFile = util::Utilities::GetSettingsFilePath(L"Options.xml", szConfigDir);
            this->szFormatsFile = util::Utilities::GetSettingsFilePath(L"Formats.xml", szConfigDir);
            this->szItemsFile = util::Utilities::GetSettingsFilePath(L"Items.xml", szConfigDir);
            this->szToolsFile = util::Utilities::GetSettingsFilePath(L"Tools.xml", szConfigDir);
        }

        CMainDlg dlg;
        m_pMainWnd = &dlg;
        dlg.DoModal();

        return FALSE;
    }
}
