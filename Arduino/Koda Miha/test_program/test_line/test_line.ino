#include <Robot.h>

void setup() {
  while(!Serial);
  delay(100);
  Serial.println("Kalibracija traja 5s. Zacne in konca se s piskom.");
  Serial.println("Pritisni gumb za zacetek.");
  Serial.println();
  while(!robot.buttonPressed());
  delay(500);
  Serial.println("START kalibracije");
  robot.beep(200,500); //pisk za konec kalibracije
  robot.line.calibrate(5000);
  robot.beep(200,500); //pisk za konec kalibracije
  Serial.println("KONEC kalibracije");
  delay(500);
  Serial.println("Pritisni gumb za nadaljevanje testa.");
  while(!robot.buttonPressed());
  delay(1000); //debouncing
  Serial.println();
  Serial.println("Crto daj pod SREDINO robota.");
  while(!robot.buttonPressed()){
    if (abs(robot.line.getPosition() * 100) < 1.0){
      Serial.println("--- OK ---");
      robot.beep(500,300);
      break;
    }
  }
  delay(1000);
  Serial.println();
  Serial.println("Crto daj pod LEVO stran robota.");
  while(!robot.buttonPressed()){
    if ((robot.line.getPosition() * 100) < -2.5){
      Serial.println("--- OK ---");
      robot.beep(500,300);
      break;
    }
  }
  delay(1000);
  Serial.println();
  Serial.println("Crto daj pod DESNO stran robota.");
  while(!robot.buttonPressed()){
    if ((robot.line.getPosition() * 100) > 2.5){
      Serial.println("--- OK ---");
      robot.beep(500,300);
      break;
    }
  }
  delay(10);
  Serial.println();
  Serial.println("Test zakljucen!!!");
  delay(10);
}

void loop() {
  
}
