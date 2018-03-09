// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Settings.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CSettings_Tests)
    {
    public:
        TEST_METHOD(CSettings_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CSettings m_Settings;
            #pragma warning(pop)
        }

        TEST_METHOD(CSettings_IsPortable)
        {
        }

        TEST_METHOD(CSettings_InitPortableSettings)
        {
        }

        TEST_METHOD(CSettings_InitUserSettings)
        {
        }

        TEST_METHOD(CSettings_Init)
        {
        }
    };
}
