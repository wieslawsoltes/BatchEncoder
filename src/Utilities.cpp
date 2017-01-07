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

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utilities.h"
#include "UnicodeUtf8.h"
#include "Utf8String.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
void LastError(LPTSTR lpszFunction)
{  
	LPVOID lpMsgBuf;
	TCHAR szBuf[1024]; 
    DWORD dwError = ::GetLastError(); 
    if(dwError != ERROR_SUCCESS)
    {
        ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            dwError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0,
            NULL);

        _stprintf(szBuf, _T("Function: %s\nError code: %u\nError message: %s"), 
            lpszFunction, 
            dwError, 
            (PWCHAR) lpMsgBuf);

        ::MessageBox(NULL, szBuf, _T("ERROR"), MB_OK | MB_ICONERROR); 
        ::LocalFree(lpMsgBuf);
    }
}
#endif

bool IsWindowsVer(WORD wMajor, WORD wMinor, bool bStrict)
{
    DWORD dwVersion = ::GetVersion();
    DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
    if(bStrict == false)
    {
        if((dwWindowsMajorVersion >= 5) && (dwWindowsMinorVersion >= 1))
            return true;
        else 
            return false;
    }
    else
    {
        if((dwWindowsMajorVersion == 5) && (dwWindowsMinorVersion == 1))
            return true;
        else 
            return false;
    }
}

void DoTheShutdown()
{
    if(!(::GetVersion() & 0x80000000)) 
    {
        HANDLE m_hToken;

        if(::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &m_hToken)) 
        {
            LUID m_Luid;
            if(::LookupPrivilegeValue(NULL, _T("SeShutdownPrivilege"), &m_Luid)) 
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
   if(bWait == TRUE)
        ::WaitForSingleObject(sei.hProcess, INFINITE);

   ::CloseHandle(sei.hProcess);
}

unsigned int StringLenght(register TCHAR *str)
{
    register TCHAR *ptr = str;
    
    while(*ptr != '\0')
        ptr++;
    
    return((unsigned int) (ptr - str));
}

int StringSearch(TCHAR *str, int str_len, TCHAR *key, int key_len)
{
    register int i = 0; 
    register int j = 0;
    register int find = 0;
    register int count = 0;
    register int size = (str_len - key_len + 1);
    
    if(size < 1)
        return 0;
    
    count = 0;
    for(i = 0; i < size; i++)
    {
        find = 0;
        for(j = 0; j < key_len; j++)
        {
            if(*(str + i + j) == *(key + j))
                find++;
            else 
                break;
        }
        if(find == key_len)
            count++;
    }
    return count;
}

bool CheckCmdLineOpt(TCHAR *str, register TCHAR *key)
{
    register int key_size = (int) StringLenght(key);
    register int ret = 0;
    ret = StringSearch(str, (int) StringLenght(str), key, key_size); 
    if(ret > 0)
        return true;
    else 
        return false;
}

void SetComboBoxHeight(HWND hDlg, int nComboBoxID)
{
    // limit the size default to: 8 to 30, system default is 30
    const int nSizeLimit = 15;

    HWND hComboxBox = ::GetDlgItem(hDlg, nComboBoxID);

    // NOTE: on WinXP standard method does not work 
    //       but we are using CB_SETMINVISIBLE message
    OSVERSIONINFO VersionInfo;
    VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    BOOL bRetVer = ::GetVersionEx(&VersionInfo);
    if(bRetVer == TRUE)
    {
        // version >= 5.1 (Windows XP or later windows)
        if(VersionInfo.dwMajorVersion >= 5 && VersionInfo.dwMinorVersion >= 1)
        {
            // well we using right now 5.0 NT define, but we need this for XP
            #if !defined(CBM_FIRST) | !defined(CB_SETMINVISIBLE)
                #define CBM_FIRST 0x1700
                #define	CB_SETMINVISIBLE (CBM_FIRST + 1)
            #endif

            ::SendMessage(hComboxBox, CB_SETMINVISIBLE, (WPARAM) nSizeLimit, 0);
            return;
        }
    }

    int nCount = (int) ::SendDlgItemMessage(hDlg, nComboBoxID, CB_GETCOUNT, 0, 0);
    int nHeight = (int) ::SendDlgItemMessage(hDlg, nComboBoxID, CB_GETITEMHEIGHT, 0, 0);
    RECT rcCB;
    int nCY = 0;

    ::GetWindowRect(hComboxBox, &rcCB);

    if(nCount > nSizeLimit)
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
    if(lpszTitle == NULL)
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
    if(dwRet > 0)
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
    // change current path to program base dir
    ::SetCurrentDirectory(::GetExeFilePath());
}

bool CopyOneFile(CString szInFile, CString szOutFile, fncCopyCallback lpCallback)
{
    try
    {
        LPBYTE lpBuffer = NULL;
        DWORD dwBufferSize = 4 * 1024;
        ULONGLONG nFileSize = 0;
        ULONGLONG nTotalBytes = 0;
        UINT nReadBytes = 0;
        int nProgress = 0;

        CFile fpIn, fpOut;

        if(fpIn.Open(szInFile, CFile::modeRead) == FALSE)
        {
            return false;
        }

        if(fpOut.Open(szOutFile, CFile::modeCreate | CFile::modeReadWrite) == FALSE)
        {
            fpIn.Close();
            return false;
        }

        nFileSize = fpIn.GetLength();
        if(nFileSize == 0)
        {
            fpIn.Close();
            fpOut.Close();
            return true;
        }

        lpBuffer = (LPBYTE) malloc(dwBufferSize);
        if(lpBuffer == NULL)
        {
            fpIn.Close();
            fpOut.Close();
            return false;
        }

        do
        {
            nReadBytes = fpIn.Read(lpBuffer, dwBufferSize);
            if(nReadBytes == 0)
                break;

            fpOut.Write(lpBuffer, nReadBytes);
            nTotalBytes += (ULONGLONG) nReadBytes;

            if(lpCallback != NULL)
            {
                nProgress = (int) ((nTotalBytes * 100) / nFileSize);
                if(lpCallback(nProgress) == true)
                    break;
            }
        }
        while(nReadBytes > 0);

        fpIn.Close();
        fpOut.Close();
        free(lpBuffer);

        if(nTotalBytes == nFileSize)
        {
            return true;
        }
        else
        {
            ::DeleteFile(szOutFile);
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
}

CString GetConfigString(const char *pszUtf8)
{
    CString szBuff;

    if(pszUtf8 == NULL)
    {
        szBuff = _T("");
        return szBuff;
    }

    if(strlen(pszUtf8) == 0)
    {
        szBuff = _T("");
        return szBuff;
    }

#ifdef _UNICODE
    // UTF-8 to UNICODE
    wchar_t *pszUnicode;
    pszUnicode = MakeUnicodeString((unsigned char *) pszUtf8);
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

void MakeSafeString(CString &szData)
{
    // remove unsafe chars from string
    szData.Remove('*');
    szData.Remove('?');
    szData.Remove(':');

    // replace unsafe chars from string
    szData.Replace('/', '-');
    szData.Replace('|', '-');
    szData.Replace('\"', '\'');
    szData.Replace('<', '(');
    szData.Replace('>', ')');
}

BOOL MakeFullPath(CString szPath)
{
    if(szPath[szPath.GetLength() - 1] != '\\')
        szPath = szPath + _T("\\");

    CString szTmpDir = szPath.Left(2);
    _tchdir(szTmpDir);

    int nStart = 3;
    int nEnd = 0;
    while(TRUE)
    {
        nEnd = szPath.Find('\\', nStart);
        if(nEnd == -1)
            return TRUE;

        CString szNextDir = szPath.Mid(nStart, nEnd - nStart);
        CString szCurDir = szTmpDir + _T("\\") + szNextDir;
        if(_tchdir(szCurDir) != 0)
        {
            _tchdir(szTmpDir);
            if(_tmkdir(szNextDir) != 0)
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

    dwTime[0] = (DWORD) fTime / (24 * 60 * 60); // DD -> [days]
    dwTime[1] = ((DWORD) fTime - (dwTime[0] * (24 * 60 * 60))) / (60 * 60); // HH -> [h]
    dwTime[2] = ((DWORD) fTime - ((dwTime[0] * (24 * 60 * 60)) + (dwTime[1] * (60 * 60)))) / 60; // MM -> [m]
    dwTime[3] = ((DWORD) fTime - ((dwTime[0] * (24 * 60 * 60)) + (dwTime[1] * (60 * 60)) + (dwTime[2] * 60))); // SS -> [s]
    dwTime[4] = (DWORD) (((double) fTime - (DWORD) fTime) * (double) 1000.1); // MS -> [ms]

    if(nFormat == 0)
    {
        // display simple time
        szTime.Format(_T("%0.3f"), fTime);
    }
    else if(nFormat == 1)
    {
        // exclude only days if not used
        if(dwTime[0] != 0)
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
    else if(nFormat == 2)
    {
        // exclude unused values from time display
        if(dwTime[0] != 0)
        {
            szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld.%03ld"),
                dwTime[0], dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
        }
        else if((dwTime[0] == 0) && (dwTime[1] != 0))
        {
            szTime.Format(_T("%02ld:%02ld:%02ld.%03ld"),
                dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
        }
        else if((dwTime[0] == 0) && (dwTime[1] == 0) && (dwTime[2] != 0))
        {
            szTime.Format(_T("%02ld:%02ld.%03ld"),
                dwTime[2], dwTime[3], dwTime[4]);
        }
        else if((dwTime[0] == 0) && (dwTime[1] == 0) && (dwTime[2] == 0) && (dwTime[3] != 0))
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
    else if(nFormat == 3)
    {
        // exclude days if not used and doąt show milliseconds
        if(dwTime[0] != 0)
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

void SetBrowsePath(CFileDialog &fileDlg, CString szPath)
{
    fileDlg.m_ofn.lpstrInitialDir = szPath;
}

CString GetBrowsePath(CFileDialog &fileDlg)
{
    CString szBuff = fileDlg.GetFileName();
    CString szPath = fileDlg.GetPathName();

    szPath.TrimRight(szBuff);

    return szPath;
}

void GradientFill(CDC *m_pMemDC, CRect &rc, COLORREF cr01, COLORREF cr02, bool bVertical)
{
    TRIVERTEX vert[2];
    GRADIENT_RECT gRect;

    vert [0] .x      = rc.TopLeft().x;
    vert [0] .y      = rc.TopLeft().y;
    vert [0] .Red    = GetRValue(cr01) << 8;
    vert [0] .Green  = GetGValue(cr01) << 8;
    vert [0] .Blue   = GetBValue(cr01) << 8;
    vert [0] .Alpha  = 0x0000;

    vert [1] .x      = rc.BottomRight().x;
    vert [1] .y      = rc.BottomRight().y; 
    vert [1] .Red    = GetRValue(cr02) << 8;
    vert [1] .Green  = GetGValue(cr02) << 8;
    vert [1] .Blue   = GetBValue(cr02) << 8;
    vert [1] .Alpha  = 0x0000;

    gRect.UpperLeft  = 0;
    gRect.LowerRight = 1;

    if(bVertical == false)
        m_pMemDC->GradientFill(vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
    else
        m_pMemDC->GradientFill(vert, 2, &gRect, 1, GRADIENT_FILL_RECT_V);
}

int GetFormatId(CString szBuff)
{
    int nId = -1;
    for(int i = 0; i < NUM_FORMAT_NAMES; i++)
    {
        if(szBuff.Compare(g_szFormatNames[i]) == 0)
        {
            nId = i;
            break;
        }
    }
    return nId;
}

void GetFullPathName(CString &szFilePath)
{
    // get full file path
    TCHAR szFullPath[MAX_PATH + 2] = _T("");
    LPTSTR pszFilePos = NULL;

    ::GetFullPathName(szFilePath, MAX_PATH + 1, szFullPath, &pszFilePos);
    szFilePath = szFullPath;
}
