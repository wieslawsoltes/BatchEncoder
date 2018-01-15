// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

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
        auto result = lua.script_file(filename);
        if (result.valid())
            return true;
        return false;
    }
    bool Init()
    {
        f = lua["GetProgress"];
        if (f.valid())
            return true;
        return false;
    }
    double GetProgress(const char *szLine)
    {
        auto result = f(szLine);
        if (result.valid()) 
        {
            if (result.get_type() == sol::type::string)
                return (double)result;
        }
        return -1;
    }
};
