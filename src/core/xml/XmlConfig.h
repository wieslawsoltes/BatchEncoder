// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <string.h>
#include "tinyxml2\tinyxml2.h" // https://github.com/leethomason/tinyxml2
#include "utilities\Utf8String.h"
#include "config\Options.h"
#include "config\Item.h"
#include "config\ItemsList.h"
#include "utilities\Language.h"
#include "utilities\LanguagesList.h"
#include "utilities\StringHelper.h"
#include "config\Preset.h"
#include "config\PresetsList.h"
#include "config\Format.h"
#include "config\FormatsList.h"
#include "config\Tool.h"
#include "config\ToolsList.h"

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
        const std::wstring m_True = _T("true");
        const std::wstring m_False = _T("false");
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
            return _wtoi(ToString(pszUtf8).c_str());
        }
        unsigned __int64 ToUInt64(const char *pszUtf8)
        {
           return _wcstoui64(ToString(pszUtf8).c_str(), nullptr, 10);
        }
        bool ToBool(const char *pszUtf8)
        {
            return _wcsicmp(ToString(pszUtf8).c_str(), m_True.c_str()) == 0;
        }
        std::wstring ToString(const int nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring ToString(const unsigned __int64 nValue)
        {
            return std::to_wstring(nValue);
        }
        std::wstring ToString(const bool bValue)
        {
            return bValue ? m_True : m_False;
        }
    public:
        bool GetAttributeValue(const XmlElement *element, const char *name, std::wstring *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToString(pszResult).c_str();
                return true;
            }
            return false;
        }
        bool GetAttributeValue(const XmlElement *element, const char *name, int *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToInt(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValue(const XmlElement *element, const char *name, unsigned __int64 *value)
        {
            const char *pszResult = element->Attribute(name);
            if (pszResult != nullptr)
            {
                (*value) = ToUInt64(pszResult);
                return true;
            }
            return false;
        }
        bool GetAttributeValue(const XmlElement *element, const char *name, bool *value)
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
        void SetAttributeValue(XmlElement *element, const char *name, const std::wstring& value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(value).c_str());
        }
        void SetAttributeValue(XmlElement *element, const char *name, const int &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(ToString(value)).c_str());
        }
        void SetAttributeValue(XmlElement *element, const char *name, const unsigned __int64 &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(ToString(value)).c_str());
        }
        void SetAttributeValue(XmlElement *element, const char *name, const bool &value)
        {
            element->SetAttribute(name, util::CUtf8String::ToUtf8(ToString(value)).c_str());
        }
    public:
        bool GetChildValue(const XmlElement *parent, const char *name, std::wstring *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToString(element->GetText()).c_str();
                return true;
            }
            return false;
        }
        bool GetChildValue(const XmlElement *parent, const char *name, int *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToInt(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValue(const XmlElement *parent, const char *name, unsigned __int64 *value)
        {
            auto element = parent->FirstChildElement(name);
            if (element != nullptr)
            {
                (*value) = ToUInt64(element->GetText());
                return true;
            }
            return false;
        }
        bool GetChildValue(const XmlElement *parent, const char *name, bool *value)
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
        void SetChildValue(XmlElement *parent, const char *name, const std::wstring& value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(value).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValue(XmlElement *parent, const char *name, const int &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(ToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValue(XmlElement *parent, const char *name, const unsigned __int64 &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(ToString(value)).c_str()));
            parent->LinkEndChild(element);
        }
        void SetChildValue(XmlElement *parent, const char *name, const bool &value)
        {
            auto element = m_Document.NewElement(name);
            element->LinkEndChild(m_Document.NewText(util::CUtf8String::ToUtf8(ToString(value)).c_str()));
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
            VALIDATE(GetChildValue(element, "SelectedLanguage", &m_Options.szSelectedLanguage));
            VALIDATE(GetChildValue(element, "SelectedFormat", &m_Options.nSelectedFormat));
            VALIDATE(GetChildValue(element, "OutputPath", &m_Options.szOutputPath));
            VALIDATE(GetChildValue(element, "DeleteSourceFiles", &m_Options.bDeleteSourceFiles));
            VALIDATE(GetChildValue(element, "RecurseChecked", &m_Options.bRecurseChecked));
            VALIDATE(GetChildValue(element, "ShutdownWhenFinished", &m_Options.bShutdownWhenFinished));
            VALIDATE(GetChildValue(element, "DoNotSaveConfiguration", &m_Options.bDoNotSaveConfiguration));
            VALIDATE(GetChildValue(element, "DeleteOnErrors", &m_Options.bDeleteOnErrors));
            VALIDATE(GetChildValue(element, "StopOnErrors", &m_Options.bStopOnErrors));
            VALIDATE(GetChildValue(element, "HideConsoleWindow", &m_Options.bHideConsoleWindow));
            VALIDATE(GetChildValue(element, "TryToFindDecoder", &m_Options.bTryToFindDecoder));
            VALIDATE(GetChildValue(element, "EnsureItemIsVisible", &m_Options.bEnsureItemIsVisible));
            VALIDATE(GetChildValue(element, "ValidateInputFiles", &m_Options.bValidateInputFiles));
            VALIDATE(GetChildValue(element, "OverwriteExistingFiles", &m_Options.bOverwriteExistingFiles));
            VALIDATE(GetChildValue(element, "TryToDownloadTools", &m_Options.bTryToDownloadTools));
            VALIDATE(GetChildValue(element, "ThreadCount", &m_Options.nThreadCount));
            VALIDATE(GetChildValue(element, "OutputBrowse", &m_Options.szOutputBrowse));
            VALIDATE(GetChildValue(element, "DirectoryBrowse", &m_Options.szDirectoryBrowse));
            VALIDATE(GetChildValue(element, "MainWindowResize", &m_Options.szMainWindowResize));
            VALIDATE(GetChildValue(element, "FileListColumns", &m_Options.szFileListColumns));
            VALIDATE(GetChildValue(element, "PresetsDialogResize", &m_Options.szPresetsDialogResize));
            VALIDATE(GetChildValue(element, "PresetsListColumns", &m_Options.szPresetsListColumns));
            VALIDATE(GetChildValue(element, "FormatsDialogResize", &m_Options.szFormatsDialogResize));
            VALIDATE(GetChildValue(element, "FormatsListColumns", &m_Options.szFormatsListColumns));
            VALIDATE(GetChildValue(element, "ToolsDialogResize", &m_Options.szToolsDialogResize));
            VALIDATE(GetChildValue(element, "ToolsListColumns", &m_Options.szToolsListColumns));
            return true;
        }
        void SetOptions(XmlElement *element, config::COptions &m_Options)
        {
            SetChildValue(element, "SelectedLanguage", m_Options.szSelectedLanguage);
            SetChildValue(element, "SelectedFormat", m_Options.nSelectedFormat);
            SetChildValue(element, "OutputPath", m_Options.szOutputPath);
            SetChildValue(element, "DeleteSourceFiles", m_Options.bDeleteSourceFiles);
            SetChildValue(element, "RecurseChecked", m_Options.bRecurseChecked);
            SetChildValue(element, "ShutdownWhenFinished", m_Options.bShutdownWhenFinished);
            SetChildValue(element, "DoNotSaveConfiguration", m_Options.bDoNotSaveConfiguration);
            SetChildValue(element, "DeleteOnErrors", m_Options.bDeleteOnErrors);
            SetChildValue(element, "StopOnErrors", m_Options.bStopOnErrors);
            SetChildValue(element, "HideConsoleWindow", m_Options.bHideConsoleWindow);
            SetChildValue(element, "TryToFindDecoder", m_Options.bTryToFindDecoder);
            SetChildValue(element, "EnsureItemIsVisible", m_Options.bEnsureItemIsVisible);
            SetChildValue(element, "ValidateInputFiles", m_Options.bValidateInputFiles);
            SetChildValue(element, "OverwriteExistingFiles", m_Options.bOverwriteExistingFiles);
            SetChildValue(element, "TryToDownloadTools", m_Options.bTryToDownloadTools);
            SetChildValue(element, "ThreadCount", m_Options.nThreadCount);
            SetChildValue(element, "OutputBrowse", m_Options.szOutputBrowse);
            SetChildValue(element, "DirectoryBrowse", m_Options.szDirectoryBrowse);
            SetChildValue(element, "MainWindowResize", m_Options.szMainWindowResize);
            SetChildValue(element, "FileListColumns", m_Options.szFileListColumns);
            SetChildValue(element, "PresetsDialogResize", m_Options.szPresetsDialogResize);
            SetChildValue(element, "PresetsListColumns", m_Options.szPresetsListColumns);
            SetChildValue(element, "FormatsDialogResize", m_Options.szFormatsDialogResize);
            SetChildValue(element, "FormatsListColumns", m_Options.szFormatsListColumns);
            SetChildValue(element, "ToolsDialogResize", m_Options.szToolsDialogResize);
            SetChildValue(element, "ToolsListColumns", m_Options.szToolsListColumns);
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
            VALIDATE(GetAttributeValue(element, "path", &m_Path.szPath));
            VALIDATE(GetAttributeValue(element, "size", &m_Path.nSize));
            return true;
        }
        void SetPath(XmlElement *element, config::CPath &m_Path)
        {
            SetAttributeValue(element, "path", m_Path.szPath);
            SetAttributeValue(element, "size", m_Path.nSize);
        }
        bool GetPaths(const XmlElement *parent, config::CPathsList &m_Paths)
        {
            auto element = parent->FirstChildElement("Path");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CPath path;
                    VALIDATE(this->GetPath(element, path));
                    m_Paths.Insert(path);
                }
                return true;
            }
            return false;
        }
        void SetPaths(XmlElement *parent, config::CPathsList &m_Paths)
        {
            int nPaths = m_Paths.Count();
            for (int i = 0; i < nPaths; i++)
            {
                config::CPath& path = m_Paths.Get(i);
                auto element = this->NewElement("Path");
                parent->LinkEndChild(element);
                this->SetPath(element, path);
            }
        }
    public:
        bool GetItem(const XmlElement *element, config::CItem &m_Item)
        {
            VALIDATE(GetAttributeValue(element, "id", &m_Item.nId));
            VALIDATE(GetAttributeValue(element, "size", &m_Item.nSize));
            VALIDATE(GetAttributeValue(element, "name", &m_Item.szName));
            VALIDATE(GetAttributeValue(element, "extension", &m_Item.szExtension));
            VALIDATE(GetAttributeValue(element, "format", &m_Item.szFormatId));
            VALIDATE(GetAttributeValue(element, "preset", &m_Item.nPreset));
            VALIDATE(GetAttributeValue(element, "options", &m_Item.szOptions));
            VALIDATE(GetAttributeValue(element, "checked", &m_Item.bChecked));
            VALIDATE(GetAttributeValue(element, "time", &m_Item.szTime));
            VALIDATE(GetAttributeValue(element, "status", &m_Item.szStatus));
            VALIDATE(this->GetPaths(element, m_Item.m_Paths));
            return true;
        }
        void SetItem(XmlElement *element, config::CItem &m_Item, int nId)
        {
            SetAttributeValue(element, "id", nId);
            SetAttributeValue(element, "size", m_Item.nSize);
            SetAttributeValue(element, "name", m_Item.szName);
            SetAttributeValue(element, "extension", m_Item.szExtension);
            SetAttributeValue(element, "format", m_Item.szFormatId);
            SetAttributeValue(element, "preset", m_Item.nPreset);
            SetAttributeValue(element, "options", m_Item.szOptions);
            SetAttributeValue(element, "checked", m_Item.bChecked);
            SetAttributeValue(element, "time", m_Item.szTime);
            SetAttributeValue(element, "status", m_Item.szStatus);
            this->SetPaths(element, m_Item.m_Paths);
        }
        bool GetItems(const XmlElement *parent, config::CItemsList &m_Items)
        {
            auto element = parent->FirstChildElement("Item");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CItem item;
                    VALIDATE(this->GetItem(element, item));
                    m_Items.Insert(item);
                }
                return true;
            }
            return false;
        }
        void SetItems(XmlElement *parent, config::CItemsList &m_Items)
        {
            int nItems = m_Items.Count();
            for (int i = 0; i < nItems; i++)
            {
                config::CItem& item = m_Items.Get(i);
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
        bool GetItems(config::CItemsList &m_Items)
        {
            auto element = this->FirstChildElement("Items");
            if (element != nullptr)
            {
                VALIDATE(this->GetItems(element, m_Items));
                return true;
            }
            return false;
        }
        void SetItems(config::CItemsList &m_Items)
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
        bool GetLanguage(const XmlElement *parent, lang::CLanguage &m_Language)
        {
            VALIDATE(GetAttributeValue(parent, "id", &m_Language.szId));
            VALIDATE(GetAttributeValue(parent, "original", &m_Language.szOriginalName));
            VALIDATE(GetAttributeValue(parent, "translated", &m_Language.szTranslatedName));

            auto element = parent->FirstChildElement("String");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    std::wstring szKey;
                    std::wstring szValue;

                    VALIDATE(GetAttributeValue(element, "key", &szKey));
                    VALIDATE(GetAttributeValue(element, "value", &szValue));

                    int nKey = util::StringHelper::ToIntFromHex(szKey);
                    m_Language.m_Strings.Insert(nKey, szValue);
                }
                return true;
            }
            return false;
        }
        void SetLanguage(XmlElement *parent, lang::CLanguage &m_Language)
        {
            SetAttributeValue(parent, "id", m_Language.szId);
            SetAttributeValue(parent, "original", m_Language.szOriginalName);
            SetAttributeValue(parent, "translated", m_Language.szTranslatedName);

            for (auto& item : m_Language.m_Strings.m_Map)
            {
                int nKey = item.first;
                std::wstring szKey = util::StringHelper::ToWStringHex(nKey);

                auto element = this->NewElement("String");
                parent->LinkEndChild(element);

                SetAttributeValue(element, "", szKey);
                SetAttributeValue(element, "", item.second);
            }
        }
    public:
        bool GetLanguage(lang::CLanguage &m_Language)
        {
            auto element = this->FirstChildElement("Language");
            if (element != nullptr)
            {
                VALIDATE(this->GetLanguage(element, m_Language));
                return true;
            }
            return false;
        }
        void SetLanguage(lang::CLanguage &m_Language)
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
            VALIDATE(GetAttributeValue(element, "name", &m_Preset.szName));
            VALIDATE(GetAttributeValue(element, "options", &m_Preset.szOptions));
            return true;
        }
        void SetPreset(XmlElement *element, config::CPreset &m_Preset)
        {
            SetAttributeValue(element, "name", m_Preset.szName);
            SetAttributeValue(element, "options", m_Preset.szOptions);
        }
        bool GetPresets(const XmlElement *parent, config::CPresetsList &m_Presets)
        {
            auto element = parent->FirstChildElement("Preset");
            if (element != nullptr)
            {
                for (element; element; element = element->NextSiblingElement())
                {
                    config::CPreset preset;
                    VALIDATE(this->GetPreset(element, preset));
                    m_Presets.Insert(preset);
                }
                return true;
            }
            return false;
        }
        void SetPresets(XmlElement *parent, config::CPresetsList &m_Presets)
        {
            int nPresets = m_Presets.Count();
            for (int i = 0; i < nPresets; i++)
            {
                config::CPreset& preset = m_Presets.Get(i);
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
        bool GetPresets(config::CPresetsList &m_Presets)
        {
            auto element = this->FirstChildElement("Presets");
            if (element != nullptr)
            {
                VALIDATE(this->GetPresets(element, m_Presets));
                return true;
            }
            return false;
        }
        void SetPresets(config::CPresetsList &m_Presets)
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
            VALIDATE(GetAttributeValue(element, "id", &m_Format.szId));
            VALIDATE(GetAttributeValue(element, "name", &m_Format.szName));
            VALIDATE(GetAttributeValue(element, "template", &m_Format.szTemplate));
            VALIDATE(GetAttributeValue(element, "input", &m_Format.bPipeInput));
            VALIDATE(GetAttributeValue(element, "output", &m_Format.bPipeOutput));
            VALIDATE(GetAttributeValue(element, "function", &m_Format.szFunction));
            VALIDATE(GetAttributeValue(element, "path", &m_Format.szPath));
            VALIDATE(GetAttributeValue(element, "success", &m_Format.nExitCodeSuccess));

            int nType;
            VALIDATE(GetAttributeValue(element, "type", &nType));
            m_Format.nType = config::CFormat::FromInt(nType);

            VALIDATE(GetAttributeValue(element, "priority", &m_Format.nPriority));

            VALIDATE(GetAttributeValue(element, "formats", &m_Format.szInputExtensions));
            VALIDATE(GetAttributeValue(element, "extension", &m_Format.szOutputExtension));
            VALIDATE(GetAttributeValue(element, "default", &m_Format.nDefaultPreset));

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
            SetAttributeValue(element, "id", m_Format.szId);
            SetAttributeValue(element, "name", m_Format.szName);
            SetAttributeValue(element, "template", m_Format.szTemplate);
            SetAttributeValue(element, "input", m_Format.bPipeInput);
            SetAttributeValue(element, "output", m_Format.bPipeOutput);
            SetAttributeValue(element, "function", m_Format.szFunction);
            SetAttributeValue(element, "path", m_Format.szPath);
            SetAttributeValue(element, "success", m_Format.nExitCodeSuccess);

            int nType = config::CFormat::ToInt(m_Format.nType);
            SetAttributeValue(element, "type", nType);

            SetAttributeValue(element, "priority", m_Format.nPriority);

            SetAttributeValue(element, "formats", m_Format.szInputExtensions);
            SetAttributeValue(element, "extension", m_Format.szOutputExtension);
            SetAttributeValue(element, "default", m_Format.nDefaultPreset);

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
            VALIDATE(GetAttributeValue(element, "name", &m_Tool.szName));
            VALIDATE(GetAttributeValue(element, "platform", &m_Tool.szPlatform));
            VALIDATE(GetAttributeValue(element, "priority", &m_Tool.nPriority));
            VALIDATE(GetAttributeValue(element, "formats", &m_Tool.szFormats));
            VALIDATE(GetAttributeValue(element, "url", &m_Tool.szUrl));
            VALIDATE(GetAttributeValue(element, "file", &m_Tool.szFile));
            VALIDATE(GetAttributeValue(element, "extract", &m_Tool.szExtract));
            VALIDATE(GetAttributeValue(element, "path", &m_Tool.szPath));
            return true;
        }
        void SetTool(XmlElement *element, config::CTool &m_Tool)
        {
            SetAttributeValue(element, "name", m_Tool.szName);
            SetAttributeValue(element, "platform", m_Tool.szPlatform);
            SetAttributeValue(element, "priority", m_Tool.nPriority);
            SetAttributeValue(element, "formats", m_Tool.szFormats);
            SetAttributeValue(element, "url", m_Tool.szUrl);
            SetAttributeValue(element, "file", m_Tool.szFile);
            SetAttributeValue(element, "extract", m_Tool.szExtract);
            SetAttributeValue(element, "path", m_Tool.szPath);
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
        static bool LoadItems(XmlDocumnent &doc, config::CItemsList &items)
        {
            XmlItems xml(doc);
            return xml.GetItems(items);
        }
        static bool LoadItems(const std::wstring& szFileXml, config::CItemsList &items)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadItems(doc, items);
            return false;
        }
        static bool SaveItems(const std::wstring& szFileXml, config::CItemsList &items)
        {
            XmlDocumnent doc;
            XmlItems xml(doc);
            xml.Create();
            xml.SetItems(items);
            return xml.Save(szFileXml);
        }
    public:
        static bool LoadLanguage(XmlDocumnent &doc, lang::CLanguage &language)
        {
            XmlLanguages xml(doc);
            return xml.GetLanguage(language);
        }
        static bool LoadLanguage(const std::wstring& szFileXml, lang::CLanguage &language)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadLanguage(doc, language);
            return false;
        }
        static bool SaveLanguage(const std::wstring& szFileXml, lang::CLanguage &language)
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
        static bool LoadPresets(XmlDocumnent &doc, config::CPresetsList &presets)
        {
            XmlPresets xml(doc);
            return xml.GetPresets(presets);
        }
        static bool LoadPresets(const std::wstring& szFileXml, config::CPresetsList &presets)
        {
            XmlDocumnent doc;
            if (XmlDoc::Open(szFileXml, doc) == true)
                return LoadPresets(doc, presets);
            return false;
        }
        static bool SavePresets(const std::wstring& szFileXml, config::CPresetsList &presets)
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
