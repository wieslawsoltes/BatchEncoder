﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <string.h>
#include <vector>
#include "tinyxml2\tinyxml2.h" // https://github.com/leethomason/tinyxml2
#include "utilities\StringHelper.h"
#include "utilities\Utf8String.h"
#include "config\Item.h"
#include "config\Format.h"
#include "config\Language.h"
#include "config\Options.h"
#include "config\Path.h"
#include "config\Preset.h"
#include "config\Tool.h"

#define VALIDATE(value) if (!value) return false

namespace xml
{
    typedef tinyxml2::XMLNode XmlNode;
    typedef tinyxml2::XMLElement XmlElement;
    typedef tinyxml2::XMLDocument XmlDocumnent;

    class XmlDoc
    {
    protected:
        XmlDocumnent & m_Document;
    public:
        XmlDoc(XmlDocumnent &doc) : m_Document(doc) { }
        virtual ~XmlDoc() { }
    public:
        static const char* GetRootName(XmlDocumnent &doc)
        {
            auto root = doc.RootElement();
            if (root != nullptr)
                return root->Name();
            return nullptr;
        }
    public:
        static void Create(XmlDocumnent & doc)
        {
            doc.LinkEndChild(doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
        }
        static bool Open(const std::wstring& szFileName, XmlDocumnent & doc)
        {
            CStdioFile fp;
            if (fp.Open(szFileName.c_str(), CFile::modeRead | CFile::typeBinary) == TRUE)
            {
                auto result = doc.LoadFile(fp.m_pStream);
                fp.Close();
                return result == tinyxml2::XMLError::XML_SUCCESS;
            }
            return false;
        }
        static bool Save(const std::wstring& szFileName, XmlDocumnent & doc)
        {
            CStdioFile fp;
            if (fp.Open(szFileName.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::typeText) == TRUE)
            {
                fputc(0xefU, fp.m_pStream);
                fputc(0xbbU, fp.m_pStream);
                fputc(0xbfU, fp.m_pStream);
                tinyxml2::XMLPrinter printer(fp.m_pStream);
                doc.Print(&printer);
                fp.Close();
                return true;
            }
            return false;
        }
    public:
        const std::wstring m_True = L"true";
        const std::wstring m_False = L"false";
    public:
        const std::wstring ToString(const char *pszUtf8)
        {
            if (pszUtf8 == nullptr)
                return std::wstring();
            if (strlen(pszUtf8) == 0)
                return std::wstring();
            return util::CUtf8String::ToUnicode(pszUtf8);
        }
        int ToInt(const char *pszUtf8)
        {
            return std::stoi(pszUtf8);
        }
        size_t ToUInt(const char *pszUtf8)
        {
            return std::stoul(pszUtf8);
        }
        unsigned __int64 ToUInt64(const char *pszUtf8)
        {
           return std::strtoull(pszUtf8, nullptr, 10);
        }
        bool ToBool(const char *pszUtf8)
        {
            return _wcsicmp(ToString(pszUtf8).c_str(), m_True.c_str()) == 0;
        }
        std::wstring IntToString(const int nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring SizeToString(const size_t nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring UInt64ToString(const unsigned __int64 nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring BoolToString(const bool bValue)
        {
            return bValue ? m_True : m_False;
        }
    public:
        bool GetAttributeValueString(const XmlElement *element, const char *name, std::wstring *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToString(pszResult).c_str();
                return true;
            }
            return false;
        }
        bool GetAttributeValueInt(const XmlElement *element, const char *name, int *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToInt(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValueSizeT(const XmlElement *element, const char *name, size_t *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToUInt(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValueUInt64(const XmlElement *element, const char *name, unsigned __int64 *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToUInt64(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValueBool(const XmlElement *element, const char *name, bool *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToBool(pszResult);
                return true;
            }
            return false;
        }
    public:
        void SetAttributeValueString(XmlElement *element, const char *name, const std::wstring& value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(value).c_str());
        }
        void SetAttributeValueInt(XmlElement *element, const char *name, const int &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(IntToString(value)).c_str());
        }
        void SetAttributeValueSizeT(XmlElement *element, const char *name, const size_t &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(SizeToString(value)).c_str());
        }
        void SetAttributeValueUInt64(XmlElement *element, const char *name, const unsigned __int64 &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(UInt64ToString(value)).c_str());
        }
        void SetAttributeValueBool(XmlElement *element, const char *name, const bool &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(BoolToString(value)).c_str());
        }
    public:
        bool GetChildValueString(const XmlElement *parent, const char *name, std::wstring *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToString(element->GetText()).c_str();
                return true;
            }
            return false;
        }
        bool GetChildValueInt(const XmlElement *parent, const char *name, int *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToInt(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValueSizeT(const XmlElement *parent, const char *name, size_t *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToUInt(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValueUInt64(const XmlElement *parent, const char *name, unsigned __int64 *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToUInt64(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValueBool(const XmlElement *parent, const char *name, bool *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToBool(element->GetText());
                return true;
            }
            return false;
        }
    public:
        void SetChildValueString(XmlElement *parent, const char *name, const std::wstring& value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(value).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValueInt(XmlElement *parent, const char *name, const int &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(IntToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValueSizeT(XmlElement *parent, const char *name, const size_t &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(SizeToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValueUInt64(XmlElement *parent, const char *name, const unsigned __int64 &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(UInt64ToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValueBool(XmlElement *parent, const char *name, const bool &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(BoolToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
    public:
        XmlElement * NewElement(const char *name)
        {
            return m_Document.NewElement(name);
        }
        XmlElement* FirstChildElement(const char *name)
        {
            return m_Document.FirstChildElement(name);
        }
        XmlNode* LinkEndChild(XmlNode* node)
        {
            return m_Document.LinkEndChild(node);
        }
    public:
        void Create()
        {
            XmlDoc::Create(m_Document);
        }
        bool Open(const std::wstring& szFileName)
        {
            return XmlDoc::Open(szFileName, m_Document);
        }
        bool Save(const std::wstring& szFileName)
        {
            return XmlDoc::Save(szFileName, m_Document);
        }
    };

    class XmlOptions : public XmlDoc
    {
    public:
        XmlOptions(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlOptions() { }
    public:
        bool GetOptions(const XmlElement *element, config::COptions &m_Options)
        {
            VALIDATE(GetChildValueString(element, "SelectedLanguage", &m_Options.szSelectedLanguage));
            VALIDATE(GetChildValueSizeT(element, "SelectedFormat", &m_Options.nSelectedFormat));
            VALIDATE(GetChildValueString(element, "OutputPath", &m_Options.szOutputPath));
            VALIDATE(GetChildValueBool(element, "DeleteSourceFiles", &m_Options.bDeleteSourceFiles));
            VALIDATE(GetChildValueBool(element, "RecurseChecked", &m_Options.bRecurseChecked));
            VALIDATE(GetChildValueBool(element, "ShutdownWhenFinished", &m_Options.bShutdownWhenFinished));
            VALIDATE(GetChildValueBool(element, "DoNotSaveConfiguration", &m_Options.bDoNotSaveConfiguration));
            VALIDATE(GetChildValueBool(element, "DeleteOnErrors", &m_Options.bDeleteOnErrors));
            VALIDATE(GetChildValueBool(element, "StopOnErrors", &m_Options.bStopOnErrors));
            VALIDATE(GetChildValueBool(element, "HideConsoleWindow", &m_Options.bHideConsoleWindow));
            VALIDATE(GetChildValueBool(element, "TryToFindDecoder", &m_Options.bTryToFindDecoder));
            VALIDATE(GetChildValueBool(element, "EnsureItemIsVisible", &m_Options.bEnsureItemIsVisible));
            VALIDATE(GetChildValueBool(element, "ValidateInputFiles", &m_Options.bValidateInputFiles));
            VALIDATE(GetChildValueBool(element, "OverwriteExistingFiles", &m_Options.bOverwriteExistingFiles));
            VALIDATE(GetChildValueBool(element, "TryToDownloadTools", &m_Options.bTryToDownloadTools));
            VALIDATE(GetChildValueInt(element, "ThreadCount", &m_Options.nThreadCount));
            VALIDATE(GetChildValueString(element, "OutputBrowse", &m_Options.szOutputBrowse));
            VALIDATE(GetChildValueString(element, "DirectoryBrowse", &m_Options.szDirectoryBrowse));
            VALIDATE(GetChildValueString(element, "MainWindowResize", &m_Options.szMainWindowResize));
            VALIDATE(GetChildValueString(element, "FileListColumns", &m_Options.szFileListColumns));
            VALIDATE(GetChildValueString(element, "PresetsDialogResize", &m_Options.szPresetsDialogResize));
            VALIDATE(GetChildValueString(element, "PresetsListColumns", &m_Options.szPresetsListColumns));
            VALIDATE(GetChildValueString(element, "FormatsDialogResize", &m_Options.szFormatsDialogResize));
            VALIDATE(GetChildValueString(element, "FormatsListColumns", &m_Options.szFormatsListColumns));
            VALIDATE(GetChildValueString(element, "ToolsDialogResize", &m_Options.szToolsDialogResize));
            VALIDATE(GetChildValueString(element, "ToolsListColumns", &m_Options.szToolsListColumns));
            return true;
        }
        void SetOptions(XmlElement *element, config::COptions &m_Options)
        {
            SetChildValueString(element, "SelectedLanguage", m_Options.szSelectedLanguage);
            SetChildValueSizeT(element, "SelectedFormat", m_Options.nSelectedFormat);
            SetChildValueString(element, "OutputPath", m_Options.szOutputPath);
            SetChildValueBool(element, "DeleteSourceFiles", m_Options.bDeleteSourceFiles);
            SetChildValueBool(element, "RecurseChecked", m_Options.bRecurseChecked);
            SetChildValueBool(element, "ShutdownWhenFinished", m_Options.bShutdownWhenFinished);
            SetChildValueBool(element, "DoNotSaveConfiguration", m_Options.bDoNotSaveConfiguration);
            SetChildValueBool(element, "DeleteOnErrors", m_Options.bDeleteOnErrors);
            SetChildValueBool(element, "StopOnErrors", m_Options.bStopOnErrors);
            SetChildValueBool(element, "HideConsoleWindow", m_Options.bHideConsoleWindow);
            SetChildValueBool(element, "TryToFindDecoder", m_Options.bTryToFindDecoder);
            SetChildValueBool(element, "EnsureItemIsVisible", m_Options.bEnsureItemIsVisible);
            SetChildValueBool(element, "ValidateInputFiles", m_Options.bValidateInputFiles);
            SetChildValueBool(element, "OverwriteExistingFiles", m_Options.bOverwriteExistingFiles);
            SetChildValueBool(element, "TryToDownloadTools", m_Options.bTryToDownloadTools);
            SetChildValueInt(element, "ThreadCount", m_Options.nThreadCount);
            SetChildValueString(element, "OutputBrowse", m_Options.szOutputBrowse);
            SetChildValueString(element, "DirectoryBrowse", m_Options.szDirectoryBrowse);
            SetChildValueString(element, "MainWindowResize", m_Options.szMainWindowResize);
            SetChildValueString(element, "FileListColumns", m_Options.szFileListColumns);
            SetChildValueString(element, "PresetsDialogResize", m_Options.szPresetsDialogResize);
            SetChildValueString(element, "PresetsListColumns", m_Options.szPresetsListColumns);
            SetChildValueString(element, "FormatsDialogResize", m_Options.szFormatsDialogResize);
            SetChildValueString(element, "FormatsListColumns", m_Options.szFormatsListColumns);
            SetChildValueString(element, "ToolsDialogResize", m_Options.szToolsDialogResize);
            SetChildValueString(element, "ToolsListColumns", m_Options.szToolsListColumns);
        }
    public:
        bool GetOptions(config::COptions &m_Options)
        {
            auto element = this->FirstChildElement("Options");
            if (element != nullptr)
            {
                VALIDATE(this->GetOptions(element, m_Options));
                return true;
            }
            return false;
        }
        void SetOptions(config::COptions &m_Options)
        {
            auto pOptionsElem = this->NewElement("Options");
            this->LinkEndChild(pOptionsElem);
            this->SetOptions(pOptionsElem, m_Options);
        }
    };

    class XmlItems : public XmlDoc
    {
    public:
        XmlItems(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlItems() { }
    public:
        bool GetPath(const XmlElement *element, config::CPath &m_Path)
        {
            VALIDATE(GetAttributeValueString(element, "path", &m_Path.szPath));
            VALIDATE(GetAttributeValueUInt64(element, "size", &m_Path.nSize));
            return true;
        }
        void SetPath(XmlElement *element, config::CPath &m_Path)
        {
            SetAttributeValueString(element, "path", m_Path.szPath);
            SetAttributeValueUInt64(element, "size", m_Path.nSize);
        }
        bool GetPaths(const XmlElement *parent, std::vector<config::CPath> &m_Paths)
        {
            auto element = parent->FirstChildElement("Path");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CPath path;
                    VALIDATE(this->GetPath(element, path));
                    m_Paths.emplace_back(path);
                }
                return true;
            }
            return false;
        }
        void SetPaths(XmlElement *parent, std::vector<config::CPath> &m_Paths)
        {
            size_t nPaths = m_Paths.size();
            for (size_t i = 0; i < nPaths; i++)
            {
                config::CPath& path = m_Paths[i];
                auto element = this->NewElement("Path");
                parent->LinkEndChild(element);
                this->SetPath(element, path);
            }
        }
    public:
        bool GetItem(const XmlElement *element, config::CItem &m_Item)
        {
            VALIDATE(GetAttributeValueInt(element, "id", &m_Item.nId));
            VALIDATE(GetAttributeValueUInt64(element, "size", &m_Item.nSize));
            VALIDATE(GetAttributeValueString(element, "name", &m_Item.szName));
            VALIDATE(GetAttributeValueString(element, "extension", &m_Item.szExtension));
            VALIDATE(GetAttributeValueString(element, "format", &m_Item.szFormatId));
            VALIDATE(GetAttributeValueSizeT(element, "preset", &m_Item.nPreset));
            VALIDATE(GetAttributeValueString(element, "options", &m_Item.szOptions));
            VALIDATE(GetAttributeValueBool(element, "checked", &m_Item.bChecked));
            VALIDATE(GetAttributeValueString(element, "time", &m_Item.szTime));
            VALIDATE(GetAttributeValueString(element, "status", &m_Item.szStatus));
            VALIDATE(this->GetPaths(element, m_Item.m_Paths));
            return true;
        }
        void SetItem(XmlElement *element, config::CItem &m_Item, int nId)
        {
            SetAttributeValueInt(element, "id", nId);
            SetAttributeValueUInt64(element, "size", m_Item.nSize);
            SetAttributeValueString(element, "name", m_Item.szName);
            SetAttributeValueString(element, "extension", m_Item.szExtension);
            SetAttributeValueString(element, "format", m_Item.szFormatId);
            SetAttributeValueSizeT(element, "preset", m_Item.nPreset);
            SetAttributeValueString(element, "options", m_Item.szOptions);
            SetAttributeValueBool(element, "checked", m_Item.bChecked);
            SetAttributeValueString(element, "time", m_Item.szTime);
            SetAttributeValueString(element, "status", m_Item.szStatus);
            this->SetPaths(element, m_Item.m_Paths);
        }
        bool GetItems(const XmlElement *parent, std::vector<config::CItem> &m_Items)
        {
            auto element = parent->FirstChildElement("Item");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CItem item;
                    VALIDATE(this->GetItem(element, item));
                    m_Items.emplace_back(item);
                }
                return true;
            }
            return false;
        }
        void SetItems(XmlElement *parent, std::vector<config::CItem> &m_Items)
        {
            size_t nItems = m_Items.size();
            for (size_t i = 0; i < nItems; i++)
            {
                config::CItem& item = m_Items[i];
                auto element = this->NewElement("Item");
                parent->LinkEndChild(element);
                this->SetItem(element, item, i);
            }
        }
    public:
        bool GetItem(config::CItem &m_Item)
        {
            auto element = this->FirstChildElement("Item");
            if (element != nullptr)
            {
                VALIDATE(this->GetItem(element, m_Item));
                return true;
            }
            return false;
        }
        void SetItem(config::CItem &m_Item)
        {
            auto element = this->NewElement("Item");
            this->LinkEndChild(element);
            this->SetItem(element, m_Item, -1);
        }
        bool GetItems(std::vector<config::CItem> &m_Items)
        {
            auto element = this->FirstChildElement("Items");
            if (element != nullptr)
            {
                VALIDATE(this->GetItems(element, m_Items));
                return true;
            }
            return false;
        }
        void SetItems(std::vector<config::CItem> &m_Items)
        {
            auto element = this->NewElement("Items");
            this->LinkEndChild(element);
            this->SetItems(element, m_Items);
        }
    };

    class XmlLanguages : public XmlDoc
    {
    public:
        XmlLanguages(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlLanguages() { }
    public:
        bool GetLanguage(const XmlElement *parent, config::CLanguage &m_Language)
        {
            VALIDATE(GetAttributeValueString(parent, "id", &m_Language.szId));
            VALIDATE(GetAttributeValueString(parent, "original", &m_Language.szOriginalName));
            VALIDATE(GetAttributeValueString(parent, "translated", &m_Language.szTranslatedName));

            auto element = parent->FirstChildElement("String");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    std::wstring szKey;
                    std::wstring szValue;

                    VALIDATE(GetAttributeValueString(element, "key", &szKey));
                    VALIDATE(GetAttributeValueString(element, "value", &szValue));

                    int nKey = util::StringHelper::ToIntFromHex(szKey);
                    m_Language.m_Strings[nKey] = szValue;
                }
                return true;
            }
            return false;
        }
        void SetLanguage(XmlElement *parent, config::CLanguage &m_Language)
        {
            SetAttributeValueString(parent, "id", m_Language.szId);
            SetAttributeValueString(parent, "original", m_Language.szOriginalName);
            SetAttributeValueString(parent, "translated", m_Language.szTranslatedName);

            for (auto& item : m_Language.m_Strings)
            {
                int nKey = item.first;
                std::wstring szKey = util::StringHelper::ToWStringHex(nKey);

                auto element = this->NewElement("String");
                parent->LinkEndChild(element);

                SetAttributeValueString(element, "", szKey);
                SetAttributeValueString(element, "", item.second);
            }
        }
    public:
        bool GetLanguage(config::CLanguage &m_Language)
        {
            auto element = this->FirstChildElement("Language");
            if (element != nullptr)
            {
                VALIDATE(this->GetLanguage(element, m_Language));
                return true;
            }
            return false;
        }
        void SetLanguage(config::CLanguage &m_Language)
        {
            auto element = this->NewElement("Language");
            this->LinkEndChild(element);
            this->SetLanguage(element, m_Language);
        }
    };

    class XmlPresets : public XmlDoc
    {
    public:
        XmlPresets(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlPresets() { }
    public:
        bool GetPreset(const XmlElement *element, config::CPreset &m_Preset)
        {
            VALIDATE(GetAttributeValueString(element, "name", &m_Preset.szName));
            VALIDATE(GetAttributeValueString(element, "options", &m_Preset.szOptions));
            return true;
        }
        void SetPreset(XmlElement *element, config::CPreset &m_Preset)
        {
            SetAttributeValueString(element, "name", m_Preset.szName);
            SetAttributeValueString(element, "options", m_Preset.szOptions);
        }
        bool GetPresets(const XmlElement *parent, std::vector<config::CPreset> &m_Presets)
        {
            auto element = parent->FirstChildElement("Preset");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CPreset preset;
                    VALIDATE(this->GetPreset(element, preset));
                    m_Presets.emplace_back(preset);
                }
                return true;
            }
            return false;
        }
        void SetPresets(XmlElement *parent, std::vector<config::CPreset> &m_Presets)
        {
            size_t nPresets = m_Presets.size();
            for (size_t i = 0; i < nPresets; i++)
            {
                config::CPreset& preset = m_Presets[i];
                auto element = this->NewElement("Preset");
                this->SetPreset(element, preset);
                parent->LinkEndChild(element);
            }
        }
    public:
        bool GetPreset(config::CPreset &m_Preset)
        {
            auto element = this->FirstChildElement("Preset");
            if (element != nullptr)
            {
                VALIDATE(this->GetPreset(element, m_Preset));
                return true;
            }
            return false;
        }
        void SetPreset(config::CPreset &m_Preset)
        {
            auto element = this->NewElement("Preset");
            this->LinkEndChild(element);
            this->SetPreset(element, m_Preset);
        }
        bool GetPresets(std::vector<config::CPreset> &m_Presets)
        {
            auto element = this->FirstChildElement("Presets");
            if (element != nullptr)
            {
                VALIDATE(this->GetPresets(element, m_Presets));
                return true;
            }
            return false;
        }
        void SetPresets(std::vector<config::CPreset> &m_Presets)
        {
            auto element = this->NewElement("Presets");
            this->LinkEndChild(element);
            this->SetPresets(element, m_Presets);
        }
    };

    class XmlFormats : public XmlDoc
    {
    public:
        XmlFormats(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlFormats() { }
    public:
        bool GetFormat(const XmlElement *element, config::CFormat &m_Format)
        {
            VALIDATE(GetAttributeValueString(element, "id", &m_Format.szId));
            VALIDATE(GetAttributeValueString(element, "name", &m_Format.szName));
            VALIDATE(GetAttributeValueString(element, "template", &m_Format.szTemplate));
            VALIDATE(GetAttributeValueBool(element, "input", &m_Format.bPipeInput));
            VALIDATE(GetAttributeValueBool(element, "output", &m_Format.bPipeOutput));
            VALIDATE(GetAttributeValueString(element, "function", &m_Format.szFunction));
            VALIDATE(GetAttributeValueString(element, "path", &m_Format.szPath));
            VALIDATE(GetAttributeValueInt(element, "success", &m_Format.nExitCodeSuccess));

            int nType;
            VALIDATE(GetAttributeValueInt(element, "type", &nType));
            m_Format.nType = config::CFormat::FromInt(nType);

            VALIDATE(GetAttributeValueInt(element, "priority", &m_Format.nPriority));

            VALIDATE(GetAttributeValueString(element, "formats", &m_Format.szInputExtensions));
            VALIDATE(GetAttributeValueString(element, "extension", &m_Format.szOutputExtension));
            VALIDATE(GetAttributeValueSizeT(element, "default", &m_Format.nDefaultPreset));

            auto parent = element->FirstChildElement("Presets");
            if (parent != nullptr)
            {
                VALIDATE(XmlPresets(m_Document).GetPresets(parent, m_Format.m_Presets));
                return true;
            }
            return false;
        }
        void SetFormat(XmlElement *element, config::CFormat &m_Format)
        {
            SetAttributeValueString(element, "id", m_Format.szId);
            SetAttributeValueString(element, "name", m_Format.szName);
            SetAttributeValueString(element, "template", m_Format.szTemplate);
            SetAttributeValueBool(element, "input", m_Format.bPipeInput);
            SetAttributeValueBool(element, "output", m_Format.bPipeOutput);
            SetAttributeValueString(element, "function", m_Format.szFunction);
            SetAttributeValueString(element, "path", m_Format.szPath);
            SetAttributeValueInt(element, "success", m_Format.nExitCodeSuccess);

            int nType = config::CFormat::ToInt(m_Format.nType);
            SetAttributeValueInt(element, "type", nType);

            SetAttributeValueInt(element, "priority", m_Format.nPriority);

            SetAttributeValueString(element, "formats", m_Format.szInputExtensions);
            SetAttributeValueString(element, "extension", m_Format.szOutputExtension);
            SetAttributeValueSizeT(element, "default", m_Format.nDefaultPreset);

            auto parent = this->NewElement("Presets");
            element->LinkEndChild(parent);
            XmlPresets(m_Document).SetPresets(parent, m_Format.m_Presets);
        }
    public:
        bool GetFormat(config::CFormat &m_Format)
        {
            auto element = this->FirstChildElement("Format");
            if (element != nullptr)
            {
                VALIDATE(this->GetFormat(element, m_Format));
                return true;
            }
            return false;
        }
        void SetFormat(config::CFormat &m_Format)
        {
            auto element = this->NewElement("Format");
            this->LinkEndChild(element);
            this->SetFormat(element, m_Format);
        }
    };

    class XmlTools : public XmlDoc
    {
    public:
        XmlTools(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlTools() { }
    public:
        bool GetTool(const XmlElement *element, config::CTool &m_Tool)
        {
            VALIDATE(GetAttributeValueString(element, "name", &m_Tool.szName));
            VALIDATE(GetAttributeValueString(element, "platform", &m_Tool.szPlatform));
            VALIDATE(GetAttributeValueInt(element, "priority", &m_Tool.nPriority));
            VALIDATE(GetAttributeValueString(element, "formats", &m_Tool.szFormats));
            VALIDATE(GetAttributeValueString(element, "url", &m_Tool.szUrl));
            VALIDATE(GetAttributeValueString(element, "file", &m_Tool.szFile));
            VALIDATE(GetAttributeValueString(element, "extract", &m_Tool.szExtract));
            VALIDATE(GetAttributeValueString(element, "path", &m_Tool.szPath));
            return true;
        }
        void SetTool(XmlElement *element, config::CTool &m_Tool)
        {
            SetAttributeValueString(element, "name", m_Tool.szName);
            SetAttributeValueString(element, "platform", m_Tool.szPlatform);
            SetAttributeValueInt(element, "priority", m_Tool.nPriority);
            SetAttributeValueString(element, "formats", m_Tool.szFormats);
            SetAttributeValueString(element, "url", m_Tool.szUrl);
            SetAttributeValueString(element, "file", m_Tool.szFile);
            SetAttributeValueString(element, "extract", m_Tool.szExtract);
            SetAttributeValueString(element, "path", m_Tool.szPath);
        }
    public:
        bool GetTool(config::CTool &m_Tool)
        {
            auto element = this->FirstChildElement("Tool");
            if (element != nullptr)
            {
                VALIDATE(this->GetTool(element, m_Tool));
                return true;
            }
            return false;
        }
        void SetTool(config::CTool &m_Tool)
        {
            auto element = this->NewElement("Tool");
            this->LinkEndChild(element);
            this->SetTool(element, m_Tool);
        }
    };

    class XmlConfig
    {
    public:
        static std::string GetRootName(const std::wstring& szFileXml, XmlDocumnent &doc)
        {
            if (XmlDoc::Open(szFileXml, doc) == true)
            {
                const char *name = XmlDoc::GetRootName(doc);
                if (name != nullptr)
                    return std::string(name);
            }
            return std::string();
        }
    public:
        static bool LoadOptions(XmlDocumnent &doc, config::COptions &options)
        {
            XmlOptions xml(doc);
            return xml.GetOptions(options);
        }
        static bool LoadOptions(const std::wstring& szFileXml, config::COptions &options)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadOptions(doc, options);
            return false;
        }
        static bool SaveOptions(const std::wstring& szFileXml, config::COptions &options)
        {
            XmlDocumnent doc;
            XmlOptions xml(doc);
            xml.Create();
            xml.SetOptions(options);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadItem(XmlDocumnent &doc, config::CItem &item)
        {
            XmlItems xml(doc);
            return xml.GetItem(item);
        }
        static bool LoadItem(const std::wstring& szFileXml, config::CItem &item)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadItem(doc, item);
            return false;
        }
        static bool SaveItem(const std::wstring& szFileXml, config::CItem &item)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItem(item);
            return xml.Save(szFileXml);
        }
        static bool LoadItems(XmlDocumnent &doc, std::vector<config::CItem> &items)
        {
            XmlItems xml(doc);
            return xml.GetItems(items);
        }
        static bool LoadItems(const std::wstring& szFileXml, std::vector<config::CItem> &items)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadItems(doc, items);
            return false;
        }
        static bool SaveItems(const std::wstring& szFileXml, std::vector<config::CItem> &items)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItems(items);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadLanguage(XmlDocumnent &doc, config::CLanguage &language)
        {
            XmlLanguages xml(doc);
            return xml.GetLanguage(language);
        }
        static bool LoadLanguage(const std::wstring& szFileXml, config::CLanguage &language)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadLanguage(doc, language);
            return false;
        }
        static bool SaveLanguage(const std::wstring& szFileXml, config::CLanguage &language)
        {
            XmlDocumnent doc;
            XmlLanguages xml(doc);
            xml.Create();
            xml.SetLanguage(language);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadPreset(XmlDocumnent &doc, config::CPreset &preset)
        {
            XmlPresets xml(doc);
            return xml.GetPreset(preset);
        }
        static bool LoadPreset(const std::wstring& szFileXml, config::CPreset &preset)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadPreset(doc, preset);
            return false;
        }
        static bool SavePreset(const std::wstring& szFileXml, config::CPreset &preset)
        {
            XmlDocumnent doc;
            XmlPresets xml(doc);
            xml.Create();
            xml.SetPreset(preset);
            return xml.Save(szFileXml);
        }
        static bool LoadPresets(XmlDocumnent &doc, std::vector<config::CPreset> &presets)
        {
            XmlPresets xml(doc);
            return xml.GetPresets(presets);
        }
        static bool LoadPresets(const std::wstring& szFileXml, std::vector<config::CPreset> &presets)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadPresets(doc, presets);
            return false;
        }
        static bool SavePresets(const std::wstring& szFileXml, std::vector<config::CPreset> &presets)
        {
            XmlDocumnent doc;
            XmlPresets xml(doc);
            xml.Create();
            xml.SetPresets(presets);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadFormat(XmlDocumnent &doc, config::CFormat &format)
        {
            XmlFormats xml(doc);
            return xml.GetFormat(format);
        }
        static bool LoadFormat(const std::wstring& szFileXml, config::CFormat &format)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadFormat(doc, format);
            return false;
        }
        static bool SaveFormat(const std::wstring& szFileXml, config::CFormat &format)
        {
            XmlDocumnent doc;
            XmlFormats xml(doc);
            xml.Create();
            xml.SetFormat(format);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadTool(XmlDocumnent &doc, config::CTool &tool)
        {
            XmlTools xml(doc);
            return xml.GetTool(tool);
        }
        static bool LoadTool(const std::wstring& szFileXml, config::CTool &tool)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadTool(doc, tool);
            return false;
        }
        static bool SaveTool(const std::wstring& szFileXml, config::CTool &tool)
        {
            XmlDocumnent doc;
            XmlTools xml(doc);
            xml.Create();
            xml.SetTool(tool);
            return xml.Save(szFileXml);
        }
    };
}
