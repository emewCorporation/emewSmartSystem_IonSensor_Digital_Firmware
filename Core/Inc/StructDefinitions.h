/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef StructDefinitions_H
#define StructDefinitions_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include "./Global_HALL.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Struct Typedef ----------------------------------------------------------*/
typedef struct sysinfo {
	_Bool		USBConnection_State;	//State of the USB connection (high = connected, low = disconnected)
	_Bool		USBConnStatus_Flag;		//Flag to check USB connection state (high = check, low = no check)
	_Bool		Button1;		    	//State of the user push button
	_Bool     	Button2;		   	 	//State of the user push button
	_Bool    	Button3;		   	 	//State of the user push button
	uint16_t	SerialNumber;			//Serial number for device identification
	uint8_t	  	SensorType;				//Type of sensor being used with the sensor
	uint16_t  	CAN_Address;			//Unique sensor address for identification and CANBus filtering (11-bits value)
	uint16_t  	EEPROM_Address;			//Current address of the EEPROM	(11-bit value)
} sysinfo;


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* StructDefinitions_H */
