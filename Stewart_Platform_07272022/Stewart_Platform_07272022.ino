#include <NintendoExtensionCtrl.h>
#include <Servo.h>

Nunchuk nchuk;
const int servoPin[] = {3,5,6,9,10,11}, servoZero[] = {1500,1500,1500,1500,1500,1500};

Servo servo[6];

int joyY, joyX, MapX, MapY, MapYOdd, MapXOdd, Home = 1500, MapZ, MapZOdd, C, Z, MapC, MapCOdd;
float zsq, a, b, csq;

void setup() {
  Serial.begin(115200);
  nchuk.begin();

  for(int i = 0; i < 6; i++){                     // attach the servos 
    servo[i].attach(servoPin[i]);
    servo[i].writeMicroseconds(servoZero[i]);
  }
  
  delay(1000);

  while (!nchuk.connect()) {
    Serial.println("Nunchuk not detected!");
    delay(1000);
  }
}

void loop() {
  boolean success = nchuk.update();               // Get new data from the controller

  if (!success) {                                 // Detect controller disconnection
    Serial.println("Controller disconnected!");
    delay(1000);
  }
  
  else {
    // Read a joystick axis (0-255, X and Y)
    joyY = nchuk.joyY();
    joyX = nchuk.joyX();
    
    while ((joyX > 116 && joyX < 136) && (joyY > 124 && joyY < 131))
    {
      
      joyY = nchuk.joyY();                        // Read joystick values
      joyX = nchuk.joyX();

      Serial.println("Do nothing");
      success = nchuk.update(); 
      for(int i = 0; i < 6; i++){                 
        servo[i].writeMicroseconds(Home);
      }
    } 

    boolean zButton = nchuk.buttonZ();            // Read if Z is pressed
    boolean cButton = nchuk.buttonC();            // Read if C is pressed

    while (cButton == 1 && zButton == 1){         // raise platform as C is held
        joyY = nchuk.joyY();
        MapY = map(joyY, 0, 255, 500, 2500);      // Map the Y coordinates then adjust the servos
        MapYOdd = map(joyY, 0, 255, 2500, 500);
      
        servo[0].writeMicroseconds(MapYOdd);
        servo[1].writeMicroseconds(MapYOdd);
        servo[2].writeMicroseconds(MapY);
        servo[3].writeMicroseconds(MapY);
        servo[4].writeMicroseconds(MapYOdd);
        servo[5].writeMicroseconds(MapY);
      
        zButton = nchuk.buttonZ();
        cButton = nchuk.buttonC();
        success = nchuk.update();                 // Get new data from the controller
      }
      
      while (cButton == 1){                       // raise platform as C is held
        joyY = nchuk.joyY();
        MapY = map(joyY, 0, 255, 500, 2500);      // Map the Y coordinates then adjust the servos
        MapYOdd = map(joyY, 0, 255, 2500, 500);
      
        servo[0].writeMicroseconds(MapY);
        servo[1].writeMicroseconds(MapYOdd);
        servo[2].writeMicroseconds(MapYOdd);
        servo[3].writeMicroseconds(MapY);
        servo[4].writeMicroseconds(MapY);
        servo[5].writeMicroseconds(MapYOdd);

        cButton = nchuk.buttonC();
        success = nchuk.update();                 // Get new data from the controller
      }
      
    while (zButton == 1){                         // twist platform as Z is held
        joyX = nchuk.joyX();
        MapX = map(joyX, 0, 255, 500, 2500);      // Map the X coordinates then adjust the servos
        MapXOdd = map(joyX, 0, 255, 2500, 500);
      
        for(int i = 0; i < 6; i++){                
          servo[i].writeMicroseconds(MapX);
        }
      
        zButton = nchuk.buttonZ();
        success = nchuk.update();                 // Get new data from the controller   
      } 
  
    while ((joyX > 110 && joyX < 130))            // Forwards/Backwards tilt
      {
      MapY = map(joyY, 0, 255, 500, 2500); 
      MapYOdd = map(joyY, 0, 255, 2500, 500);
      
      servo[4].writeMicroseconds(MapYOdd);
      servo[5].writeMicroseconds(MapY);
      servo[2].writeMicroseconds(MapYOdd);
      servo[0].writeMicroseconds(MapY);
    
      success = nchuk.update();
      joyX = nchuk.joyX();
      joyY = nchuk.joyY();
      }
      
    while ((joyX > 130 && joyY >= 131) || (joyX < 116 && joyY <= 125))            // Right diagonal tilt
      {
        success = nchuk.update();
        joyX = nchuk.joyX();
        joyY = nchuk.joyY();
        a = joyX;
        b = joyY;
        zsq = (sq(a) + sq(b));
        Z = sqrt(zsq);                         // Calculate the distance from the center 
      
        MapZ = map(Z, 118, 270, 500, 2500); 
        MapZOdd = map(Z, 118, 270, 2500, 500);
      
        servo[3].writeMicroseconds(MapZOdd);
        servo[5].writeMicroseconds(MapZ);
        servo[1].writeMicroseconds(MapZOdd);
        servo[0].writeMicroseconds(MapZ);
      }

      while ((joyX < 110 && joyY > 125) || (joyX > 136 && joyY < 131))            // Left diagonal tilt
      {
        success = nchuk.update();
        joyX = nchuk.joyX();
        joyY = nchuk.joyY();
        a = joyX;
        b = joyX;
        csq = sq(a)+sq(b);                         // Calculate the distance from the center 
        C = sqrt(csq);

        MapC = map(C, 66, 270, 500, 2500); 
        MapCOdd = map(C, 66, 270, 2500, 500);
      
        servo[4].writeMicroseconds(MapC);
        servo[1].writeMicroseconds(MapCOdd);
        servo[2].writeMicroseconds(MapC);
        servo[3].writeMicroseconds(MapCOdd);
      }
  
}
}
