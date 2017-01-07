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

/* $Id: TimeCount.h 437 2008-03-24 12:15:45Z wisodev $ */

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
        this->fTime = (double)((this->liCount[2].QuadPart-this->liCount[1].QuadPart)
            /(double)this->liCount[0].QuadPart);

        return(this->fTime);
    }
};
