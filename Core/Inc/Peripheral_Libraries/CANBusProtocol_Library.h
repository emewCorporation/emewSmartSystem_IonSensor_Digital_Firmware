/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANBus_Protocol_H
#define CANBus_Protocol_H

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "./Global_HALL.h"
#include "./GenericLibraries/DataSerializationLibrary.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Functions prototypes ---------------------------------------------*/
void CAN_ClearFIFO(uint32_t RxFifo);
uint8_t CAN_ReceiveRoutine(uint8_t* OutputDataArray);
uint8_t CAN_TransmitRoutine(uint16_t DeviceID, uint16_t ComandBytes, uint8_t* InputDataArray, int ArraySize);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* CANBus_Protocols_H */

