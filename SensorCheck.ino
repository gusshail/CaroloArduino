
  #include <Smartcar.h>             // Import smartcar shield library with sketch->include library->manage library->search for smartcarshield
  
  
  #define IR_REAR_LEFT_PIN A1     //A1 - green wire
  #define IR_REAR_RIGHT_PIN A2    //A2 - blue wire
  #define IR_MIDDLE_REAR_PIN A0   //A0 - yellow wire 
  #define IR_MIDDLE_FRONT_PIN A3  //A3 - violet wire
//  #define IR_TEST 22  //A3               

  #define US_FRONT_ADDRESS 0x69                //Att! Sonars on I2C bus work badly -> fix will come someday...
  #define US_REAR_ADDRESS 0x73
  
  
  
  SRF08 frontSonar, rearSonar; 
  GP2D120 rearLeftIR, rearRightIR, middleRearIR, middleFrontIR;
  //GP2D120 testIR;
    
  void setup() {
    // put your setup code here, to run once:
  
     
   // testIR.attach(IR_TEST);
    rearLeftIR.attach(IR_REAR_LEFT_PIN);
    rearRightIR.attach(IR_REAR_RIGHT_PIN);
    middleRearIR.attach(IR_MIDDLE_REAR_PIN);
    middleFrontIR.attach(IR_MIDDLE_FRONT_PIN);
    frontSonar.attach(US_FRONT_ADDRESS);
    rearSonar.attach(US_REAR_ADDRESS);
     
    Serial.begin(9600);  // You can use higher bauds for testing IR
  
    //Serial.print("I am in a setup ");
  
  }
  void loop() {
    // put your main code here, to run repeatedly:
  
  //Serial.print("Rear left is ");
  // printSonar();                        //Uncomment to test sonars
   printfront();
  // printSecondSonar(); 
   
  }
  
  void printfront(){
     Serial.print("Rear left is ");       //works A1 - green
     int value = analogRead(IR_REAR_LEFT_PIN);
  //  float distance = value * (5.0 / 1023.0);
     int distance = rearLeftIR.getDistance(); //get distance detected by sonarFront
   //  Serial.println(value,DEC);
    Serial.println(distance);
     delay (300);
   
    Serial.print("Rear right is ");            //works - A2 - blue 
    int distance1 = rearRightIR.getDistance(); 
    Serial.println(distance1); 
    delay (300);
  
    Serial.print("Rear middle rear is ");   //works - A0 - yellow
    int distance2 = middleRearIR.getDistance(); 
    Serial.println(distance2); 
    delay (300);
  
   Serial.print("Front middle rear is ");  //works - A3 - violet 
    int distance3 = middleFrontIR.getDistance(); 
    Serial.println(distance3); 
  
   delay(300); 
  
      
    
  }
  
  void printSonar() {
  
  Serial.print("I am here");
  
    // int distance4 = rearSonar.getDistance(); //get distance detected by sonarFront
     //Serial.println(distance4);
     delay (600); 
  
    
     int maps[5];
     int tick; 
  
     for (int tick = 0; tick < 5; tick++) {
  
  
        int distance4 = rearSonar.getDistance();
         maps[tick] = distance4;
        // Serial.println(maps[tick]);
         delay(500);
     
        
     }
  
     if (tick = 5) {
  
       int y = map(maps[0],maps[1],maps[2],maps[3],maps[4]);
  
       Serial.println(y);
  
       tick == 0;
      
     }
  
      
      
  
    
  }
  
  void printSecondSonar() {
  
  Serial.print("Front is");
  int distance5 = rearSonar.getDistance();
  Serial.println(distance5);
  delay(600);
  
  }
