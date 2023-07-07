#pragma once
#include "FlashMemoryDevice.h"

using namespace std;

class ReadFailException: public exception
{
};
class WriteFailException : public exception
{
};

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;

private:
    const int READ_CNT = 5;
    const int DELAY_TIME = 200;
};