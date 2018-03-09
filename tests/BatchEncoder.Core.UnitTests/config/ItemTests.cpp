// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Item.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CItem_Tests)
    {
    public:
        TEST_METHOD(CItem_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CItem m_Item;
            #pragma warning(pop)
        }

        TEST_METHOD(CItem_ResetProgress)
        {
        }

        TEST_METHOD(CItem_CompareName)
        {
        }

        TEST_METHOD(CItem_Sort)
        {
        }
    };
}
