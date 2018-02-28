// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Configuration.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CConfiguration_Tests)
    {
    public:
        TEST_METHOD(CConfiguration_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CConfiguration m_Config;
            #pragma warning(pop)
        }
    };
}
