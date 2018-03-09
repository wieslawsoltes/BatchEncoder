// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    int LoggerReportHook(int reportType, char *message, int *returnValue)
    {
        //Logger::WriteMessage(message);
        return 1;
    }

    TEST_MODULE_INITIALIZE(TestModuleInitialize)
    {
        _CrtSetReportHook(LoggerReportHook);
    }

    TEST_MODULE_CLEANUP(TestModuleCleanup)
    {
    }

    TEST_CLASS(MemoryLeak_Tests)
    {
    public:
        _CrtMemState beforeTest;
        _CrtMemState afterTest;
    public:
        TEST_CLASS_INITIALIZE(TestClassInitialize)
        {
        }

        TEST_CLASS_CLEANUP(TestClassCleanup)
        {
        }

        TEST_METHOD_INITIALIZE(TestMethodInitialize)
        {
            //Logger::WriteMessage(L"Recording memory check point before test.\n");
            _CrtMemCheckpoint(&beforeTest);
        }

        TEST_METHOD_CLEANUP(TestMethodCleanup)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)

            //Logger::WriteMessage(L"Recording memory check point after test.\n");
            _CrtMemCheckpoint(&afterTest);
            _CrtMemState difference;

            //Logger::WriteMessage(L"Calcluating difference between tests.\n");
            if (_CrtMemDifference(&difference, &beforeTest, &afterTest))
            {
                _CrtMemDumpStatistics(&difference);
                Assert::Fail(L"Memory leak found.");
            }

            #pragma warning(pop)
        }

        TEST_METHOD(TestMethodLeakingMemory)
        {
            {
                //int *data = new int[1024];
                //delete[] data;
            }
            //_CrtMemDumpAllObjectsSince(NULL);
        }
    };
}
