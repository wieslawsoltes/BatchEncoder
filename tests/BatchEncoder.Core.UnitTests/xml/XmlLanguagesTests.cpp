#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlLanguages_Tests)
    {
    public:
        TEST_METHOD(XmlLanguages_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlLanguages xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlLanguages_GetLanguage_parent)
        {
        }

        TEST_METHOD(XmlLanguages_SetLanguage_parent)
        {
        }

        TEST_METHOD(XmlLanguages_GetLanguage)
        {
        }

        TEST_METHOD(XmlLanguages_SetLanguage)
        {
        }
    };
}
