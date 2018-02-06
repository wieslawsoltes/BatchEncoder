// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

namespace worker
{
    class CLuaProgess
    {
        sol::state lua;
        sol::protected_function f;
    public:
        CLuaProgess()
        {
            lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);
        }
        virtual ~CLuaProgess() { }
    public:
        bool Open(const char *filename)
        {
            try
            {
                auto result = lua.script_file(filename);
                if (result.valid())
                    return true;
            }
            catch (...) {}
            return false;
        }
        bool Init()
        {
            try
            {
                f = lua["GetProgress"];
                if (f.valid())
                    return true;
            }
            catch (...) {}
            return false;
        }
        double GetProgress(const char *szLine)
        {
            try
            {
                auto result = f(szLine);
                if (result.valid())
                {
                    if (result.get_type() == sol::type::string)
                        return (double)result;
                }
            }
            catch (...) {}
            return -1;
        }
    };
}
