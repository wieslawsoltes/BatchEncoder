// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlOutputs_Tests)
    {
    public:
        TEST_METHOD(XmlOutputs_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlOutputs xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlOutputs_GetOutputs_parent)
        {
        }

        TEST_METHOD(XmlOutputs_SetOutputs_parent)
        {
        }

        TEST_METHOD(XmlOutputs_GetOutputs)
        {
        }

        TEST_METHOD(XmlOutputs_SetOutputs)
        {
        }
    };
}
