// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\Worker.h"
#include "worker\WorkerContext.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CWorker_Tests)
    {
    public:
        TEST_METHOD(CWorker_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CWorker m_Worker;
            #pragma warning(pop)
        }
    };
}
