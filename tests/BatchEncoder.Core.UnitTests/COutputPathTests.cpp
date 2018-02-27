// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\OutputPath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(COutputPath_Tests)
    {
    public:
        TEST_METHOD(COutputPath_Constructor)
        {
            worker::COutputPath m_Output;
        }
    };
}
