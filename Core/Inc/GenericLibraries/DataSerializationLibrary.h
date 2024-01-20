/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DataSerialization_H
#define DataSerialization_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"
#include <stdio.h>
#include <string.h>

/* Functions prototypes ------------------------------------------------------*/
void float_to_bytes(float InputValue, uint8_t *OutputByteArray);
float bytes_to_float(uint8_t *InputByteArray);
void uint32_to_bytes(uint32_t InputValue, uint8_t *OutputByteArray);
uint32_t bytes_to_uint32(uint8_t *InputByteArray);
void uint16_to_bytes(uint16_t InputValue, uint8_t *OutputByteArray);
uint16_t bytes_to_uint16(uint8_t *InputByteArray);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* DataSerialization_H */

