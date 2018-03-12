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
            { L"MP2_ENC", L"MP2 Encoder", config::FormatType::Encoder, 0, L"WAV", L"MP2" },
            { L"MP3_ENC", L"MP3 Encoder", config::FormatType::Encoder, 0, L"WAV", L"MP3" },
            { L"MP4_ENC", L"MP4 Encoder", config::FormatType::Encoder, 0, L"WAV", L"MP4" },
            { L"MP2_DEC", L"MP2 Decoder", config::FormatType::Decoder, 0, L"MP2", L"WAV" },
            { L"MP3_DEC", L"MP3 Decoder", config::FormatType::Decoder, 0, L"MP3", L"WAV" },
            { L"MP4_DEC", L"MP4 Decoder", config::FormatType::Decoder, 0, L"MP4", L"WAV" }
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
            int index = config::CFormat::GetFormatById(formats, L"MP4_ENC");
            Assert::AreEqual(2, index);

            const auto& format = formats[index];
            Assert::AreEqual(L"MP4_ENC", format.szId.c_str());
        }

        TEST_METHOD(CFormat_IsValidInputExtension_Single_string)
        {
            std::wstring szInputExtensions = L"WAV";

            std::wstring szExtValid = L"WAV";
            bool bResultValid = config::CFormat::IsValidInputExtension(szInputExtensions, szExtValid);
            Assert::IsTrue(bResultValid);

            std::wstring szExtInvalid = L"AVI";
            bool bResultInvalid = config::CFormat::IsValidInputExtension(szInputExtensions, szExtInvalid);
            Assert::IsFalse(bResultInvalid);
        }

        TEST_METHOD(CFormat_IsValidInputExtension_Multi_string)
        {
            std::wstring szInputExtensions = L"RAW,WAV,AVS";

            std::wstring szExtValid = L"WAV";
            bool bResultValid = config::CFormat::IsValidInputExtension(szInputExtensions, szExtValid);
            Assert::IsTrue(bResultValid);

            std::wstring szExtInvalid = L"AVI";
            bool bResultInvalid = config::CFormat::IsValidInputExtension(szInputExtensions, szExtInvalid);
            Assert::IsFalse(bResultInvalid);
        }

        TEST_METHOD(CFormat_IsValidInputExtension_vector)
        {
            std::wstring szExtValid = L"WAV";
            bool bResultValid = config::CFormat::IsValidInputExtension(formats, szExtValid);
            Assert::IsTrue(bResultValid);

            std::wstring szExtInvalid = L"AVI";
            bool bResultInvalid = config::CFormat::IsValidInputExtension(formats, szExtInvalid);
            Assert::IsFalse(bResultInvalid);
        }

        TEST_METHOD(CFormat_GetDecoderByExtension)
        {
            std::wstring szExt = L"MP3";
            size_t nDecoder = config::CFormat::GetDecoderByExtension(formats, szExt);
            Assert::AreEqual(size_t(4), nDecoder);
        }

        TEST_METHOD(CFormat_GetDecoderByExtensionAndFormat)
        {
            auto& encoder = formats[1];
            std::wstring szExt = L"MP3";
            size_t nDecoder = config::CFormat::GetDecoderByExtensionAndFormat(formats, szExt, encoder);
            Assert::AreEqual(size_t(4), nDecoder);
        }

        TEST_METHOD(CFormat_CompareName)
        {
            std::vector<config::CFormat> formats
            {
                { L"MP2_ENC", L"MP2 Encoder", config::FormatType::Encoder, 0 },
                { L"MP3_ENC", L"MP3 Encoder", config::FormatType::Encoder, 0 }
            };

            bool bResult01 = config::CFormat::CompareName(formats[0], formats[1]);
            Assert::IsTrue(bResult01);

            bool bResult10 = config::CFormat::CompareName(formats[1], formats[0]);
            Assert::IsFalse(bResult10);
        }

        TEST_METHOD(CFormat_CompareType)
        {
            std::vector<config::CFormat> formats
            {
                { L"MP4_ENC", L"MP4 Encoder", config::FormatType::Encoder, 0, L"WAV", L"MP4" },
                { L"MP2_DEC", L"MP2 Decoder", config::FormatType::Decoder, 0, L"MP2", L"WAV" }
            };

            bool bResult01 = config::CFormat::CompareType(formats[0], formats[1]);
            Assert::IsTrue(bResult01);

            bool bResult10 = config::CFormat::CompareType(formats[1], formats[0]);
            Assert::IsFalse(bResult10);
        }

        TEST_METHOD(CFormat_ComparePriority)
        {
            std::vector<config::CFormat> formats
            {
                { L"MP2_ENC", L"MP2 Encoder", config::FormatType::Encoder, 0 },
                { L"MP3_ENC", L"MP3 Encoder", config::FormatType::Encoder, 1 }
            };

            bool bResult01 = config::CFormat::ComparePriority(formats[0], formats[1]);
            Assert::IsFalse(bResult01);

            bool bResult10 = config::CFormat::ComparePriority(formats[1], formats[0]);
            Assert::IsTrue(bResult10);
        }

        TEST_METHOD(CFormat_Sort)
        {
            std::vector<config::CFormat> formats
            {
                { L"MP4_ENC", L"MP4 Encoder", config::FormatType::Encoder, 1, L"WAV", L"MP4" },
                { L"MP3_DEC", L"MP3 Decoder", config::FormatType::Decoder, 2, L"MP3", L"WAV" },
                { L"MP4_DEC", L"MP4 Decoder", config::FormatType::Decoder, 1, L"MP4", L"WAV" },
                { L"MP2_ENC", L"MP2 Encoder", config::FormatType::Encoder, 3, L"WAV", L"MP2" },
                { L"MP3_ENC", L"MP3 Encoder", config::FormatType::Encoder, 2, L"WAV", L"MP3" },
                { L"MP2_DEC", L"MP2 Decoder", config::FormatType::Decoder, 3, L"MP2", L"WAV" }
            };

            config::CFormat::Sort(formats);

            Assert::AreEqual(L"MP2_ENC", formats[0].szId.c_str());
            Assert::AreEqual(L"MP3_ENC", formats[1].szId.c_str());
            Assert::AreEqual(L"MP4_ENC", formats[2].szId.c_str());
            Assert::AreEqual(L"MP2_DEC", formats[3].szId.c_str());
            Assert::AreEqual(L"MP3_DEC", formats[4].szId.c_str());
            Assert::AreEqual(L"MP4_DEC", formats[5].szId.c_str());
        }
    };
}
