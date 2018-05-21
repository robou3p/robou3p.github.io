#include <Robot.h>
#include <Test.h>

void setup() {
  while (!Serial); //nic se ne zgodi dokler serijski monitor ni odprt
  Serial.println("Zapuscas setup!");
}

void loop() {

  if(robot.buttonPressed()){ //ob pritisku gumba se zacnejo izvajati testi po vrsti
    delay(1000); //zakasnitev za debouncing

    Serial.println("--- ZAČETEK TESTA LEVEGA MOTORJA ---");
    Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    test.motor(LEFT);
    Serial.println("--- KONEC TESTA LEVEGA MOTORJA ---");
    Serial.println(); //izpis prazne vrstice na serijski monitor za lepši pregled
    delay(2000); //zakasnitev za zagotavljanje stabilnosti delovanja

    Serial.println("--- ZAČETEK TESTA DESNEGA MOTORJA ---");
    Serial.println();
    test.motor(RIGHT);
    Serial.println("--- KONEC TESTA DESNEGA MOTORJA ---");
    Serial.println();
    delay(2000);

    Serial.println("--- ZAČETEK TESTA SENZORJEV ZA RAZDALJO ---");
    Serial.println();
    test.distance();
    Serial.println("--- KONEC TESTA SENZORJEV ZA RAZDALJO ---");
    Serial.println();
    delay(2000);

    Serial.println("--- ZAČETEK TESTA SENZORJEV ZA SVETLOBO ---");
    Serial.println();
    test.line();
    Serial.println("--- KONEC TESTA SENZORJEV ZA SVETLOBO ---");
    Serial.println();
    delay(2000);

    Serial.println("--- ZAČETEK TESTA BRENČAČA ---");
    Serial.println();
    test.buzzer();
    Serial.println("--- KONEC TESTA BRENČAČA ---");
    Serial.println();
    delay(2000);

    Serial.println("--- ZAČETEK TESTA BATERIJE ---");
    Serial.println();
    test.battery();
    Serial.println("--- KONEC TESTA BATERIJE ---");
    Serial.println();
    delay(2000);
  }

  delay(10); //zakasnitev za zagotavljanje stabilnosti delovanja
}
