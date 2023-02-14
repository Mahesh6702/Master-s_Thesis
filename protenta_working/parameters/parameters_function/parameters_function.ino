
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_MAX31865.h>
#include <PortentaEthernet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>



Adafruit_INA219 ina219;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(D2, 11, 12, 13);
I2C_HandleTypeDef hi2c;
SPI_HandleTypeDef hspi;
EthernetUDP Udp;



// Data structure to monitor parameters
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

// Local IP address of source and destination
IPAddress source_ip(192, 168, 1, 10);
IPAddress destination_ip(192, 168, 1, 20);

unsigned int localPort = 50000;  // local port to listen on

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

// Global variable containing parameters information
char str1[30];

unsigned long previousMillis = 0;  // Stores last time temperature was published
const long interval = 10000;       // Interval at which to publish sensor readings
unsigned long myTime;



//******** Parameters Function definition about Energy  monitoring parameters *****///////
const char* parameters()
{

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
        strcat(strcat(str1, " ||"),str2);
      
  //parameter3: Peripherals
     // SPI peripheral
        spi =(hspi.State == HAL_SPI_STATE_READY)?spi1:Noperi1;
        sprintf(str2,"%d", spi);
        strcat(strcat(str1, "||"),str2);
     // I2C peripheral
        i2c =(hi2c.State == HAL_I2C_STATE_READY)?i2c:Noperi;
        sprintf(str2,"%d", i2c);
        strcat(strcat(str1, "||"),str2);
        

  //parameter4: Current
        mcu.current = ina219.getCurrent_mA();
        sprintf(str2,"%u",mcu.current);
        strcat(strcat(str1, " ||"),str2);
        
  //parameter5: Power 
        mcu.power = ina219.getPower_mW();
        sprintf(str2,"%u",mcu.power);
        strcat(strcat(str1, " ||"),str2);

        
  //parameter6:  Voltage
        mcu.power = ina219.getPower_mW();
        sprintf(str2,"%u",mcu.power);
        strcat(strcat(str1, " ||"),str2);
        
 //parameter7: Temperature 
        mcu.temperature = thermo.temperature(RNOMINAL, RREF);
        sprintf(str2,"%u",mcu.temperature);
        strcat(strcat(str1, " ||"),str2);

        return str1;
}

void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  thermo.begin(MAX31865_3WIRE);
  Ethernet.begin(source_ip);
  Udp.begin(localPort);
 

}

void loop() {
  // put your main code here, to run repeatedly:
  char values[50];
  //Serial.print("Time: ");
 // myTime = millis();
 // Serial.println(myTime);// Gibt die Zeit seit dem Programmstart aus
//  delay(1000);

  Serial.print("||"); Serial.print(parameters());Serial.println(" ");
    Udp.beginPacket(destination_ip,50000);
    Udp.write(str1);
    Udp.endPacket();
    delay(100);

}
