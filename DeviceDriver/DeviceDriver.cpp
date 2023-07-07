#include <stdexcept>
#include <windows.h>
#include "DeviceDriver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int read_value = (int)(m_hardware->read(address));
    for(int i = 0; i < READ_CNT - 1; i++)
    {
        Sleep(DELAY_TIME);
        if (read_value != (int)(m_hardware->read(address)))
            throw ReadFailException();
    }
    return read_value;
}

void DeviceDriver::write(long address, int data)
{
    int read_value = (int)(m_hardware->read(address));
    if (read_value == EMPTY_VAL)
		m_hardware->write(address, (unsigned char)data);
    else
        throw WriteFailException();
}