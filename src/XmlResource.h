// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

LPVOID LoadXmlResource(LPCTSTR lpType, int nResID, PINT64 lpdwSize);
VOID FreeXmlResource(LPVOID lpvBuf);
