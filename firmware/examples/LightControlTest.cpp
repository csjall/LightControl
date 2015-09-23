#include "LightControl/LightControl.h"

// Connection for LightControl module to Spark Care
// Connect sensor GND to Core GND
// Connect sensor VDD to 3.3V
// Connect sensor SCL to D0
// Connect sensor SDA to D1
// Connect a 10K resistor from SCL (clock) to 3.3V (power)
// Connect a 10K resistor from SDA (data) to 3.3V (power)

RegisterMap registerMap;

void setup()
{
	registerMap.setAddress(2);

    Wire.begin();
    
	Serial.begin(9600);
	Serial.println("LightControl Example");
}

void loop()
{
	registerMap.readModel();
	registerMap.readVersion();
	registerMap.readSerialNumber();
	registerMap.readWavelengthCount();

	Serial.print("Model: ");
	Serial.println(registerMap.getModel());
	Serial.print("Version: ");
	Serial.println(registerMap.getVersion());
	Serial.print("Serial Number: ");
	Serial.println(registerMap.getSerialNumber());
	Serial.print("Wavelenght Count: ");
	Serial.println(registerMap.getWavelengthCount());

	delay(1000);
}
