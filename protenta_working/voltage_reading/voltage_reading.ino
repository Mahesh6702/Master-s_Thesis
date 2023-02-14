

void setup() {
  // put your setup code here, to run once:
  Serial.begin(96000);

}

void loop() {
  // put your main code here, to run repeatedly:
  HAL_StatusTypeDef HAL_PWREx_ConfigSupply();
  
 // Serial.print("value   "); Serial.print(value); Serial.println(" Volts");
  delay(100);
