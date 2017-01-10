// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

typedef bool(*fncCopyCallback) (int nProgress); // nProgress = 0..100

#ifdef _DEBUG
void LastError(LPTSTR lpszFunction);
#else
#define LastError(x)
#endif

void DoTheShutdown();
void LaunchAndWait(LPCTSTR file, LPCTSTR params, BOOL bWait);
unsigned int StringLenght(register TCHAR *str);
int StringSearch(TCHAR *str, int str_len, TCHAR *key, int key_len);
bool CheckCmdLineOpt(TCHAR *str, register TCHAR *key);
void SetComboBoxHeight(HWND hDlg, int nComboBoxID);
CString GetFileName(CString szFilePath);
CString GetExeFilePath();
void UpdatePath();
bool CopyOneFile(CString szInFile, CString szOutFile, fncCopyCallback lpCallback = NULL);
CString GetConfigString(const char *pszUtf8);
int stoi(CString szData);
void MakeSafeString(CString &szData);
BOOL MakeFullPath(CString szPath);
CString FormatTime(double fTime, int nFormat);
void SetBrowsePath(CFileDialog &fileDlg, CString szPath);
CString GetBrowsePath(CFileDialog &fileDlg);
void GradientFill(CDC *m_pMemDC, CRect &rc, COLORREF cr01, COLORREF cr02, bool bVertical);
int GetFormatId(CString szBuff);
void GetFullPathName(CString &szFilePath);
