// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "ResizeDialog.h"
#include "ItemsList.h"
#include "BatchEncoderDlg.h"
#include "PresetsDlg.h"
#include "FormatsDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBatchEncoderApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CBatchEncoderApp::CBatchEncoderApp()
{

}

CBatchEncoderApp theApp;

BOOL CBatchEncoderApp::InitInstance()
{
    InitCommonControls();
    CWinApp::InitInstance();

    CBatchEncoderDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
