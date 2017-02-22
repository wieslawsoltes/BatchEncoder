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

CString GetOnlyFileName(CString szFilePath);

CString GetFileExtension(CString szFilePath);

CString GetFileExtension(CString szFilePath);

ULONGLONG GetFileSize64(HANDLE hFile);

CString GetExeFilePath();

void GetFullPathName(CString &szFilePath);

BOOL DirectoryExists(LPCTSTR szPath);

BOOL MakeFullPath(CString szPath);

bool FileExists(CString szPath);

CString GenerateUuidString();
