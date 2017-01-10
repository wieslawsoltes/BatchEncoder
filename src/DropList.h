// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CDropList : public CComboBox
{
    DECLARE_DYNAMIC(CDropList)
public:
    CDropList();
    virtual ~CDropList();
protected:
    DECLARE_MESSAGE_MAP()
};
