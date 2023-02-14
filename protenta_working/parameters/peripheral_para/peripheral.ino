
I2C_HandleTypeDef hi2c;
SPI_HandleTypeDef hspi;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  char str1[10];
  char str2[10];
  // put your main code here, to run repeatedly:
  int value = (hi2c.State == HAL_I2C_STATE_READY)?1:0;
  sprintf(str1,"%d",value);
  strcat(strcat(str1, "|"),str2);
  int value1 = (hspi.State == HAL_SPI_STATE_READY)?1:0; 
  sprintf(str2,"%d",value1);
  strcat(strcat(str1, "|"),str2);
  Serial.println("||"); Serial.print(str1); Serial.println(":");
  delay(1000);
/*  
 if(hi2c.State == HAL_I2C_STATE_READY)
 {
  Serial.println(" I2C");
 }
 else
 {
 Serial.println(" NO I2C");
 }
  
  delay(100);
if (hspi.State == HAL_SPI_STATE_READY)
 {
  Serial.println(" SPI");
 }
 else
 {
 Serial.println(" NO SPI");
 }
  
  delay(100);
  */
}
