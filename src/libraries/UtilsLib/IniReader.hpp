// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_INI_READER_HPP__
#define __RD_INI_READER_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>

#include <ColorDebug.hpp>

namespace rd{

/**
 * @ingroup UtilsLib
 *
 * @brief A template-based class to read ini files.
 */
template < class T1, class T2 >
class IniReader {
    public:
        IniReader() { }
        ~IniReader() { }

        void parseFile(const char* fileName, std::vector< std::pair< T1, T2 > >& data) {
            std::ifstream ifs(fileName);
            if(!ifs.is_open()) {
                CD_ERROR("vector implementation could not open file: %s\n",fileName);
            } else {
                CD_SUCCESS("vector implementation opened file: %s\n",fileName);
            }
            std::string line;
            while (std::getline(ifs, line)) {
                //std::cout << "--------------------------" << std::endl;
                int separator = (int)line.find(' ', 0);
                if(separator == std::string::npos) continue;
                //std::cout <<  line << std::endl;
                std::stringstream stringstream1( line.substr(0, separator) );
                //std::cout << "["<<  stringstream1.str() << "]" << std::endl;
                int end = (int)line.find(' ', separator+1);
                std::stringstream stringstream2( line.substr(separator+1,end-separator-1) );
                //std::cout << "[" <<  stringstream2.str() << "]" << std::endl;
                //std::cout << "[info] Configured [ " << stringstream1.str() << " <-> " << stringstream2.str() << " ]" << std::endl;
                T1 val1;
                stringstream1 >> val1;
                T2 val2;
                stringstream2 >> val2;
                data.push_back ( std::pair< T1, T2 >(val1, val2) );
            }
            //std::cout << "--------------------------" << std::endl;
            ifs.close();
            CD_INFO("vector implementation past close file: %s\n",fileName);
        }

        void parseFile(const char* fileName, std::map< T1, T2 >& data) {
            std::ifstream ifs(fileName);
            if(!ifs.is_open()) {
                CD_ERROR("map implementation could not open file: %s\n",fileName);
            } else {
                CD_SUCCESS("map implementation opened file: %s\n",fileName);
            }
            std::string line;
            while (std::getline(ifs, line)) {
                //std::cout << "--------------------------" << std::endl;
                int separator = (int)line.find(' ', 0);
                if(separator == std::string::npos) continue;
                //std::cout <<  line << std::endl;
                std::stringstream stringstream1( line.substr(0, separator) );
                //std::cout << "["<<  stringstream1.str() << "]" << std::endl;
                int end = (int)line.find(' ', separator+1);
                std::stringstream stringstream2( line.substr(separator+1,end-separator-1) );
                //std::cout << "[" <<  stringstream2.str() << "]" << std::endl;
                //std::cout << "[info] Configured [ " << stringstream1.str() << " <-> " << stringstream2.str() << " ]" << std::endl;
                T1 val1;
                stringstream1 >> val1;
                T2 val2;
                stringstream2 >> val2;
                data.insert ( std::pair< T1, T2 >(val1, val2) );
            }
            //std::cout << "--------------------------" << std::endl;
            ifs.close();
            CD_INFO("map implementation past close file: %s\n",fileName);
        }
};

} //rd

#endif  // __RD_INI_READER_HPP__
