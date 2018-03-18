// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlItems_Tests)
    {
    public:
        TEST_METHOD(XmlItems_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlItems xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlItems_GetPath_element)
        {
        }

        TEST_METHOD(XmlItems_SetPath_element)
        {
        }

        TEST_METHOD(XmlItems_GetPaths_parent)
        {
        }

        TEST_METHOD(XmlItems_SetPaths_parent)
        {
        }

        TEST_METHOD(XmlItems_GetItem_element)
        {
        }

        TEST_METHOD(XmlItems_SetItem_element)
        {
        }

        TEST_METHOD(XmlItems_GetItems_parent)
        {
        }

        TEST_METHOD(XmlItems_SetItems_parent)
        {
        }

        TEST_METHOD(XmlItems_GetPaths)
        {
        }

        TEST_METHOD(XmlItems_SetPaths)
        {
        }

        TEST_METHOD(XmlItems_GetItem)
        {
        }

        TEST_METHOD(XmlItems_SetItem)
        {
        }

        TEST_METHOD(XmlItems_GetItems)
        {
        }

        TEST_METHOD(XmlItems_SetItems)
        {
        }
    };
}
