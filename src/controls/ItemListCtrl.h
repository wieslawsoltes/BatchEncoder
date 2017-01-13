// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#define WM_ITEMCHANGED (WM_USER + 0x20)

class CItemListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CItemListCtrl)
public:
    CItemListCtrl();
    virtual ~CItemListCtrl();
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};
