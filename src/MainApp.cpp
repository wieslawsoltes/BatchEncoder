// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "controls\MyDialogEx.h"
#include "dialogs\MainDlg.h"

namespace app
{
    CMainApp m_App;

    BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
        ON_COMMAND(ID_HELP, CWinAppEx::OnHelp)
    END_MESSAGE_MAP()

    CMainApp::CMainApp()
    {
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

        m_Settings.Init();

        CMainDlg dlg;
        m_pMainWnd = &dlg;
        dlg.DoModal();

        return FALSE;
    }
}
