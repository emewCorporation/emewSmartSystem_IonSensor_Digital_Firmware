/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MCP4726A1T_E_Library_H
#define MCP4726A1T_E_Library_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include "./Global_HALL.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Voltage Reference Config
#define VDD_UNBUFF 	0x00	//VDD Unbuffered
#define VREF_UNBUFF 0x02	//VREF Unbuffered
#define VREF_BUFF	0x03	//VREF Buffered

//Power Down Config
#define PD_NO	0x00	//Normal Operation
#define PD_1K	0x01	//Powered Down -> 1k pull-down
#define PD_100K	0x02	//Powered Down -> 100k pull-down
#define PD_500K 0x03	//Powered Down -> 500k pull-down

//Gain Selection Config
#define G_1X	0x00	//Gain -> 1x
#define G_2X	0x01	//Gain -> 2x

//Status return codes for the MCP4726A1T_E commands
typedef enum{
	DAC_STATUS_OK 			=  0,			//Operation success
	DAC_ERROR_I2C_READ		= -1,			//Error from hal i2c read operation
	DAC_ERROR_I2C_WRITE		= -2,			//Error from hal i2c write operation
	DAC_ERROR_PARAMETER		= -3,			//Passed parameter is invalid
	DAC_ERROR_UNKNOWN		= -5			//Unknown error
}dac_status_t;



/* Functions prototypes ---------------------------------------------*/
dac_status_t MCP4726A1T_VoltageOut(float VoltageOutput);
dac_status_t MCP4726A1T_WriteVolatileMemory(uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit, uint16_t DAC_Data);
dac_status_t MCP4726A1T_WriteAllMemory(uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit, uint16_t DAC_Data);
dac_status_t MCP4726A1T_WriteVolatileDACRegister(uint8_t PowerDownBits, uint16_t DAC_Data);
dac_status_t MCP4726A1T_WriteVolatileConfiguration(uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit);

dac_status_t MCP4726A1T_ReadAllMemory(uint8_t* ReadArray);

/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* MCP4726A1T_E_Library_H */
