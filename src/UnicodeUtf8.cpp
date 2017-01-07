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

/* $Id: UnicodeUtf8.cpp 437 2008-03-24 12:15:45Z wisodev $ */

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "UnicodeUtf8.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// UNICODE to UTF8

//
// use free(...) to release memory (returned buffer pointer) and avoid memory leaks
//
// WideCharToMultiByte()
//
unsigned char *MakeUtf8String(const wchar_t *unicode)
{
    int size = 0, index = 0, out_index = 0;
    unsigned char *out;
    unsigned short c;

    c = unicode[index++];
    while(c) 
    {
        if(c < 0x0080) 
            size += 1;
        else if(c < 0x0800) 
            size += 2;
        else 
            size += 3;

        c = unicode[index++];
    }	

    out = (unsigned char*) malloc(size + 1);
    if (out == NULL)
        return NULL;
    index = 0;

    c = unicode[index++];
    while(c)
    {
        if(c < 0x080) 
        {
            out[out_index++] = (unsigned char)c;
        } 
        else if(c < 0x800) 
        {
            out[out_index++] = 0xc0 | (c >> 6);
            out[out_index++] = 0x80 | (c & 0x3f);
        } 
        else 
        {
            out[out_index++] = 0xe0 | (c >> 12);
            out[out_index++] = 0x80 | ((c >> 6) & 0x3f);
            out[out_index++] = 0x80 | (c & 0x3f);
        }
        c = unicode[index++];
    }

    out[out_index] = 0x00;
    return out;
}

// UTF8 to UNICODE

//
// use free(...) to release memory (returned buffer pointer) and avoid memory leaks
//
// MultiByteToWideChar(CP_UTF8)
//
wchar_t *MakeUnicodeString(const unsigned char *utf8)
{
    int size = 0, index = 0, out_index = 0;
    wchar_t *out;
    unsigned char c;

    c = utf8[index++];
    while(c) 
    {
        if((c & 0x80) == 0) 
            index += 0; 
        else if((c & 0xe0) == 0xe0) 
            index += 2;
        else 
            index += 1;

        size += 1;
        c = utf8[index++];
    }	

    out = (wchar_t *) malloc((size + 1) * sizeof(wchar_t));
    if(out == NULL)
        return NULL;
    index = 0;

    c = utf8[index++];
    while(c)
    {
        if((c & 0x80) == 0) 
        {
            out[out_index++] = c;
        } 
        else if((c & 0xe0) == 0xe0) 
        {
            out[out_index] = (c & 0x1F) << 12;
	        c = utf8[index++];
            out[out_index] |= (c & 0x3F) << 6;
	        c = utf8[index++];
            out[out_index++] |= (c & 0x3F);
        } 
        else 
        {
            out[out_index] = (c & 0x3F) << 6;
	        c = utf8[index++];
            out[out_index++] |= (c & 0x3F);
        }
        c = utf8[index++];
    }

    out[out_index] = 0;
    return out;
}

// ANSI (char) to UNICODE

//
// use free(...) to release 'to' buffer after using it
//
int UnicodeEncode(const char *from, wchar_t **to)
{
	wchar_t *unicode;
	int wchars, err;

	wchars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, from, (int) strlen(from), NULL, 0);
	if(wchars == 0)
		return -1;

	unicode = (wchar_t *) calloc(wchars + 1, sizeof(unsigned short));
	if(unicode == NULL) 
		return -1;

    err = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, from, (int) strlen(from), unicode, wchars);
	if(err != wchars)
	{
        *to = NULL;
		free(unicode);
		return -1;
	}

	*to = unicode;
	return 0;
}

// UNICODE to ANSI (char)

//
// use free(...) to release 'to' buffer after using it
//
int UnicodeDecode(const wchar_t *from, char **to)
{
    int chars, err;

    chars = ::WideCharToMultiByte(GetConsoleCP(), WC_COMPOSITECHECK, from, -1, NULL, 0, NULL, NULL);
    if(chars == 0)
        return -1;

    *to = (char *) calloc(chars + 1, sizeof(unsigned char));
    if(*to == NULL) 
        return -1;

    err = ::WideCharToMultiByte(GetConsoleCP(), WC_COMPOSITECHECK, from, -1, *to, chars, NULL, NULL);
    if(err != chars)
    {
        free(*to);
        *to = NULL;
        return -1;
    }

    return 0;
}

//
// use free(...) to release 'to' buffer after using it
//
int Utf8Encode(const char *from, char **to)
{
	wchar_t *unicode;
	int wchars, err;

	wchars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, from, (int) strlen(from), NULL, 0);
	if(wchars == 0)
		return -1;

	unicode = (wchar_t *) calloc(wchars + 1, sizeof(unsigned short));
	if(unicode == NULL) 
		return -1;

    err = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, from, (int) strlen(from), unicode, wchars);
	if(err != wchars)
	{
		free(unicode);
		return -1;
	}

	*to = (char *) MakeUtf8String(unicode);
	free(unicode);
	return 0;
}

//
// use free(...) to release 'to' buffer after using it
//
int Utf8Decode(const char *from, char **to)
{
    wchar_t *unicode;
    int chars, err;

    unicode = MakeUnicodeString((unsigned char *) from);
    if(unicode == NULL) 
        return -1;

    chars = ::WideCharToMultiByte(GetConsoleCP(), WC_COMPOSITECHECK, unicode, -1, NULL, 0, NULL, NULL);
    if(chars == 0)
    {
        free(unicode);
        return -1;
    }

    *to = (char *) calloc(chars + 1, sizeof(unsigned char));
    if(*to == NULL) 
    {
        free(unicode);
        return -1;
    }

    err = ::WideCharToMultiByte(GetConsoleCP(), WC_COMPOSITECHECK, unicode, -1, *to, chars, NULL, NULL);
    if(err != chars)
    {
        free(unicode);
        free(*to);
        *to = NULL;
        return -1;
    }

    free(unicode);
    return 0;
}
