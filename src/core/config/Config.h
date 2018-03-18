// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <cstring>
#include <utility>
#include <vector>
#include <memory>
#include <cstdio>
#include <functional>
#include "utilities\FileSystem.h"
#include "utilities\Log.h"
#include "utilities\String.h"
#include "utilities\Utf8String.h"
#include "tinyxml2\tinyxml2.h"
#include "Format.h"
#include "Item.h"
#include "Language.h"
#include "Options.h"
#include "Path.h"
#include "Preset.h"
#include "Settings.h"
#include "Strings.h"
#include "Tool.h"

#define VALIDATE(value) if (!value) return false

namespace config::xml
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
            FILE *fs = nullptr;
            errno_t error = _wfopen_s(&fs, szFileName.c_str(), L"rb");
            if (error != 0 || fs == nullptr)
                return false;

            tinyxml2::XMLError result = doc.LoadFile(fs);
            fclose(fs);

            return result == tinyxml2::XMLError::XML_SUCCESS;
        }
        static bool Parse(const char* xml, XmlDocumnent & doc)
        {
            tinyxml2::XMLError result = doc.Parse(xml);
            return result == tinyxml2::XMLError::XML_SUCCESS;
        }
        static bool Save(const std::wstring& szFileName, XmlDocumnent & doc)
        {
            FILE *fs = nullptr;
            errno_t error = _wfopen_s(&fs, szFileName.c_str(), L"wt");
            if (error != 0 || fs == nullptr)
                return false;

            fputc(0xefU, fs);
            fputc(0xbbU, fs);
            fputc(0xbfU, fs);

            tinyxml2::XMLPrinter printer(fs);
            doc.Print(&printer);

            fclose(fs);
            return true;
        }
    public:
        static inline const std::wstring ToString(const char *pszUtf8)
        {
            if (pszUtf8 == nullptr)
                return std::wstring();
            if (strlen(pszUtf8) == 0)
                return std::wstring();
            return std::move(util::ToUnicode(pszUtf8));
        }
        static inline int ToInt(const char *pszUtf8)
        {
            return std::move(std::stoi(pszUtf8));
        }
        static inline size_t ToUInt(const char *pszUtf8)
        {
            return std::move(std::stoul(pszUtf8));
        }
        static inline unsigned __int64 ToUInt64(const char *pszUtf8)
        {
            return std::move(std::strtoull(pszUtf8, nullptr, 10));
        }
        static inline bool ToBool(const char *pszUtf8)
        {
            return _stricmp(pszUtf8, u8"true") == 0;
        }
        static inline std::string IntToString(const int nValue)
        {
            return std::move(std::to_string(nValue));
        }
        static inline std::string SizeToString(const size_t nValue)
        {
            return std::move(std::to_string(nValue));
        }
        static inline std::string UInt64ToString(const unsigned __int64 nValue)
        {
            return std::move(std::to_string(nValue));
        }
        static inline const char * BoolToString(const bool bValue)
        {
            return bValue ? u8"true" : u8"false";
        }
    public:
        inline bool GetAttributeValueString(const XmlElement *element, const char *name, std::wstring *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = std::move(ToString(pszResult));
                return true;
            }
            return false;
        }
        inline bool GetAttributeValueInt(const XmlElement *element, const char *name, int *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToInt(pszResult);
                return true;
            }
            return false;
        }
        inline bool GetAttributeValueSizeT(const XmlElement *element, const char *name, size_t *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToUInt(pszResult);
                return true;
            }
            return false;
        }
        inline bool GetAttributeValueUInt64(const XmlElement *element, const char *name, unsigned __int64 *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToUInt64(pszResult);
                return true;
            }
            return false;
        }
        inline bool GetAttributeValueBool(const XmlElement *element, const char *name, bool *value)
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
        inline void SetAttributeValueString(XmlElement *element, const char *name, const std::wstring& value)
        {
            element->SetAttribute(name, util::ToUtf8(value).c_str());
        }
        inline void SetAttributeValueInt(XmlElement *element, const char *name, const int &value)
        {
            element->SetAttribute(name, IntToString(value).c_str());
        }
        inline void SetAttributeValueSizeT(XmlElement *element, const char *name, const size_t &value)
        {
            element->SetAttribute(name, SizeToString(value).c_str());
        }
        inline void SetAttributeValueUInt64(XmlElement *element, const char *name, const unsigned __int64 &value)
        {
            element->SetAttribute(name, UInt64ToString(value).c_str());
        }
        inline void SetAttributeValueBool(XmlElement *element, const char *name, const bool &value)
        {
            element->SetAttribute(name, BoolToString(value));
        }
    public:
        inline bool GetChildValueString(const XmlElement *parent, const char *name, std::wstring *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = std::move(ToString(element->GetText()));
                return true;
            }
            return false;
        }
        inline bool GetChildValueInt(const XmlElement *parent, const char *name, int *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToInt(element->GetText());
                return true;
            }
            return false;
        }
        inline bool GetChildValueSizeT(const XmlElement *parent, const char *name, size_t *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToUInt(element->GetText());
                return true;
            }
            return false;
        }
        inline bool GetChildValueUInt64(const XmlElement *parent, const char *name, unsigned __int64 *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToUInt64(element->GetText());
                return true;
            }
            return false;
        }
        inline bool GetChildValueBool(const XmlElement *parent, const char *name, bool *value)
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
        inline void SetChildValueString(XmlElement *parent, const char *name, const std::wstring& value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::ToUtf8(value).c_str()));
            parent->LinkEndChild(element);
        }
        inline void SetChildValueInt(XmlElement *parent, const char *name, const int &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(IntToString(value).c_str()));
            parent->LinkEndChild(element);
        }
        inline void SetChildValueSizeT(XmlElement *parent, const char *name, const size_t &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(SizeToString(value).c_str()));
            parent->LinkEndChild(element);
        }
        inline void SetChildValueUInt64(XmlElement *parent, const char *name, const unsigned __int64 &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(UInt64ToString(value).c_str()));
            parent->LinkEndChild(element);
        }
        inline void SetChildValueBool(XmlElement *parent, const char *name, const bool &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(BoolToString(value)));
            parent->LinkEndChild(element);
        }
    public:
        inline XmlElement * NewElement(const char *name)
        {
            return m_Document.NewElement(name);
        }
        inline XmlElement* FirstChildElement(const char *name)
        {
            return m_Document.FirstChildElement(name);
        }
        inline XmlNode* LinkEndChild(XmlNode* node)
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
            GetChildValueString(element, "SelectedLanguage", &m_Options.szSelectedLanguage);
            GetChildValueSizeT(element, "SelectedFormat", &m_Options.nSelectedFormat);
            GetChildValueString(element, "OutputPath", &m_Options.szOutputPath);
            GetChildValueBool(element, "DeleteSourceFiles", &m_Options.bDeleteSourceFiles);
            GetChildValueBool(element, "RecurseChecked", &m_Options.bRecurseChecked);
            GetChildValueBool(element, "ShutdownWhenFinished", &m_Options.bShutdownWhenFinished);
            GetChildValueBool(element, "DoNotSaveConfiguration", &m_Options.bDoNotSaveConfiguration);
            GetChildValueBool(element, "DeleteOnErrors", &m_Options.bDeleteOnErrors);
            GetChildValueBool(element, "StopOnErrors", &m_Options.bStopOnErrors);
            GetChildValueBool(element, "HideConsoleWindow", &m_Options.bHideConsoleWindow);
            GetChildValueBool(element, "TryToFindDecoder", &m_Options.bTryToFindDecoder);
            GetChildValueBool(element, "EnsureItemIsVisible", &m_Options.bEnsureItemIsVisible);
            GetChildValueBool(element, "ValidateInputFiles", &m_Options.bValidateInputFiles);
            GetChildValueBool(element, "OverwriteExistingFiles", &m_Options.bOverwriteExistingFiles);
            GetChildValueBool(element, "RenameExistingFiles", &m_Options.bRenameExistingFiles);
            GetChildValueInt(element, "RenameExistingFilesLimit", &m_Options.nRenameExistingFilesLimit);
            GetChildValueBool(element, "TryToDownloadTools", &m_Options.bTryToDownloadTools);
            GetChildValueInt(element, "ThreadCount", &m_Options.nThreadCount);
            GetChildValueString(element, "OutputBrowse", &m_Options.szOutputBrowse);
            GetChildValueString(element, "DirectoryBrowse", &m_Options.szDirectoryBrowse);
            GetChildValueString(element, "MainWindowResize", &m_Options.szMainWindowResize);
            GetChildValueString(element, "FileListColumns", &m_Options.szFileListColumns);
            GetChildValueString(element, "PathsDialogResize", &m_Options.szPathsDialogResize);
            GetChildValueString(element, "PathsListColumns", &m_Options.szPathsListColumns);
            GetChildValueString(element, "PresetsDialogResize", &m_Options.szPresetsDialogResize);
            GetChildValueString(element, "PresetsListColumns", &m_Options.szPresetsListColumns);
            GetChildValueString(element, "FormatsDialogResize", &m_Options.szFormatsDialogResize);
            GetChildValueString(element, "FormatsListColumns", &m_Options.szFormatsListColumns);
            GetChildValueString(element, "ToolsDialogResize", &m_Options.szToolsDialogResize);
            GetChildValueString(element, "ToolsListColumns", &m_Options.szToolsListColumns);
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
            SetChildValueBool(element, "RenameExistingFiles", m_Options.bRenameExistingFiles);
            SetChildValueInt(element, "RenameExistingFilesLimit", m_Options.nRenameExistingFilesLimit);
            SetChildValueBool(element, "TryToDownloadTools", m_Options.bTryToDownloadTools);
            SetChildValueInt(element, "ThreadCount", m_Options.nThreadCount);
            SetChildValueString(element, "OutputBrowse", m_Options.szOutputBrowse);
            SetChildValueString(element, "DirectoryBrowse", m_Options.szDirectoryBrowse);
            SetChildValueString(element, "MainWindowResize", m_Options.szMainWindowResize);
            SetChildValueString(element, "FileListColumns", m_Options.szFileListColumns);
            SetChildValueString(element, "PathsDialogResize", m_Options.szPathsDialogResize);
            SetChildValueString(element, "PathsListColumns", m_Options.szPathsListColumns);
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
                    m_Paths.emplace_back(std::move(path));
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
                    m_Items.emplace_back(std::move(item));
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
        bool GetPaths(std::vector<config::CPath> &m_Paths)
        {
            auto element = this->FirstChildElement("Paths");
            if (element != nullptr)
            {
                VALIDATE(this->GetPaths(element, m_Paths));
                return true;
            }
            return false;
        }
        void SetPaths(std::vector<config::CPath> &m_Paths)
        {
            auto element = this->NewElement("Paths");
            this->LinkEndChild(element);
            this->SetPaths(element, m_Paths);
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

                    int nKey = util::string::ToIntFromHex(szKey);
                    m_Language.m_Strings[nKey] = std::move(szValue);
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
                std::wstring szKey = util::string::ToWStringHex(nKey);

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

    class XmlOutputs : public XmlDoc
    {
    public:
        XmlOutputs(XmlDocumnent &doc) : XmlDoc(doc) { }
        virtual ~XmlOutputs() { }
    public:
        bool GetOutputs(const XmlElement *parent, std::vector<std::wstring> &outputs)
        {
            auto element = parent->FirstChildElement("Output");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    std::wstring pattern;
                    VALIDATE(GetAttributeValueString(element, "pattern", &pattern));
                    outputs.emplace_back(std::move(pattern));
                }
                return true;
            }
            return false;
        }
        void SetOutputs(XmlElement *parent, std::vector<std::wstring> &outputs)
        {
            for (auto& output : outputs)
            {
                auto element = this->NewElement("Output");
                parent->LinkEndChild(element);
                SetAttributeValueString(element, "pattern", output);
            }
        }
    public:
        bool GetOutputs(std::vector<std::wstring> &outputs)
        {
            auto element = this->FirstChildElement("Outputs");
            if (element != nullptr)
            {
                VALIDATE(this->GetOutputs(element, outputs));
                return true;
            }
            return false;
        }
        void SetOutputs(std::vector<std::wstring> &outputs)
        {
            auto element = this->NewElement("Outputs");
            this->LinkEndChild(element);
            this->SetOutputs(element, outputs);
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
                    m_Presets.emplace_back(std::move(preset));
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
        static bool LoadPaths(XmlDocumnent &doc, std::vector<config::CPath> &paths)
        {
            XmlItems xml(doc);
            return xml.GetPaths(paths);
        }
        static bool LoadPaths(const std::wstring& szFileXml, std::vector<config::CPath> &paths)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadPaths(doc, paths);
            return false;
        }
        static bool SavePaths(const std::wstring& szFileXml, std::vector<config::CPath> &paths)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetPaths(paths);
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
        static bool LoadOutputs(XmlDocumnent &doc, std::vector<std::wstring> &outputs)
        {
            XmlOutputs xml(doc);
            return xml.GetOutputs(outputs);
        }
        static bool LoadOutputs(const std::wstring& szFileXml, std::vector<std::wstring> &outputs)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadOutputs(doc, outputs);
            return false;
        }
        static bool SaveOutputs(const std::wstring& szFileXml, std::vector<std::wstring> &outputs)
        {
            XmlDocumnent doc;
            XmlOutputs xml(doc);
            xml.Create();
            xml.SetOutputs(outputs);
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

namespace config
{
    class CConfig
    {
    public:
        std::unique_ptr<util::IFileSystem> FileSystem;
        std::unique_ptr<util::ILog> Log;
        CSettings m_Settings;
        COptions m_Options;
        std::vector<CFormat> m_Formats;
        std::vector<CItem> m_Items;
        std::vector<CTool> m_Tools;
        size_t nLangId;
        std::vector<CLanguage> m_Languages;
        std::vector<std::wstring> m_Outputs;
    public:
        int AddItem(const std::wstring& szPath, int nFormat, int nPreset)
        {
            std::wstring szFormatId = L"";
            int nFormatId = nFormat;
            int nPresetId = nPreset;
            std::wstring szExt = FileSystem->GetFileExtension(szPath);
            std::wstring szName = FileSystem->GetOnlyFileName(szPath);
            unsigned __int64 nFileSize = FileSystem->GetFileSize64(szPath);

            if ((nFormatId >= 0) && (nFormatId < (int)this->m_Formats.size()))
            {
                const auto& format = this->m_Formats[nFormatId];
                szFormatId = format.szId;
            }

            if (this->m_Options.bTryToFindDecoder == true)
            {
                int nDecoder = CFormat::GetDecoderByExtension(this->m_Formats, szExt);
                if ((nDecoder >= 0) && (nDecoder < (int)this->m_Formats.size()))
                {
                    const auto& format = this->m_Formats[nDecoder];
                    szFormatId = format.szId;
                    nPresetId = format.nDefaultPreset;
                }
            }

            CItem item;

            CPath path;
            path.szPath = szPath;
            path.nSize = nFileSize;
            item.m_Paths.emplace_back(std::move(path));

            item.nSize = nFileSize;
            item.szName = std::move(szName);
            item.szExtension = util::string::ToUpper(szExt);
            item.szFormatId = std::move(szFormatId);
            item.nPreset = nPresetId;
            item.bChecked = true;

            this->m_Items.emplace_back(std::move(item));

            return (int)this->m_Items.size() - 1;
        }
        void RemoveItems(std::vector<int>& keep)
        {
            std::vector<CItem> items;

            for (size_t i = 0; i < keep.size(); i++)
            {
                int nIndex = keep[i];
                auto& item = this->m_Items[nIndex];
                items.emplace_back(std::move(item));
            }

            this->m_Items = std::move(items);
        }
    public:
        inline bool LookupString(const int nKey, std::wstring& rValue)
        {
            if (this->nLangId >= 0 && this->nLangId < this->m_Languages.size())
            {
                auto& language = this->m_Languages[this->nLangId];
                if (language.m_Strings.count(nKey) == 1)
                {
                    rValue = language.m_Strings[nKey];
                    return true;
                }
            }
            return false;
        }
        inline std::wstring GetString(int nKey, const std::wstring& szDefault)
        {
            std::wstring rValue;
            if (this->LookupString(nKey, rValue))
                return rValue;
            return szDefault;
        }
        inline std::wstring GetString(int nKey)
        {
            std::wstring rValue;
            if (this->LookupString(nKey, rValue))
                return rValue;

            if (config::m_Strings.count(nKey) == 1)
                return config::m_Strings.at(nKey);

            return L"??";
        }
    public:
        static inline int FindTool(std::vector<config::CTool>& m_Tools, const std::wstring& szPlatform, const std::wstring& szFormatId)
        {
            int nTool = config::CTool::GetToolByFormatAndPlatform(m_Tools, szFormatId, szPlatform);
            if (nTool >= 0)
            {
                return nTool;
            }
            return -1;
        }
        static inline int FindTool(std::vector<config::CTool>& m_Tools, const std::wstring& szFormatId)
        {
            const std::wstring szPlatformX86 = L"x86";
            const std::wstring szPlatformX64 = L"x64";
#if defined(_WIN32) & !defined(_WIN64)
            return FindTool(m_Tools, szPlatformX86, szFormatId);
#else
            int nTool = FindTool(m_Tools, szPlatformX64, szFormatId);
            if (nTool == -1)
                return FindTool(m_Tools, szPlatformX86, szFormatId);
            return nTool;
#endif
        }
        static inline void SetFormatPaths(std::vector<config::CFormat>& m_Formats, std::vector<config::CTool>& m_Tools, const std::wstring& szPlatform)
        {
            size_t nFormats = m_Formats.size();
            for (size_t i = 0; i < nFormats; i++)
            {
                config::CFormat& format = m_Formats[i];
                int nTool = config::CTool::GetToolByFormatAndPlatform(m_Tools, format.szId, szPlatform);
                if (nTool >= 0)
                {
                    config::CTool& tool = m_Tools[nTool];
                    format.szPath = tool.szPath;
                }
            }
        }
        static inline void SetFormatPaths(std::vector<config::CFormat>& m_Formats, std::vector<config::CTool>& m_Tools, std::function<bool(int, config::CTool&)> filter)
        {
            size_t nTools = m_Tools.size();
            size_t nFormats = m_Formats.size();
            if ((nTools > 0) && (nFormats > 0))
            {
                for (size_t i = 0; i < nTools; i++)
                {
                    config::CTool& tool = m_Tools[i];
                    if (filter(i, tool) == true)
                    {
                        for (size_t i = 0; i < nFormats; i++)
                        {
                            config::CFormat& format = m_Formats[i];
                            if (CTool::IsValidFormat(tool.szFormats, format.szId))
                            {
                                format.szPath = tool.szPath;
                            }
                        }
                    }
                }
            }
        }
    public:
        void CleanFiles(const std::wstring& szPath)
        {
            std::vector<std::wstring> files;
            if (FileSystem->FindFiles(szPath, files, false) == true)
            {
                for (auto& file : files)
                {
                    std::wstring szExt = FileSystem->GetFileExtension(file);
                    if (util::string::CompareNoCase(szExt, L"xml"))
                    {
                        FileSystem->DeleteFile_(file);
                    }
                }
            }
        }
    public:
        bool LoadOptions(const std::wstring& szFileXml)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Options"))
            {
                return this->LoadOptions(doc);
            }
            return false;
        }
        bool LoadOptions(xml::XmlDocumnent &doc)
        {
            config::COptions options;
            if (xml::XmlConfig::LoadOptions(doc, options))
            {
                this->m_Options = std::move(options);
                return true;
            }
            return false;
        }
        bool SaveOptions(const std::wstring& szFileXml)
        {
            return xml::XmlConfig::SaveOptions(szFileXml, this->m_Options);
        }
        bool LoadFormats(const std::wstring& szPath)
        {
            std::vector<std::wstring> files;
            bool bResult = FileSystem->FindFiles(szPath, files, false);
            if (bResult == true)
            {
                std::vector<config::CFormat> formats;
                for (auto& file : files)
                {
                    xml::XmlDocumnent doc;
                    if (xml::XmlDoc::Open(file, doc) == true)
                    {
                        std::string szName = xml::XmlDoc::GetRootName(doc);
                        if (util::string::CompareNoCase(szName, "Format"))
                        {
                            config::CFormat format;
                            if (xml::XmlConfig::LoadFormat(doc, format))
                            {
                                formats.emplace_back(std::move(format));
                            }
                        }
                    }
                }

                config::CFormat::Sort(formats);
                this->m_Formats = std::move(formats);

                return true;
            }
            return false;
        }
        bool SaveFormats(const std::wstring& szPath)
        {
            FileSystem->CreateDirectory_(szPath);
            this->CleanFiles(szPath);
            for (auto& format : this->m_Formats)
            {
                std::wstring path = FileSystem->CombinePath(szPath, format.szId + L".xml");
                if (this->SaveFormat(path, format) == false)
                    return false;
            }

            return true;
        }
        bool LoadFormat(const std::wstring& szFileXml)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Format"))
            {
                return this->LoadFormat(doc);
            }
            return false;
        }
        bool LoadFormat(xml::XmlDocumnent &doc)
        {
            config::CFormat format;
            if (xml::XmlConfig::LoadFormat(doc, format))
            {
                this->m_Formats.emplace_back(std::move(format));
                config::CFormat::Sort(this->m_Formats);
                return true;
            }
            return false;
        }
        bool SaveFormat(const std::wstring& szFileXml, config::CFormat& format)
        {
            return xml::XmlConfig::SaveFormat(szFileXml, format);
        }
        bool LoadPresets(const std::wstring& szFileXml, const int nFormat)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Presets"))
            {
                return this->LoadPresets(doc, nFormat);
            }
            return false;
        }
        bool LoadPresets(xml::XmlDocumnent &doc, const int nFormat)
        {
            std::vector<config::CPreset> presets;
            if (xml::XmlConfig::LoadPresets(doc, presets))
            {
                config::CFormat& format = this->m_Formats[nFormat];
                format.m_Presets = std::move(presets);
                return true;
            }
            return false;
        }
        bool SavePresets(const std::wstring& szFileXml, const int nFormat)
        {
            config::CFormat& format = this->m_Formats[nFormat];
            return xml::XmlConfig::SavePresets(szFileXml, format.m_Presets);
        }
        bool LoadTools(const std::wstring& szPath)
        {
            std::vector<std::wstring> files;
            bool bResult = FileSystem->FindFiles(szPath, files, false);
            if (bResult == true)
            {
                std::vector<config::CTool> tools;
                for (auto& file : files)
                {
                    xml::XmlDocumnent doc;
                    if (xml::XmlDoc::Open(file, doc) == true)
                    {
                        std::string szName = xml::XmlDoc::GetRootName(doc);
                        if (util::string::CompareNoCase(szName, "Tool"))
                        {
                            config::CTool tool;
                            if (xml::XmlConfig::LoadTool(doc, tool))
                            {
                                tools.emplace_back(std::move(tool));
                            }
                        }
                    }
                }

                config::CTool::Sort(tools);
                this->m_Tools = std::move(tools);

                return true;
            }
            return false;
        }
        bool SaveTools(const std::wstring& szPath)
        {
            FileSystem->CreateDirectory_(szPath);
            this->CleanFiles(szPath);
            for (auto& tool : this->m_Tools)
            {
                std::wstring path = FileSystem->CombinePath(szPath, tool.szName + L".xml");
                if (this->SaveTool(path, tool) == false)
                    return false;
            }

            return true;
        }
        bool LoadTool(const std::wstring& szFileXml)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Tool"))
            {
                return this->LoadTool(doc);
            }
            return false;
        }
        bool LoadTool(xml::XmlDocumnent &doc)
        {
            config::CTool tool;
            if (xml::XmlConfig::LoadTool(doc, tool))
            {
                this->m_Tools.emplace_back(std::move(tool));
                config::CTool::Sort(this->m_Tools);
                return true;
            }
            return false;
        }
        bool SaveTool(const std::wstring& szFileXml, config::CTool& tool)
        {
            return xml::XmlConfig::SaveTool(szFileXml, tool);
        }
        bool LoadItems(const std::wstring& szFileXml)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Items"))
            {
                return this->LoadItems(doc);
            }
            return false;
        }
        bool LoadItems(xml::XmlDocumnent &doc)
        {
            std::vector<config::CItem> items;
            if (xml::XmlConfig::LoadItems(doc, items))
            {
                this->m_Items = std::move(items);
                return true;
            }
            return false;
        }
        bool SaveItems(const std::wstring& szFileXml)
        {
            return xml::XmlConfig::SaveItems(szFileXml, this->m_Items);
        }
        bool LoadOutputs(const std::wstring& szFileXml)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Outputs"))
            {
                return this->LoadOutputs(doc);
            }
            return false;
        }
        bool LoadOutputs(xml::XmlDocumnent &doc)
        {
            std::vector<std::wstring> outputs;
            if (xml::XmlConfig::LoadOutputs(doc, outputs))
            {
                this->m_Outputs = std::move(outputs);
                return true;
            }
            return false;
        }
        bool SaveOutputs(const std::wstring& szFileXml)
        {
            return xml::XmlConfig::SaveOutputs(szFileXml, this->m_Outputs);
        }
        bool LoadLanguages(const std::wstring& szPath)
        {
            std::vector<std::wstring> files;
            bool bResult = FileSystem->FindFiles(szPath, files, false);
            if (bResult == true)
            {
                std::vector<config::CLanguage> languages;
                for (auto& file : files)
                {
                    xml::XmlDocumnent doc;
                    if (xml::XmlDoc::Open(file, doc) == true)
                    {
                        std::string szName = xml::XmlDoc::GetRootName(doc);
                        if (util::string::CompareNoCase(szName, "Language"))
                        {
                            config::CLanguage language;
                            if (xml::XmlConfig::LoadLanguage(doc, language))
                            {
                                languages.emplace_back(std::move(language));
                            }
                        }
                    }
                }

                this->m_Languages = std::move(languages);

                return true;
            }
            return false;
        }
        bool LoadLanguage(const std::wstring& szFileXml)
        {
            xml::XmlDocumnent doc;
            std::string szName = xml::XmlConfig::GetRootName(szFileXml, doc);
            if (!szName.empty() && util::string::CompareNoCase(szName, "Language"))
            {
                return this->LoadLanguage(doc);
            }
            return false;
        }
        bool LoadLanguage(xml::XmlDocumnent &doc)
        {
            config::CLanguage language;
            if (xml::XmlConfig::LoadLanguage(doc, language))
            {
                this->m_Languages.emplace_back(std::move(language));
                return true;
            }
            return false;
        }
    };
}
