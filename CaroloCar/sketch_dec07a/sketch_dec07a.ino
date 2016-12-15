#include <Smartcar.h>
#define pin_1 A0

GP2D120 sensor;


int value;
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);

sensor.attach(pin_1);
}

void loop() {
  // put your main code here, to run repeatedly:

value = sensor.getDistance();
float volts = (float)readVal * 0.0048828125;
Serial.println(volts);

}
