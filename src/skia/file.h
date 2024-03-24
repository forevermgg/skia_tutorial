#ifndef SKIA_TUTORIAL_FILE_H
#define SKIA_TUTORIAL_FILE_H

#include <filesystem>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else

#include <unistd.h>

#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define PATH_SEPARATOR '\\'
#else

#include <unistd.h>

#define PATH_SEPARATOR '/'
#endif

class File {
public:
    static std::string get_current_directory();
};


#endif //SKIA_TUTORIAL_FILE_H
