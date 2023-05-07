//
// Created by plaka on 15.04.2023.
//

#include "FileLoader.h"
#include "../main.h"
#include "CFileMgr.h"

// Load line into static buffer (`ms_line`)
char* CFileLoader::LoadLine(FILE* file) {
   // if(!fgets(ms_line, sizeof(ms_line), file))
     //   return nullptr;

  //  return ms_line;

    if (!CFileMgr::ReadLine(file, ms_line, sizeof(ms_line)))
        return nullptr;

    // Sanitize it (otherwise random crashes appear)
    for (char* it = ms_line; *it; it++) {
        // Have to cast to uint8, because signed ASCII is retarded
        if ((uint8)*it < (uint8)' ' || *it == ',')
            *it = ' ';
    }

    return FindFirstNonNullOrWS(ms_line);
  //  return ( ( char*(*)(FILE*) )(g_libGTASA + 0x00394A30 + 1) )(file);
}

bool CFileMgr::ReadLine(FILESTREAM file, char *str, int32 num)
{
    return fgets(str, num, file) != nullptr;
}

/*!
* Load line from a text buffer with sanitization (replaces chars < 32 (space) with a space)
* @param bufferIt Iterator into buffer. It is modified by this function to point after the last character of this line
* @param buffSize Size of buffer. It is modified to represent the size of the buffer remaining after the end of this line
* @returns The beginning of the line - Note, this isn't a pointer into the passed in buffer!
* @addr 0x536FE0
*/
char* CFileLoader::LoadLine(char*& bufferIt, int32& buffSize) {
    return ( ( char*(*)(char*, int32) )(g_libGTASA + 0x00394A80 + 1) )(bufferIt, buffSize);
}

char* CFileLoader::FindFirstNonNullOrWS(char* it) {
    // Have to cast to uint8, because signed ASCII is retarded
    for (; *it && (uint8)*it <= (uint8)' '; it++);
    return it;
}