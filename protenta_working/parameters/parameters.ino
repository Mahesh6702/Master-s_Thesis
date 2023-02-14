
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MAX31865.h>

Adafruit_INA219 ina219;
I2C_HandleTypeDef hi2c;
SPI_HandleTypeDef hspi;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(D2, 11, 12, 13);

typedef struct MCU_parameters
{
  
      uint16_t  power;
      uint16_t Clockfrequency;
      uint8_t  current;
      uint8_t  voltage;
      uint8_t  temperature;
      uint8_t  payload;
}MCU_parameters;

typedef enum spi_peri {Noperi1, spi1, spi2, spi3};
typedef enum i2c_peri {Noperi, i2c1, i2c2, i2c3};
typedef enum mode {Nomode,RUN, SLEEP, STOP, STANDBY};

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0



void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  thermo.begin(MAX31865_3WIRE);

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
 // if (! ina219.begin()) {
 //  Serial.println("Failed to find INA219 chip");
 //   while (1) { delay(10); }

}

void loop()
{
  // put your main code here, to run repeatedly:
  char str1[30];
  char str2[10];
      MCU_parameters mcu;
      enum spi_peri spi;
      enum i2c_peri i2c;
      enum mode md;

  //Parameter1: Clock Frequency
        mcu.Clockfrequency= HAL_RCC_GetSysClockFreq()/ 1000000;
        sprintf(str1,"%u",mcu.Clockfrequency );
      
  //parameter2: Mode
        md = RUN;
        sprintf(str2,"%u",md);
        strcat(strcat(str1, "||"),str2);
      
  //parameter3: Peripherals
     // SPI peripheral
        spi =(hspi.State == HAL_SPI_STATE_READY)?spi1:Noperi1;
        sprintf(str2,"%d", spi);
        strcat(strcat(str1, "|"),str2);
     // I2C peripheral
        i2c =(hi2c.State == HAL_I2C_STATE_READY)?i2c:Noperi;
        sprintf(str2,"%d", i2c);
        strcat(strcat(str1, "|"),str2);

  //parameter4: Current
        mcu.current = ina219.getCurrent_mA();
        sprintf(str2,"%u",mcu.current);
        strcat(strcat(str1, "||"),str2);
        
  //parameter5: Power 
        mcu.power = ina219.getPower_mW();
        sprintf(str2,"%u",mcu.current);
        strcat(strcat(str1, "||"),str2);
        
  //parameter6: Temperature 
        mcu.temperature = thermo.temperature(RNOMINAL, RREF);
        sprintf(str2,"%u",mcu.temperature);
        strcat(strcat(str1, "||"),str2);
    
    Serial.print("||"); Serial.print(str1); Serial.println(":");
    delay(100);
  

}
