// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "worker\WorkerContext.h"

namespace BatchEncoderCoreUnitTests
{
    class CTestOutputParser : public worker::IOutputParser
    {
    public:
        bool Open(worker::IWorkerContext* ctx, const std::wstring& szFunction)
        {
             return true;
        }
        bool Parse(worker::IWorkerContext* ctx, const char *szLine)
        {
            return true;
        }
    };
}
