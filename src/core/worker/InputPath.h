#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

namespace worker
{
    class CInputPath
    {
    public:
        wchar_t szInputFile[_MAX_PATH];
        wchar_t szInputFilePath[_MAX_PATH];
        wchar_t szSourceDrive[_MAX_DRIVE];
        wchar_t szSourceDir[_MAX_DIR];
        wchar_t szSourceName[_MAX_FNAME];
        wchar_t szSourceExt[_MAX_EXT];
        wchar_t szSourcePath[_MAX_PATH];
        std::vector<std::wstring> szSourceFolders;
    public:
        CInputPath(const wchar_t szInputFile[])
        {
            wcscpy_s(this->szInputFile, szInputFile);

            // make full path
            _wfullpath(this->szInputFilePath, this->szInputFile, _MAX_PATH);

            // split full path
            _wsplitpath_s(this->szInputFilePath, this->szSourceDrive, this->szSourceDir, this->szSourceName, this->szSourceExt);

            // source directory
            _wmakepath_s(this->szSourcePath, this->szSourceDrive, this->szSourceDir, nullptr, nullptr);

            // find dir folder names
            wchar_t szSourceDirCopy[_MAX_DIR];
            wcscpy_s(szSourceDirCopy, this->szSourceDir);

            wchar_t szSeparators[] = L"\\/";
            wchar_t *szNextToken = nullptr;
            wchar_t *szToken = wcstok_s(szSourceDirCopy, szSeparators, &szNextToken);
            if (szToken != nullptr)
                this->szSourceFolders.emplace_back(szToken);

            while (szToken != nullptr)
            {
                szToken = wcstok_s(nullptr, szSeparators, &szNextToken);
                if (szToken != nullptr)
                    this->szSourceFolders.emplace_back(szToken);
            }
        }
    };
}
