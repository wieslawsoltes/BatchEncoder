// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "dialogs\MainDlg.h"

namespace app
{
    CMainApp m_App;

    BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
    END_MESSAGE_MAP()

    CMainApp::CMainApp()
    {
    }

    BOOL CMainApp::InitInstance()
    {
        dialogs::CMainDlg dlg;

        dlg.m_Config.FileSystem = std::make_unique<worker::Win32FileSystem>();

        dlg.m_Config.m_Settings.Init(dlg.m_Config.FileSystem.get());

        dlg.m_Config.Log = std::make_unique<util::FileLog>(dlg.m_Config.m_Settings.szLogFile);
        dlg.m_Config.Log->Open();

        std::wstring szConfigMode = dlg.m_Config.m_Settings.IsPortable(dlg.m_Config.FileSystem.get()) ? L"Portable" : L"Roaming";
        dlg.m_Config.Log->Log(L"[Info] Program started: " + szConfigMode);

        try
        {
            INITCOMMONCONTROLSEX InitCtrls;
            InitCtrls.dwSize = sizeof(InitCtrls);
            InitCtrls.dwICC = ICC_WIN95_CLASSES;
            InitCommonControlsEx(&InitCtrls);

            CWinApp::InitInstance();
        }
        catch (...)
        {
            dlg.m_Config.Log->Log(L"[Error] Failed to init application.");
        }

        try
        {
            dlg.m_Config.nLangId = -1;

            dlg.m_Config.LoadTools(dlg.m_Config.m_Settings.szToolsPath);
            dlg.m_Config.LoadFormats(dlg.m_Config.m_Settings.szFormatsPath);

            if (dlg.m_Config.LoadOutputs(dlg.m_Config.m_Settings.szOutputsFile) == false)
                dlg.m_Config.m_Outputs = config::m_OutpuPathsPresets;

            if (dlg.m_Config.LoadOptions(dlg.m_Config.m_Settings.szOptionsFile) == false)
                dlg.m_Config.m_Options.Defaults();

            dlg.m_Config.LoadItems(dlg.m_Config.m_Settings.szItemsFile);

            dlg.m_Config.LoadLanguages(dlg.m_Config.m_Settings.szSettingsPath);
            dlg.m_Config.LoadLanguages(dlg.m_Config.m_Settings.szLanguagesPath);
        }
        catch (...)
        {
            dlg.m_Config.Log->Log(L"[Error] Failed to load config.");
        }

        try
        {
            m_pMainWnd = &dlg;
            dlg.DoModal();
        }
        catch (...)
        {
            dlg.m_Config.Log->Log(L"[Error] Main dialog exception.");
        }

        try
        {
            if (dlg.m_Config.m_Options.bDoNotSaveConfiguration == false)
            {
                dlg.m_Config.SaveTools(dlg.m_Config.m_Settings.szToolsPath);
                dlg.m_Config.SaveFormats(dlg.m_Config.m_Settings.szFormatsPath);
                dlg.m_Config.SaveOutputs(dlg.m_Config.m_Settings.szOutputsFile);

                dlg.m_Config.SaveOptions(dlg.m_Config.m_Settings.szOptionsFile);

                config::CItem::SetIds(dlg.m_Config.m_Items);
                dlg.m_Config.SaveItems(dlg.m_Config.m_Settings.szItemsFile);
            }
        }
        catch (...)
        {
            dlg.m_Config.Log->Log(L"[Error] Failed to save config.");
        }

        dlg.m_Config.Log->Log(L"[Info] Program exited: " + szConfigMode);
        dlg.m_Config.Log->Close();

        return FALSE;
    }
}
