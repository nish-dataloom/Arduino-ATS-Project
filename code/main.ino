int i, j, k;
float sen, senv, dg, dvtg, kb, kvtg;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(2, OUTPUT);    // Buzzer
  pinMode(8, OUTPUT);    // Relay 1
  pinMode(11, OUTPUT);   // Relay 2
  pinMode(12, OUTPUT);   // Relay 3

  pinMode(kb, INPUT);    // KEB voltage analog input
  pinMode(dg, INPUT);    // DG voltage analog input

  if (dvtg > 2) {
    k = 1;
  }
}

void loop() {
  dg = analogRead(A1);
  kb = analogRead(A0);
  sen = analogRead(A2);

  senv = sen * (5.0 / 1023.0);
  dvtg = dg * (5.0 / 1023.0);
  kvtg = kb * (5.0 / 1023.0);

  Serial.print("kvtg=");
  Serial.println(kvtg);
  Serial.print("dvtg=");
  Serial.println(dvtg);
  Serial.print("sen=");
  Serial.println(senv);
  delay(1000);

  if (kvtg > 2) i = 230;
  if (kvtg < 2) i = 0;
  lcd.setCursor(0, 1);
  lcd.print("KEB:");
  lcd.print(i);
  lcd.print("v");

  if (dvtg > 2) j = 230;
  if (dvtg < 2) j = 0;
  lcd.setCursor(9, 1);
  lcd.print("DG:");
  lcd.print(j);
  lcd.print("v");

  if (kvtg > 2 && dvtg < 2) {
    lcd.setCursor(0, 0);
    lcd.print("MAINS ON");
  }

  if (kvtg < 2) {
    lcd.setCursor(0, 0);
    lcd.print("INITIALIZING ATS");
  }

  if (dvtg > 2 && kvtg < 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DG ON");
    lcd.setCursor(0, 1);
    lcd.print("KEB:");
    lcd.print(i);
    lcd.print("v");
    lcd.setCursor(9, 1);
    lcd.print("DG:");
    lcd.print(j);
    lcd.print("v");
  }

  if (kvtg > 2 && dvtg > 2) {
    lcd.setCursor(0, 0);
    lcd.print("MAINS DETECTED");
    delay(1500);
    lcd.setCursor(0, 0);
    lcd.print("SWITCHING MAINS");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("KEB:");
    lcd.print(i);
    lcd.print("v");
  }

  bon();
  initial();
  poff();
  dfail();
}

void initial() {
  if (kvtg > 2 && dvtg < 2) {
    digitalWrite(8, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
  }
}

void poff() {
  if (kvtg < 2 && senv > 2) {
    digitalWrite(8, HIGH);
    delay(1500);
    digitalWrite(11, LOW);
    delay(1500);
    digitalWrite(12, LOW);
  }
}

void bon() {
  if (kvtg > 2 && dvtg > 2) {
    digitalWrite(12, HIGH);
    delay(1500);
    digitalWrite(11, HIGH);
    delay(1500);
    digitalWrite(8, LOW);
  }
}

void dfail() {
  if (senv < 2 && kvtg < 2) {
    buzz();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DG FAILURE");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RESTARTING ATS");
    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("KEB:");
    lcd.print(i);
    lcd.print("v");
    lcd.setCursor(9, 1);
    lcd.print("DG:");
    lcd.print(j);
    lcd.print("v");
    digitalWrite(12, HIGH);
    digitalWrite(8, HIGH);
  }
}

void buzz() {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
