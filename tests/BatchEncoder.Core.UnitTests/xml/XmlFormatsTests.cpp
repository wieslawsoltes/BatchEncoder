// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlFormats_Tests)
    {
    public:
        TEST_METHOD(XmlFormats_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlFormats xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlFormats_GetFormat_element)
        {
        }

        TEST_METHOD(XmlFormats_SetFormat_element)
        {
        }

        TEST_METHOD(XmlFormats_GetFormat)
        {
        }

        TEST_METHOD(XmlFormats_SetFormat)
        {
        }
    };
}
