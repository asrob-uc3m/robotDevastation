// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MACROS_HPP__
#define __RD_MACROS_HPP__

#include <stdio.h>
#include <string>  // std::string
#include <string.h>  // strrchr

namespace rdlib{

// begin{git/openrave/include/openrave/openrave.h copy}

// terminal attributes
//#define RESET           0
//#define BRIGHT          1
//#define DIM             2
//#define UNDERLINE       3
//#define BLINK           4
//#define REVERSE         7
//#define HIDDEN          8
// terminal colors
//#define BLACK           0
//#define RED             1
//#define GREEN           2
//#define YELLOW          3
//#define BLUE            4
//#define MAGENTA         5
//#define CYAN            6
//#define WHITE           7

/// Change the text color (on either stdout or stderr) with an attr:fg:bg (thanks to Radu Rusu for the code)
inline std::string ChangeTextColor (int attribute, int fg, int bg)
{
    char command[13];
    sprintf (command, "%c[%d;%d;%dm", 0x1B, attribute, fg + 30, bg + 40);
    return command;
}

/// Change the text color (on either stdout or stderr) with an attr:fg (thanks to Radu Rusu for the code)
inline std::string ChangeTextColor (int attribute, int fg)
{
    char command[13];
    sprintf (command, "%c[%d;%dm", 0x1B, attribute, fg + 30);
    return command;
}

/// Reset the text color (on either stdout or stderr) to its original state (thanks to Radu Rusu for the code)
inline std::string ResetTextColor()
{
    char command[12];
    sprintf (command, "%c[0;38;48m", 0x1B);
    return command;
}

inline std::wstring ChangeTextColorW (int attribute, int fg)
{
    wchar_t command[13];
    swprintf (command, 13, L"%c[%d;%dm", 0x1B, attribute, fg + 30);
    return command;
}

// end{git/openrave/include/openrave/openrave.h copy}

//-- Thanks http://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __REL_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// For Windows use '\\' instead of '/'.

// http://en.wikipedia.org/wiki/Variadic_macro
// http://stackoverflow.com/questions/15549893/modify-printfs-via-macro-to-include-file-and-line-number-information
#define RD_ERROR(...) do{fprintf(stderr, "[error] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                         fprintf(stderr, __VA_ARGS__);} while(0)

#define RD_WARNING(...) do{fprintf(stderr, "[warning] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                           fprintf(stderr, __VA_ARGS__);} while(0)

#define RD_SUCCESS(...) do{printf("[success] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                           printf(__VA_ARGS__);} while(0)

#define RD_INFO(...) do{printf("[info] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                           printf(__VA_ARGS__);} while(0)

} //rdlib

#endif  // __RD_MACROS_HPP__

