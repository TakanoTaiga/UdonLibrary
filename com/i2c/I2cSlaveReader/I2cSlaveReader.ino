#include "I2cSlaveReader.hpp"

I2cSlaveReader<4> receive(5);

void setup() {
	Serial.begin(115200);
}

void loop() {
	if (receive.update()) {
		Serial.println(receive.getByteData(0));
	}
	else {
		Serial.println("通信切断");
	}
	delay(10);
}
