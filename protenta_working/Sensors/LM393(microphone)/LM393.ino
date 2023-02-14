/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/
#include<math.h>
//#include<mbed.h>
//#define Serial _UART_USB
//#define HSI_VALUE 4000
// the setup routine runs once when you press reset:
void setup() {
  //digitalWrite(D4,OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(96000);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
   //RCC->CFGR ;
   //RCC_CFGR_SWS;
   //HSE_VALUE = 300000000;
   //SystemCoreClockUpdate();
  int value=SystemCoreClock;
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
  delay(1000);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
  delay(1000);  
  Serial.println(value);
  int sensorValue = analogRead(A0);
  //float decibles = 20* log10(sensorValue).
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1);        // delay in between reads for stability
}
