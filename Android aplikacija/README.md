# Upravljanje robota z Android telefonom

Z aplikacijo *LakosBot* za Android je mogoče upravljati robota s pomočjo joysticka ali senzorjev (pospeškomera) telefona.

<a href='https://play.google.com/store/apps/details?id=org.lakos.lakosbot&pcampaignid=MKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='na voljo v Google Play' src='https://play.google.com/intl/en_us/badges/images/generic/sl_badge_web_generic.png' height='60px'/></a>

## Namestitev aplikacije na telefon

Aplikacijo lahko [prenesete z Google Play-a](https://play.google.com/store/apps/details?id=org.lakos.lakosbot) ali [prenesete neposredno s tega repozitorija](https://github.com/robou3p/robou3p.github.io/raw/android/Android/lakosbot-2-v1.0.1.apk).

V primeru prenosa s tega repozitorija je aplikacijo potrebno namestiti ročno tako, da na telefonu med prenosi (Downloads) poiščemo apk, ga s klikom nanj odpremo in namestimo:

![Slika 1](http://shrani.si/f/3R/Pf/1DrsdcVJ/install1.png)
![Slika 2](http://shrani.si/f/m/8d/4UtZfmmJ/install2.png)

## Namestitev kode na Arduino robota

Za povezovanje mobilne aplikacije z robotom je potrebno na robota naložiti Arduino kodo za Bluetooth.
[Na tej povezavi](https://raw.githubusercontent.com/robou3p/robou3p.github.io/android/Android/android.ino) boste našli kodo, ki jo je potrebno naložiti. Na svoj računalnik jo lahko prenesete s pritiskom tipk **CTRL + S**.

Nato odprite Arduino IDE. Če še nimate nameščenega, si lahko navodila za pripravo okolja ogledate [tukaj](https://github.com/robou3p/robou3p.github.io/wiki/Za%C4%8Detni-koraki).
Na isti povezavi si oglejte tudi točko **#2 Nalaganje programske knjižnice**, saj je potrebno naložiti knjižnico **"Robot.h"**.

V Arduino IDE pojdite na **Datoteka/File** -> **Odpri.../Open...** in odprite datoteko **android.ino**, ki ste jo prenesli na računalnik. Če se datoteka ne nahaja na naslovu android/android.ino, vas bo program na to opozoril - preprosto izberite 'V redu' in nadaljujte.

![Slika 1](http://shrani.si/f/2c/x0/2EosWzZV/arduino1v2.png)

Nato z USB/microUSB kablom povežite računalnik z robotom in v Arduino IDE kliknite na puščico (možnost **Naloži/Upload**). Ko se koda uspešno naloži, dobite izpis **Nalaganje končano**:

![Slika 2](http://shrani.si/f/Z/EC/3yeM9hj9/arduino2.png)

Sedaj je robot pripravljen, da ga preko Bluetootha povežete z mobilno napravo.

## Povezovanje z robotom v aplikaciji

Najprej v meniju izberemo možnost **Poveži se z robotom**. Aplikacija ob tem zahteva pravico do uporabe Bluetootha.
Če uporabo Bluetootha dovolimo, se nam pojavi seznam naprav, s katerimi se lahko povežemo. Vsak robot ima oznako na spodnji strani čipa - v primeru robota, ki smo ga uporabili za sliko je to oznaka VF, zato na seznamu **izberemo ustrezno ime naprave** in možnost **Poveži**.

![Slika 1](http://shrani.si/f/3E/yl/4WEkxMBY/1.png)
![Slika 2](http://shrani.si/f/C/nv/OEiH2ux/2.png)
![Slika 3](http://shrani.si/f/2q/ly/1WyThCIl/3.png)

Potek povezovanja lahko spremljamo v zgornjem levem kotu aplikacije, kjer piše '**Povezovanje...**'. Ko se povezava vzpostavi, dobimo Zahtevo za seznanitev Bluetooth, kjer izberemo možnost **Seznani**. Ob uspešni povezavi bo sedaj v zgornjem levem kotu izpisano ime robota, s katerim smo trenutno povezani.

![Slika 4](http://shrani.si/f/41/OG/3LVXPvjT/4.png)
![Slika 5](http://shrani.si/f/3m/Av/4yP03jYf/5.png)
![Slika 6](http://shrani.si/f/1V/ST/4xUmi7Ce/6.png)

Robota sedaj lahko upravljamo s premikanjem rumenega krogca (joysticka). 

![Slika 7](http://shrani.si/f/2n/zC/4D5tgU2S/8.png)

Vkolikor bi radi robota upravljali z nagibanjem mobilne naprave, v zgornjem desnem meniju najprej opravite kalibracijo naprave (izberite možnost **Kalibriraj senzor telefona** v zgornjem desnem meniju). Ko je kalibracija zaključena, lahko preko gumba **Začni upravljanje z nagibom** preidete na drugi način premikanja robota.

Povezavo z robotom lahko kadarkoli prekinete v zgornjem desnem meniju.

![Slika 8](http://shrani.si/f/3E/B7/1Fz4tnPz/9.png)
