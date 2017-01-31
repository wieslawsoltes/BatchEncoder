// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <afxstr.h>
#include <afxtempl.h>
#include "Language.h"

class CLanguagesList
{
public:
    CList<CLanguage, const CLanguage&> m_Languages;
public:
    void SetData(CLanguage& language, int idx)
    {
        m_Languages.SetAt(m_Languages.FindIndex(idx), language);
    }
    CLanguage& GetData(int idx)
    {
        return m_Languages.GetAt(m_Languages.FindIndex(idx));
    }
public:
    CLanguagesList()
    {
    }
    CLanguagesList(const CLanguagesList &other)
    {
        Copy(other);
    }
    CLanguagesList& operator=(const CLanguagesList &other)
    {
        Copy(other);
        return *this;
    }
    virtual ~CLanguagesList()
    {
        if (m_Languages.GetCount() != 0)
            m_Languages.RemoveAll();
    }
public:
    void Copy(const CLanguagesList &other)
    {
        this->RemoveAllNodes();
        int nLanguages = (int)other.m_Languages.GetCount();
        for (int i = 0; i < nLanguages; i++)
        {
            CLanguage language = other.m_Languages.GetAt(other.m_Languages.FindIndex(i));
            this->InsertNode(language);
        }
    }
public:
    bool IsEmpty()
    {
        return (m_Languages.GetCount() == 0) ? true : false;
    }
    int GetSize()
    {
        return (int)m_Languages.GetCount();
    }
public:
    void InsertNode(CLanguage &language)
    {
        m_Languages.AddTail(language);
    }
public:
    void RemoveNode(int pstn = -1)
    {
        m_Languages.RemoveAt(m_Languages.FindIndex(pstn));
    }
    void RemoveAllNodes(void)
    {
        if (m_Languages.GetCount() != 0)
            m_Languages.RemoveAll();
    }
public:
    void Copy(CLanguagesList& other)
    {
        int nLanguages = this->GetSize();
        for (int i = 0; i < nLanguages; i++)
        {
            CLanguage& language = this->GetData(i);
            CLanguage copy = language;
            other.InsertNode(copy);
        }
    }
public:
    void SwapItems(int idx1, int idx2)
    {
        if ((idx1 < 0) || (idx2 < 0) || (idx1 >= GetSize()) || (idx2 >= GetSize()))
            return;
        CLanguage language1 = this->GetData(idx1);
        CLanguage language2 = this->GetData(idx2);
        this->SetData(language1, idx2);
        this->SetData(language2, idx1);
    }
public:
    int GetLanguageById(CString szLanguageId)
    {
        int nCount = this->GetSize();
        for (int i = 0; i < nCount; i++)
        {
            CLanguage& language = this->GetData(i);
            if (szLanguageId.CompareNoCase(language.szId) == 0)
                return i;
        }
        return -1;
    }
};
