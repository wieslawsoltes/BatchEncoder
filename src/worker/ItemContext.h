// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CItemContext
{
public:
    CWorkerContext* pWorkerContext;
    CItem* item;
public:
    CItemContext()
    {
    }
    CItemContext(CWorkerContext* pWorkerContext, CItem* item)
    {
        this->pWorkerContext = pWorkerContext;
        this->item = item;
    }
    CItemContext(const CItemContext& context)
    {
        pWorkerContext = context.pWorkerContext;
        item = context.item;
    }
    virtual ~CItemContext()
    {
    }
    const CItemContext& operator=(const CItemContext& context)
    {
        pWorkerContext = context.pWorkerContext;
        item = context.item;
        return *this;
    }
    BOOL operator==(CItemContext context)
    {
        return pWorkerContext == context.pWorkerContext && item == context.item;
    }
};
