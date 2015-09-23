#include "LightControl.h"
#include "Application.h"

//----------------------------------------------------------------------------

RegisterMap::RegisterMap() :
	offset(0),
	size(1)
{
	memset(buffer, 0, sizeof(buffer));
}

void RegisterMap::readRegister(uint8_t offset, uint8_t size)
{
	uint8_t address = getAddress();
	Wire.beginTransmission(address);
	writeHeader(offset, size);
	Wire.endTransmission();
	Wire.requestFrom(address, size);
	while (Wire.available() && size != 0)
	{
		buffer[offset++] = Wire.read();
		size--;
	}
}

void RegisterMap::writeRegister(uint8_t offset, uint8_t size)
{
	uint8_t address = getAddress();
	Wire.beginTransmission(address);
	writeHeader(offset, size);
	if (size != 0)
	{
		Wire.write(buffer + offset, size);
	}
	Wire.endTransmission();
}

void RegisterMap::readHeader()
{
	if (2 <= Wire.available())
	{
		size = Wire.read();
		offset = Wire.read();
	}
}

void RegisterMap::writeHeader(uint8_t offset, uint8_t size)
{
	uint16_t header = offset << 8 | size;
	Wire.write(reinterpret_cast<uint8_t*>(&header), sizeof(header));
}

void RegisterMap::receiveEvent(int numberOfBytes)
{
	readHeader();
}

void RegisterMap::requestEvent()
{
	Wire.write(reinterpret_cast<uint8_t*>(&buffer[offset]), size);
}

//----------------------------------------------------------------------------

Scanner::Scanner(bool _reserved)
{
	reserved = _reserved;
	reset();
}

void Scanner::reset()
{
	address = 0x1;
}

uint8_t Scanner::next()
{
	while (address != 0x7F)
	{
		uint8_t scanAddress = address++;
		if (Scanner::isReserved(scanAddress) == reserved)
		{
			return scanAddress;
		}
	}
	
	return address;
}

bool Scanner::isReserved(uint8_t address)
{
	// TODO: Figure out why status is always 0.
	Wire.beginTransmission(address);
	int status = Wire.endTransmission();
	return status == 0;
}

//----------------------------------------------------------------------------
