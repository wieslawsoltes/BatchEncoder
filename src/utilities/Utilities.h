// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

void DoTheShutdown();
void LaunchAndWait(LPCTSTR file, LPCTSTR params, BOOL bWait);
void SetComboBoxHeight(HWND hDlg, int nComboBoxID);
CString GetFileName(CString szFilePath);
ULONGLONG GetFileSize64(HANDLE hFile);
CString GetExeFilePath();
void UpdatePath();
CString GetConfigString(const char *pszUtf8);
int stoi(CString szData);
BOOL MakeFullPath(CString szPath);
CString FormatTime(double fTime, int nFormat);
void GetFullPathName(CString &szFilePath);
