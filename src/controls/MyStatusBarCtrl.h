// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CMyStatusBarCtrl : public CStatusBarCtrl
{
    DECLARE_DYNAMIC(CMyStatusBarCtrl)
public:
    CMyStatusBarCtrl();
    virtual ~CMyStatusBarCtrl();
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnStatusMenuReset();
};
