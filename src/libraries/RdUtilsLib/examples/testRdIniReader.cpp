// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "../RdIniReader.hpp"

int main(int argc, char *argv[])
{
    std::map< std::string, std::string > mymap;
    rdlib::RdIniReader< std::string, std::string > rdIniReader;
    rdIniReader.parseFile("../testRdIniReader.ini", mymap);
    std::cout << "[info] testRdIniReader: mymap contains:" << std::endl;
    for (std::map< std::string, std::string >::iterator it = mymap.begin(); it != mymap.end(); ++it)
        std::cout << "[info] testRdIniReader: " << it->first << " => " << it->second << std::endl;
    std::cout << "[info] testRdIniReader: end mymap contains."  << std::endl;
    return 0;
}

