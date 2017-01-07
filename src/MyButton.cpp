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
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMyButton::CMyButton()
{
    bIsBold = false;
}

CMyButton::~CMyButton()
{
    m_BoldFont.DeleteObject();
    m_StdFont.DeleteObject();
}

void CMyButton::PreSubclassWindow() 
{
    CFont* pFont = GetFont();
    if(!pFont)
    {
        HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
        if(hFont == NULL)
            hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
        if(hFont)
            pFont = CFont::FromHandle(hFont);
    }

    ASSERT(pFont->GetSafeHandle());

    LOGFONT lf;

    pFont->GetLogFont(&lf);
    m_StdFont.CreateFontIndirect(&lf);

    lf.lfWeight = FW_BOLD;
    m_BoldFont.CreateFontIndirect(&lf);

    SetBold(bIsBold);

    CButton::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
END_MESSAGE_MAP()

void CMyButton::SetBold(bool bBold)
{
    if(::IsWindow(GetSafeHwnd()))
    {
        if(bBold == true)
        {
            SetFont(&m_BoldFont);
            bIsBold = true;
        }
        else
        {
            SetFont(&m_StdFont);
            bIsBold = false;
        }
        Invalidate(); 
    }
}

bool CMyButton::GetBold()
{
    return bIsBold;
}
