//
// Created by plaka on 14.04.2023.
//

#pragma once


#include <cstdint>
#include <cstdio>

// For backward compatibility
typedef FILE* FILESTREAM;
constexpr size_t DIRNAMELENGTH = 128;

class CFileMgr {
public:
    static char (&ms_dirName)[128];
    static char (&ms_rootDirName)[128];

    static void Initialise();
    static int32_t ChangeDir(const char* path);
    static void SetDir(const char* path);
    static int32_t SetDirMyDocuments();
    static size_t LoadFile(const char* path, uint8_t* buf, size_t size, const char* mode);
    static FILESTREAM OpenFile(const char* path, const char* mode);
    static FILESTREAM OpenFileForWriting(const char* path);
    static FILESTREAM OpenFileForAppending(const char* path);
    static size_t Read(FILESTREAM file, void* buf, size_t size);
    static size_t Write(FILESTREAM file, const void* buf, size_t size);
    static bool Seek(FILESTREAM file, long offset, int32_t origin);
    static bool ReadLine(FILESTREAM file, char* str, int32_t num);
    static int32_t CloseFile(FILESTREAM file);
    static int32_t GetTotalSize(FILESTREAM file);
    static int32_t Tell(FILESTREAM file);
    static bool GetErrorReadWrite(FILESTREAM file);

    //! Increment the file's seek pointer until after the next new line (`\n`) (make sure file is open in non-binary mode!)
    static void SeekNextLine(FILESTREAM file);
};

