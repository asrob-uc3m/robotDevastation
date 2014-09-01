// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "NanoBot.hpp"

// -----------------------------------------------------------------------------

bool NanoBot::open(Searchable& config) {
    char serialport[13] = "/dev/ttyUSB1";
    int baudrate = B9600;  // default
    char buf[256];
    int rc,n;

    fd = serialport_init(serialport, baudrate);
    if(!fd) {
        printf("NULL fd, bye!\n");
        ::exit(-1);
    }
    printf("[success] Nanobot open(), fd: %d\n",fd);

    return true;
}

// -----------------------------------------------------------------------------

bool NanoBot::getAxes(int *axes) {
    *axes = DEFAULT_NUM_MOTORS;
    return true;
}

// -----------------------------------------------------------------------------

bool NanoBot::setVelocityMode() {
    return false;
}

// -----------------------------------------------------------------------------

bool NanoBot::velocityMove(int j, double sp) {
    printf("[NanoBot] velocityMove(%d, %f)\n",j,sp);
    if (j>DEFAULT_NUM_MOTORS) return false;
    unsigned char cmdByte = (unsigned int)sp/46;  // 3000/64 = 46; usamos 6 bits -> 64
    //printf("cmdByte (pre-dir): 0x%x\n",cmdByte);
    unsigned char direccion = j;
    direccion <<= 6;  // shift 6 positions
    cmdByte |= direccion;
    //printf("cmdByte (post-dir): 0x%x\n",cmdByte);
    int res = serialport_writebyte(fd, cmdByte);
    if(res==-1) return false;
    return true;
}

// -----------------------------------------------------------------------------

bool NanoBot::velocityMove(const double *sp) {
    bool ok = true;
    for(int i=0;i<DEFAULT_NUM_MOTORS;i++)
        ok &= velocityMove(i,sp[i]);
    return ok;
}

// -----------------------------------------------------------------------------

bool NanoBot::setRefAcceleration(int j, double acc) {
    return false;
}

// -----------------------------------------------------------------------------

bool NanoBot::setRefAccelerations(const double *accs) {
    return false;
}

// -----------------------------------------------------------------------------

bool NanoBot::getRefAcceleration(int j, double *acc) {
    return false;
}

// -----------------------------------------------------------------------------

bool NanoBot::getRefAccelerations(double *accs) {
    return false;
}

// -----------------------------------------------------------------------------

bool NanoBot::stop(int j) {
    return false;
}

// -----------------------------------------------------------------------------

bool NanoBot::stop() {
    return false;
}

