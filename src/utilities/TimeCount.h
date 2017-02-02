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
public:
    CString Format(double fTime, int nFormat)
    {
        CString szTime = _T("");
        DWORD dwTime[5] = { 0, 0, 0, 0, 0 }; // DD HH MM SS MS

        dwTime[0] = (DWORD)fTime / (24 * 60 * 60); // DD -> [days]
        dwTime[1] = ((DWORD)fTime - (dwTime[0] * (24 * 60 * 60))) / (60 * 60); // HH -> [h]
        dwTime[2] = ((DWORD)fTime - ((dwTime[0] * (24 * 60 * 60)) + (dwTime[1] * (60 * 60)))) / 60; // MM -> [m]
        dwTime[3] = ((DWORD)fTime - ((dwTime[0] * (24 * 60 * 60)) + (dwTime[1] * (60 * 60)) + (dwTime[2] * 60))); // SS -> [s]
        dwTime[4] = (DWORD)(((double)fTime - (DWORD)fTime) * (double) 1000.1); // MS -> [ms]

        if (nFormat == 0)
        {
            // display simple time
            szTime.Format(_T("%0.3f"), fTime);
        }
        else if (nFormat == 1)
        {
            // exclude days if not used
            if (dwTime[0] != 0)
            {
                szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld.%03ld"),
                    dwTime[0], dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
            }
            else
            {
                szTime.Format(_T("%02ld:%02ld:%02ld.%03ld"),
                    dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
            }
        }
        else if (nFormat == 2)
        {
            // exclude unused values from time display
            if (dwTime[0] != 0)
            {
                szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld.%03ld"),
                    dwTime[0], dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
            }
            else if ((dwTime[0] == 0) && (dwTime[1] != 0))
            {
                szTime.Format(_T("%02ld:%02ld:%02ld.%03ld"),
                    dwTime[1], dwTime[2], dwTime[3], dwTime[4]);
            }
            else if ((dwTime[0] == 0) && (dwTime[1] == 0) && (dwTime[2] != 0))
            {
                szTime.Format(_T("%02ld:%02ld.%03ld"),
                    dwTime[2], dwTime[3], dwTime[4]);
            }
            else if ((dwTime[0] == 0) && (dwTime[1] == 0) && (dwTime[2] == 0) && (dwTime[3] != 0))
            {
                szTime.Format(_T("%02ld.%03ld"),
                    dwTime[3], dwTime[4]);
            }
            else
            {
                szTime.Format(_T("%03ld"),
                    dwTime[4]);
            }
        }
        else if (nFormat == 3)
        {
            // exclude days if not used and don't show milliseconds
            if (dwTime[0] != 0)
            {
                szTime.Format(_T("(%02ld:%02ld:%02ld:%02ld"),
                    dwTime[0], dwTime[1], dwTime[2], dwTime[3]);
            }
            else
            {
                szTime.Format(_T("%02ld:%02ld:%02ld"),
                    dwTime[1], dwTime[2], dwTime[3]);
            }
        }

        return szTime;
    }
};
