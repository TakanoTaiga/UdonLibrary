#include "CanCommon.hpp"

CanBusTeensy<CAN1> bus;
CanWriter<Message::Motor> writer(bus, 0);

void setup() {
	bus.begin();
}

void loop() {
	writer.setMessage({ -123 });
	bus.update();
	delay(10);
}
