/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PrimaryStateMachine_H
#define PrimaryStateMachine_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "./Global_HALL.h"
#include "./StructDefinitions.h"
#include "./GenericLibraries/DataSerializationLibrary.h"
#include "./SystemRoutines/PairingRoutines.h"
#include "./SystemRoutines/SensorMeasurementRoutine.h"


/* Function Prototypes --------------------------------------------------------*/
void PrimaryStateMachine(void);

/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* PrimaryStateMachine_H */
