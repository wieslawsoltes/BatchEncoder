// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include <lua.hpp>

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "sol.hpp"

class CLuaProgess
{
    sol::state lua;
    sol::protected_function f;
public:
    const char *name = "GetProgress";
public:
    CLuaProgess() 
    { 
        lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);
        lua.open_libraries(sol::lib::base);
    }
    virtual ~CLuaProgess() { }
public:
    bool Open(const char *filename)
    {
        auto result = lua.script_file(filename);
        if (result.valid())
        {
            return true;
        }
        else
        {
            sol::error err = result;
            OutputDebugStringA(err.what());
            return false;
        }
    }
    bool Valid()
    {
        f = lua[name];
        if (f.valid())
        {
            return true;
        }
        else
        {
            OutputDebugStringA("Failed to get progress function from script.");
            return false;
        }
    }
    double GetProgress(const char *szLine)
    {
        auto result = f(szLine);
        if (result.valid()) 
        {
            auto type = result.get_type();
            if (type == sol::type::string)
                return (double)result;
            else
                return -1;
        }
        else 
        {
            sol::error err = result;
            OutputDebugStringA(err.what());
            return -1;
        }
    }
};
