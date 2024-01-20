/* Private includes ----------------------------------------------------------------------------*/
#include "./Global_HALL.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  Pin Read Functions //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GPIO_PinState Read_Button1(void) { return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_5 ); }
GPIO_PinState Read_Button2(void) { return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_6 ); }
GPIO_PinState Read_Button3(void) { return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_7 ); }

GPIO_PinState Read_LED_HeartBeat(void)  { return HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_13 ); }
GPIO_PinState Read_LED_Indication1(void){ return HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_0 );  }
GPIO_PinState Read_LED_Indication2(void){ return HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_1 );  }
GPIO_PinState Read_LED_Indication3(void){ return HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_2 );  }
GPIO_PinState Read_LED_Indication4(void){ return HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_10 ); }
GPIO_PinState Read_LED_Indication5(void){ return HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_11 ); }
GPIO_PinState Read_LED_Metal(void)      {  return HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_14 ); }
GPIO_PinState Read_LED_Turbidity(void)  {  return HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_15 ); }



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////    LED Pin Control Functions ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Set_LED_HeartBeat(_Bool State)  {  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_13, State ); }
void Set_LED_Indication1(_Bool State){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_0, State );  }
void Set_LED_Indication2(_Bool State){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_1, State );  }
void Set_LED_Indication3(_Bool State){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2, State );  }
void Set_LED_Indication4(_Bool State){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_10, State ); }
void Set_LED_Indication5(_Bool State){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_11, State ); }
void Set_LED_Metal(_Bool State)		{  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_14, State );  }
void Set_LED_Turbidity(_Bool State) {  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_15, State );  }
void Set_Flash_CS(_Bool State)		{  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_12, State );  }
void Set_Flash_WP(_Bool State) 		{  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_10, State );  }


void Toggle_LED_HeartBeat(void)  {  HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_13 ); }
void Toggle_LED_Indication1(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_0 );  }
void Toggle_LED_Indication2(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_1 );  }
void Toggle_LED_Indication3(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_2 );  }
void Toggle_LED_Indication4(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_10 ); }
void Toggle_LED_Indication5(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_11 ); }
void Toggle_LED_Metal(void){  HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_14 );  }
void Toggle_LED_Turbidity(void){  HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_15 );  }



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////   ADC Functions  //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
float ucADC_getValue(void){

	uint16_t Raw_ADCValue;
 	float ADC_Value;

 	//HAL_ADC_Stop(&ADC_uC);							       		//Stop ADC
 	//HAL_ADCEx_Calibration_Start(&ADC_uC, ADC_SINGLE_ENDED);		//Calibrate uC's ADC
    //HAL_Delay(10);
    HAL_ADC_Start(&ADC_uC);							         	//Start ADC
	HAL_ADC_PollForConversion(&ADC_uC, HAL_MAX_DELAY);		 	//Poll for ADC value
 	HAL_ADC_Stop(&ADC_uC);							       		//Stop ADC
	Raw_ADCValue = HAL_ADC_GetValue(&hadc1) & 0x0FFF;     	    //Read ADC value
	ADC_Value = (float)Raw_ADCValue/4095 * 3.08;				//Convert ADC value to float

	return ADC_Value;

}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  CONSOLE PRINT  //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Print string data to console over UART
void DebugPrint(uint8_t PrintEnable, char* StringBuffer, uint16_t Size){

	//If print condition is enabled, send data over UART
	if(PrintEnable == true){
		HAL_UART_Transmit(&UART_COMM, (uint8_t*)StringBuffer, Size, 100 );
	}

}







