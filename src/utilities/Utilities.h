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

void DoTheShutdown();

void LaunchAndWait(LPCTSTR file, LPCTSTR params, BOOL bWait);

void SetComboBoxHeight(HWND hDlg, int nComboBoxID);

CString GetFileName(CString szFilePath);

CString GetFileExtension(CString szFilePath);

ULONGLONG GetFileSize64(HANDLE hFile);

CString GetExeFilePath();

void GetFullPathName(CString &szFilePath);

void UpdatePath();

BOOL MakeFullPath(CString szPath);

CString FormatTime(double fTime, int nFormat);
