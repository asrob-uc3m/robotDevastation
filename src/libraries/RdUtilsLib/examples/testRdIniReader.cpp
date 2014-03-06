// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "../RdIniReader.hpp"

int main(int argc, char *argv[])
{
    rdlib::RdIniReader< std::string, std::string > rdIniReader;

    //-- map implementation example
    std::vector< std::pair< std::string, std::string > > myvector;
    rdIniReader.parseFile("../testRdIniReader.ini", myvector);
    std::cout << "[info] testRdIniReader: myvector contains:" << std::endl;
    for (size_t it = 0; it < myvector.size(); ++it)
        std::cout << "[info] testRdIniReader: " << myvector[it].first << " => " << myvector[it].second << std::endl;
    std::cout << "[info] testRdIniReader: end myvector contains."  << std::endl;

    //-- map implementation example
    std::map< std::string, std::string > mymap;
    rdIniReader.parseFile("../testRdIniReader.ini", mymap);
    std::cout << "[info] testRdIniReader: mymap contains:" << std::endl;
    for (std::map< std::string, std::string >::iterator it = mymap.begin(); it != mymap.end(); ++it)
        std::cout << "[info] testRdIniReader: " << it->first << " => " << it->second << std::endl;
    std::cout << "[info] testRdIniReader: end mymap contains."  << std::endl;
    return 0;
}

