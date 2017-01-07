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

#pragma once

unsigned char *MakeUtf8String(const wchar_t *unicode);
wchar_t *MakeUnicodeString(const unsigned char *utf8);

int UnicodeEncode(const char *from, wchar_t **to);
int UnicodeDecode(const wchar_t *from, char **to);

int Utf8Encode(const char *from, char **to);
int Utf8Decode(const char *from, char **to);
