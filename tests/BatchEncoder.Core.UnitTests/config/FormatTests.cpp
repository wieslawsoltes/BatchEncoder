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
            Assert::AreEqual(config::FormatType::Encoder, encoder);

            config::FormatType decoder = config::CFormat::FromInt(1);
            Assert::AreEqual(config::FormatType::Decoder, decoder);
        }

        TEST_METHOD(CFormat_IsValidInputExtension_vector)
        {
        }

        TEST_METHOD(CFormat_IsValidInputExtension_string)
        {
        }

        TEST_METHOD(CFormat_GetDecoderByExtensionAndFormat)
        {
        }

        TEST_METHOD(CFormat_GetDecoderByExtension)
        {
        }

        TEST_METHOD(CFormat_GetFormatById)
        {
        }

        TEST_METHOD(CFormat_IsUniqueName)
        {
        }

        TEST_METHOD(CFormat_IsUniqueId)
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
