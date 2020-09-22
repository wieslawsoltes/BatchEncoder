#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CToolDownloader_Tests)
    {
    public:
        TEST_METHOD(CToolDownloader_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CToolDownloader m_Downloader;
            #pragma warning(pop)
        }

        TEST_METHOD(CToolDownloader_Download)
        {
        }
    };
}
