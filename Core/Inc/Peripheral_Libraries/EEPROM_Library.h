/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef eepromLibrary_H
#define eepromLibrary_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include "./Global_HALL.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


//Status return codes for the 24FC16HT-I/OT commands
typedef enum{
	EEPROM_STATUS_OK 			=  0,			//Operation success
	EEPROM_ERROR_I2C_READ		= -1,			//Error from hal i2c read operation
	EEPROM_ERROR_I2C_WRITE		= -2,			//Error from hal i2c write operation
	EEPROM_ERROR_PARAMETER		= -3,			//Passed parameter is invalid
	EEPROM_ERROR_UNKNOWN		= -5			//Unknown error
}eeprom_status_t;



/* Functions prototypes ---------------------------------------------*/
eeprom_status_t eeprom_Write(uint16_t MemADDR, uint8_t* WriteArray, int WriteArraySize);
eeprom_status_t eeprom_Read(uint16_t MemADDR, uint8_t* ReadArray, int ReadArraySize);

/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* eepromLibrary_H */

