// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MACROS_HPP__
#define __RD_MACROS_HPP__

#include <stdio.h>
#include <string>  // std::string
#include <string.h>  // strrchr

namespace rdlib{

//-- Thanks http://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __REL_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// For Windows use '\\' instead of '/'.

// http://stackoverflow.com/questions/1961209/making-some-text-in-printf-appear-in-green-and-red
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

// http://en.wikipedia.org/wiki/Variadic_macro
// http://stackoverflow.com/questions/15549893/modify-printfs-via-macro-to-include-file-and-line-number-information
#define RD_ERROR(...) fprintf(stderr,RED); do{fprintf(stderr, "[error] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                         fprintf(stderr, __VA_ARGS__);} while(0); fprintf(stderr,RESET);

#define RD_WARNING(...) fprintf(stderr,YELLOW); do{fprintf(stderr, "[warning] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                           fprintf(stderr, __VA_ARGS__);} while(0); fprintf(stderr,RESET);

#define RD_SUCCESS(...) fprintf(stderr,GREEN); do{printf("[success] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                           printf(__VA_ARGS__);} while(0); fprintf(stderr,RESET); 

#define RD_INFO(...) do{printf("[info] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                           printf(__VA_ARGS__);} while(0)

} //rdlib

#endif  // __RD_MACROS_HPP__

