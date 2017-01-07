//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
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

/* $Id: TiXmlDocumentW.h 437 2008-03-24 12:15:45Z wisodev $ */

// handle UNICODE file names with TinyXml

// NOTE:
// get TinyXml library from http:/www.sourceforge.net/projects/tinyxml/
// and unpack source package to 'tinyXml\src' dir

#pragma once

#include "tinyxml\tinyxml.h"

class CTiXmlDocumentW : public TiXmlDocument
{
public:
    CTiXmlDocumentW();
    virtual ~CTiXmlDocumentW();
public:
    bool LoadFileW(CString szFileName);
    bool SaveFileW(CString szFileName);
};
