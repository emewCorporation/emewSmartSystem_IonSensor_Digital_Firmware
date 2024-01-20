/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ProgramEEPROM_H
#define ProgramEEPROM_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include "./Global_HALL.h"
#include "./GenericLibraries/DataSerializationLibrary.h"
#include "./Peripheral_Libraries/EEPROM_Library.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Functions prototypes ---------------------------------------------*/
void ProgramEEPROM_All(void);
void ProgramEEPROM_CAN(void);
void ProgramEEPROM_SensorType(void);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* TestingRoutine_H */

