// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#define WM_ITEMCHANGED (WM_USER + 0x20)

class CMyListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CMyListCtrl)
public:
    CMyListCtrl();
    virtual ~CMyListCtrl();
protected:
    afx_msg void PreSubclassWindow();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
protected:
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
private:
    CMyListCtrl& operator=(const CMyListCtrl &x);
    CMyListCtrl(const CMyListCtrl &x);
public:
    bool bUseTooltipsList; // use only for LVS_REPORT style
    CList<CString, CString&> listTooltips;
private:
    CString szToolTipText;
    bool bHaveToolTipText;
public:
    void SetTooltipText(CString szText);
    CString GetTooltipText();
    void DelTooltipText();
    bool HaveTooltipText();
};
