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

    TEST_CLASS(XmlOptions_Tests)
    {
    public:
        TEST_METHOD(XmlOptions_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlOptions xml(doc);
            #pragma warning(pop)
        }
    };

    TEST_CLASS(XmlItems_Tests)
    {
    public:
        TEST_METHOD(XmlItems_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlItems xml(doc);
            #pragma warning(pop)
        }
    };

    TEST_CLASS(XmlLanguages_Tests)
    {
    public:
        TEST_METHOD(XmlLanguages_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlLanguages xml(doc);
            #pragma warning(pop)
        }
    };

    TEST_CLASS(XmlOutputs_Tests)
    {
    public:
        TEST_METHOD(XmlOutputs_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlOutputs xml(doc);
            #pragma warning(pop)
        }
    };

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

    TEST_CLASS(XmlFormats_Tests)
    {
    public:
        TEST_METHOD(XmlFormats_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlFormats xml(doc);
            #pragma warning(pop)
        }
    };

    TEST_CLASS(XmlTools_Tests)
    {
    public:
        TEST_METHOD(XmlTools_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlDocumnent doc;
            xml::XmlTools xml(doc);
            #pragma warning(pop)
        }
    };

    TEST_CLASS(XmlConfig_Tests)
    {
    public:
        TEST_METHOD(XmlConfig_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            xml::XmlConfig xml;
            #pragma warning(pop)
        }
    };
}
