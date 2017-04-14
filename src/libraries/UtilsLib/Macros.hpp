// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MACROS_HPP__
#define __RD_MACROS_HPP__

#include <stdio.h>
#include <string>  // std::string
#include <string.h>  // strrchr

//-- Fix for old Windows versions.
//-- Thanks: tomlogic @ http://stackoverflow.com/questions/2281970/cross-platform-defining-define-for-macros-function-and-func
#if defined(WIN32) && !defined(__func__)
#define __func__ __FUNCTION__
#endif

//-- Thanks http://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#ifdef WIN32
#define __REL_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __REL_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef WIN32
// Could be implemented for Win10: http://stackoverflow.com/a/38617204
#define RESET   ""
#define BLACK   ""      /* Black */
#define RED     ""      /* Red */
#define GREEN   ""      /* Green */
#define YELLOW  ""      /* Yellow */
#define BLUE    ""      /* Blue */
#define MAGENTA ""      /* Magenta */
#define CYAN    ""      /* Cyan */
#define WHITE   ""      /* White */
#define BOLDBLACK   ""      /* Bold Black */
#define BOLDRED     ""      /* Bold Red */
#define BOLDGREEN   ""      /* Bold Green */
#define BOLDYELLOW  ""      /* Bold Yellow */
#define BOLDBLUE    ""      /* Bold Blue */
#define BOLDMAGENTA ""      /* Bold Magenta */
#define BOLDCYAN    ""      /* Bold Cyan */
#define BOLDWHITE   ""      /* Bold White */
#else
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
#endif

// http://en.wikipedia.org/wiki/Variadic_macro
// http://stackoverflow.com/questions/15549893/modify-printfs-via-macro-to-include-file-and-line-number-information
#define RD_ERROR(...) { fprintf(stderr,RED); do{fprintf(stderr, "[error] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                        fprintf(stderr, __VA_ARGS__);} while(0); fprintf(stderr,RESET); }

#define RD_WARNING(...) { fprintf(stderr,YELLOW); do{fprintf(stderr, "[warning] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                          fprintf(stderr, __VA_ARGS__);} while(0); fprintf(stderr,RESET); }

#define RD_SUCCESS(...) { printf(GREEN); do{printf("[success] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                          printf(__VA_ARGS__);} while(0); printf(RESET); }

#define RD_INFO(...) { do{printf("[info] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                          printf(__VA_ARGS__);} while(0); }

#define RD_DEBUG(...) { printf(BLUE); do{printf("[debug] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                        printf(__VA_ARGS__);} while(0); printf(RESET); }

#endif  // __RD_MACROS_HPP__
