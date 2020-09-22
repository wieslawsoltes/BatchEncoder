#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlTools_Tests)
    {
    public:
        TEST_METHOD(XmlTools_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlTools xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlTools_GetTool_element)
        {
        }

        TEST_METHOD(XmlTools_SetTool_element)
        {
        }

        TEST_METHOD(XmlTools_GetTool)
        {
        }

        TEST_METHOD(XmlTools_SetTool)
        {
        }
    };
}
