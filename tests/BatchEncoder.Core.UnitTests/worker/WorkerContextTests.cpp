// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(IWorkerContext_Tests)
    {
    public:
        TEST_METHOD(IWorkerContext_Constructor)
        {
            TestWorkerContext ctx;
            Assert::IsTrue(ctx.bDone);
            Assert::IsFalse(ctx.bRunning);
            Assert::IsNull(ctx.pConfig);
        }
    };
}
