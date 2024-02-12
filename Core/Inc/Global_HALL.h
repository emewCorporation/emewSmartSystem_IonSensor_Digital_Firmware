/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_Library_H
#define HAL_Library_H

/* Private includes ----------------------------------------------------------*/
#include "./main.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/* Global HAL Defines ------------------------------------------------------------*/
#define UART_COMM    	huart2		//Comm. Handler -> External UART communication
#define CANBUS_EXT 		hcan		//Comm. Handler -> External CANBus communication
#define I2C_SYS     	hi2c1		//Comm. Handler -> System I2C
#define SPI_SYS 		hspi2		//Comm. Handler -> System SPI

#define TIMER_HRT		htim6		//Timer handler -> Drives heart beat LED
#define TIMER_TIMEOUT   htim17		//Timer Handler -> Drives Millisecond timer for time out operations
#define TIMER_MICROSEC	htim16		//Timer Handler -> Drives Generic microsecond timer (delay operations)

#define UC_ADC			hadc1		//ADC MicroController Handler

/* Global Defines ------------------------------------------------------------------*/
#define COMM_SIZE 128

#define CC1_CH 1
#define CC2_CH 2

//Status return codes for the uC ADC commands
typedef enum{
	ADC_STM_STATUS_OK 			=  0,			//Operation success
	ADC_STM_ERROR_HAL_CONFIG	= -1,			//Error from hal on config operation
	ADC_STM_ERROR_HAL_START		= -2,			//Error from hal on start ADC operation
	ADC_STM_ERROR_HAL_STOP		= -3,			//Error from hal on stop ADC operation
	ADC_STM_ERROR_HAL_POLL		= -4,			//Error from hal on ADC poll operation
	ADC_STM_ERROR_HAL_CALIBRATE	= -5,			//Error from hal on calibrate ADC operation
	ADC_STM_ERROR_PARAMETER		= -6,			//Passed parameter is invalid
	ADC_STM_ERROR_UNKNOWN		= -7			//Unknown error
}stm_adc_status_t;



/* Function Prototypes ------------------------------------------------------------------*/

//Functions to read GPIO pins
GPIO_PinState Read_Button1(void);
GPIO_PinState Read_Button2(void);
GPIO_PinState Read_Button3(void);
GPIO_PinState Read_LED_HeartBeat(void);
GPIO_PinState Read_LED_Indication1(void);
GPIO_PinState Read_LED_Indication2(void);
GPIO_PinState Read_LED_Indication3(void);
GPIO_PinState Read_LED_Indication4(void);
GPIO_PinState Read_LED_Indication5(void);
GPIO_PinState Read_USBC_CC1(void);
GPIO_PinState Read_USBC_CC2(void);
GPIO_PinState Read_USBC_SpareTS(void);
GPIO_PinState Read_LED_Turbidity(void);
GPIO_PinState Read_LED_Metal(void);
GPIO_PinState Read_Flash_CS(void);
GPIO_PinState Read_Flash_WP(void);

//Functions to set state of GPIO pins
void Set_LED_HeartBeat(GPIO_PinState PinState);
void Set_LED_Indication1(GPIO_PinState PinState);
void Set_LED_Indication2(GPIO_PinState PinState);
void Set_LED_Indication3(GPIO_PinState PinState);
void Set_LED_Indication4(GPIO_PinState PinState);
void Set_LED_Indication5(GPIO_PinState PinState);
void Set_Flash_CS(GPIO_PinState PinState);
void Set_Flash_WP(GPIO_PinState PinState);

//Functions to toggle the state of the GPIO pins
void Toggle_LED_HeartBeat(void);
void Toggle_LED_Indication1(void);
void Toggle_LED_Indication2(void);
void Toggle_LED_Indication3(void);
void Toggle_LED_Indication4(void);
void Toggle_LED_Indication5(void);
void Toggle_LED_Metal(void);
void Toggle_LED_Turbidity(void);

//Functions to set the state of the sensor LEDs (extra functionality to ensure they cannot be simultaneously active)
void Set_LED_Metal(GPIO_PinState PinState);
void Set_LED_Turbidity(GPIO_PinState PinState);

//Function to sample the uC ADC on a specified channel
stm_adc_status_t STM32_ADC_ReadVoltage(float* ReturnValue, uint8_t Channel);

//Function to send information to the debugging console
void DebugPrint(uint8_t PrintEnable, char* StringBuffer, uint16_t Size);


/* #endif define to prevent recursive inclusion -------------------------------------*/
#endif /* HAL_Library_H */
