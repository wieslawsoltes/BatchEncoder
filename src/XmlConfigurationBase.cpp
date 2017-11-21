// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "utilities\UnicodeUtf8.h"
#include "utilities\Utf8String.h"
#include "XmlConfigurationBase.h"

XmlConfigurationBase::XmlConfigurationBase()
{

}

XmlConfigurationBase::~XmlConfigurationBase()
{

}

#ifdef _UNICODE
CString XmlConfigurationBase::ToCString(const char *pszUtf8)
{
	if (pszUtf8 == NULL)
		return _T("");
	if (strlen(pszUtf8) == 0)
		return _T("");
	wchar_t *pszUnicode = MakeUnicodeString((unsigned char *)pszUtf8);
	CString szBuff = pszUnicode;
	free(pszUnicode);
	return szBuff;
}
#else
CString XmlConfigurationBase::ToCString(const char *pszUtf8)
{
	if (pszUtf8 == NULL)
		return _T("");
	if (strlen(pszUtf8) == 0)
		return _T("");
	char *pszAnsi;
	Utf8Decode(pszUtf8, &pszAnsi);
	CString szBuff = pszAnsi;
	free(pszAnsi);
	return szBuff;
}
#endif

bool XmlConfigurationBase::ToBool(const char *pszUtf8)
{
	return ToCString(pszUtf8).CompareNoCase(m_True) == 0;
}

int XmlConfigurationBase::ToInt(const char *pszUtf8)
{
	return _tstoi(ToCString(pszUtf8));
}

CString XmlConfigurationBase::ToCString(const int nValue)
{
	CString rValue;
	rValue.Format(_T("%d\0"), nValue);
	return rValue;
}

CString XmlConfigurationBase::ToCString(const bool bValue)
{
	CString rValue;
	rValue.Format(_T("%s\0"), bValue ? m_True : m_False);
	return rValue;
}
