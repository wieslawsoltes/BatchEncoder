// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlPresets_Tests)
    {
    public:
        TEST_METHOD(XmlPresets_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlPresets xml(doc);
            #pragma warning(pop)
        }
    };
}
