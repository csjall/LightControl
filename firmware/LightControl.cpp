#include "LightControl.h"

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

void RegisterMap::receiveEvent(int16_t numberOfBytes)
{
	readHeader();
}

void RegisterMap::requestEvent()
{
	Wire.write(reinterpret_cast<uint8_t*>(&buffer[offset]), size);
}


//----------------------------------------------------------------------------


CircularBuffer::CircularBuffer() :
	runningTotal(0),
	lastIndex(0)
{
	memset(values, 0, sizeof(values));
}

void CircularBuffer::setup(uint16_t value)
{
	// Fill the circular buffer for averaging.
	for (uint8_t index = 0; index != TotalValues; ++index)
	{
		addValue(value);
	}
}

void CircularBuffer::addValue(uint16_t value)
{
	// Add the latest value to the end of the circular buffer.
	// Update running total and index.
	runningTotal -= values[lastIndex];
	values[lastIndex] = value;
	runningTotal += values[lastIndex];
	if (++lastIndex == TotalValues)
	{
		lastIndex = 0;
	}
}

uint16_t CircularBuffer::getAverage() const
{
	return runningTotal / TotalValues;
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
