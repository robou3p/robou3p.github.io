#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int StartPin = 7;
const int ResetPin = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint32_t time1, seconds, decimals, Normalize;
bool Start = 0, Reset=0, check = 0, check2 = 0;
const int LaserPin = 6;

void setup() {
  Serial.begin(9600);
  pinMode(StartPin,INPUT);
  pinMode(ResetPin, INPUT_PULLUP);
  pinMode(LaserPin, OUTPUT);
  digitalWrite(LaserPin, HIGH);
  lcd.begin(16, 2);
  lcd.setCursor(6,0);
  lcd.print("Cas:");
  
}

void loop() {
  Start = digitalRead(StartPin);
  Reset = digitalRead(ResetPin);
  Serial.println(Start);

  
  if(Start == LOW && check == 1 && seconds > 5)
  {
    check = 0;
    delay(500);
    }
    
  else if(Start == LOW && check == 0)
  {
    check = 1;
    Normalize = millis();
    delay(175);
    }

  if(Reset == LOW && check == 0)
  {
    lcd.clear();
    delay(500);
    lcd.setCursor(6,0);
    lcd.print("Cas:");
    }
  
  if(check == 1)
  {
    lcd.setCursor(6, 1);
    time1 = (millis() - Normalize);
    seconds = time1/1000;
    decimals = time1 - seconds*1000;
    lcd.print(seconds);
    lcd.print(".");
    lcd.print(decimals);
  }
  
}
