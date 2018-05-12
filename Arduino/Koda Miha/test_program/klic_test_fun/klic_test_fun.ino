#include <Robot.h>
#include <Test.h>

void setup() {
  while (!Serial);
  Serial.println("Zapuscas setup!");
}

void loop() {
	
  if(robot.buttonPressed()){
		Serial.println("--- ZAČETEK TESTA BATERIJE ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.battery();
		Serial.println("--- KONEC TESTA BATERIJE ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
		delay(1000); //zakasnitev za zagotavljanje stabilnosti delovanja
		
		Serial.println("--- ZAČETEK TESTA LEVEGA MOTORJA ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.motor(LEFT);
		Serial.println("--- KONEC TESTA LEVEGA MOTORJA ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
		delay(1000); //zakasnitev za zagotavljanje stabilnosti delovanja
		
		Serial.println("--- ZAČETEK TESTA DESNEGA MOTORJA ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.motor(RIGHT);
		Serial.println("--- KONEC TESTA DESNEGA MOTORJA ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
		delay(1000); //zakasnitev za zagotavljanje stabilnosti delovanja
		
		Serial.println("--- ZAČETEK TESTA BRENČAČA ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.buzzer();
		Serial.println("--- KONEC TESTA BRENČAČA ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
		delay(1000); //zakasnitev za zagotavljanje stabilnosti delovanja
		
		Serial.println("--- ZAČETEK TESTA SENZORJEV ZA RAZDALJO ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.distance();
		Serial.println("--- KONEC TESTA SENZORJEV ZA RAZDALJO ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
		delay(1000); //zakasnitev za zagotavljanje stabilnosti delovanja
		
		Serial.println("--- ZAČETEK TESTA SENZORJEV ZA SVETLOBO ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.line();
		Serial.println("--- KONEC TESTA SENZORJEV ZA SVETLOBO ---");
		Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
		delay(1000); //zakasnitev za zagotavljanje stabilnosti delovanja
		
    //To spodaj ne deluje na 2. verziji robota
		/*
    test.temp();
    test.mag();
    test.accel();
    test.gyro();
		*/
  }

  delay(100); //zakasnitev za zagotavljanje stabilnosti delovanja
}
