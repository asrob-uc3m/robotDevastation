#include "RdRobotLaserTowerOfDeath.hpp"



rdlib::RdRobotLaserTowerOfDeath::RdRobotLaserTowerOfDeath()
{
    //-- Put function names in the map:
    functionMap["panIncrement"] = (void *) &panIncrementWrapper;
    functionMap["panDecrement"] = (void *) &panDecrementWrapper;
    functionMap["tiltIncrement"] = (void *) &tiltIncrementWrapper;
    functionMap["tiltDecrement"] = (void *) &tiltDecrementWrapper;

    //-- Serial port
    serialPort = new SerialPort( "/dev/ttyUSB0" );
    initSerialPort();

    //-- Initial values:
    panJointValue = panInitial;
    tiltJointValue = tiltInitial;


}

rdlib::RdRobotLaserTowerOfDeath::~RdRobotLaserTowerOfDeath()
{
    serialPort->Close();
    delete serialPort;
}

bool rdlib::RdRobotLaserTowerOfDeath::panIncrement()
{
    if (panJointValue < panRangeMax)
        panJointValue++;

    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::panDecrement()
{
    if (  panJointValue > panRangeMin )
        panJointValue--;

    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::tiltIncrement()
{
    if (tiltJointValue < tiltRangeMax)
        tiltJointValue++;

    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::tiltDecrement()
{
    if (  tiltJointValue > tiltRangeMin )
        tiltJointValue--;

    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::shoot()
{
    SerialPort::DataBuffer outputBuff;
    outputBuff.push_back(0x5F); //-- 0x5F -> Toggle LED
    serialPort->Write( outputBuff );
}

bool rdlib::RdRobotLaserTowerOfDeath::reset()
{
    //-- Initial values:
    panJointValue = panInitial;
    tiltJointValue = tiltInitial;

    //-- Reset serial connecton
    serialPort->Close();
    initSerialPort();
}

bool rdlib::RdRobotLaserTowerOfDeath::panIncrementWrapper(void *This)
{
    return (( rdlib::RdRobotLaserTowerOfDeath * ) This)->panIncrement();
}

bool rdlib::RdRobotLaserTowerOfDeath::panDecrementWrapper(void *This)
{
    return (( rdlib::RdRobotLaserTowerOfDeath * ) This)->panDecrement();
}

bool rdlib::RdRobotLaserTowerOfDeath::tiltIncrementWrapper(void *This)
{
    return (( rdlib::RdRobotLaserTowerOfDeath * ) This)->tiltIncrement();
}

bool rdlib::RdRobotLaserTowerOfDeath::tiltDecrementWrapper(void *This)
{
    return (( rdlib::RdRobotLaserTowerOfDeath * ) This)->tiltDecrement();
}

bool rdlib::RdRobotLaserTowerOfDeath::initSerialPort()
{
    try
    {
        serialPort->Open( SerialPort::BAUD_57600, SerialPort::CHAR_SIZE_8,
                           SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1,
                           SerialPort::FLOW_CONTROL_NONE );
    }
    catch ( SerialPort::OpenFailed e )
    {
        std::cerr << "Error opening the serial port \"" << "/dev/ttyUSB0" << "\"" << std::endl;
        return false;
    }

    if ( ! checkConnection() )
    {
        std::cerr << "Error communicating with the robot. Exiting..." << std::endl;
        return false;
    }

    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::checkConnection()
{
    //-- Read welcome message to check if connected to the robot
    SerialPort::DataBuffer buffer;
    try {
        serialPort->Read( buffer, 13, 1500);
    }
    catch ( SerialPort::ReadTimeout e)
    {
        std::cout << "Timeout! Exiting..." << std::endl;
        return false;
    }

    //-- Check if connected
    std::string welcomeMessage = "[Debug] Ok!\r\n";
    bool diffFlag = false;

    for (int i = 0; i < (int) buffer.size(); i++)
    {
        if ( welcomeMessage[i] != buffer[i] )
            diffFlag = true;
    }

    return !diffFlag;
}

bool rdlib::RdRobotLaserTowerOfDeath::sendCurrentJointValues()
{
    SerialPort::DataBuffer outputBuff;
    outputBuff.push_back(0x50); //-- 0x50 -> Set pos to all joints

    outputBuff.push_back( (char) panJointValue );
    outputBuff.push_back( (char) tiltJointValue );

    serialPort->Write( outputBuff );
}


