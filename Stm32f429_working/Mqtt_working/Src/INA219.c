/*
 * INA219.c
 *
 *  Created on: Sep 7, 2022
 *      Author: eegam
 */
#include <stdint.h>
#include <string.h>
#include "ina219.h"
#include "FreeRTOS.h"

#include "stm32F4xx_hal.h"
#include "stm32F4xx.h"



extern I2C_HandleTypeDef hi2c1;



// The following multipliers are used to convert raw current and power
// values to mA and mW, taking into account the current config settings
uint32_t ina219_currentDivider_mA;
uint32_t ina219_powerMultiplier_mW;

uint32_t ina219_calValue;

#define BUFFERLEN 4096
int16_t contBuffer[BUFFERLEN];
unsigned int bufferPos;

/* Holds currently type of continuous measurement (voltage, power, current).
 * Used to avoid floating point numbers prolification */
int measureType;

/**************************************************************************/
/*!
 @brief  Sends a single command byte over I2C
 */
/**************************************************************************/
uint8_t adrs_219 = 0x41; // you can call this from main function when necessary especially when using multiple INA219 on board
void wireWriteRegister (uint8_t reg, uint16_t value)
{
    uint8_t i2c_temp[2];
    i2c_temp[0] = value>>8;
    i2c_temp[1] = value;
    HAL_I2C_Mem_Write(&hi2c1, adrs_219<<1, (uint16_t)reg, 1, i2c_temp, 2, 0xffffffff);
    HAL_Delay(1);
}

/**************************************************************************/
/*!
 @brief  Reads a 16 bit values over I2C
 */
/**************************************************************************/
void wireReadRegister(uint8_t reg, uint16_t *value)
{
    uint8_t i2c_temp[2];
    HAL_I2C_Mem_Read(&hi2c1, adrs_219<<1, (uint16_t)reg, 1,i2c_temp, 2, 0xffffffff);
    HAL_Delay(1);
    *value = ((uint16_t)i2c_temp[0]<<8 )|(uint16_t)i2c_temp[1];
}

/**************************************************************************/
/*!
 @brief  Configures to INA219 to be able to measure up to 32V and 2A
 of current.  Each unit of current corresponds to 100uA, and
 each unit of power corresponds to 2mW. Counter overflow
 occurs at 3.2A.

 @note   These calculations assume a 0.1 ohm resistor is present
 */
/**************************************************************************/
void setCalibration_32V_2A(void) {
    // By default we use a pretty huge range for the input voltage,
    // which probably isn't the most appropriate choice for system
    // that don't use a lot of power.  But all of the calculations
    // are shown below if you want to change the settings.  You will
    // also need to change any relevant register settings, such as
    // setting the VBUS_MAX to 16V instead of 32V, etc.

    // VBUS_MAX = 32V             (Assumes 32V, can also be set to 16V)
    // VSHUNT_MAX = 0.32          (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
    // RSHUNT = 0.1               (Resistor value in ohms)

    // 1. Determine max possible current
    // MaxPossible_I = VSHUNT_MAX / RSHUNT
    // MaxPossible_I = 3.2A

    // 2. Determine max expected current
    // MaxExpected_I = 2.0A

    // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
    // MinimumLSB = MaxExpected_I/32767
    // MinimumLSB = 0.000061              (61uA per bit)
    // MaximumLSB = MaxExpected_I/4096
    // MaximumLSB = 0,000488              (488uA per bit)

    // 4. Choose an LSB between the min and max values
    //    (Preferrably a roundish number close to MinLSB)
    // CurrentLSB = 0.0001 (100uA per bit)

    // 5. Compute the calibration register
    // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
    // Cal = 4096 (0x1000)

    // ina219_calValue = 4096;
    ina219_calValue = 5120;  // Rsh=0.04R,

    // 6. Calculate the power LSB
    // PowerLSB = 20 * CurrentLSB
    // PowerLSB = 0.002 (2mW per bit)

    // 7. Compute the maximum current and shunt voltage values before overflow
    //
    // Max_Current = Current_LSB * 32767
    // Max_Current = 3.2767A before overflow
    //
    // If Max_Current > Max_Possible_I then
    //    Max_Current_Before_Overflow = MaxPossible_I
    // Else
    //    Max_Current_Before_Overflow = Max_Current
    // End If
    //
    // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
    // Max_ShuntVoltage = 0.32V
    //
    // If Max_ShuntVoltage >= VSHUNT_MAX
    //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
    // Else
    //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
    // End If

    // 8. Compute the Maximum Power
    // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
    // MaximumPower = 3.2 * 32V
    // MaximumPower = 102.4W

    // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = 6; // Current LSB = 100uA per bit (1000/100 = 10)
    ina219_powerMultiplier_mW = 3;     // Power LSB = 1mW per bit (2/1)

    // Set Calibration register to 'Cal' calculated above
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V
            | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT
            | INA219_CONFIG_SADCRES_12BIT_1S_532US
            | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    wireWriteRegister(INA219_REG_CONFIG, config);
}

/**************************************************************************/
/*!
 @brief  Configures to INA219 to be able to measure up to 32V and 1A
 of current.  Each unit of current corresponds to 40uA, and each
 unit of power corresponds to 800�W. Counter overflow occurs at
 1.3A.

 @note   These calculations assume a 0.1 ohm resistor is present
 */
/**************************************************************************/
void setCalibration_32V_1A(void) {
    // By default we use a pretty huge range for the input voltage,
    // which probably isn't the most appropriate choice for system
    // that don't use a lot of power.  But all of the calculations
    // are shown below if you want to change the settings.  You will
    // also need to change any relevant register settings, such as
    // setting the VBUS_MAX to 16V instead of 32V, etc.

    // VBUS_MAX = 32V        (Assumes 32V, can also be set to 16V)
    // VSHUNT_MAX = 0.32    (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
    // RSHUNT = 0.1            (Resistor value in ohms)

    // 1. Determine max possible current
    // MaxPossible_I = VSHUNT_MAX / RSHUNT
    // MaxPossible_I = 3.2A

    // 2. Determine max expected current
    // MaxExpected_I = 1.0A

    // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
    // MinimumLSB = MaxExpected_I/32767
    // MinimumLSB = 0.0000305             (30.5�A per bit)
    // MaximumLSB = MaxExpected_I/4096
    // MaximumLSB = 0.000244              (244�A per bit)

    // 4. Choose an LSB between the min and max values
    //    (Preferrably a roundish number close to MinLSB)
    // CurrentLSB = 0.0000400 (40�A per bit)

    // 5. Compute the calibration register
    // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
    // Cal = 10240 (0x2800)

    ina219_calValue = 10240;

    // 6. Calculate the power LSB
    // PowerLSB = 20 * CurrentLSB
    // PowerLSB = 0.0008 (800�W per bit)

    // 7. Compute the maximum current and shunt voltage values before overflow
    //
    // Max_Current = Current_LSB * 32767
    // Max_Current = 1.31068A before overflow
    //
    // If Max_Current > Max_Possible_I then
    //    Max_Current_Before_Overflow = MaxPossible_I
    // Else
    //    Max_Current_Before_Overflow = Max_Current
    // End If
    //
    // ... In this case, we're good though since Max_Current is less than MaxPossible_I
    //
    // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
    // Max_ShuntVoltage = 0.131068V
    //
    // If Max_ShuntVoltage >= VSHUNT_MAX
    //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
    // Else
    //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
    // End If

    // 8. Compute the Maximum Power
    // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
    // MaximumPower = 1.31068 * 32V
    // MaximumPower = 41.94176W

    // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = 25;  // Current LSB = 40uA per bit (1000/40 = 25)
    ina219_powerMultiplier_mW = 1;         // Power LSB = 800mW per bit

    // Set Calibration register to 'Cal' calculated above
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V
            | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT
            | INA219_CONFIG_SADCRES_12BIT_1S_532US
            | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    wireWriteRegister(INA219_REG_CONFIG, config);
}

/**************************************************************************/
/*!
 @brief set device to alibration which uses the highest precision for
 current measurement (0.1mA), at the expense of
 only supporting 16V at 400mA max.
 */
/**************************************************************************/
void setCalibration_16V_400mA(void) {

    // Calibration which uses the highest precision for
    // current measurement (0.1mA), at the expense of
    // only supporting 16V at 400mA max.

    // VBUS_MAX = 16V
    // VSHUNT_MAX = 0.04          (Assumes Gain 1, 40mV)
    // RSHUNT = 0.1               (Resistor value in ohms)

    // 1. Determine max possible current
    // MaxPossible_I = VSHUNT_MAX / RSHUNT
    // MaxPossible_I = 0.4A

    // 2. Determine max expected current
    // MaxExpected_I = 0.4A

    // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
    // MinimumLSB = MaxExpected_I/32767
    // MinimumLSB = 0.0000122              (12uA per bit)
    // MaximumLSB = MaxExpected_I/4096
    // MaximumLSB = 0.0000977              (98uA per bit)

    // 4. Choose an LSB between the min and max values
    //    (Preferrably a roundish number close to MinLSB)
    // CurrentLSB = 0.00005 (50uA per bit)

    // 5. Compute the calibration register
    // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
    // Cal = 8192 (0x2000)

    ina219_calValue = 8192;

    // 6. Calculate the power LSB
    // PowerLSB = 20 * CurrentLSB
    // PowerLSB = 0.001 (1mW per bit)

    // 7. Compute the maximum current and shunt voltage values before overflow
    //
    // Max_Current = Current_LSB * 32767
    // Max_Current = 1.63835A before overflow
    //
    // If Max_Current > Max_Possible_I then
    //    Max_Current_Before_Overflow = MaxPossible_I
    // Else
    //    Max_Current_Before_Overflow = Max_Current
    // End If
    //
    // Max_Current_Before_Overflow = MaxPossible_I
    // Max_Current_Before_Overflow = 0.4
    //
    // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
    // Max_ShuntVoltage = 0.04V
    //
    // If Max_ShuntVoltage >= VSHUNT_MAX
    //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
    // Else
    //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
    // End If
    //
    // Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
    // Max_ShuntVoltage_Before_Overflow = 0.04V

    // 8. Compute the Maximum Power
    // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
    // MaximumPower = 0.4 * 16V
    // MaximumPower = 6.4W

    // Set multipliers to convert raw current/power values
    ina219_currentDivider_mA = 20;  // Current LSB = 50uA per bit (1000/50 = 20)
    ina219_powerMultiplier_mW = 1;     // Power LSB = 1mW per bit

    // Set Calibration register to 'Cal' calculated above
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Set Config register to take into account the settings above
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V
            | INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT
            | INA219_CONFIG_SADCRES_12BIT_1S_532US
            | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    wireWriteRegister(INA219_REG_CONFIG, config);
}

/**************************************************************************/
/*!
 @brief  Gets the raw bus voltage (16-bit signed integer, so +-32767)
 @return the raw bus voltage reading
 */
/**************************************************************************/
int16_t getBusVoltage_raw() {
    uint16_t value;
    wireReadRegister(INA219_REG_BUSVOLTAGE, &value);

    // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
    return (int16_t) ((value >> 3) * 4);
}

/**************************************************************************/
/*!
 @brief  Gets the raw shunt voltage (16-bit signed integer, so +-32767)
 @return the raw shunt voltage reading
 */
/**************************************************************************/
int16_t getShuntVoltage_raw() {
    uint16_t value;
    wireReadRegister(INA219_REG_SHUNTVOLTAGE, &value);
    return (int16_t) value;
}

/**************************************************************************/
/*!
 @brief  Gets the raw current value (16-bit signed integer, so +-32767)
 @return the raw current reading
 */
/**************************************************************************/
int16_t getCurrent_raw() {
    uint16_t value;

    // Sometimes a sharp load will reset the INA219, which will
    // reset the cal register, meaning CURRENT and POWER will
    // not be available ... avoid this by always setting a cal
    // value even if it's an unfortunate extra step
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Now we can safely read the CURRENT register!
    wireReadRegister(INA219_REG_CURRENT, &value);

    return (int16_t) value;
}

/**************************************************************************/
/*!
 @brief  Gets the raw power value (16-bit signed integer, so +-32767)
 @return raw power reading
 */
/**************************************************************************/
int16_t getPower_raw() {
    uint16_t value;

    // Sometimes a sharp load will reset the INA219, which will
    // reset the cal register, meaning CURRENT and POWER will
    // not be available ... avoid this by always setting a cal
    // value even if it's an unfortunate extra step
    wireWriteRegister(INA219_REG_CALIBRATION, ina219_calValue);

    // Now we can safely read the POWER register!
    wireReadRegister(INA219_REG_POWER, &value);

    return (int16_t) value;
}

/**************************************************************************/
/*!
 @brief  Gets the shunt voltage in mV (so +-327mV)
 @return the shunt voltage converted to millivolts
 */
/**************************************************************************/
float getShuntVoltage_mV() {
    int16_t value;
    value = getShuntVoltage_raw();
    return value * 0.01;
}

/**************************************************************************/
/*!
 @brief  Gets the shunt voltage in volts
 @return the bus voltage converted to volts
 */
/**************************************************************************/
float getBusVoltage_V() {
    int16_t value = getBusVoltage_raw();
    return value * 0.001;
}

/**************************************************************************/
/*!
 @brief  Gets the current value in mA, taking into account the
 config settings and current LSB
 @return the current reading convereted to milliamps
 */
/**************************************************************************/
float getCurrent_mA() {
    float valueDec = getCurrent_raw();
    valueDec /= ina219_currentDivider_mA;
    return valueDec;
}

/**************************************************************************/
/*!
 @brief  Gets the power value in mW, taking into account the
 config settings and current LSB
 @return power reading converted to milliwatts
 */
/**************************************************************************/
float getPower_mW() {
    float valueDec = getPower_raw();
    valueDec *= ina219_powerMultiplier_mW;
    return valueDec;
}

/* Initialize continuous measurement.
 * Sets register pointer to desired measurement type (current, bus voltage, shunt voltage, power).
 * @param reg Device register
 */
int contMeasureInit(uint8_t reg) {
    HAL_StatusTypeDef status;
    measureType = reg;

    /* Set register pointer to desired register */
    status = HAL_I2C_Master_Transmit(&hi2c1, adrs_219 << 1, &reg, 1,
            0xffffffff);
    if (status != HAL_OK)
        while (1)
            ;

    bufferPos = 0;
    memset(contBuffer, 0, sizeof(contBuffer));

    return 0;
}

/* Updates measurement buffers. To be called periodically by application.
 * @retval Current buffer position
 */
int contMeasureUpdate(void) {
    HAL_StatusTypeDef status;
    uint8_t measure[2];

    status = HAL_I2C_Master_Receive(&hi2c1, adrs_219 << 1,
            (uint8_t*) &measure, 2, 0xffffffff);
    if (status != HAL_OK)
        while (1)
            ;

    /* Change endinanness */
    if (bufferPos < BUFFERLEN)
        contBuffer[bufferPos++] = ina219_powerMultiplier_mW
                * (((uint16_t) measure[0] << 8) | (uint16_t) measure[1]);

    return bufferPos;
}

/** Converts a given integer raw measure in actual floating point one
 * @param rawValue Raw value as obtained from continuous measurement buffer
 * @retval Actual floating point value of given raw measurement
 */
float convertMeasure(int rawValue) {
    float val = rawValue;

    switch (measureType) {
    case INA219_REG_SHUNTVOLTAGE:
        val *= 0.01f;
        break;

    case INA219_REG_BUSVOLTAGE:
        val *= 0.001f;
        break;

    case INA219_REG_POWER:
        val *= ina219_powerMultiplier_mW;
        break;

    case INA219_REG_CURRENT:
        val /= ina219_currentDivider_mA;
        break;

    default:
        val = -1;
    }

    return val;
}

/** Returns the number of samples that have been acquired during continous measurement
 * @retval Numner of samples acquired
 */
int getNSamples(void) {
    return bufferPos;
}


