/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MCP3421_Library_H
#define MCP3421_Library_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include "./Global_HALL.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/* #Defines for Config -----------------------------------------------------------*/
#define CONV_AUTO 		0x10
#define CONV_ONESHOT	0x00
#define	SAMPLE_12BIT	0x00
#define SAMPLE_14BIT	0x04
#define SAMPLE_16BIT	0x08
#define SAMPLE_18BIT	0x0C
#define PGA_X1			0x00
#define PGA_X2			0x01
#define PGA_X4			0x02
#define PGA_X8			0x03

//Status return codes for the MCP3421 commands
typedef enum{
	ADC_STATUS_OK 			=  0,			//Operation success
	ADC_ERROR_I2C_READ		= -1,			//Error from hal i2c read operation
	ADC_ERROR_I2C_WRITE		= -2,			//Error from hal i2c write operation
	ADC_ERROR_PARAMETER		= -3,			//Passed parameter is invalid
	ADC_ERROR_READBACK		= -4,			//Data integrity verification failed
	ADC_ERROR_OPERATIONAL	= -5,			//Low level function failed in a higher level operation
	ADC_ERROR_UNKNOWN		= -6			//Unknown error
}mcp3421_status_t;


/* Functions prototypes ---------------------------------------------*/
mcp3421_status_t MCP3421_ADC_ReadVoltage(float* ReturnValue);
mcp3421_status_t MCP3421_ADC_Reset(void);
mcp3421_status_t MCP3421_ADC_Init( uint8_t ConfigValue );
mcp3421_status_t MCP3421_ADC_Read_Config( uint8_t* ReadByte );

mcp3421_status_t MCP3421_ADC_Write_Config( uint8_t* WriteArray );
mcp3421_status_t MCP3421_ADC_Read( uint8_t* ReadArray, uint16_t ReadArraySize );

#endif /* MCP3421_Library_H */

