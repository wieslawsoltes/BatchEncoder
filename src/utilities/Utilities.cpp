// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "Utilities.h"

DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}

int GetLogicalProcessorInformation(LogicalProcessorInformation* info)
{
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;

    glpi = (LPFN_GLPI)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation");
    if (NULL == glpi)
    {
#ifdef _DEBUG
        _tprintf(_T("\nGetLogicalProcessorInformation is not supported.\n"));
#endif
        return (1);
    }

    while (!done)
    {
        DWORD rc = glpi(buffer, &returnLength);
        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
                if (NULL == buffer)
                {
#ifdef _DEBUG
                    _tprintf(_T("\nError: Allocation failure\n"));
#endif
                    return (2);
                }
            }
            else
            {
#ifdef _DEBUG
                _tprintf(_T("\nError %d\n"), GetLastError());
#endif
                return (3);
            }
        }
        else
        {
            done = TRUE;
        }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        switch (ptr->Relationship)
        {
        case RelationNumaNode:
            // Non-NUMA systems report a single record of this type.
            info->numaNodeCount++;
            break;
        case RelationProcessorCore:
            info->processorCoreCount++;
            // A hyperthreaded core supplies more than one logical processor.
            info->logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;
        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
            Cache = &ptr->Cache;
            if (Cache->Level == 1)
            {
                info->processorL1CacheCount++;
            }
            else if (Cache->Level == 2)
            {
                info->processorL2CacheCount++;
            }
            else if (Cache->Level == 3)
            {
                info->processorL3CacheCount++;
            }
            break;
        case RelationProcessorPackage:
            // Logical processors share a physical package.
            info->processorPackageCount++;
            break;
        default:
#ifdef _DEBUG
            _tprintf(_T("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
#endif
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }

#ifdef _DEBUG
    _tprintf(_T("\nGetLogicalProcessorInformation results:\n"));
    _tprintf(_T("Number of NUMA nodes: %d\n"), info->numaNodeCount);
    _tprintf(_T("Number of physical processor packages: %d\n"), info->processorPackageCount);
    _tprintf(_T("Number of processor cores: %d\n"), info->processorCoreCount);
    _tprintf(_T("Number of logical processors: %d\n"), info->logicalProcessorCount);
    _tprintf(_T("Number of processor L1/L2/L3 caches: %d/%d/%d\n"),
        info->processorL1CacheCount,
        info->processorL2CacheCount,
        info->processorL3CacheCount);
#endif

    free(buffer);
    return (0);
}

void DoTheShutdown()
{
    // Windows XP or later
    if (true)
    {
        HANDLE m_hToken;

        if (::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &m_hToken))
        {
            LUID m_Luid;
            if (::LookupPrivilegeValue(NULL, _T("SeShutdownPrivilege"), &m_Luid))
            {
                TOKEN_PRIVILEGES tp;

                tp.PrivilegeCount = 1;
                tp.Privileges[0].Luid = m_Luid;
                tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

                ::AdjustTokenPrivileges(m_hToken, FALSE, &tp, 0, NULL, NULL);
            }

            ::CloseHandle(m_hToken);
        }
    }

    ::ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF | EWX_FORCEIFHUNG, 0);

    ::PostQuitMessage(0);
}

void LaunchAndWait(LPCTSTR file, LPCTSTR params, BOOL bWait)
{
    SHELLEXECUTEINFO sei;
    ::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));

    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    // sei.lpVerb is uninitialized, so that default action will be taken
    sei.nShow = SW_SHOWNORMAL;
    sei.lpFile = file;
    sei.lpParameters = params;

    // check the return value
    ::ShellExecuteEx(&sei);

    // wait till the child terminates
    if (bWait == TRUE)
        ::WaitForSingleObject(sei.hProcess, INFINITE);

    ::CloseHandle(sei.hProcess);
}

void SetComboBoxHeight(HWND hDlg, int nComboBoxID)
{
    // limit the size default to: 8 to 30, system default is 30
    const int nSizeLimit = 15;

    HWND hComboxBox = ::GetDlgItem(hDlg, nComboBoxID);

    // NOTE: on WinXP standard method does not work so we are using CB_SETMINVISIBLE message
    // Windows XP or later
    if (true)
    {
        // well we using right now 5.0 NT define, but we need this for XP
#if !defined(CBM_FIRST) | !defined(CB_SETMINVISIBLE)
#define CBM_FIRST 0x1700
#define	CB_SETMINVISIBLE (CBM_FIRST + 1)
#endif

        ::SendMessage(hComboxBox, CB_SETMINVISIBLE, (WPARAM)nSizeLimit, 0);
        return;
    }

    int nCount = (int) ::SendDlgItemMessage(hDlg, nComboBoxID, CB_GETCOUNT, 0, 0);
    int nHeight = (int) ::SendDlgItemMessage(hDlg, nComboBoxID, CB_GETITEMHEIGHT, 0, 0);
    RECT rcCB;
    int nCY = 0;

    ::GetWindowRect(hComboxBox, &rcCB);
    if (nCount > nSizeLimit)
        nCY = nHeight * nSizeLimit;
    else
        nCY = 2 * nHeight * nCount;

    ::SetWindowPos(hComboxBox,
        NULL,
        0,
        0,
        rcCB.right - rcCB.left,
        nCY,
        SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
}

UINT MyGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
    LPTSTR lpszTemp = ::PathFindFileName(lpszPathName);
    if (lpszTitle == NULL)
        return lstrlen(lpszTemp) + 1;

    lstrcpyn(lpszTitle, lpszTemp, nMax);
    return(0);
}

CString GetFileName(CString szFilePath)
{
    CString strResult;
    MyGetFileName(szFilePath, strResult.GetBuffer(_MAX_FNAME), _MAX_FNAME);
    strResult.ReleaseBuffer();
    return strResult;
}

CString GetFileExtension(CString szFilePath)
{
    CString szExt = ::PathFindExtension(szFilePath);
    szExt.Remove('.');
    return szExt;
}

ULONGLONG GetFileSize64(HANDLE hFile)
{
    ULARGE_INTEGER liSize;

    liSize.LowPart = ::GetFileSize(hFile, &liSize.HighPart);
    if (liSize.LowPart == (DWORD)-1)
    {
        if (::GetLastError() != NO_ERROR)
            return(0);
    }

    return liSize.QuadPart;
}

CString GetExeFilePath()
{
    TCHAR szExeFilePath[MAX_PATH + 1] = _T("");
    DWORD dwRet = ::GetModuleFileName(::GetModuleHandle(NULL), szExeFilePath, MAX_PATH);
    if (dwRet > 0)
    {
        CString szTempBuff1;
        CString szTempBuff2;

        szTempBuff1.Format(_T("%s"), szExeFilePath);
        szTempBuff2 = ::GetFileName(szTempBuff1);
        szTempBuff1.TrimRight(szTempBuff2);

        return szTempBuff1;
    }
    return NULL;
}

void GetFullPathName(CString &szFilePath)
{
    TCHAR szFullPath[MAX_PATH + 2] = _T("");
    LPTSTR pszFilePos = NULL;
    ::GetFullPathName(szFilePath, MAX_PATH + 1, szFullPath, &pszFilePos);
    szFilePath = szFullPath;
}

void UpdatePath()
{
    ::SetCurrentDirectory(::GetExeFilePath());
}

BOOL MakeFullPath(CString szPath)
{
    if (szPath[szPath.GetLength() - 1] != '\\')
        szPath = szPath + _T("\\");

    CString szTmpDir = szPath.Left(2);
    _tchdir(szTmpDir);

    int nStart = 3;
    int nEnd = 0;
    while (TRUE)
    {
        nEnd = szPath.Find('\\', nStart);
        if (nEnd == -1)
            return TRUE;

        CString szNextDir = szPath.Mid(nStart, nEnd - nStart);
        CString szCurDir = szTmpDir + _T("\\") + szNextDir;
        if (_tchdir(szCurDir) != 0)
        {
            _tchdir(szTmpDir);
            if (_tmkdir(szNextDir) != 0)
                return FALSE;
        }

        szTmpDir += _T("\\") + szNextDir;
        nStart = nEnd + 1;
    }
    return FALSE;
}

CString FormatTime(double fTime, int nFormat)
{
    CString szTime = _T("");
    DWORD dwTime[5] = { 0, 0, 0, 0, 0 }; // DD HH MM SS MS

    dwTime[0] = (DWORD)fTime / (24 * 60 * 60); // DD -> [days]
    dwTime[1] = ((DWORD)fTime - (dwTime[0] * (24 * 60 * 60))) / (60 * 60); // HH -> [h]
    dwTime[2] = ((DWORD)fTime - ((dwTime[0] * (24 * 60 * 60)) + (dwTime[1] * (60 * 60)))) / 60; // MM -> [m]
    dwTime[3] = ((DWORD)fTime - ((dwTime[0] * (24 * 60 * 60)) + (dwTime[1] * (60 * 60)) + (dwTime[2] * 60))); // SS -> [s]
    dwTime[4] = (DWORD)(((double)fTime - (DWORD)fTime) * (double) 1000.1); // MS -> [ms]

    if (nFormat == 0)
    {
        // display simple time
        szTime.Format(_T("%0.3f"), fTime);
    }
    else if (nFormat == 1)
    {
        // exclude days if not used
        if (dwTime[0] != 0)
        {
            szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld.%03ld"),
                dwTime[0], dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
        }
        else
        {
            szTime.Format(_T("%02ld:%02ld:%02ld.%03ld"),
                dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
        }
    }
    else if (nFormat == 2)
    {
        // exclude unused values from time display
        if (dwTime[0] != 0)
        {
            szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld.%03ld"),
                dwTime[0], dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
        }
        else if ((dwTime[0] == 0) && (dwTime[1] != 0))
        {
            szTime.Format(_T("%02ld:%02ld:%02ld.%03ld"),
                dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
        }
        else if ((dwTime[0] == 0) && (dwTime[1] == 0) && (dwTime[2] != 0))
        {
            szTime.Format(_T("%02ld:%02ld.%03ld"),
                dwTime[2], dwTime[3], dwTime[4]);
        }
        else if ((dwTime[0] == 0) && (dwTime[1] == 0) && (dwTime[2] == 0) && (dwTime[3] != 0))
        {
            szTime.Format(_T("%02ld.%03ld"),
                dwTime[3], dwTime[4]);
        }
        else
        {
            szTime.Format(_T("%03ld"),
                dwTime[4]);
        }
    }
    else if (nFormat == 3)
    {
        // exclude days if not used and don't show milliseconds
        if (dwTime[0] != 0)
        {
            szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld"),
                dwTime[0], dwTime[1], dwTime[2], dwTime[3]);
        }
        else
        {
            szTime.Format(_T("%02ld:%02ld:%02ld"),
                dwTime[1], dwTime[2], dwTime[3]);
        }
    }

    return szTime;
}
