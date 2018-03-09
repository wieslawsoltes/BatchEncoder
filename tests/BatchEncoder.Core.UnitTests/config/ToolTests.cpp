// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Tool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CTool_Tests)
    {
    public:
        TEST_METHOD(CTool_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CTool m_Tool;
            #pragma warning(pop)
        }

        TEST_METHOD(CTool_GetToolByFormatAndPlatform)
        {
        }

        TEST_METHOD(CTool_GetToolByFormat)
        {
        }

        TEST_METHOD(CTool_GetToolByPath)
        {
        }

        TEST_METHOD(CTool_GetToolByName)
        {
        }

        TEST_METHOD(CTool_IsUniqueName)
        {
        }

        TEST_METHOD(CTool_IsValidFormat_string)
        {
        }

        TEST_METHOD(CTool_IsValidFormat_vector)
        {
        }

        TEST_METHOD(CTool_CompareName)
        {
        }

        TEST_METHOD(CTool_ComparePriority)
        {
        }

        TEST_METHOD(CTool_Sort)
        {
        }
    };
}
