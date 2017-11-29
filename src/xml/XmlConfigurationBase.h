// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "tinyxml2\tinyxml2.h" // https://github.com/leethomason/tinyxml2

class XmlConfigurationBase : protected tinyxml2::XMLDocument
{
public:
	const char *m_Utf8DocumentDeclaration = "xml version=\"1.0\" encoding=\"UTF-8\"";
public:
	XmlConfigurationBase();
	virtual ~XmlConfigurationBase();
private:
	const LPCTSTR m_True = _T("true");
	const LPCTSTR m_False = _T("false");
protected:
	CString ToCString(const char *pszUtf8);
	bool ToBool(const char *pszUtf8);
	int ToInt(const char *pszUtf8);
	CString ToCString(const int nValue);
	CString ToCString(const bool bValue);
};
