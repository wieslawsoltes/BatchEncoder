#include "stdafx.h"
#include "CppUnitTest.h"
#include "config\Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(XmlPresets_Tests)
    {
    public:
        TEST_METHOD(XmlPresets_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            config::xml::XmlDocumnent doc;
            config::xml::XmlPresets xml(doc);
            #pragma warning(pop)
        }

        TEST_METHOD(XmlPresets_GetPreset_element)
        {
        }

        TEST_METHOD(XmlPresets_SetPreset_element)
        {
        }

        TEST_METHOD(XmlPresets_GetPresets_parent)
        {
        }

        TEST_METHOD(XmlPresets_SetPresets_parent)
        {
        }

        TEST_METHOD(XmlPresets_GetPreset)
        {
        }

        TEST_METHOD(XmlPresets_SetPreset)
        {
        }

        TEST_METHOD(XmlPresets_GetPresets)
        {
        }

        TEST_METHOD(XmlPresets_SetPresets)
        {
        }
    };
}
