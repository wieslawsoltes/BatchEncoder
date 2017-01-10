// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

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
