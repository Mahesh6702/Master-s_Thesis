/*
 * max6675.c
 *
 *  Created on: Sep 10, 2022
 *      Author: eegam
 */

/*
 * Max6675 is temperature sensor
 * In which 12 bit ADC is present where we are reading the hexa decimal value
 * From D14 - D3 bits of ADC contains temperature data
 */
#include"MAX6675.h"
#include "stm32f4xx_hal.h"
#include "stm32F4xx.h"
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"



SPI_HandleTypeDef hspi1; // SPI handle structure definition

// Required Global variables declaration

uint8_t data[2];   // Sensor raw data (12 bit data, array of 2 bit size is declared with 8 bit integer)

// Temperature Variable

uint8_t MSB_temp ;    // SPI selected as 8 bit data register
uint8_t LSB_temp1;
uint8_t Temp1 =0;
uint8_t Temp2;
uint8_t Temp3;
uint8_t Temp4;
uint8_t  Temp ;
float temperature1;



float Max6675_temparature(void)
{
HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4,GPIO_PIN_RESET); // Chip select is reset(No spi communication)
HAL_SPI_Receive(&hspi1,data,1,50);                  // Receiving 8 bit data from sensor through internal receiver buffer
HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4,GPIO_PIN_SET); // Chip select is reset (spi communication)
MSB_temp = data[0];
LSB_temp1 = data[1];
Temp = MSB_temp|LSB_temp1;
Temp2 &= LSB_temp1 >>5;
Temp1 =((((MSB_temp|LSB_temp1 <<8)))>>3); // Hexadecimal value is converted into decimal value
Temp3 &= MSB_temp <<1;
Temp4 = Temp3|Temp2;
/*
 * As mentioned in data sheet
 * 000...0(12-"0") = 0
 * 111.111 (12-"1") = +1023.75
 * Then FFF or 111.111 (12-"1") or 4095
 * 1 hexadecimal value =  +1023.75 / FFF = 0.25
 * Below temperature = obtained raw sensor data in decimal * 0.25
 */
temperature1 = Temp1* 0.25;                                           // Data to Centigrade Conversation
HAL_Delay(250);                                       // Waits for Chip Ready(according to Datasheet, the max time for conversion is 220ms)
return temperature1 ;
}

