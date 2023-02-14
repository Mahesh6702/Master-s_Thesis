void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly: 
  uint16_t data;
  char str2[10];
  PWR_PVDTypeDef pvdconfig;
  HAL_PWR_EnablePVD(); //PWR->CR1 |= (1<<4); // Enabling the programmable voltage detector.
 // uint32_t mask = 0x00000007;
    data  =( uint16_t)(( (PWR->CR1)>>5 )&(0x00000007));
   data = pvdconfig.PVDLevel;
  sprintf(str2,"%u",data);
   Serial.print(str2);Serial.println(".");

}
