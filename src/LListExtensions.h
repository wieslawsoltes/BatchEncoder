//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2017 Wiesław Šoltés <wieslaw.soltes@gmail.com>
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

#pragma once

#include <afxtempl.h>

class CLListExtensions
{
private:
    typedef struct _EDIT_DATA_PRESET_EXT_
    {
        CString szPresetName;
        CString szPresetExtensions; // extesions seprated by '|'
    } LIST_DATA_PRESET_EXT, *PLIST_DATA_PRESET_EXT;
private:
    CList<LIST_DATA_PRESET_EXT, LIST_DATA_PRESET_EXT&> myList;
private:
    void SetData(LIST_DATA_PRESET_EXT listData, int idx)
    {
        myList.SetAt(myList.FindIndex(idx), listData);
    }

    LIST_DATA_PRESET_EXT GetData(int idx)
    {
        return myList.GetAt(myList.FindIndex(idx));
    }
public:
    CLListExtensions()
    {

    }
    virtual ~CLListExtensions()
    {
        if (myList.GetCount() != 0)
            myList.RemoveAll();
    }
public:
    bool IsEmpty()
    {
        return (myList.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)myList.GetCount();
    }
public:
    void InsertNode(CString szPresetName, CString szPresetExtensions = _T(""))
    {
        LIST_DATA_PRESET_EXT tmp;
        tmp.szPresetName = szPresetName;
        tmp.szPresetExtensions = szPresetExtensions;
        myList.AddTail(tmp);
    }
    void RemoveNode(int pstn = -1)
    {
        myList.RemoveAt(myList.FindIndex(pstn));
    }

    void RemoveAllNodes(void)
    {
        if (myList.GetCount() != 0)
            myList.RemoveAll();
    }
public:
    void SetPresetName(CString szPresetName, int idx)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetName = szPresetName;
        this->SetData(tmpList, idx);
    }
    CString GetPresetName(int idx)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szPresetName;
    }
public:
    void SetPresetExtensions(CString szPresetExtensions, int idx)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetExtensions = szPresetExtensions;
        this->SetData(tmpList, idx);
    }
    CString GetPresetExtensions(int idx)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szPresetExtensions;
    }
public:
    void AddPresetExtension(CString szExtension, int idx)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetExtensions += szExtension + '|';
        this->SetData(tmpList, idx);
    }
    CString GetPresetExtension(int idx, int ext)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);

        CString szExt = _T("");

        int nStart = 0, nEnd = 0, nCount = 0;
        do
        {
            nStart = nEnd;
            nEnd = tmpList.szPresetExtensions.Find('|', nEnd);
            if (nEnd != -1)
            {
                if (nCount == ext)
                {
                    szExt = tmpList.szPresetExtensions.Mid(nStart, nEnd - nStart);
                    break;
                }

                nCount++;
                nEnd++;
            }
            else
                break;
        } while (nEnd != -1);

        return szExt;
    }
    void DelPresetExtension(int idx, int ext)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);

        int nStart = 0, nEnd = 0, nCount = 0;
        do
        {
            nStart = nEnd;
            nEnd = tmpList.szPresetExtensions.Find('|', nEnd);
            if (nEnd == -1)
            {
                if (nCount == ext)
                {
                    tmpList.szPresetExtensions.Delete(nStart, nEnd - nStart);
                    this->SetData(tmpList, idx);
                    break;
                }

                nCount++;
                nEnd++;
            }
            else
                break;
        } while (nEnd != -1);
    }
    int GetExtensionsSize(int idx)
    {
        LIST_DATA_PRESET_EXT tmpList;
        tmpList = this->GetData(idx);

        int nEnd = 0, nCount = 0;
        do
        {
            nEnd = tmpList.szPresetExtensions.Find('|', nEnd);
            if (nEnd != -1)
            {
                nCount++;
                nEnd++;
            }
            else
            {
                break;
            }
        } while (nEnd != -1);

        return nCount;
    }
};
