// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "Utilities.h"
#include <Rpc.h>

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

void ShutdownWindows()
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
    ::ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF | EWX_FORCEIFHUNG, 0);
    ::PostQuitMessage(0);
}

void LaunchAndWait(LPCTSTR file, LPCTSTR params, BOOL bWait)
{
    SHELLEXECUTEINFO sei;
    ::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    // leave sei.lpVerb uninitialized for default action
    sei.nShow = SW_SHOWNORMAL;
    sei.lpFile = file;
    sei.lpParameters = params;
    ::ShellExecuteEx(&sei);
    if (bWait == TRUE)
        ::WaitForSingleObject(sei.hProcess, INFINITE);
    ::CloseHandle(sei.hProcess);
}

void SetComboBoxHeight(HWND hDlg, int nComboBoxID, int nSizeLimit)
{
    HWND hComboxBox = ::GetDlgItem(hDlg, nComboBoxID);
    if (hComboxBox != NULL)
        ::SendMessage(hComboxBox, CB_SETMINVISIBLE, (WPARAM)nSizeLimit, 0);
}

UINT GetFileNameInternal(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
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
    ::GetFileNameInternal(szFilePath, strResult.GetBuffer(_MAX_FNAME), _MAX_FNAME);
    strResult.ReleaseBuffer();
    return strResult;
}

CString GetFilePath(CString szFilePath)
{
    CString strResult = szFilePath;
    CString szFileName = ::GetFileName(szFilePath);
    int nPathLength = szFilePath.GetLength() - szFileName.GetLength();
    strResult.Truncate(nPathLength);
    return strResult;
}

CString GetFileExtension(CString szFilePath)
{
    CString szExt = ::PathFindExtension(szFilePath);
    szExt.Remove('.');
    return szExt;
}

CString GetOnlyFileName(CString szFilePath)
{
    CString szFileName = ::GetFileName(szFilePath);
    szFileName.TrimRight(::GetFileExtension(szFilePath));
    szFileName.TrimRight(_T("."));
    return szFileName;
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

ULONGLONG GetFileSize64(CString szFileName)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    ULARGE_INTEGER ulSize;
    ULONGLONG nFileSize;

    hFind = ::FindFirstFile(szFileName, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
        return 0;

    ::FindClose(hFind);

    ulSize.HighPart = FindFileData.nFileSizeHigh;
    ulSize.LowPart = FindFileData.nFileSizeLow;
    nFileSize = ulSize.QuadPart;

    return nFileSize;
}

__int64 GetFileSizeInt64(FILE *fp)
{
    __int64 nCurPos, nSize;
    nCurPos = _ftelli64(fp);
    _fseeki64(fp, 0, SEEK_END);
    nSize = _ftelli64(fp);
    _fseeki64(fp, nCurPos, SEEK_SET);
    return nSize;
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

CString GetSettingsFilePath(CString szFileName, CString szConfigDirectory)
{
    TCHAR szPath[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL,
        CSIDL_APPDATA | CSIDL_FLAG_CREATE,
        NULL,
        0,
        szPath)))
    {
        PathAppend(szPath, szConfigDirectory);
        PathAppend(szPath, szFileName);
        return szPath;
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

BOOL DirectoryExists(LPCTSTR szPath)
{
    if (_taccess_s(szPath, 0) == 0)
    {
        struct _stat status;
        _tstat(szPath, &status);
        return (status.st_mode & S_IFDIR) != 0;
    }
    return FALSE;
}

bool MakeFullPath(CString szPath)
{
    if (szPath[szPath.GetLength() - 1] != '\\')
        szPath = szPath + _T("\\");

    CString szTmpDir = szPath.Left(2);
    _tchdir(szTmpDir);

    int nStart = 3;
    int nEnd = 0;
    while (true)
    {
        nEnd = szPath.Find('\\', nStart);
        if (nEnd == -1)
            return true;

        CString szNextDir = szPath.Mid(nStart, nEnd - nStart);
        CString szCurDir = szTmpDir + _T("\\") + szNextDir;
        if (_tchdir(szCurDir) != 0)
        {
            _tchdir(szTmpDir);
            if (_tmkdir(szNextDir) != 0)
                return false;
        }

        szTmpDir += _T("\\") + szNextDir;
        nStart = nEnd + 1;
    }
    return false;
}

bool FileExists(CString szPath)
{
    WIN32_FIND_DATA w32FileData;
    ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = ::FindFirstFile(szPath, &w32FileData);
    bool bInvalidHandle = hFind == INVALID_HANDLE_VALUE;
    ::FindClose(hFind);
    return bInvalidHandle == false;
}

CString GenerateUuidString()
{
    CString strKey;
    UUID uuid;
    if (UuidCreate(&uuid) == RPC_S_OK)
    {
        RPC_WSTR szUuid;
        if (UuidToString(&uuid, &szUuid) == RPC_S_OK)
        {
            strKey = (LPTSTR)szUuid;
            strKey.MakeUpper();
            RpcStringFree(&szUuid);
        }
    }
    return strKey;
}

CString ReplaceNoCase(LPCTSTR instr, LPCTSTR oldstr, LPCTSTR newstr)
{
    CString output(instr);
    CString inputLower(instr);
    CString oldLower(oldstr);
    inputLower.MakeLower();
    oldLower.MakeLower();
    int pos = 0;
    for (;;)
    {
        pos = inputLower.Find(oldLower, pos);
        if (pos == -1)
            break;
        inputLower.Delete(pos, lstrlen(oldstr));
        inputLower.Insert(pos, newstr);
        output.Delete(pos, lstrlen(oldstr));
        output.Insert(pos, newstr);
        pos += lstrlen(newstr);
    }
    return output;
}

int FindNoCase(LPCTSTR pszString, LPCTSTR pszSearch)
{
    int lenString = lstrlen(pszString);
    int lenSearch = lstrlen(pszSearch);
    if (lenSearch == 0 || lenSearch > lenString)
        return -1;
    for (int i = 0; i < lenString - lenSearch + 1; ++i)
    {
        if (_tcsncicmp(&pszString[i], pszSearch, lenSearch) == 0)
            return i;
    }
    return -1;
}

void ConvertAnsiToUnicode(const char *szAnsi, wchar_t *szUnicode, ULONG nLength)
{
    // use always + 1 to null-terminate string
    _mbstowcsz(szUnicode, szAnsi, nLength + 1);
}

void ConvertUnicodeToAnsi(const wchar_t *szUnicode, char *szAnsi, ULONG nLength)
{
    // use always + 1 to null-terminate string
    _wcstombsz(szAnsi, szUnicode, nLength + 1);
}
