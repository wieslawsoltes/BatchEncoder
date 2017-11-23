// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

typedef struct tagLogicalProcessorInformation
{
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
} LogicalProcessorInformation;

int GetLogicalProcessorInformation(LogicalProcessorInformation* info);

void ShutdownWindows();

void LaunchAndWait(LPCTSTR file, LPCTSTR params, BOOL bWait);

void SetComboBoxHeight(HWND hDlg, int nComboBoxID, int nSizeLimit);

CString GetFileName(CString szFilePath);

CString GetFilePath(CString szFilePath);

CString GetFileExtension(CString szFilePath);

CString GetOnlyFileName(CString szFilePath);

ULONGLONG GetFileSize64(HANDLE hFile);

ULONGLONG GetFileSize64(CString szFileName);

__int64 GetFileSizeInt64(FILE *fp);

CString GetExeFilePath();

CString GetSettingsFilePath(CString szFileName, CString szConfigDirectory);

void GetFullPathName(CString &szFilePath);

BOOL DirectoryExists(LPCTSTR szPath);

bool MakeFullPath(CString szPath);

bool FileExists(CString szPath);

CString GenerateUuidString();

CString ReplaceNoCase(LPCTSTR instr, LPCTSTR oldstr, LPCTSTR newstr);

int FindNoCase(LPCTSTR pszString, LPCTSTR pszSearch);

void ConvertAnsiToUnicode(const char *szAnsi, wchar_t *szUnicode, ULONG nLength);

void ConvertUnicodeToAnsi(const wchar_t *szUnicode, char *szAnsi, ULONG nLength);
