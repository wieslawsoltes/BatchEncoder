// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CToolUtilities_Tests)
    {
    public:
        TEST_METHOD(CToolUtilities_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CToolUtilities m_Utilities;
            #pragma warning(pop)
        }
    };
}
