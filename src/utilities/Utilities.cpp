// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "Utilities.h"
#include "UnicodeUtf8.h"
#include "Utf8String.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DoTheShutdown()
{
    if (::IsWindowsXPOrGreater())
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
    // sei.lpVerb is uninitialised, so that default action will be taken
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

    // NOTE: on WinXP standard method does not work 
    //       but we are using CB_SETMINVISIBLE message
    // version >= 5.1 (Windows XP or later windows)
    if (::IsWindowsXPOrGreater())
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

static UINT MyGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
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

void UpdatePath()
{
    ::SetCurrentDirectory(::GetExeFilePath());
}

CString GetConfigString(const char *pszUtf8)
{
    CString szBuff;

    if (pszUtf8 == NULL)
    {
        szBuff = _T("");
        return szBuff;
    }

    if (strlen(pszUtf8) == 0)
    {
        szBuff = _T("");
        return szBuff;
    }

#ifdef _UNICODE
    // UTF-8 to UNICODE
    wchar_t *pszUnicode;
    pszUnicode = MakeUnicodeString((unsigned char *)pszUtf8);
    szBuff = pszUnicode;
    free(pszUnicode);
#else
    // UTF-8 to ANSI
    char *pszAnsi;
    Utf8Decode(pszUtf8, &pszAnsi);
    szBuff = pszAnsi;
    free(pszAnsi);
#endif

    return szBuff;
}

int stoi(CString szData)
{
    return _tstoi(szData);
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
        // exclude only days if not used
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
        // exclude days if not used and doąt show milliseconds
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

void GetFullPathName(CString &szFilePath)
{
    TCHAR szFullPath[MAX_PATH + 2] = _T("");
    LPTSTR pszFilePos = NULL;
    ::GetFullPathName(szFilePath, MAX_PATH + 1, szFullPath, &pszFilePos);
    szFilePath = szFullPath;
}
