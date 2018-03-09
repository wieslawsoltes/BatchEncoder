// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CLuaProgess_Tests)
    {
    public:
        TEST_METHOD(CLuaProgess_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CLuaProgess m_Progess;
            #pragma warning(pop)
        }

        TEST_METHOD(CLuaProgess_Open)
        {
        }

        TEST_METHOD(CLuaProgess_Init)
        {
        }

        TEST_METHOD(CLuaProgess_GetProgress)
        {
        }
    };
}
