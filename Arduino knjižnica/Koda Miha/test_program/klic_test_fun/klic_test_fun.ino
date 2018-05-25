#include <Test.h> //pomembno je, da vključimo testno knjižnico

void setup() {
  while (!Serial); //nič se ne zgodi, dokler serijski monitor ni odprt
}

void loop() {

  if(robot.buttonPressed()){ //ob pritisku funkcijskega gumba na robotu, se začnejo izvajati testi po vrsti
    delay(1000); //zakasnitev za debouncing

    Serial.println("TEST LEVEGA MOTORJA");
    Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.motor(LEFT);
    Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    delay(2000); //zakasnitev za zagotavljanje stabilnosti delovanja

    Serial.println("TEST DESNEGA MOTORJA");
    Serial.println();
    test.motor(RIGHT);
    Serial.println();
    delay(2000);

    Serial.println("TEST SENZORJEV ZA RAZDALJO");
    Serial.println();
    test.distance();
    Serial.println();
    delay(2000);

    Serial.println("TEST SENZORJEV ZA SVETLOBO");
    Serial.println();
    test.line();
    Serial.println();
    delay(2000);

    Serial.println("TEST BRENČAČA");
    Serial.println();
    test.buzzer();
    Serial.println();
    delay(2000);

    Serial.println("TEST BATERIJE");
    Serial.println();
    test.battery();
    Serial.println();
    delay(2000);

  }
  delay(10); //zakasnitev za zagotavljanje stabilnosti delovanja
}
