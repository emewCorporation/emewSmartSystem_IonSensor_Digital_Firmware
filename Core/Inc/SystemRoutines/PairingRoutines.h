/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PAIRING_ROUTINES_H
#define PAIRING_ROUTINES_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "./Global_HALL.h"
#include "./StructDefinitions.h"
#include "./GenericLibraries/DataSerializationLibrary.h"

/* Functions prototypes ------------------------------------------------------*/
void ControllerPairRoutine(void);
void ControllerPingRoutine(void);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* PAIRING_ROUTINES_H */
