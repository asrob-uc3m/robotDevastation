// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MACROS_HPP__
#define __RD_MACROS_HPP__

#include <cstdio>   // fflush()
#include <cstring>  // strrchr()

//-- Fix for old Windows versions.
//-- Thanks: tomlogic @ http://stackoverflow.com/questions/2281970/cross-platform-defining-define-for-macros-function-and-func
#if defined(WIN32) && !defined(__func__)
#define __func__ __FUNCTION__
#endif

//-- Thanks http://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#ifdef WIN32
#define __REL_FILE__ (std::strrchr(__FILE__, '\\') ? std::strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __REL_FILE__ (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)
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
#define RD_ERROR(...) { std::fprintf(stderr, RED); do{std::fprintf(stderr, "[error] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                        std::fprintf(stderr, __VA_ARGS__);} while(0); std::fprintf(stderr, RESET); std::fflush(stderr); }

#define RD_WARNING(...) { std::fprintf(stderr, YELLOW); do{std::fprintf(stderr, "[warning] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                          std::fprintf(stderr, __VA_ARGS__);} while(0); std::fprintf(stderr, RESET); std::fflush(stderr); }

#define RD_SUCCESS(...) { std::printf(GREEN); do{std::printf("[success] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                          std::printf(__VA_ARGS__);} while(0); std::printf(RESET); std::fflush(stdout); }

#define RD_INFO(...) { do{std::printf("[info] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                          std::printf(__VA_ARGS__);} while(0); std::fflush(stdout); }

#define RD_DEBUG(...) { std::printf(BLUE); do{std::printf("[debug] %s:%d %s(): ", __REL_FILE__, __LINE__, __func__); \
                        std::printf(__VA_ARGS__);} while(0); std::printf(RESET); std::fflush(stdout); }

#endif  // __RD_MACROS_HPP__
