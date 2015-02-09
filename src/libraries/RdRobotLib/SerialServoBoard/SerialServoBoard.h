// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2008 Giacomo Spigler
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>

#include <yarp/dev/SerialInterfaces.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))
#define FABS(x) (x>=0?x:-x)

#define SSC32 1
#define MINISSC 2
#define PONTECHSV203X 3
#define MONDOTRONICSMI 4
#define POLOLUUSB16 6
#define PICOPIC 7


using namespace yarp::os;
using namespace yarp::dev;



bool movessc32(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);
bool moveminissc(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);
bool movepontech(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);
bool movemondotronic(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);
bool movepololu(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);
bool movepicopic(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);



class SerialServoBoard : public DeviceDriver, public IPositionControl {
public:
    int servoboard;

    ISerialDevice *serial;

    PolyDriver dd;

    //bool (*move)(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);

    double *positions;
    double *speeds;

    bool getAxes(int *ax);
    bool setPositionMode();
    bool positionMove(int j, double ref);
    bool positionMove(const double *refs);
    bool relativeMove(int j, double delta);
    bool relativeMove(const double *deltas);
    bool checkMotionDone(int j, bool *flag);
    bool checkMotionDone(bool *flag);
    bool setRefSpeed(int j, double sp);
    bool setRefSpeeds(const double *spds);
    bool setRefAcceleration(int j, double acc);
    bool setRefAccelerations(const double *accs);
    bool getRefSpeed(int j, double *ref);
    bool getRefSpeeds(double *spds);
    bool getRefAcceleration(int j, double *acc);
    bool getRefAccelerations(double *accs);
    bool stop(int j);
    bool stop();

    virtual bool open(Searchable& config);

    virtual bool close();

private:
    bool move(int j, double ref, double *positions, double *speeds, ISerialDevice *serial);
};



