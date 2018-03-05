// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "xml\XmlConfig.h"

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
            xml::XmlDocumnent doc;
            xml::XmlDoc xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlDoc_Parse_Declaration)
        {
            const char* xml = u8"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
            xml::XmlDocumnent doc;
            bool bResult = xml::XmlDoc::Parse(xml, doc);
            Assert::IsTrue(bResult);
        }
    };
}
