/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SENSOR_CONTROL_ROUTINES_H
#define SENSOR_CONTROL_ROUTINES_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "./Global_HALL.h"
#include "./StructDefinitions.h"
#include "./GenericLibraries/DataSerializationLibrary.h"

#define METAL_LED	 	0
#define TURBIDITY_LED	1


/* Functions prototypes ------------------------------------------------------*/
void Transimpedance_TakeMeasurement(float* VoltageReturn, uint8_t SampleNumber);
void SensorLED_BrightnessControl(uint8_t SensorType, float SetCurrent );


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* SENSOR_CONTROL_ROUTINES_H */
