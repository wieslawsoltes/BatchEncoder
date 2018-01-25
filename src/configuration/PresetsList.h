// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "utilities\ListT.h"
#include "Preset.h"

namespace config
{
    class CPresetsList : public util::CListT<CPreset>
    {
    };
}
