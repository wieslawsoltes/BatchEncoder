#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlOptions_Tests)
    {
    public:
        TEST_METHOD(XmlOptions_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlOptions xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlOptions_GetOptions_element)
        {
        }

        TEST_METHOD(XmlOptions_SetOptions_element)
        {
        }

        TEST_METHOD(XmlOptions_GetOptions)
        {
        }

        TEST_METHOD(XmlOptions_SetOptions)
        {
        }
    };
}
