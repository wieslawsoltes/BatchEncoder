//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

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
