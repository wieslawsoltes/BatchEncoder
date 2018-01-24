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

    std::wstring CMainApp::CombinePath(const std::wstring& szPath, const std::wstring& szFile)
    {
        std::wstring szOutputFile = szFile;
        if (szPath.length() >= 1)
        {
            auto cLast = szPath[szPath.length() - 1];
            if ((cLast == '\\') || (cLast == '/'))
                szOutputFile = szPath + szOutputFile;
            else
                szOutputFile = szPath + _T("\\") + szOutputFile;
        }
        return szOutputFile;
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

        this->m_bIsPortable = PathFileExists(util::GetExeFilePath() + "BatchEncoder.portable") == TRUE ? true : false;

        if (this->m_bIsPortable == true)
        {
            this->szSettingsPath = util::GetExeFilePath();

            this->szFormatsPath = this->szSettingsPath + _T("formats");
            this->szPresetsPath = this->szSettingsPath + _T("presets");
            this->szLanguagesPath = this->szSettingsPath + _T("lang");
            this->szProgressPath = this->szSettingsPath + _T("progress");
            this->szToolsPath = this->szSettingsPath + _T("tools");

            try
            {
                ::CreateDirectory(this->szFormatsPath, NULL);
                ::CreateDirectory(this->szPresetsPath, NULL);
                ::CreateDirectory(this->szLanguagesPath, NULL);
                ::CreateDirectory(this->szProgressPath, NULL);
                ::CreateDirectory(this->szToolsPath, NULL);
            }
            catch (...) {}

            this->szOptionsFile = this->szSettingsPath + _T("Options.xml");
            this->szFormatsFile = this->szSettingsPath + _T("formats.xml");
            this->szItemsFile = this->szSettingsPath + _T("Items.xml");
            this->szToolsFile = this->szSettingsPath + _T("Tools.xml");
        }
        else
        {
            CString szConfigDir = _T("BatchEncoder");

            this->szSettingsPath = util::GetSettingsFilePath(_T(""), szConfigDir);

            this->szFormatsPath = util::GetSettingsFilePath(_T(""), szConfigDir + _T("\\formats"));
            this->szPresetsPath = util::GetSettingsFilePath(_T(""), szConfigDir + _T("\\presets"));
            this->szLanguagesPath = util::GetSettingsFilePath(_T(""), szConfigDir + _T("\\lang"));
            this->szProgressPath = util::GetSettingsFilePath(_T(""), szConfigDir + _T("\\progress"));
            this->szToolsPath = util::GetSettingsFilePath(_T(""), szConfigDir + _T("\\tools"));

            try
            {
                ::CreateDirectory(szSettingsPath, NULL);
                ::CreateDirectory(szFormatsPath, NULL);
                ::CreateDirectory(szPresetsPath, NULL);
                ::CreateDirectory(szLanguagesPath, NULL);
                ::CreateDirectory(szProgressPath, NULL);
                ::CreateDirectory(szToolsPath, NULL);
            }
            catch (...) {}

            this->szOptionsFile = util::GetSettingsFilePath(_T("Options.xml"), szConfigDir);
            this->szFormatsFile = util::GetSettingsFilePath(_T("Formats.xml"), szConfigDir);
            this->szItemsFile = util::GetSettingsFilePath(_T("Items.xml"), szConfigDir);
            this->szToolsFile = util::GetSettingsFilePath(_T("Tools.xml"), szConfigDir);
        }

        CMainDlg dlg;
        m_pMainWnd = &dlg;
        dlg.DoModal();

        return FALSE;
    }
}
