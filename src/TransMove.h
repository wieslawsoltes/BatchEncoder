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

// Description:
// When user moves window using title-bar we are making window transparent.
// Using SetLayeredWindowAttributes and _WIN32_WINNT >= 0x0500 and user32.dll.

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif // WS_EX_LAYERED

#ifndef LWA_ALPHA
#define LWA_ALPHA 0x00000002
#endif // LWA_ALPHA

typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes) (HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
const PTCHAR pszLibrary = _T("user32.dll");
const LPCSTR pszProcName = "SetLayeredWindowAttributes";

class CTransMove
{
public:
    CTransMove()
    {
        this->Reset();
    };

    virtual ~CTransMove()
    {

    };
private:
    HWND hMyWnd;
    lpfnSetLayeredWindowAttributes pSetLayeredWindowAttributes;
    bool bHaveProc;
    bool bHaveDown;
    BYTE bTransparent;
    BYTE bOpaque;
private:
    void Reset()
    {
        hMyWnd = NULL;
        pSetLayeredWindowAttributes = NULL;
        bHaveProc = false;
        bHaveDown = false;
        bTransparent = 164;
        bOpaque = 255;
    }
public:
    bool Init(HWND hWnd)
    {
        if(!hWnd)
            return false;
        
        if(bHaveProc)
            return true;
		
        if(!::IsWindowsXPOrGreater())
            return false;

        this->Reset();

        HMODULE hLib = NULL;
        hLib = ::LoadLibrary(pszLibrary);
        if(!hLib)
            return false;

        pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes) 
            ::GetProcAddress(hLib, pszProcName); 
        if(!pSetLayeredWindowAttributes)
            return false;

        hMyWnd = hWnd;
        bHaveProc = true;
        bHaveDown = false;

        ::SetWindowLong(hMyWnd, 
            GWL_EXSTYLE, 
            ::GetWindowLong(hMyWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        pSetLayeredWindowAttributes(hMyWnd, 
            RGB(0x00, 0x00, 0x00), 
            bOpaque, 
            LWA_ALPHA);

        return true;
    };

    bool Clean()
    {
        if(!hMyWnd)
            return false;
        
        if(!bHaveProc)
            return false;

        if(!pSetLayeredWindowAttributes)
            return false;

        pSetLayeredWindowAttributes(hMyWnd, 
            RGB(0x00, 0x00, 0x00), 
            255, 
            LWA_ALPHA);

        ::SetWindowLong(hMyWnd, 
            GWL_EXSTYLE, 
            ::GetWindowLong(hMyWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

        this->Reset();
        return true;
    }

    // WM_NCLBUTTONDOWN, afx_msg void CDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
    bool Down(UINT nHitTest, CPoint point)
    {
        if(!hMyWnd)
            return false;
        
        if(!bHaveProc)
            return false;

        if(!pSetLayeredWindowAttributes)
            return false;

        if((nHitTest == HTCAPTION) && (bHaveDown == false))
        {
            pSetLayeredWindowAttributes(hMyWnd, 
                RGB(0x00, 0x00, 0x00), 
                bTransparent, 
                LWA_ALPHA);

            bHaveDown = true;
            return true;
        }

        return false;
    }

    // WM_NCHITTEST, afx_msg UINT CDialog::OnNcHitTest(CPoint point);
    bool Up(CPoint point)
    {
        if(!hMyWnd)
            return false;

        if(!bHaveProc)
            return false;

        if(!pSetLayeredWindowAttributes)
            return false;

        if(bHaveDown == true)
        {
            pSetLayeredWindowAttributes(hMyWnd, 
                RGB(0x00, 0x00, 0x00), 
                bOpaque, 
                LWA_ALPHA);

            bHaveDown = false;
            return true;
        }

        return false;
    };
};
