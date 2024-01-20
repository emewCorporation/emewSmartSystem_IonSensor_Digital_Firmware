/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_Library_H
#define HAL_Library_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/* Global HAL Defines ------------------------------------------------------------*/
#define UART_COMM    	huart2
#define I2C_SYS     	hi2c1
#define CANBUS_EXT 		hcan
#define SPI_SYS 		hspi2
//#define ADC_uC      	hadc1

#define TIMER_HRT		htim6	 //Drives heart beat LED
#define TIMER_TIMEOUT   htim17		//Millisecond timer for time out operations
#define TIMER_MICROSEC	htim16		//Generic microsecond timer (delay operations)


//#define TIM2  htim2
//#define TIM12 htim12
//#define TIM13 htim13

/* Global Defines ------------------------------------------------------------------*/
#define COMM_SIZE 128

/* Function Prototypes ------------------------------------------------------------------*/

GPIO_PinState Read_Button1(void);
GPIO_PinState Read_Button2(void);
GPIO_PinState Read_Button3(void);
GPIO_PinState Read_LED_HeartBeat(void);
GPIO_PinState Read_LED_Indication1(void);
GPIO_PinState Read_LED_Indication2(void);
GPIO_PinState Read_LED_Indication3(void);
GPIO_PinState Read_LED_Indication4(void);
GPIO_PinState Read_LED_Indication5(void);
GPIO_PinState Read_LED_Turbidity(void);
GPIO_PinState Read_LED_Metal(void);


/*
void Set_LED_HeartBeat(_Bool State);
void Set_LED_Indication(_Bool State);
void Set_LED_Turbidity(_Bool State);
void Set_LED_Metal(_Bool State);
void Toggle_LED_HeartBeat(void);
void Toggle_LED_Indication(void);

float ucADC_getValue(void);
*/

void Set_LED_HeartBeat(_Bool State);
void Set_LED_Indication1(_Bool State);
void Set_LED_Indication2(_Bool State);
void Set_LED_Indication3(_Bool State);
void Set_LED_Indication4(_Bool State);
void Set_LED_Indication5(_Bool State);
void Set_LED_Metal(_Bool State);
void Set_LED_Turbidity(_Bool State);
void Set_Flash_CS(_Bool State);
void Set_Flash_WP(_Bool State);


void Toggle_LED_HeartBeat(void);
void Toggle_LED_Indication1(void);
void Toggle_LED_Indication2(void);
void Toggle_LED_Indication3(void);
void Toggle_LED_Indication4(void);
void Toggle_LED_Indication5(void);
void Toggle_LED_Metal(void);
void Toggle_LED_Turbidity(void);

void DebugPrint(uint8_t PrintEnable, char* StringBuffer, uint16_t Size);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* HAL_Library_H */
