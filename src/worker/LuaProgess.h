// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxwin.h>
#include "lua\lua\src\lua.hpp"

class CLuaProgess
{
    lua_State *L = NULL;
public:
    const char *name = "GetProgress";
public:
    CLuaProgess()
    {
    }
    virtual ~CLuaProgess()
    {
        Close();
    }
private:
    void OutputError(lua_State* L)
    {
        const char* message = lua_tostring(L, -1);
        OutputDebugStringA(message);
        lua_pop(L, 1);
    }
public:
    bool Open(const char *filename)
    {
        L = luaL_newstate();
        if (L == NULL)
            return false;

        luaL_openlibs(L); // [-0, +0, e]

        if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) // [-0, +1, m], [-(nargs + 1), +(nresults|1), –]
        {
            OutputError(L);
            return false;
        }

        return true;
    }
    bool HaveGetProgress()
    {
        if (L == NULL)
            return false;

        lua_getglobal(L, name); // [-0, +1, e]
        if (!lua_isfunction(L, -1)) // [-0, +0, –]
        {
            OutputError(L);
            lua_pop(L, 1); // lua_getglobal
            return false;
        }

        lua_pop(L, 1); // lua_getglobal

        return true;
    }
    double GetProgress(const char *szLineBuff, size_t nLineLen)
    {
        if (L == NULL)
            return -1;

        lua_getglobal(L, name); // [-0, +1, e]
        if (!lua_isfunction(L, -1)) // [-0, +0, –]
        {
            OutputError(L);
            lua_pop(L, 1); // lua_getglobal
            return -1;
        }

        lua_pushlstring(L, szLineBuff, nLineLen); // [-0, +1, m]

        if (lua_pcall(L, 1, 1, 0) != LUA_OK) // [-(nargs + 1), +(nresults|1), –]
        {
            OutputError(L);
            return -1;
        }

        if (lua_isnil(L, -1) == 1)  // [-0, +0, –]
        {
            OutputError(L);
            return -1;
        }

        if (!lua_isstring(L, -1))  // [-0, +0, –]
        {
            OutputError(L);
            return -1;
        }

        int isnum;
        double result = lua_tonumberx(L, -1, &isnum); // [-0, +0, –]
        if (isnum != 1)
        {
            OutputError(L);
            return -1;
        }

        return result;
    }
    void Close()
    {
        if (L != NULL)
        {
            lua_close(L);
            L = NULL;
        }
    }
};
