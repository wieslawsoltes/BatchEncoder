// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "..\BatchEncoder.h"
#include "..\utilities\Utilities.h"
#include "WorkThread.h"

enum Mode { None = -1, Encode = 0, Transcode = 1 };

bool FileExists(CString szPath)
{
    WIN32_FIND_DATA w32FileData;
    ZeroMemory(&w32FileData, sizeof(WIN32_FIND_DATA));
    HANDLE hFind = ::FindFirstFile(szPath, &w32FileData);
    bool bInvalidHandle = hFind == INVALID_HANDLE_VALUE;
    ::FindClose(hFind);
    return bInvalidHandle == false;
}

bool ConvertItem(CItemContext* pContext)
{
    // input file
    CString szInputFile = pContext->item->szPath;

    // validate input file
    if (FileExists(szInputFile) == false)
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find input file."));
        return false;
    }

    // output path
    CString szOutPath;
    if (pContext->pWorkerContext->pConfig->m_Options.bOutputPathChecked == true)
    {
        szOutPath = pContext->pWorkerContext->pConfig->m_Options.szOutputPath;
    }
    else
    {
        CString szInputFileName = ::GetFileName(szInputFile);
        szOutPath = szInputFile;
        szOutPath.Truncate(szOutPath.GetLength() - szInputFileName.GetLength());
    }

    // find encoder
    int nEncoder = pContext->pWorkerContext->pConfig->m_Formats.GetFormatById(pContext->item->szFormatId);
    if (nEncoder == -1)
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find valid encoder by id."));
        return false;
    }

    CFormat& encoderFormat = pContext->pWorkerContext->pConfig->m_Formats.GetData(nEncoder);

    // validate encoder executable
    if (FileExists(encoderFormat.szPath) == false)
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find encoder executable."));
        return false;
    }

    // validate encoder preset
    if (pContext->item->nPreset >= encoderFormat.m_Presets.GetSize())
    {
        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find encoder format preset."));
        return false;
    }

    // validate input extension
    CString szInputFileExt = ::GetFileExtension(szInputFile).MakeUpper();
    bool bIsValidEncoderInput = encoderFormat.IsValidInputExtension(szInputFileExt);

    // processing mode
    Mode nProcessingMode = Mode::None;
    if (bIsValidEncoderInput)
        nProcessingMode = Mode::Encode;
    else
        nProcessingMode = Mode::Transcode;

    // output path
    CString szEncoderExtension = encoderFormat.szOutputExtension;
    CString szName = pContext->item->szName + _T(".") + szEncoderExtension.MakeLower();
    CString szOutputFile;
    if (szOutPath.GetLength() >= 1)
    {
        if (szOutPath[szOutPath.GetLength() - 1] == '\\' || szOutPath[szOutPath.GetLength() - 1] == '/')
            szOutputFile = szOutPath + szName;
        else
            szOutputFile = szOutPath + _T("\\") + szName;
    }
    else
    {
        szOutputFile = szName;
    }

    CString szOrgInputFile = szInputFile;
    CString szOrgOutputFile = szOutputFile;

    if (nProcessingMode == Mode::Transcode)
    {
        // find decoder
        int nDecoder = pContext->pWorkerContext->pConfig->m_Formats.GetDecoderByExtension(pContext->item->szExtension);
        if (nDecoder == -1)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find valid decoder by extension."));
            return false;
        }

        CFormat& decoderFormat = pContext->pWorkerContext->pConfig->m_Formats.GetData(nDecoder);

        // validate decoder executable
        if (FileExists(decoderFormat.szPath) == false)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find decoder executable."));
            return false;
        }

        // validate decoder preset
        if (decoderFormat.nDefaultPreset >= decoderFormat.m_Presets.GetSize())
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find decoder format preset."));
            return false;
        }

        // validate decoder output extension
        bIsValidEncoderInput = encoderFormat.IsValidInputExtension(decoderFormat.szOutputExtension);
        if (bIsValidEncoderInput == false)
        {
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: decoder output not supported by encoder."));
            return false;
        }

        CString szDecoderExtension = decoderFormat.szOutputExtension;
        szOutputFile = szOutputFile + +_T(".") + szDecoderExtension.MakeLower();

        pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Decoding..."));
        try
        {
            CFileContext context(pContext->pWorkerContext, decoderFormat, decoderFormat.nDefaultPreset, pContext->item->nId, szInputFile, szOutputFile);
            if (::ConvertFile(&context) == false)
            {
                if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szOutputFile);

                return false;
            }

            // validate decoded file
            if (FileExists(szOutputFile) == false)
            {
                pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find decoded file."));
                return false;
            }
        }
        catch (...)
        {
            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szOutputFile);

            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: exception thrown while converting file."));
            pContext->pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    if (pContext->pWorkerContext->bRunning == false)
        return false;

    if (nProcessingMode == Mode::Transcode)
    {
        // decoder output file as encoder input file
        szInputFile = szOutputFile;

        // original encoder output file
        szOutputFile = szOrgOutputFile;
    }

    if ((nProcessingMode == Mode::Encode) || (nProcessingMode == Mode::Transcode))
    {
        if (encoderFormat.nType == 0)
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Encoding..."));
        else if (encoderFormat.nType == 1)
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Decoding..."));
        else
            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Processing..."));

        try
        {
            CFileContext context(pContext->pWorkerContext, encoderFormat, pContext->item->nPreset, pContext->item->nId, szInputFile, szOutputFile);
            if (::ConvertFile(&context) == true)
            {
                // validate encoded file
                if (FileExists(szOutputFile) == false)
                {
                    if (nProcessingMode == Mode::Transcode)
                    {
                        if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                            ::DeleteFile(szInputFile);
                    }

                    pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: can not find encoded file."));
                    return false;
                }

                if (nProcessingMode == Mode::Transcode)
                    ::DeleteFile(szInputFile);

                if (pContext->pWorkerContext->pConfig->m_Options.bDeleteSourceFiles == true)
                    ::DeleteFile(szOrgInputFile);

                return true;
            }
            else
            {
                if (nProcessingMode == Mode::Transcode)
                    ::DeleteFile(szInputFile);

                if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                    ::DeleteFile(szOutputFile);

                return false;
            }
        }
        catch (...)
        {
            if (nProcessingMode == Mode::Transcode)
                ::DeleteFile(szInputFile);

            if (pContext->pWorkerContext->pConfig->m_Options.bDeleteOnErrors == true)
                ::DeleteFile(szOutputFile);

            pContext->pWorkerContext->Status(pContext->item->nId, _T("--:--"), _T("Error: exception thrown while converting file."));
            pContext->pWorkerContext->Callback(pContext->item->nId, -1, true, true);
        }
    }

    return false;
}
