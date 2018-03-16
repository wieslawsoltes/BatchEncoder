// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Preset.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CPreset_Tests)
    {
        std::vector<config::CPreset> presets
        {
            { L"Abc", L"--test 0" },
            { L"Dfg", L"--test 1" },
            { L"Ghj", L"--test 2" }
        };
    public:
        TEST_METHOD(CPreset_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CPreset m_Preset;
            #pragma warning(pop)
        }

        TEST_METHOD(CPreset_IsUniqueName)
        {
            bool isUniqueCaseSensitive = config::CPreset::IsUniqueName(presets, L"dfg");
            Assert::AreEqual(false, isUniqueCaseSensitive);

            bool isUnique = config::CPreset::IsUniqueName(presets, L"Jkl");
            Assert::AreEqual(true, isUnique);
        }

        TEST_METHOD(CPreset_AreNamesUnique)
        {
            bool areUnique = config::CPreset::AreNamesUnique(presets);
            Assert::AreEqual(true, areUnique);
        }

        TEST_METHOD(CPreset_CompareName)
        {
            std::vector<config::CPreset> presets
            {
                { L"Abc", L"--test 0" },
                { L"Ghj", L"--test 1" }
            };

            bool bResult01 = config::CPreset::CompareName(presets[0], presets[1]);
            Assert::IsTrue(bResult01);

            bool bResult10 = config::CPreset::CompareName(presets[1], presets[0]);
            Assert::IsFalse(bResult10);
        }

        TEST_METHOD(CPreset_Sort)
        {
            std::vector<config::CPreset> presets
            {
                { L"Dfg", L"--test 1" },
                { L"Abc", L"--test 0" },
                { L"Ghj", L"--test 2" }
            };

            config::CPreset::Sort(presets);

            Assert::AreEqual(L"Abc", presets[0].szName.c_str());
            Assert::AreEqual(L"Dfg", presets[1].szName.c_str());
            Assert::AreEqual(L"Ghj", presets[2].szName.c_str());
        }
    };
}
