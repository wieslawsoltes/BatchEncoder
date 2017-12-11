// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "controls\MyDialogEx.h"
#include "dialogs\MainDlg.h"
#include "utilities\Utilities.h"

CMainApp mainApp;

BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
    ON_COMMAND(ID_HELP, CWinAppEx::OnHelp)
END_MESSAGE_MAP()

CMainApp::CMainApp()
{

}

CString CMainApp::CombinePath(CString szPath, CString szFile)
{
    CString szOutputFile = szFile;
    if (szPath.GetLength() >= 1)
    {
        auto cLast = szPath[szPath.GetLength() - 1];
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

    this->m_bIsPortable = PathFileExists(::GetExeFilePath() + "BatchEncoder.portable") == TRUE ? true : false;

    if (this->m_bIsPortable == true)
    {
        this->szSettingsPath = ::GetExeFilePath();

        this->szFormatsPath = this->szSettingsPath + _T("formats");
        this->szLanguagesPath = this->szSettingsPath + _T("lang");
        this->szProgressPath = this->szSettingsPath + _T("progress");
        this->szToolsPath = this->szSettingsPath + _T("tools");

        try
        {
            ::CreateDirectory(this->szFormatsPath, NULL);
            ::CreateDirectory(this->szLanguagesPath, NULL);
            ::CreateDirectory(this->szProgressPath, NULL);
            ::CreateDirectory(this->szToolsPath, NULL);
        }
        catch (...) {}

        this->szOptionsFile = this->szSettingsPath + _T("BatchEncoder.options");
        this->szFormatsFile = this->szSettingsPath + _T("BatchEncoder.formats");
        this->szItemsFile = this->szSettingsPath + _T("BatchEncoder.items");
        this->szToolsFile = this->szSettingsPath + _T("BatchEncoder.tools");
    }
    else
    {
        CString szConfigDir = _T("BatchEncoder");

        this->szSettingsPath = GetSettingsFilePath(_T(""), szConfigDir);

        this->szFormatsPath = GetSettingsFilePath(_T(""), szConfigDir + _T("\\formats"));
        this->szLanguagesPath = GetSettingsFilePath(_T(""), szConfigDir + _T("\\lang"));
        this->szProgressPath = GetSettingsFilePath(_T(""), szConfigDir + _T("\\progress"));
        this->szToolsPath = GetSettingsFilePath(_T(""), szConfigDir + _T("\\tools"));

        try
        {
            ::CreateDirectory(szSettingsPath, NULL);
            ::CreateDirectory(szFormatsPath, NULL);
            ::CreateDirectory(szLanguagesPath, NULL);
            ::CreateDirectory(szProgressPath, NULL);
            ::CreateDirectory(szToolsPath, NULL);
        }
        catch (...) {}

        this->szOptionsFile = ::GetSettingsFilePath(_T("BatchEncoder.options"), szConfigDir);
        this->szFormatsFile = ::GetSettingsFilePath(_T("BatchEncoder.formats"), szConfigDir);
        this->szItemsFile = ::GetSettingsFilePath(_T("BatchEncoder.items"), szConfigDir);
        this->szToolsFile = ::GetSettingsFilePath(_T("BatchEncoder.tools"), szConfigDir);
    }

    CMainDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
