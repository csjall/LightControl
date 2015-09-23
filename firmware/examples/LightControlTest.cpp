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
	Serial.begin(9600);
	Serial.println("LightControl Sensor Example");

	registerMap.setAddress(2);
	registerMap.readModel();
	registerMap.readVersion();
	registerMap.readSerialNumber();

	Serial.print("Model: ");
	Serial.println(registerMap.getModel());
	Serial.print("Version: ");
	Serial.println(registerMap.getVersion());
	Serial.print("Serial Number: ");
	Serial.println(registerMap.getSerialNumber());
}

void loop()
{
	registerMap.readTemperature();
	Serial.print("Temperature: ");
	Serial.println(registerMap.getTemperature(););

	delay(1000);
}
