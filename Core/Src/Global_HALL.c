/* Private includes ----------------------------------------------------------------------------*/
#include "./Global_HALL.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern ADC_HandleTypeDef UC_ADC;



static ADC_ChannelConfTypeDef adcConfig1 = { .Channel = ADC_CHANNEL_1, .Rank=ADC_REGULAR_RANK_1, .SamplingTime=ADC_SAMPLETIME_1CYCLE_5,
									  	  	 .SingleDiff=ADC_SINGLE_ENDED, .OffsetNumber = ADC_OFFSET_NONE, .Offset = 0 };

static ADC_ChannelConfTypeDef adcConfig2 = { .Channel = ADC_CHANNEL_2, .Rank=ADC_REGULAR_RANK_1, .SamplingTime=ADC_SAMPLETIME_1CYCLE_5,
											 .SingleDiff=ADC_SINGLE_ENDED, .OffsetNumber = ADC_OFFSET_NONE, .Offset = 0 };

/* Local Function Prototype --------------------------------------------------------------------*/
void LED_Metal_Control(GPIO_PinState PinState);
void LED_Turbidity_Control(GPIO_PinState PinState);

extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// GPIO Pin Read Functions //////////////////////////////////////////////////////////////////////////
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
GPIO_PinState Read_USBC_CC1(void) 		{ return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_11 );  }
GPIO_PinState Read_USBC_CC2(void) 		{ return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_12 ); }
GPIO_PinState Read_USBC_SpareTS(void) 	{ return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_15 ); }
GPIO_PinState Read_LED_Metal(void)      {  return HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_14 ); }
GPIO_PinState Read_LED_Turbidity(void)  {  return HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_15 ); }

GPIO_PinState Read_Flash_CS(void){ return HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_12 ); }
GPIO_PinState Read_Flash_WP(void){ return HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_10 ); }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GPIO Pin Control Functions //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_LED_HeartBeat(GPIO_PinState PinState)  {  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_13, PinState ); }
void Set_LED_Indication1(GPIO_PinState PinState){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_0,  PinState ); }
void Set_LED_Indication2(GPIO_PinState PinState){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_1,  PinState ); }
void Set_LED_Indication3(GPIO_PinState PinState){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_2,  PinState ); }
void Set_LED_Indication4(GPIO_PinState PinState){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_10, PinState ); }
void Set_LED_Indication5(GPIO_PinState PinState){  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_11, PinState ); }
void Set_Flash_CS(GPIO_PinState PinState)		{  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_12, PinState ); }
void Set_Flash_WP(GPIO_PinState PinState) 		{  HAL_GPIO_WritePin( GPIOA, GPIO_PIN_10, PinState ); }

void Toggle_LED_HeartBeat(void)  {  HAL_GPIO_TogglePin( GPIOC, GPIO_PIN_13 ); }
void Toggle_LED_Indication1(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_0 );  }
void Toggle_LED_Indication2(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_1 );  }
void Toggle_LED_Indication3(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_2 );  }
void Toggle_LED_Indication4(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_10 ); }
void Toggle_LED_Indication5(void){  HAL_GPIO_TogglePin( GPIOB, GPIO_PIN_11 ); }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////    LED Sensor Pin Control Functions /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LED_Metal_Control(GPIO_PinState PinState)		{  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_14, PinState );  }
void LED_Turbidity_Control(GPIO_PinState PinState) {  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_15, PinState );  }


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Sensor LED Activation Control Functions /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Turn on Metal LED but ensure the Turbidity LED is disabled
void Set_LED_Metal(GPIO_PinState PinState){

	//Turn off the metal LED
	if(PinState==GPIO_PIN_RESET){ LED_Metal_Control(GPIO_PIN_RESET); }

	//Turn on the metal LED
	if(PinState==GPIO_PIN_SET){
		if( Read_LED_Turbidity() == GPIO_PIN_SET ){ LED_Turbidity_Control(GPIO_PIN_RESET); } 	//Check Turbidity LED state -> If on, turn off
		LED_Metal_Control(GPIO_PIN_SET);
	}

}


//Turn on Turbidity LED but ensure the Metal LED is disabled
void Set_LED_Turbidity(GPIO_PinState PinState){

	//Turn off the turbidity LED
	if(PinState==GPIO_PIN_RESET){ LED_Turbidity_Control(GPIO_PIN_RESET); }

	//Turn on the turbidity LED
	if(PinState==GPIO_PIN_SET){
		if( Read_LED_Metal() == GPIO_PIN_SET ){ LED_Metal_Control(GPIO_PIN_RESET); } 	//Check Metal LED state -> If on, turn off
		LED_Turbidity_Control(GPIO_PIN_SET);
	}

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  uC ADC Sample  //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

stm_adc_status_t STM32_ADC_ReadVoltage(float* ReturnValue, uint8_t Channel){

	HAL_StatusTypeDef ret_adc;

	//Change ADC to sample on the CC1 pin (Channel 2)
	if(Channel==CC1_CH){
		  adcConfig1.Rank = ADC_REGULAR_RANK_14;
		  adcConfig2.Rank = ADC_REGULAR_RANK_1;
		  ret_adc=HAL_ADC_ConfigChannel(&UC_ADC, &adcConfig1 );		//Modify channel 1 to be lowest rank
		  if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_CONFIG;}
		  ret_adc=HAL_ADC_ConfigChannel(&UC_ADC, &adcConfig2 );		//Modify channel 2 to be highest rank
		  if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_CONFIG;}
	}
	//Change ADC to sample on the CC2 pin (Channel 1)
	else if(Channel==CC2_CH){
		  adcConfig1.Rank = ADC_REGULAR_RANK_1;
		  adcConfig2.Rank = ADC_REGULAR_RANK_14;
		  ret_adc=HAL_ADC_ConfigChannel(&UC_ADC, &adcConfig2 );		//Modify channel 2 to be the lowest rank
		  if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_CONFIG;}
		  ret_adc=HAL_ADC_ConfigChannel(&UC_ADC, &adcConfig1 );		//Modify channel 1 to be the highest rank
		  if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_CONFIG;}
	}else{
		return ADC_STM_ERROR_PARAMETER;
	}

	//Take ADC sample
	ret_adc=HAL_ADCEx_Calibration_Start(&UC_ADC, ADC_SINGLE_ENDED);		//Calibrate ADC
	if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_CALIBRATE;}
	ret_adc=HAL_ADC_Start(&UC_ADC);							       		//Start ADC
	if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_START; }
	ret_adc=HAL_ADC_PollForConversion(&UC_ADC, HAL_MAX_DELAY);			//Poll for ADC value
	if(ret_adc!=HAL_OK){
		HAL_ADC_Stop(&UC_ADC);
		return ADC_STM_ERROR_HAL_POLL;
	}
	ret_adc=HAL_ADC_Stop(&UC_ADC);							       		//Stop ADC
	if(ret_adc!=HAL_OK){ return ADC_STM_ERROR_HAL_STOP; }

	//Process sample data into a float
	uint32_t Raw_ADCValue = HAL_ADC_GetValue(&UC_ADC) & 0x0FFF;  		//Read ADC value
	float ADC_Value = (float)Raw_ADCValue/4095 * 3.30;					//Convert ADC value to float
	*ReturnValue = ADC_Value;											//Save calculated value to return pointer
	//memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rCC%u Value -> %0.3f", Channel, ADC_Value1 );
	//DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	return ADC_STM_STATUS_OK;

}




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







