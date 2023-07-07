#include <stdexcept>
#include <windows.h>
#include <iostream>
#include "DeviceDriver.h"

using namespace std;

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
    if (read_value != EMPTY_VAL)
		throw WriteFailException();
	m_hardware->write(address, (unsigned char)data);
}

class Application
{
public:
	explicit Application(DeviceDriver* kernel)
		: kernel(kernel)
	{
	}

	void ReadAndPrint(int startAddr, int endAddr)
    {
        for (int addr = startAddr; addr < endAddr; addr++)
        {
            int read_value = kernel->read(addr);
            cout << read_value << ' ';
        }
    }
    void WriteAll(int value)
    {
	    for (int addr = WRITE_MIN_ADDRESS; addr < WRITE_MAX_ADDRESS; addr++)
	    {
            kernel->write(addr, value);
	    }
    }
private:
    DeviceDriver* kernel;
};