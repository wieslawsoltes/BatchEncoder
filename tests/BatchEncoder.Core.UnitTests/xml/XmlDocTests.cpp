// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlDoc_Tests)
    {
    public:
        TEST_METHOD(XmlDoc_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlDoc xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlDoc_Parse_Declaration)
        {
            const char* xml = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
            config::xml::XmlDocumnent doc;
            bool bResult = config::xml::XmlDoc::Parse(xml, doc);
            Assert::IsTrue(bResult);
        }
    };
}
