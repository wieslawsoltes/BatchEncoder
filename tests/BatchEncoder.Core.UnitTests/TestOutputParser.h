// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "worker\WorkerContext.h"
#include "worker\CommandLine.h"
#include "worker\OutputParser.h"

namespace BatchEncoderCoreUnitTests
{
    class CTestOutputParser : public worker::IOutputParser
    {
        bool Init(worker::IWorkerContext* ctx, worker::CCommandLine* cl)
        {
            return true;
        }
        bool Parse(const char *szLine)
        {
            return true;
        }
    };
}
