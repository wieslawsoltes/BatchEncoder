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

        TEST_METHOD(XmlDoc_GetRootName)
        {
        }

        TEST_METHOD(XmlDoc_Create_doc)
        {
        }

        TEST_METHOD(XmlDoc_Open_file_doc)
        {
        }

        TEST_METHOD(XmlDoc_Parse_xml_doc)
        {
        }

        TEST_METHOD(XmlDoc_Save_file_doc)
        {
        }

        TEST_METHOD(XmlDoc_ToString)
        {
        }

        TEST_METHOD(XmlDoc_ToInt)
        {
        }

        TEST_METHOD(XmlDoc_ToUInt)
        {
        }

        TEST_METHOD(XmlDoc_ToUInt64)
        {
        }

        TEST_METHOD(XmlDoc_ToBool)
        {
        }

        TEST_METHOD(XmlDoc_IntToString)
        {
        }

        TEST_METHOD(XmlDoc_SizeToString)
        {
        }

        TEST_METHOD(XmlDoc_UInt64ToString)
        {
        }

        TEST_METHOD(XmlDoc_BoolToString)
        {
        }

        TEST_METHOD(XmlDoc_GetAttributeValueString)
        {
        }

        TEST_METHOD(XmlDoc_GetAttributeValueInt)
        {
        }

        TEST_METHOD(XmlDoc_GetAttributeValueSizeT)
        {
        }

        TEST_METHOD(XmlDoc_GetAttributeValueUInt64)
        {
        }

        TEST_METHOD(XmlDoc_GetAttributeValueBool)
        {
        }

        TEST_METHOD(XmlDoc_SetAttributeValueString)
        {
        }

        TEST_METHOD(XmlDoc_SetAttributeValueInt)
        {
        }

        TEST_METHOD(XmlDoc_SetAttributeValueSizeT)
        {
        }

        TEST_METHOD(XmlDoc_SetAttributeValueUInt64)
        {
        }

        TEST_METHOD(XmlDoc_SetAttributeValueBool)
        {
        }

        TEST_METHOD(XmlDoc_GetChildValueString)
        {
        }

        TEST_METHOD(XmlDoc_GetChildValueInt)
        {
        }

        TEST_METHOD(XmlDoc_GetChildValueSizeT)
        {
        }

        TEST_METHOD(XmlDoc_GetChildValueUInt64)
        {
        }

        TEST_METHOD(XmlDoc_GetChildValueBool)
        {
        }

        TEST_METHOD(XmlDoc_SetChildValueString)
        {
        }

        TEST_METHOD(XmlDoc_SetChildValueInt)
        {
        }

        TEST_METHOD(XmlDoc_SetChildValueSizeT)
        {
        }

        TEST_METHOD(XmlDoc_SetChildValueUInt64)
        {
        }

        TEST_METHOD(XmlDoc_SetChildValueBool)
        {
        }

        TEST_METHOD(XmlDoc_NewElement)
        {
        }

        TEST_METHOD(XmlDoc_FirstChildElement)
        {
        }

        TEST_METHOD(XmlDoc_LinkEndChild)
        {
        }

        TEST_METHOD(XmlDoc_Create)
        {
        }

        TEST_METHOD(XmlDoc_Open)
        {
        }

        TEST_METHOD(XmlDoc_Save)
        {
        }
    };
}
