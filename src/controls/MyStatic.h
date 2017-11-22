// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CMyStatic : public CStatic
{
public:
    CMyStatic();
    virtual ~CMyStatic();
protected:
    virtual void PreSubclassWindow();
protected:
    CFont m_BoldFont;
    CFont m_StdFont;
    bool bIsBold;
public:
    void SetBold(bool bBold = true);
    bool GetBold();
protected:
    DECLARE_MESSAGE_MAP()
protected:
    afx_msg BOOL OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
public:
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
private:
    CString szToolTipText;
    bool bHaveToolTipText;
public:
    void SetTooltipText(CString szText);
    CString GetTooltipText();
    void DelTooltipText();
    bool HaveTooltipText();
};
