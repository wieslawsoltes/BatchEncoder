// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxtempl.h>

class CLListExtensions
{
private:
    typedef struct _EDIT_DATA_PRESET_EXT_
    {
        CString szName;
        CString szPresetExtensions; // extesions seprated by '|'
    } CPreset_EXT, *PCPreset_EXT;
private:
    CList<CPreset_EXT, CPreset_EXT&> myList;
private:
    void SetData(CPreset_EXT listData, int idx)
    {
        myList.SetAt(myList.FindIndex(idx), listData);
    }

    CPreset_EXT GetData(int idx)
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
    void InsertNode(CString szName, CString szPresetExtensions = _T(""))
    {
        CPreset_EXT tmp;
        tmp.szName = szName;
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
    void SetPresetName(CString szName, int idx)
    {
        CPreset_EXT tmpList;
        tmpList = this->GetData(idx);
        tmpList.szName = szName;
        this->SetData(tmpList, idx);
    }
    CString GetPresetName(int idx)
    {
        CPreset_EXT tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szName;
    }
public:
    void SetPresetExtensions(CString szPresetExtensions, int idx)
    {
        CPreset_EXT tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetExtensions = szPresetExtensions;
        this->SetData(tmpList, idx);
    }
    CString GetPresetExtensions(int idx)
    {
        CPreset_EXT tmpList;
        tmpList = this->GetData(idx);
        return tmpList.szPresetExtensions;
    }
public:
    void AddPresetExtension(CString szExtension, int idx)
    {
        CPreset_EXT tmpList;
        tmpList = this->GetData(idx);
        tmpList.szPresetExtensions += szExtension + '|';
        this->SetData(tmpList, idx);
    }
    CString GetPresetExtension(int idx, int ext)
    {
        CPreset_EXT tmpList;
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
        CPreset_EXT tmpList;
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
        CPreset_EXT tmpList;
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
