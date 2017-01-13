// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "..\StdAfx.h"
#include "..\BatchEncoder.h"
#include "MyStatic.h"

CMyStatic::CMyStatic()
{
    bIsBold = true;
}

CMyStatic::~CMyStatic()
{
    m_BoldFont.DeleteObject();
    m_StdFont.DeleteObject();
}

void CMyStatic::PreSubclassWindow()
{
    CFont* pFont = GetFont();
    if (!pFont)
    {
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        if (hFont == NULL)
            hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
        if (hFont)
            pFont = CFont::FromHandle(hFont);
    }

    ASSERT(pFont->GetSafeHandle());

    LOGFONT lf;

    pFont->GetLogFont(&lf);
    m_StdFont.CreateFontIndirect(&lf);

    lf.lfWeight = FW_BOLD;
    m_BoldFont.CreateFontIndirect(&lf);

    SetBold(bIsBold);

    CStatic::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
END_MESSAGE_MAP()

void CMyStatic::SetBold(bool bBold)
{
    if (::IsWindow(GetSafeHwnd()))
    {
        if (bBold == true)
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

bool CMyStatic::GetBold()
{
    return bIsBold;
}
