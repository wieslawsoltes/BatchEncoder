#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\OutputPath.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(COutputPath_Tests)
    {
    public:
        TEST_METHOD(COutputPath_Constructor)
        {
            worker::COutputPath m_Output;

            Assert::IsFalse(m_Output.bIsEmpty);
            Assert::IsFalse(m_Output.bHaveSourceDirectory);
            Assert::IsFalse(m_Output.bHaveName);
            Assert::IsFalse(m_Output.bHaveExt);
        }
        TEST_METHOD(COutputPath_Validate_Output_Empty)
        {
            worker::COutputPath m_Output;
            std::wstring szOutput = L"";
            bool bValidate = m_Output.Validate(szOutput);

            Assert::IsTrue(m_Output.bIsEmpty);
            Assert::IsFalse(m_Output.bHaveSourceDirectory);
            Assert::IsFalse(m_Output.bHaveName);
            Assert::IsFalse(m_Output.bHaveExt);
            Assert::IsTrue(bValidate);
        }
    };
}
