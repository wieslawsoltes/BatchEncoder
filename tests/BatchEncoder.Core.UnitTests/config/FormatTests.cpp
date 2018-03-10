// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Format.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CFormat_Tests)
    {
        std::vector<config::CFormat> formats
        {
            { L"MP2_ENC", L"MP2 Encoder", config::FormatType::Encoder, 0 },
            { L"MP3_ENC", L"MP3 Encoder", config::FormatType::Encoder, 0 },
            { L"MP4_ENC", L"MP4 Encoder", config::FormatType::Encoder, 0 },
            { L"MP2_DEC", L"MP2 Decoder", config::FormatType::Decoder, 0 },
            { L"MP3_DEC", L"MP3 Decoder", config::FormatType::Decoder, 0 },
            { L"MP4_DEC", L"MP4 Decoder", config::FormatType::Decoder, 0 }
        };
    public:
        TEST_METHOD(CFormat_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CFormat m_Format;
            #pragma warning(pop)
        }

        TEST_METHOD(CFormat_ToInt)
        {
            int encoder = config::CFormat::ToInt(config::FormatType::Encoder);
            Assert::AreEqual(0, encoder);

            int decoder = config::CFormat::ToInt(config::FormatType::Decoder);
            Assert::AreEqual(1, decoder);
        }

        TEST_METHOD(CFormat_FromInt)
        {
            config::FormatType encoder = config::CFormat::FromInt(0);
            Assert::IsTrue(config::FormatType::Encoder == encoder);

            config::FormatType decoder = config::CFormat::FromInt(1);
            Assert::IsTrue(config::FormatType::Decoder == decoder);
        }

        TEST_METHOD(CFormat_IsUniqueId)
        {
            bool isUniqueCaseSensitive = config::CFormat::IsUniqueId(formats, L"mp3_enc");
            Assert::AreEqual(false, isUniqueCaseSensitive);

            bool isUnique = config::CFormat::IsUniqueId(formats, L"AC3_ENC");
            Assert::AreEqual(true, isUnique);
        }

        TEST_METHOD(CFormat_IsUniqueName)
        {
            bool isUniqueCaseSensitive = config::CFormat::IsUniqueName(formats, L"mp2 encoder");
            Assert::AreEqual(false, isUniqueCaseSensitive);

            bool isUnique = config::CFormat::IsUniqueName(formats, L"AC3 Encoder");
            Assert::AreEqual(true, isUnique);
        }

        TEST_METHOD(CFormat_AreIdsUnique)
        {
            bool areUnique = config::CFormat::AreIdsUnique(formats);
            Assert::AreEqual(true, areUnique);
        }

        TEST_METHOD(CFormat_AreNamesUnique)
        {
            bool areUnique = config::CFormat::AreNamesUnique(formats);
            Assert::AreEqual(true, areUnique);
        }

        TEST_METHOD(CFormat_GetFormatById)
        {
        }

        TEST_METHOD(CFormat_GetDecoderByExtension)
        {
        }

        TEST_METHOD(CFormat_GetDecoderByExtensionAndFormat)
        {
        }

        TEST_METHOD(CFormat_IsValidInputExtension_string)
        {
        }

        TEST_METHOD(CFormat_IsValidInputExtension_vector)
        {
        }

        TEST_METHOD(CFormat_CompareName)
        {
        }

        TEST_METHOD(CFormat_CompareType)
        {
        }

        TEST_METHOD(CFormat_ComparePriority)
        {
        }

        TEST_METHOD(CFormat_Sort)
        {
        }
    };
}
