// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CConfig_Tests)
    {
    public:
        TEST_METHOD(CConfig_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CConfig m_Config;
            #pragma warning(pop)
        }

        TEST_METHOD(CConfig_AddItem)
        {
        }

        TEST_METHOD(CConfig_RemoveItems)
        {
        }

        TEST_METHOD(CConfig_LookupString)
        {
        }

        TEST_METHOD(CConfig_GetString_key_default)
        {
        }

        TEST_METHOD(CConfig_GetString_key)
        {
        }

        TEST_METHOD(CConfig_FindTool_platform_format)
        {
        }

        TEST_METHOD(CConfig_FindTool_format)
        {
        }

        TEST_METHOD(CConfig_SetFormatPaths_platform)
        {
        }

        TEST_METHOD(CConfig_SetFormatPaths_filter)
        {
        }

        TEST_METHOD(CConfig_CleanFiles)
        {
        }

        TEST_METHOD(CConfig_LoadOptions_file)
        {
        }

        TEST_METHOD(CConfig_LoadOptions_doc)
        {
        }

        TEST_METHOD(CConfig_SaveOptions_file)
        {
        }

        TEST_METHOD(CConfig_LoadFormats_path)
        {
        }

        TEST_METHOD(CConfig_SaveFormats_path)
        {
        }

        TEST_METHOD(CConfig_LoadFormat_file)
        {
        }

        TEST_METHOD(CConfig_LoadFormat_doc)
        {
        }

        TEST_METHOD(CConfig_SaveFormat_file_format)
        {
        }

        TEST_METHOD(CConfig_LoadPresets_file_format)
        {
        }

        TEST_METHOD(CConfig_LoadPresets_doc_format)
        {
        }

        TEST_METHOD(CConfig_SavePresets_file_format)
        {
        }

        TEST_METHOD(CConfig_LoadTools_path)
        {
        }

        TEST_METHOD(CConfig_SaveTools_path)
        {
        }

        TEST_METHOD(CConfig_LoadTool_file)
        {
        }

        TEST_METHOD(CConfig_LoadTool_doc)
        {
        }

        TEST_METHOD(CConfig_SaveTool_file_tool)
        {
        }

        TEST_METHOD(CConfig_LoadItems_file)
        {
        }

        TEST_METHOD(CConfig_LoadItems_doc)
        {
        }

        TEST_METHOD(CConfig_SaveItems_file)
        {
        }

        TEST_METHOD(CConfig_LoadOutputs_file)
        {
        }

        TEST_METHOD(CConfig_LoadOutputs_doc)
        {
        }

        TEST_METHOD(CConfig_SaveOutputs_file)
        {
        }

        TEST_METHOD(CConfig_LoadLanguages_path)
        {
        }

        TEST_METHOD(CConfig_LoadLanguage_file)
        {
        }

        TEST_METHOD(CConfig_LoadLanguage_doc)
        {
        }
    };
}
