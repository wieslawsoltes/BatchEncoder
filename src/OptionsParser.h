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

/* $Id: OptionsParser.h 437 2008-03-24 12:15:45Z wisodev $ */

#pragma once

// My simple command-line parser implementation COptionsParser.
//
// COptionsParser supports ANSI and UNICODE chars, 
// use TCHAR type in your program for strings.
//
// Long (--) and short (-,/) format of options is supported,
// for long use two or more chars and for short only one char for name.
//
// Uncomment '#define USE_PARSER_TOKENS' to use multiple tokens 
// for each option name as "option;o;opt;opt", available tokens: " ;|,\t".
//
// You need to remove all tokens from configuration 
// if you disable token parsing.

#define USE_PARSER_TOKENS

class COptionsParser
{
public:
    const static int NEXT_LAST_OPTION = -1;
    const static int NEXT_INVALID_ARG = -2;
public:
    struct CL_OPTIONS
    {
        LPTSTR szOptionName;
        int nOptionId;
        int nParams; // 0 = not used, -1 = infinite
        bool bParamsRequired;
    };
public:
    COptionsParser(void);
    COptionsParser(int nArgc, LPTSTR *pArgv);
    COptionsParser(const CL_OPTIONS *pclOptions);
    COptionsParser(int nArgc, LPTSTR *pArgv, const CL_OPTIONS *pclOptions);
    virtual ~COptionsParser();
protected:
    volatile bool bInit;
protected:
    bool bHaveArgv;
    bool bHaveOptions;
protected:
    int nArgc;
    LPTSTR *pArgv;
    const CL_OPTIONS *pclOptions;
protected:
    bool InitDone(void);
public:
    bool Init(int nArgc, LPTSTR *pArgv);
    bool Init(const CL_OPTIONS *pclOptions);
    bool Init(int nArgc, LPTSTR *pArgv, const CL_OPTIONS *pclOptions);
public:
    void Clean(void);
protected:
    int nPosArgv;
    int nParams;
    int nParamsPos;
    int nOptionId;
protected:
    bool IsOpt(LPTSTR szOption);
    bool IsLongOpt(LPTSTR szOption);
    bool IsShortOpt(LPTSTR szOption);
public:
    int Next(void);
    bool GetParam(CString &szParam, int nNum = 0);
};
