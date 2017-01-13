// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CMyComboBox : public CComboBox
{
    DECLARE_DYNAMIC(CMyComboBox)
public:
    CMyComboBox();
    virtual ~CMyComboBox();
protected:
    DECLARE_MESSAGE_MAP()
};
