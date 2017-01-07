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

/* $Id: ConversionStatus.h 437 2008-03-24 12:15:45Z wisodev $ */

#pragma once

class CConversionStatus : public CStatic
{
    DECLARE_DYNAMIC(CConversionStatus)
public:
    CConversionStatus();
    virtual ~CConversionStatus();
protected:
    DECLARE_MESSAGE_MAP()
private:
    CDC *m_MemDC;
public:
    COLORREF crText;
    COLORREF crTextError;
    COLORREF crProgress;
    COLORREF crBorder;
    COLORREF crBack;
private:
    CFont *m_pFontNormal;
    CFont *m_pFontBold;
private:
    void SetFontMemDC(bool bBold, COLORREF crBack, COLORREF crText);
    void DrawTextMemDC(CRect &rcText, CString &szText);
    void DrawPercentageMemDC(bool bBold, CRect &rcText, CString &szText, COLORREF crText);
private:
    void DrawStatus(int nCurrentProgress);
private:
    void Paint(void);
public:
	BOOL Create(const RECT &rect, CWnd *pParentWnd, UINT uID, bool bVisible);
public:
    BOOL Init(void);
    void Clean(void);
public:
    void Erase(bool bPaint);
    void Draw(int nCurrentProgress);
private:
    int nFile;
    int nTotalFiles;
    int nStatusDone;
    CString szInFile;
    CString szOutFile;
    CString szOutPath;
public:
    void SetCurrentInfo(int nFile, 
        int nTotalFiles,
        int nStatusDone,
        CString szInFile,
        CString szOutFile,
        CString szOutPath);
public:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};
