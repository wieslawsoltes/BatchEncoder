// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Item.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CItem_Tests)
    {
    public:
        TEST_METHOD(CItem_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::CItem m_Item;
            #pragma warning(pop)
        }

        TEST_METHOD(CItem_ResetProgress)
        {
            std::vector<config::CItem> items
            {
                { -1, L"", L"", L"", 0, L"", true, 0, { { L"" } }, L"", L"", true, 50, 40 },
                { -1, L"", L"", L"", 0, L"", true, 0, { { L"" } }, L"", L"", true, 50, 40 },
                { -1, L"", L"", L"", 0, L"", true, 0, { { L"" } }, L"", L"", true, 50, 40 }
            };

            for (auto& item : items)
            {
                item.ResetProgress();

                Assert::IsFalse(item.bFinished);
                Assert::AreEqual(0, item.nProgress);
                Assert::AreEqual(0, item.nPreviousProgress);
            }
        }

        TEST_METHOD(CItem_SetIds)
        {
            std::vector<config::CItem> items
            {
                { -1 },
                { -1 },
                { -1 }
            };

            config::CItem::SetIds(items);

            Assert::AreEqual(0, items[0].nId);
            Assert::AreEqual(1, items[1].nId);
            Assert::AreEqual(2, items[2].nId);
        }

        TEST_METHOD(CItem_CompareName)
        {
            std::vector<config::CItem> items
            {
                { 0, L"Abc" },
                { 1, L"Ghj" }
            };

            bool bResult01 = config::CItem::CompareName(items[0], items[1]);
            Assert::IsTrue(bResult01);

            bool bResult10 = config::CItem::CompareName(items[1], items[0]);
            Assert::IsFalse(bResult10);
        }

        TEST_METHOD(CItem_Sort)
        {
            std::vector<config::CItem> items
            {
                { 1, L"Dfg" },
                { 0, L"Abc" },
                { 2, L"Ghj" }
            };

            config::CItem::Sort(items);

            Assert::AreEqual(L"Abc", items[0].szName.c_str());
            Assert::AreEqual(L"Dfg", items[1].szName.c_str());
            Assert::AreEqual(L"Ghj", items[2].szName.c_str());
        }
    };
}
