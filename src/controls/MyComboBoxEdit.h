// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#define WM_MY_EN_CHANGE WM_USER + 0x20

class CMyComboBoxEdit : public CEdit
{
    DECLARE_DYNAMIC(CMyComboBoxEdit)
public:
    CMyComboBoxEdit();
    virtual ~CMyComboBoxEdit();
protected:
    virtual void PreSubclassWindow();
protected:
    DECLARE_MESSAGE_MAP()
protected:
    afx_msg BOOL OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
public:
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
public:
    afx_msg void OnEnChange();
private:
    CString szToolTipText;
    bool bHaveToolTipText;
public:
    void SetTooltipText(CString szText);
    CString GetTooltipText();
    void DelTooltipText();
    bool HaveTooltipText();
};
