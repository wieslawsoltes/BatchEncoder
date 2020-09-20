#define VERSION_MAJOR               5
#define VERSION_MINOR               0
#define VERSION_REVISION            0
#define VERSION_BUILD               0

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VER_FILE_COMPANY_NAME_STR   "Wieslaw Soltes"
#define VER_FILE_DESCRIPTION_STR    "BatchEncoder (Audio Conversion GUI)"
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_LONG_STR   STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)    \
                                    "." STRINGIZE(VERSION_REVISION) \
                                    "." STRINGIZE(VERSION_BUILD)
#define VER_FILE_VERSION_SHORT_STR  STRINGIZE(VERSION_MAJOR)        \
                                    "." STRINGIZE(VERSION_MINOR)

#define VER_PRODUCTNAME_STR         "BatchEncoder"
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_LONG_STR

#if LIBRARY_EXPORTS
#define VER_ORIGINAL_FILENAME_STR VER_PRODUCTNAME_STR ".dll"
#else
#define VER_ORIGINAL_FILENAME_STR VER_PRODUCTNAME_STR ".exe"
#endif
#define VER_INTERNAL_NAME_STR       VER_ORIGINAL_FILENAME_STR

#define VER_COPYRIGHT_STR           "Copyright (c) Wieslaw Soltes. All rights reserved."

#ifdef _DEBUG
#define VER_VER_DEBUG             VS_FF_DEBUG
#else
#define VER_VER_DEBUG             0
#endif

#define VER_FILEOS                  VOS_NT_WINDOWS32
#define VER_FILEFLAGS               VER_VER_DEBUG

#if LIBRARY_EXPORTS
#define VER_FILETYPE              VFT_DLL
#else
#define VER_FILETYPE              VFT_APP
#endif
