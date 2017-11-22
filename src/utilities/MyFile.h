// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CMyFile : private CFile
{
private:
    static const UINT nOpenFlagsRead = modeRead | typeBinary;
    static const UINT nOpenFlagsWrite = modeCreate | modeRead | modeWrite | typeBinary;
private:
    CString _szFileName;
    bool _bOpen;
    bool _bWrite;
private:
    static const size_t nSizeOf_TCHAR = sizeof(TCHAR);
    static const size_t nSizeOf_wchar_t = sizeof(wchar_t);
    static const size_t nSizeOf_char = sizeof(char);
private:
    // Ansi buffer
    char BufferA;
    // Unicode buffer
    wchar_t BufferU;
private:
    // read & write modes
    // 0 -> src = Unicode, dst = Unicode
    // 1 -> src = Ansi, dst = Unicode
    // 2 -> src = Unicode, dst = Ansi
    // 3 -> src = Ansi, dst = Ansi
    int _nMode;
public:
    CMyFile();
    CMyFile(CString szFileName, bool bWrite = false);
    virtual ~CMyFile();
public:
#ifdef _UNICODE
    bool FSetMode(int nMode = 0);
#else
    bool FSetMode(int nMode = 3);
#endif
    int FMode();
    ULONGLONG FSize();
    ULONGLONG FPos();
    bool FOpen(CString szFileName = _T(""), bool bWrite = false);
    bool FClose();
    bool FRead(TCHAR &Buffer);
    bool FWrite(TCHAR &Buffer);
    UINT FReadString(TCHAR *Buffer, UINT nLength);
    bool FWriteString(TCHAR *Buffer, UINT nLength);
};
