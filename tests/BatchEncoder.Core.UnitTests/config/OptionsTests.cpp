#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Options.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(COptions_Tests)
    {
    public:
        TEST_METHOD(COptions_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::COptions m_Options;
            #pragma warning(pop)
        }

        TEST_METHOD(COptions_Defaults)
        {
            config::COptions m_Options;
            m_Options.Defaults();
        }
    };
}
