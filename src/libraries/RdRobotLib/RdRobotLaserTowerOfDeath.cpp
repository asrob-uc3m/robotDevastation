#include "RdRobotLaserTowerOfDeath.hpp"


rdlib::RdRobotLaserTowerOfDeath::RdRobotLaserTowerOfDeath()
{
    RD_DEBUG("this: %p.\n",this);
    //-- Initial values:
    panJointValue = panInitial;
    tiltJointValue = tiltInitial;
}

rdlib::RdRobotLaserTowerOfDeath::~RdRobotLaserTowerOfDeath()
{
    serialPort->Close();
    delete serialPort;
    serialPort = 0;
}

bool rdlib::RdRobotLaserTowerOfDeath::callFunctionByName(const std::string& cmd)
{
    if (cmd == "camera_left")
        panIncrement();
    else if ( cmd == "camera_right")
        panDecrement();
    else if ( cmd == "camera_up")
        tiltIncrement();
    else if ( cmd == "camera_down")
        tiltDecrement();
    else if ( cmd == "shoot")
        shoot();
}


bool rdlib::RdRobotLaserTowerOfDeath::panIncrement()
{
    if (panJointValue < panRangeMax)
        panJointValue+=panStep;

    return sendCurrentJointValues();
}

bool rdlib::RdRobotLaserTowerOfDeath::panDecrement()
{
    if (  panJointValue > panRangeMin )
        panJointValue-=panStep;

    return sendCurrentJointValues();
}

bool rdlib::RdRobotLaserTowerOfDeath::tiltIncrement()
{
    if (tiltJointValue < tiltRangeMax)
        tiltJointValue+=tiltStep;

    return sendCurrentJointValues();
}

bool rdlib::RdRobotLaserTowerOfDeath::tiltDecrement()
{
    if (  tiltJointValue > tiltRangeMin )
        tiltJointValue-=tiltStep;

    return sendCurrentJointValues();
}

bool rdlib::RdRobotLaserTowerOfDeath::shoot()
{
    SerialPort::DataBuffer outputBuff;
    outputBuff.push_back(0x5F); //-- 0x5F -> Toggle LED
    serialPort->Write( outputBuff );

    RD_SUCCESS("Shooting!!\n");
}

bool rdlib::RdRobotLaserTowerOfDeath::askToStop()
{
    RD_INFO("Stopping connection...\n");

    //-- Close serial port
    if ( serialPort->IsOpen() )
        serialPort->Close();

    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::quit()
{
    //-- Initial values:
    panJointValue = panInitial;
    tiltJointValue = tiltInitial;

    if ( serialPort->IsOpen() )
        serialPort->Close();
}


bool rdlib::RdRobotLaserTowerOfDeath::connect()
{
    std::string serial_port = rdIniMap.at("serial_port");
    initSerialPort(serial_port.c_str());
    return true;
}

bool rdlib::RdRobotLaserTowerOfDeath::initSerialPort(const char* serialPortName)
{
    RD_DEBUG("init Serial Port.\n");
    serialPort = new SerialPort( serialPortName );  // "/dev/ttyUSB0"
    try
    {
        serialPort->Open( SerialPort::BAUD_57600, SerialPort::CHAR_SIZE_8,
                           SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1,
                           SerialPort::FLOW_CONTROL_NONE );
    }
    catch ( SerialPort::OpenFailed e )
    {
        RD_ERROR("Error opening the serial port \"%s\"\n", serialPortName);
        return false;
    }

    if ( ! checkConnection() )
    {
        RD_ERROR("Error communicating with the robot. Exiting...\n");
        return false;
    }
    RD_SUCCESS("Ok Serial Port: %p\n",serialPort);
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


