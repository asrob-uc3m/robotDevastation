// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INI_READER_HPP__
#define __RD_INI_READER_HPP__

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdIniReaderLib
 *
 * @brief The RdIniReaderLib library contains the \ref RdIniReader class.
 */

namespace rdlib{

/**
 * @ingroup RdIniReaderLib
 *
 * @brief An class to read ini files.
 *
 */
template < class T1, class T2 >
class RdIniReader {
    public:
        RdIniReader() { }
        ~RdIniReader() { }
        void parseFile(const char* fileName, std::map< T1, T2 > data) {
            std::ifstream ifs(fileName);
            if(!ifs.is_open()) {
                std::cerr << "[error] Bad file open." << std::endl;
            } else {
                std::cout << "[success] Begin file open: " << fileName << std::endl;
            }
            std::string line;
            while (getline(ifs, line)) {
                //std::cout << "--------------------------" << std::endl;
                int separator = (int)line.find(' ', 0);
                if(separator == std::string::npos) continue;
                //std::cout <<  line << std::endl;
                std::stringstream stringstream1( line.substr(0, separator) );
                //std::cout << "["<<  stringstream1.str() << "]" << std::endl;
                int end = (int)line.find(' ', separator+1);
                std::stringstream stringstream2( line.substr(separator+1,end-separator-1) );
                //std::cout << "[" <<  stringstream2.str() << "]" << std::endl;
                std::cout << "[info] Configured [ " << stringstream1.str() << " <-> " << stringstream2.str() << " ]" << std::endl;
                T1 val1;
                stringstream1 >> val1;
                T2 val2;
                stringstream2 >> val2;
            }
            //std::cout << "--------------------------" << std::endl;
            ifs.close();
            std::cout << "[info] End file close: " << fileName << std::endl;
        }
};

} //rdlib

#endif  // __RD_INI_READER_HPP__

