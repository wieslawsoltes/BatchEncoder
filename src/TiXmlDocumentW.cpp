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
#include "TiXmlDocumentW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTiXmlDocumentW::CTiXmlDocumentW()
{

}

CTiXmlDocumentW::~CTiXmlDocumentW()
{

}

// load file - using char buffer
bool CTiXmlDocumentW::LoadFileW(CString szFileName)
{
    CFile fp;
    char *pBuffer;
    ULONGLONG nSize;

    // using CFile here becose we don't need FILE *
    if(fp.Open(szFileName, CFile::modeRead) == TRUE)
    {
        nSize = fp.GetLength();
        if(nSize == 0UL)
            return false;

        pBuffer = (char *) malloc((size_t) nSize);
        if(pBuffer == NULL)
        {
            fp.Close();
            return false;
        }

        if(fp.Read((char *) pBuffer, (UINT) nSize) != (UINT) nSize)
        {
            fp.Close();
            return false;
        }

        // TODO: maybe set the UTF-8 encoding in params?
        // need to save all data inside tags in UTF-8 format
        // TiXmlDocument::Parse(pBuffer, 0, TIXML_ENCODING_UTF8);
        TiXmlDocument::Parse(pBuffer);

        fp.Close();
        free(pBuffer);

        return true;
    }
    else
    {
        return false;
    }
}

// save file - using char buffer
// part of code is from 'tinyxml.cpp'
bool CTiXmlDocumentW::SaveFileW(CString szFileName)
{
    // using _tfopen becose of Print function
    // only accepts FILE * input
    FILE* fp = _tfopen(szFileName, _T("w"));        
    if(fp != NULL)        
    {        
        // TODO: 
        // need to find way to obtain useMicrosoftBOM state   
        // need to save all data inside tags in UTF-8 format
        // or just always write UTF-8 file header
        // if(TiXmlDocument::useMicrosoftBOM)         
        // { 

        const unsigned char TIXML_UTF_LEAD_0 = 0xefU;        
        const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;        
        const unsigned char TIXML_UTF_LEAD_2 = 0xbfU; 

        fputc(TIXML_UTF_LEAD_0, fp);                
        fputc(TIXML_UTF_LEAD_1, fp);        
        fputc(TIXML_UTF_LEAD_2, fp);  

        // } 

        TiXmlDocument::Print(fp, 0);
        fclose(fp);

        return true;
    }
    return false; 
}
