// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CMyEdit : public CEdit
{
    DECLARE_DYNAMIC(CMyEdit)
public:
    CMyEdit();
    virtual ~CMyEdit();
protected:
    DECLARE_MESSAGE_MAP()
};
