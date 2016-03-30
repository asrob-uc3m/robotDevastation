#ifndef __RD_UTILS_HPP__
#define __RD_UTILS_HPP__

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdUtilsLib
 *
 * @brief The RdUtilsLib library contains miscellaneous macros and utilities
 */
//-- Include all utils
#include "RdVocabs.hpp"
#include "RdMacros.hpp"
#include "RdVector2dBase.hpp"
#include "RdIniReader.hpp"

//-- Misc utils ----------------------------------------------//
#include "string"

template<typename T> std::string number2str(T number)
{
    std::stringstream sstream;
    sstream << number;
    return sstream.str();
}

#endif
