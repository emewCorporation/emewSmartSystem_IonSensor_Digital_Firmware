/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TestingRoutine_H
#define TestingRoutine_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "./StructDefinitions.h"
#include "./Global_HALL.h"

#include "./GenericLibraries/DataSerializationLibrary.h"
#include <Peripheral_Libraries/EEPROM_Library.h>
#include "./Peripheral_Libraries/FlashMemory_Library.h"
#include <Peripheral_Libraries/MCP4726A1T_E.h>
#include <Peripheral_Libraries/MCP3421.h>
#include "./Peripheral_Libraries/CANBusProtocol_Library.h"




/* Functions prototypes ---------------------------------------------*/
void TestingRoutine(void);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* TestingRoutine_H */

