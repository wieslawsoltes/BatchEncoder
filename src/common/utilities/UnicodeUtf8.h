// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

unsigned char *MakeUtf8String(const wchar_t *unicode);

wchar_t *MakeUnicodeString(const unsigned char *utf8);

int UnicodeEncode(const char *from, wchar_t **to);

int UnicodeDecode(const wchar_t *from, char **to);

int Utf8Encode(const char *from, char **to);

int Utf8Decode(const char *from, char **to);
