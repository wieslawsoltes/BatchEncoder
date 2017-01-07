//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
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

typedef struct TREAD_DATA
{
    CString szFileName;
    HANDLE hPipe;
    volatile bool bError;
    CBatchEncoderDlg *pDlg;
    volatile bool bFinished;
} READ_DATA, *PREAD_DATA;

ULONGLONG GetFileSize64(HANDLE hFile);
DWORD WINAPI ReadThread(LPVOID lpParam);
DWORD WINAPI WriteThread(LPVOID lpParam);
bool ConvertFile(CBatchEncoderDlg *pDlg, 
                 CString szInputFile, 
                 CString szOutputFile,
                 TCHAR *szCommandLine, 
                 int nIndex, 
                 bool bDecode, 
                 int nTool,
                 bool bUseReadPipes,
                 bool bUseWritePipes);
DWORD WINAPI WorkThread(LPVOID lpParam);
