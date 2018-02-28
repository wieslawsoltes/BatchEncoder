// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Path.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CPath_Tests)
    {
    public:
        TEST_METHOD(CPath_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CPath m_Path;
            #pragma warning(pop)
        }
    };
}
