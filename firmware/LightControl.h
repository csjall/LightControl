#ifndef LightControl_h
#define LightControl_h

#include <Particle.h>

//----------------------------------------------------------------------------

#define IMPLEMENT_REGISTER(name, type, offset, mask) \
	type get##name() const \
	{ \
		type value; \
		memcpy(&value, &buffer[offset], sizeof(type)); \
		value &= mask; \
		return value; \
	} \
	void set##name(type value) \
	{ \
		value &= mask; \
		memcpy(&buffer[offset], &value, sizeof(type)); \
	} \
	void read##name() \
	{ \
		readRegister(offset, sizeof(type)); \
	} \
	void write##name() \
	{ \
		writeRegister(offset, sizeof(type)); \
	} \
	
// Container for current register map.
class RegisterMap
{
	public:
		RegisterMap();

		IMPLEMENT_REGISTER(Model, uint8_t, 0x00, 0xFF)
		IMPLEMENT_REGISTER(Version, uint8_t, 0x1, 0xFF)
		IMPLEMENT_REGISTER(SerialNumber, uint32_t, 0x2, 0xFFFFFFFF)
		IMPLEMENT_REGISTER(WavelengthCount, uint8_t, 0x6, 0xFF)
		IMPLEMENT_REGISTER(CalibratedDutyCycle1, uint16_t, 0x7, 0xFFFF)
		IMPLEMENT_REGISTER(CalibratedDutyCycle2, uint16_t, 0x9, 0xFFFF)
		IMPLEMENT_REGISTER(CalibratedDutyCycle3, uint16_t, 0xB, 0xFFFF)
		IMPLEMENT_REGISTER(CalibratedDutyCycle4, uint16_t, 0xD, 0xFFFF)
		IMPLEMENT_REGISTER(CalibrationTemperature, uint16_t, 0xF, 0xFFFF)
		IMPLEMENT_REGISTER(TemperatureCoefficient1, uint16_t, 0x11, 0xFFFF)
		IMPLEMENT_REGISTER(TemperatureCoefficient2, uint16_t, 0x13, 0xFFFF)
		IMPLEMENT_REGISTER(Address, uint8_t, 0x15, 0x7F)
		IMPLEMENT_REGISTER(CompensationInterval, uint16_t, 0x16, 0xFFFF)
		IMPLEMENT_REGISTER(LightOutputFactor1, uint16_t, 0x18, 0xFFFF)
		IMPLEMENT_REGISTER(LightOutputFactor2, uint16_t, 0x1A, 0xFFFF)
		IMPLEMENT_REGISTER(SensorTemperature, uint16_t, 0x1C, 0xFFFF)
		IMPLEMENT_REGISTER(CalculatedDutyCycle1, uint16_t, 0x1E, 0xFFFF)
		IMPLEMENT_REGISTER(CalculatedDutyCycle2, uint16_t, 0x20, 0xFFFF)
		IMPLEMENT_REGISTER(VoltageReading1, uint16_t, 0x22, 0xFFFF)
		IMPLEMENT_REGISTER(VoltageReading2, uint16_t, 0x24, 0xFFFF)
		IMPLEMENT_REGISTER(VoltageReading3, uint16_t, 0x26, 0xFFFF)
		IMPLEMENT_REGISTER(VoltageReading4, uint16_t, 0x28, 0xFFFF)
		IMPLEMENT_REGISTER(VoltageReading5, uint16_t, 0x2A, 0xFFFF)

		void receiveEvent(int16_t numberOfBytes);
		void requestEvent();

	private:
		void readRegister(uint8_t offset, uint8_t size);
		void writeRegister(uint8_t, uint8_t size);
		void readHeader();
		void writeHeader(uint8_t offset, uint8_t size);

		uint8_t buffer[0x30];
		uint8_t offset;
		uint8_t size;
};

//----------------------------------------------------------------------------

// Container for averaged circular buffer
class CircularBuffer
{
	public:
		static const uint8_t TotalValues = 4;

		CircularBuffer();

		void setup(uint16_t value);
		void addValue(uint16_t value);
		uint16_t getAverage() const;

	private:
		uint16_t values[TotalValues];
		uint16_t runningTotal;
		uint8_t lastIndex;
};

//----------------------------------------------------------------------------

class Scanner
{
	public:
		Scanner(bool reserved);
		
		void reset();
		uint8_t next();
		
	private:
		static bool isReserved(uint8_t address);
		
		bool reserved;
		uint8_t address;
};

#endif
