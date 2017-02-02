// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CTimeCount
{
private:
    LARGE_INTEGER f[3];
public:
    CTimeCount()
    {
    };
    virtual ~CTimeCount()
    {
    };
public:
    void Start()
    {
        ZeroMemory(this->f, (3 * sizeof(LARGE_INTEGER)));
        ::QueryPerformanceFrequency(&this->f[0]);
        ::QueryPerformanceCounter(&this->f[1]);
    }
    void Stop()
    {
        ::QueryPerformanceCounter(&this->f[2]);
    }
    double ElapsedTime()
    {
        return (double)((this->f[2].QuadPart - this->f[1].QuadPart) / (double)this->f[0].QuadPart);
    }
};
