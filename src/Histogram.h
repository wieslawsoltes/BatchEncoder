// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "LameHistogram.h"

class CHistogram : public CStatic
{
    DECLARE_DYNAMIC(CHistogram)
public:
    CHistogram();
    virtual ~CHistogram();
protected:
    DECLARE_MESSAGE_MAP()
private:
    CDC *m_MemDC;
public:
    COLORREF crLR;
    COLORREF crMS;
    COLORREF crBorder;
    COLORREF crBack;
private:
    CFont *m_pFont;
private:
    void DrawColumn(int nColumns,
        int nPos,
        int nLR,
        int nMS,
        bool bText,
        CString szText);
private:
    void Paint(void);
public:
    BOOL Create(const RECT &rect, CWnd *pParentWnd, UINT uID, bool bVisible);
public:
    BOOL Init(bool bBoldFont = false);
    void Clean(void);
public:
    void Erase(bool bPaint);
    void Draw(PLAME_ENC_HISTOGRAM plehData);
public:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};
