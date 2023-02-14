unsigned long myTime;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  myTime = millis();
  Serial.println(myTime);// Gibt die Zeit seit dem Programmstart aus
  delay(1000);         // Eine Sekunde warten, um keine riesigen Datenmengen zu senden
}
