//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

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
    if(fp.Open(szFileName, CFile::modeRead | CFile::typeBinary) == TRUE)
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
