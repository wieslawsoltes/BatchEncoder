// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CTimeCount
{
private:
    LARGE_INTEGER liCount[3];
    double fTime;
public:
    CTimeCount(void)
    {
        this->InitCounter();
    };
    virtual ~CTimeCount(void) { };
public:
    void InitCounter(void)
    {
        this->fTime = 0.0;
        ZeroMemory(this->liCount, (3 * sizeof(LARGE_INTEGER)));
        ::QueryPerformanceFrequency(&this->liCount[0]);
    }
    void StartCounter(void)
    {
        ::QueryPerformanceCounter(&this->liCount[1]);
    }
    void StopCounter(void)
    {
        ::QueryPerformanceCounter(&this->liCount[2]);
    }
    double GetTime(void)
    {
        this->fTime = (double)((this->liCount[2].QuadPart - this->liCount[1].QuadPart)
            / (double)this->liCount[0].QuadPart);

        return(this->fTime);
    }
};
