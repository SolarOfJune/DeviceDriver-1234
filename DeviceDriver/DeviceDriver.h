#pragma once
#include "FlashMemoryDevice.h"

#define WRITE_MIN_ADDRESS   (0x00)
#define WRITE_MAX_ADDRESS   (0x04)

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