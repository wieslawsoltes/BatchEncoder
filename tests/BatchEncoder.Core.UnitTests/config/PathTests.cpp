// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Path.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CPath_Tests)
    {
    public:
        TEST_METHOD(CPath_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CPath m_Path;
            #pragma warning(pop)
        }

        TEST_METHOD(CPath_ComparePath)
        {
            std::vector<config::CPath> paths
            {
                { L"C:\\Music\\Abc.wav", 0 },
                { L"C:\\Music\\Ghj.wav", 0 }
            };

            bool bResult01 = config::CPath::ComparePath(paths[0], paths[1]);
            Assert::IsTrue(bResult01);

            bool bResult10 = config::CPath::ComparePath(paths[1], paths[0]);
            Assert::IsFalse(bResult10);
        }

        TEST_METHOD(CPath_Sort)
        {
            std::vector<config::CPath> paths
            {
                { L"C:\\Music\\Dfg.wav", 0 },
                { L"C:\\Music\\Abc.wav", 0 },
                { L"C:\\Music\\Ghj.wav", 0 }
            };

            config::CPath::Sort(paths);

            Assert::AreEqual(L"C:\\Music\\Abc.wav", paths[0].szPath.c_str());
            Assert::AreEqual(L"C:\\Music\\Dfg.wav", paths[1].szPath.c_str());
            Assert::AreEqual(L"C:\\Music\\Ghj.wav", paths[2].szPath.c_str());
        }
    };
}
