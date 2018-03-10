// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Tool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CTool_Tests)
    {
        std::vector<config::CTool> tools
        {
            { L"mp2enc_x86", L"x86", 0, L"MP2_ENC" },
            { L"mp2enc_x64", L"x64", 0, L"MP2_ENC" },
            { L"mp3enc_x86", L"x86", 0, L"MP3_ENC" },
            { L"mp3enc_x64", L"x64", 0, L"MP3_ENC" },
            { L"mp4enc_x86", L"x86", 0, L"MP4_ENC" },
            { L"mp4enc_x64", L"x64", 0, L"MP4_ENC" },
            { L"mp2dec_x86", L"x86", 0, L"MP2_DEC" },
            { L"mp2dec_x64", L"x64", 0, L"MP2_DEC" },
            { L"mp3dec_x86", L"x86", 0, L"MP3_DEC" },
            { L"mp3dec_x64", L"x64", 0, L"MP3_DEC" },
            { L"mp4dec_x86", L"x86", 0, L"MP4_DEC" },
            { L"mp4dec_x64", L"x64", 0, L"MP4_DEC" }
        };
    public:
        TEST_METHOD(CTool_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CTool m_Tool;
            #pragma warning(pop)
        }

        TEST_METHOD(CTool_IsUniqueName)
        {
            bool isUniqueCaseSensitive = config::CTool::IsUniqueName(tools, L"MP2ENC_X86");
            Assert::AreEqual(false, isUniqueCaseSensitive);

            bool isUnique = config::CTool::IsUniqueName(tools, L"ac3enc_x86");
            Assert::AreEqual(true, isUnique);
        }

        TEST_METHOD(CTool_AreNamesUnique)
        {
            bool areUnique = config::CTool::AreNamesUnique(tools);
            Assert::AreEqual(true, areUnique);
        }

        TEST_METHOD(CTool_GetToolByName)
        {
            int index = config::CTool::GetToolByName(tools, L"mp3enc_x86");
            Assert::AreEqual(2, index);

            const auto& tool = tools[index];
            Assert::AreEqual(L"mp3enc_x86", tool.szName.c_str());
        }

        TEST_METHOD(CTool_IsValidFormat_string)
        {
        }

        TEST_METHOD(CTool_IsValidFormat_vector)
        {
        }

        TEST_METHOD(CTool_GetToolByPath)
        {
        }

        TEST_METHOD(CTool_GetToolByFormat)
        {
        }

        TEST_METHOD(CTool_GetToolByFormatAndPlatform)
        {
        }

        TEST_METHOD(CTool_CompareName)
        {
        }

        TEST_METHOD(CTool_ComparePriority)
        {
        }

        TEST_METHOD(CTool_Sort)
        {
        }
    };
}
