// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\WorkerContext.h"
#include "TestWorkerContext.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(IWorkerContext_Tests)
    {
    public:
        TEST_METHOD(IWorkerContext_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            CTestWorkerContext m_Context;
            #pragma warning(pop)
        }
    };
}
