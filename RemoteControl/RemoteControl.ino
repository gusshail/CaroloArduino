   // --------- //
// Libraries //

// --------- //
#include <Servo.h> 

// --------- //
// Constants //
// --------- //
const int servoPin = 9; // pin to which the servo motor is attached
const int escPin = 8; // pin to which the ESC is attached
const int rcPinSteer = A8; // rc steering
const int rcPinESC = A9;  // rc motor

// ----------------------- //
// Instatiation of objects //
// ----------------------- //
Servo motor, steering;
unsigned long rcControllerFlag, distance1, distance2;
String input;
int steer, velocity, controlFlag;
int QualityCheck = 0;
int pulse;

void setup() {
  //SERIAL CONNECTION
  Serial.begin(9600);

  motor.attach(escPin);
  motor.writeMicroseconds(1500);  // set servo to mid-point
  steering.attach(servoPin);
  steering.write(90);  // set servo to mid-point
//  attachInterrupt(digitalPinToInterrupt(18), wheelDistance1, HIGH);        //If we need to do stuff with wheel encoder, pls add pins in definition stage
 // attachInterrupt(digitalPinToInterrupt(19), wheelDistance2, HIGH);
  attachInterrupt(digitalPinToInterrupt(A8), rcControllerInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(A9), rcControllerInterrupt, RISING);
  pinMode(10, INPUT); //button 1
  pinMode(11, INPUT); //button 2
  pinMode(12, INPUT); //button 3
  rcControllerFlag = 0;
  controlFlag = 1;
  distance1 = 0;
  distance2 = 0;
}

void loop() {
  rcControllerFlag = pulseIn(rcPinSteer, HIGH, 25000); // if the timeout is lower it sometimes time out before getting a value
  Serial.println("Pulse read: ");
 Serial.print(rcControllerFlag);
// 
Serial.print("in LOOP");

  
  if(rcControllerFlag > 1000) {
    Serial.print("Going to RC CONTROLER");
    rcControl();
    //controlFlag = 0;
  
  if (pulseIn(rcPinSteer, LOW, 25000) == 0){

      rcControllerFlag = 0;
      Serial.print("RC control set to off!");
    }
  }else if(controlFlag == 0){
              Serial.print("control flag is 0");
  
    motor.writeMicroseconds(1500);
    steering.write(90);
    controlFlag = 1;
  }else{
    manualControl();
              Serial.print("we are in manual control");
    
    //handleInput();
  }
  buttonRead();
  //Serial.println("Odometer counter: ");
  //Serial.println(distance1);
  //Serial.println(distance2);
}

void rcControl(){
  //delay(200); //take away if you dont want to read
  Serial.println("RC Control took over!");
  pulse = pulseIn(rcPinESC, HIGH, 25000);
  Serial.print("Pulse read in throttle: ");
 // Serial.println(pulse);
  
 //velocity = 1500;

  
  if(pulse > 1560) {
  velocity = 1560; 
  }

   else if(pulse < 1300) {
    velocity = 1250;
  }

  else if(pulse > 1450 && pulse < 1560) {
    velocity = 1500; 
  }
 
  int i;
  int bond; 
  int steerVals[10] = {90};
  for(i = 0; i < 3; i++){
   //bond = constrain(rcPinSteer, 1290, 1750);
   // steerVals[i] = map(pulseIn(rcPinSteer, HIGH, 25000), 1000, 2000, 45 , 130);
  steerVals[i] = map(constrain(pulseIn(rcPinSteer, HIGH, 25000), 1200, 1800), 1200, 1800, 65, 115);
  }
  steer = median(steerVals, 3);   // +7
  if(steer > 85 && steer < 94) {
    steer = 90;
  }
  //velocity = map(velocity, 1000, 2000, 0, 150); 
  //Serial.print("steer ");
 // Serial.println(steer);
  //Serial.print("velocity ");
  //Serial.println(velocity);
  steering.write(steer);
  motor.writeMicroseconds(velocity);   //1650 - vel
  int temp = pulseIn(rcPinSteer, LOW, 25000);
  //Serial.println(temp);
  if (pulseIn(rcPinSteer, LOW, 25000) == 0){
    rcControllerFlag = 0;
    Serial.print("RC control set to off!");
  }
}

void buttonRead() {

  int start = 1560;
  int plsStop = 1500;
  int buttonPulse = pulseIn(rcPinESC, HIGH, 25000);
  //Serial.println(buttonPulse);
  

  if(buttonPulse < 1400) {
        motor.writeMicroseconds(plsStop);
      }


 if (digitalRead(10) && buttonPulse > 1400) {
      Serial.print("Button 1 on");
      motor.writeMicroseconds(start);
}


if (digitalRead(11) && buttonPulse > 1400) {
      motor.writeMicroseconds(start);
       // steering.write(70);
      
      Serial.print("Button 2 on");
    }
if (digitalRead(12) && buttonPulse > 1400) {
      motor.writeMicroseconds(start);
      //  steering.write(110);
      Serial.print("Button 3 on");
    }


  
}

void manualControl(){
  if (Serial.available()){

    Serial.print("Hi!");
    input = Serial.readStringUntil('\n');
    
    if (input.startsWith("t")){  // turning
      steer = input.substring(1).toInt();
      if (steer <= 180 && steer >=0){  // check that the value is in range
        steering.write(steer);
        Serial.println("Turning set to: ");
        Serial.println(steer);
      }
    }else if (input.startsWith("v")){  // velocity
      velocity = input.substring(1).toInt();
      if (velocity <= 2000 && velocity >= 1000){  // check that the value is in range
        motor.writeMicroseconds(velocity);
        Serial.print("Velocity set to: ");
        Serial.println(velocity);
      }
    }
  }
}

void handleInput() { //handle serial input if there is any
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');
    if (input.startsWith("w")) {
      motor.writeMicroseconds(1620);
      Serial.println("Going forward");
    }else if (input.startsWith("s")){
      motor.writeMicroseconds(1200);
      Serial.println("Going Backward");
    }else if (input.startsWith("q")){
      motor.writeMicroseconds(1500);
      Serial.println("Neutral");
    }else if (input.startsWith("a")){
      steering.write(60);
      Serial.println("Turning Left");
    }else if (input.startsWith("d")){
      steering.write(120);
      Serial.println("Turning Right");
    }else if (input.startsWith("x")){
      steering.write(90);
      Serial.println("Straight Forward");
    }
  }
}

void calibrateESC() {
  if (Serial.available()) {
    input = Serial.readStringUntil('\n');
    if (input.startsWith("u")){
        motor.writeMicroseconds(2000);
        Serial.println("Full forward");
      }else if (input.startsWith("i")){
        motor.writeMicroseconds(1000);
        Serial.println("Full back");
      }else if (input.startsWith("n")){
        motor.writeMicroseconds(1500);
        Serial.println("Neutral");
      }
  }
}

int median(int vals[], int len) {
  int minimum = vals[0];
  int maximum = vals[0];
  int sum = 0;
  int median = 90;
  for (int i = 0; i < len; i ++) {
    if (vals[i] < minimum) {
      minimum = vals[i];
    } else if (vals[i] > maximum) {
      maximum = vals[i];
    }
    sum += vals[i];
  }
  sum = sum - (minimum + maximum);
  median = floor(sum / (len-2));
  return median;
}

void wheelDistance1(){
  distance1++;
}
void wheelDistance2(){
  distance2++;
}
void rcControllerInterrupt(){
  rcControllerFlag = 1;
}
