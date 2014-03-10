// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "../RdMacros.hpp"

int main(int argc, char *argv[])
{
    int ten = 10;
    RD_INFO("Ten is \"%d\"\n",ten);
    RD_SUCCESS("Open.\n");
    RD_WARNING("Dangerous!\n");
    RD_ERROR("Failed!\n");
    return 0;
}

