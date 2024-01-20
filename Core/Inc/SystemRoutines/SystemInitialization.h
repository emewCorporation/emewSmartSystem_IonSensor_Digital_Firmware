/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SystemInitialization_H
#define SystemInitialization_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "./Global_HALL.h"
#include "./StructDefinitions.h"
#include "./GenericLibraries/DataSerializationLibrary.h"

#include <Peripheral_Libraries/EEPROM_Library.h>
#include "./Peripheral_Libraries/FlashMemory_Library.h"
#include <Peripheral_Libraries/MCP4726A1T_E.h>
#include <Peripheral_Libraries/MCP3421.h>
#include "./Peripheral_Libraries/CANBusProtocol_Library.h"

/* Functions prototypes ------------------------------------------------------*/
_Bool SystemActivation(void);
_Bool InitSystemStruct(void);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* SystemInitialization_H */
