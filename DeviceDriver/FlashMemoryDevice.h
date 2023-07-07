#pragma once
#define interface struct

/*
 * This class represents the interface to a Flash Memory Device. The hardware has only two methods - 'read' and 'write'
 * However, the interface for using the device is a lot more complex than that. It is outlined in the top-level README file.
 */

interface FlashMemoryDevice
{
public:
    virtual unsigned char read(long address) = 0;
    virtual void write(long address, unsigned char data) = 0;
};