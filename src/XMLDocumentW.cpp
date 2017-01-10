// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "XMLDocumentW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CXMLDocumentW::CXMLDocumentW()
{

}

CXMLDocumentW::~CXMLDocumentW()
{

}

// load file - using char buffer
bool CXMLDocumentW::LoadFileW(CString szFileName)
{
    CStdioFile fp;
    if (fp.Open(szFileName, CFile::modeRead | CFile::typeBinary) == TRUE)
    {
        tinyxml2::XMLError result = this->LoadFile(fp.m_pStream);
        fp.Close();
        return result == tinyxml2::XMLError::XML_SUCCESS;
    }
    return false;
}

// save file - using char buffer
// part of code is from 'tinyxml.cpp'
bool CXMLDocumentW::SaveFileW(CString szFileName)
{
    CStdioFile fp;
    if (fp.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText) == TRUE)
    {
        const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
        const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
        const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

        fputc(TIXML_UTF_LEAD_0, fp.m_pStream);
        fputc(TIXML_UTF_LEAD_1, fp.m_pStream);
        fputc(TIXML_UTF_LEAD_2, fp.m_pStream);

        tinyxml2::XMLPrinter printer(fp.m_pStream);
        this->Print(&printer);

        fp.Close();
        return true;
    }
    return false;
}
