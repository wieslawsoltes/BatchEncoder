// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CLuaOutputParser_Tests)
    {
    public:
        TEST_METHOD(CLuaOutputParser_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CLuaOutputParser m_Parser;
            #pragma warning(pop)
        }

        TEST_METHOD(CLuaOutputParser_Open)
        {
        }

        TEST_METHOD(CLuaOutputParser_Parse)
        {
        }
    };
}
