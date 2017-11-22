// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "OptionsParser.h"

COptionsParser::COptionsParser()
{
    this->Clean();
}

COptionsParser::COptionsParser(int nArgc, LPTSTR *pArgv)
{
    (void) this->Init(nArgc, pArgv);
}

COptionsParser::COptionsParser(const CL_OPTIONS *pclOptions)
{
    (void) this->Init(pclOptions);
}

COptionsParser::COptionsParser(int nArgc, LPTSTR *pArgv, const CL_OPTIONS *pclOptions)
{
    (void) this->Init(nArgc, pArgv, pclOptions);
}

COptionsParser::~COptionsParser(void)
{
    this->Clean();
}

bool COptionsParser::InitDone(void)
{
    if ((this->bInit == true) && (this->pArgv != NULL))
        return true;

    if ((this->bHaveArgv == true) &&
        (this->bHaveOptions == true))
    {
        this->nPosArgv = 0;
        this->nParams = 0;
        this->nParamsPos = -1;
        this->nOptionId = -2;
        this->bInit = true;

        return true;
    }
    return false;
}

bool COptionsParser::Init(int nArgc, LPTSTR *pArgv)
{
    if ((pArgv != NULL) && (nArgc >= 2))
    {
        this->nArgc = nArgc;
        this->pArgv = pArgv;
        this->bHaveArgv = true;
    }

    return this->InitDone();
}

bool COptionsParser::Init(const CL_OPTIONS *pclOptions)
{
    if (pclOptions != NULL)
    {
        this->pclOptions = pclOptions;
        this->bHaveOptions = true;
    }

    return this->InitDone();
}

bool COptionsParser::Init(int nArgc, LPTSTR *pArgv, const CL_OPTIONS *pclOptions)
{
    if ((pArgv != NULL) && (nArgc >= 2))
    {
        this->nPosArgv = 0;
        this->nArgc = nArgc;
        this->pArgv = pArgv;
        this->bHaveArgv = true;
    }

    if (pclOptions != NULL)
    {
        this->pclOptions = pclOptions;
        this->bHaveOptions = true;
    }

    return this->InitDone();
}

void COptionsParser::Clean(void)
{
    this->nPosArgv = -1;
    this->nParams = -1;
    this->nParamsPos = -1;
    this->nOptionId = -2;

    this->nArgc = -1;
    this->pArgv = NULL;
    this->pclOptions = NULL;

    bool bHaveArgv = false;
    bool bHaveOptions = false;

    this->bInit = false;
}

bool COptionsParser::IsOpt(LPTSTR szOption)
{
    if (szOption == NULL)
        return false;

    if (this->IsLongOpt(szOption) == true)
        return true;
    else if (this->IsShortOpt(szOption) == true)
        return true;
    else
        return false;
}

bool COptionsParser::IsLongOpt(LPTSTR szOption)
{
    if (szOption == NULL)
        return false;

    // is argument in long option format: --option, --o, --opt, etc...
    if (lstrlen(szOption) >= 3)
    {
        if ((szOption[0] == '-') && (szOption[1] == '-'))
            return true;
        else
            return false;
    }

    return false;
}

bool COptionsParser::IsShortOpt(LPTSTR szOption)
{
    if (szOption == NULL)
        return false;

    // is argument in short option format: -o, /o, etc...
    if (lstrlen(szOption) == 2)
    {
        if ((szOption[0] == '-') || (szOption[0] == '/'))
            return true;
        else
            return false;
    }

    return false;
}

int COptionsParser::Next(void)
{
    // Return values:
    // 0...N - valid option
    // -1    - finished
    // -2    - error (invalid option or param)

    if (this->InitDone() == false)
        return(-2); // ERROR

    // clean last options status
    this->nParams = 0;
    this->nParamsPos = -1;
    this->nOptionId = -2;

    // increment args counter
    this->nPosArgv++;

    // check if are at end of args
    if (this->nPosArgv >= nArgc)
        return(-1); // SUCCESS

    LPTSTR szOption = NULL;
    szOption = this->pArgv[this->nPosArgv];

    // get valid option name
    if (this->IsLongOpt(szOption) == true)
    {
        // long option format
        szOption = (LPTSTR)(szOption + 2);
    }
    else if (this->IsShortOpt(szOption) == true)
    {
        // short option format
        szOption = (LPTSTR)(szOption + 1);
    }
    else
    {
        // invalid option format
        return(-2); // ERROR
    }

    int nOptionId = -2;
    int i = 0;

#ifdef USE_PARSER_TOKENS
    const TCHAR szTokenSeps[] = TEXT(" ;|,\t");
#endif // USE_PARSER_TOKENS

    while (pclOptions[i].szOptionName != NULL)
    {
#ifdef USE_PARSER_TOKENS
        LPTSTR szOptionName = NULL;
        size_t nLen = _tcslen(pclOptions[i].szOptionName);
        szOptionName = (LPTSTR)malloc((nLen + 1) * sizeof(TCHAR));
        if (szOptionName == NULL)
            return(-2); // ERROR

        memcpy(szOptionName, pclOptions[i].szOptionName, nLen * sizeof(TCHAR));
        szOptionName[nLen] = '\0';
        LPTSTR szToken = _tcstok(szOptionName, szTokenSeps);
        while (szToken != NULL)
        {
            // check next token
            if (lstrcmp(szOption, szToken) == 0)
#else
        if (lstrcmp(szOption, pclOptions[i].szOptionName) == 0)
#endif // USE_PARSER_TOKENS
        {
            nOptionId = pclOptions[i].nOptionId;
            if (pclOptions[i].bParamsRequired == false)
            {
                if (pclOptions[i].nParams == 1)
                {
                    // check if we have 1 param that is not required 
                    if ((this->nPosArgv + 1) >= nArgc)
                    {
                        // no more args available
                        this->nParams = 0;
                        this->nParamsPos = -1;
                    }
                    else
                    {
                        // if next arg is not an option then we have 1 param
                        if (this->IsOpt(this->pArgv[this->nPosArgv + 1]) == false)
                        {
                            this->nParams = 1;
                            this->nParamsPos = this->nPosArgv + 1;

                            // update position
                            this->nPosArgv++;
                        }
                    }
                }

#ifdef USE_PARSER_TOKENS
                free(szOptionName);
#endif // USE_PARSER_TOKENS

                // SUCCESS
                this->nOptionId = nOptionId;
                return nOptionId;
            }
            else
            {
                // check if we have all required params
                if (pclOptions[i].nParams > 0)
                {
                    // check if we have all params 
                    if ((this->nPosArgv + pclOptions[i].nParams) >= nArgc)
                    {
#ifdef USE_PARSER_TOKENS
                        free(szOptionName);
#endif // USE_PARSER_TOKENS

                        // out off args
                        return(-2); // ERROR
                    }

                    // check if all params are valid
                    for (int j = 0; j < pclOptions[i].nParams; j++)
                    {
                        if (this->IsOpt(this->pArgv[this->nPosArgv + 1 + j]) == true)
                        {
#ifdef USE_PARSER_TOKENS
                            free(szOptionName);
#endif // USE_PARSER_TOKENS
                            return(-2); // ERROR
                        }
                    }

                    // we have all params
                    this->nParams = pclOptions[i].nParams;
                    this->nParamsPos = this->nPosArgv + 1;

                    // update position
                    this->nPosArgv += this->nParams;
                }

#ifdef USE_PARSER_TOKENS
                free(szOptionName);
#endif // USE_PARSER_TOKENS

                // SUCCESS
                this->nOptionId = nOptionId;
                return nOptionId;
            }
        }

#ifdef USE_PARSER_TOKENS
        // get next token
        szToken = _tcstok(NULL, szTokenSeps);
        }

    // free memory used for tokens options
    free(szOptionName);
#endif // USE_PARSER_TOKENS

    // check next option
    i++;
    }

return(-2); // ERROR
}

bool COptionsParser::GetParam(CString &szParam, int nNum)
{
    if (this->InitDone() == false)
        return false;

    if ((this->nParams == 0) || (this->nParamsPos < 0))
        return false;

    if ((nNum < 0) || (nNum >= this->nParams))
        return false;

    szParam = this->pArgv[this->nParamsPos + nNum];
    return true;
}
