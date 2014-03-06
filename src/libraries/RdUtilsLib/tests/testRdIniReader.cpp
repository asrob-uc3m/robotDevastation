// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "../RdIniReader.hpp"

int main(int argc, char *argv[])
{
    std::map< std::string, std::string > mymap;
    rdlib::RdIniReader< std::string, std::string > rdIniReader;
    rdIniReader.parseFile("../testRdIniReader.ini", mymap);
    return 0;
}

