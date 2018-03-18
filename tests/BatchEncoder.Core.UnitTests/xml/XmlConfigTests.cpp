// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlConfig_Tests)
    {
    public:
        TEST_METHOD(XmlConfig_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlConfig xml;
            #pragma warning(pop)
        }

        TEST_METHOD(XmlConfig_GetRootName)
        {
        }

        TEST_METHOD(XmlConfig_LoadOptions_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadOptions_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveOptions_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadPaths_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadPaths_file)
        {
        }

        TEST_METHOD(XmlConfig_SavePaths_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadItem_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadItem_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveItem_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadItems_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadItems_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveItems_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadLanguage_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadLanguage_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveLanguage_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadOutputs_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadOutputs_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveOutputs_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadPreset_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadPreset_file)
        {
        }

        TEST_METHOD(XmlConfig_SavePreset_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadPresets_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadPresets_file)
        {
        }

        TEST_METHOD(XmlConfig_SavePresets_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadFormat_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadFormat_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveFormat_file)
        {
        }

        TEST_METHOD(XmlConfig_LoadTool_doc)
        {
        }

        TEST_METHOD(XmlConfig_LoadTool_file)
        {
        }

        TEST_METHOD(XmlConfig_SaveTool_file)
        {
        }
    };
}
