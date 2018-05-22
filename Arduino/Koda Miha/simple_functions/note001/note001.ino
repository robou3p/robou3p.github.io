void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
//DEKLERACIJA FUNKCIJE
void nota(char ton[], float trajanje){
  int16_t frequency, timeBuzzerON; //frekvenca in cas delovanja brencaca
  //priredi frekvenco glede na vpisano noto
  //kodo iz spodnjega linka bi lahko predelali v C kodo
  //https://codepen.io/enxaneta/post/frequencies-of-musical-notes
  
  //priredi cas delovanja brencaca glede na vpisano trajanje note
  switch (trajanje) {
  case 2:
    timeBuzzerON = ;
    break;
  case 1:
    timeBuzzerON = ;
    break;
  case 1/2:
    timeBuzzerON = ;
    break;
  case 1/4:
    timeBuzzerON = ;
    break;
  case 1/8:
    timeBuzzerON = ;
    break;
  case 1/16:
    timeBuzzerON = ;
    break;
  case 1/32:
    timeBuzzerON = ;
    break;
  default:
  Serial.println("NAPAKA! - Vpiši trajanje note, npr. 1 ali 1/2 ali 1/4...");
  }
  robot.beep(frequency, timeBuzzerON);
}
