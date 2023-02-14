void setup() {
  // put your setup code here, to run once:
  Serial.begin(96000);

}

void loop() {
  // put your main code here, to run repeatedly:
  int value =HAL_RCC_GetSysClockFreq() / 1000000;
  Serial.print("Clock Frequency   "); Serial.print(value); Serial.println(" MHz");
  delay(100);

}
