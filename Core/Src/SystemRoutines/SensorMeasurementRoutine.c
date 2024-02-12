/* Private includes ----------------------------------------------------------------------------*/
#include "./SystemRoutines/SensorControlRoutines.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern CAN_HandleTypeDef  CANBUS_EXT;
extern TIM_HandleTypeDef  TIMER_TIMEOUT;

/* External Struct Declarations ----------------------------------------------------------------*/
extern sysinfo SysInfo;

/* External Variable Declarations --------------------------------------------------------------*/
extern CAN_TxHeaderTypeDef pTxHeader;			//CAN transmit properties struct
extern CAN_RxHeaderTypeDef pRxHeader;			//CAN receive properties struct
extern uint32_t TxMailbox;						//CAN Mailbox

extern HAL_StatusTypeDef ret;
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Ion Measurement and Transmit Routines //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Routine to measure data from the two sensors and send it to the controller
void SensorMeasurementRoutine(void){

	/*
	//Print routine start message
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rSensor Measurement Routine Entered" );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Variable declaration
	_Bool SuccessFlag = true;
	float ADC_SampleValue;
	uint8_t MessageBuffer[8] = {0x00};

	//Measure turbidity value
	Set_LED_Turbidity(1); 					//Turn turbidity LED on
	HAL_Delay(100);							//Wait 100ms
	ADC_SampleValue = ucADC_getValue();		//Get ADC value
	Set_LED_Turbidity(0); 					//Turn turbidity LED off
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\tTurbidity Measured at: %0.3f V", ADC_SampleValue );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Store turbidity value
	float_to_bytes( ADC_SampleValue, &MessageBuffer[0] );

    //Measure metal value
	Set_LED_Metal(1); 						 //Turn metal LED on
	HAL_Delay(100);							 //Wait
	ADC_SampleValue = ucADC_getValue();		 //Get ADC value
	Set_LED_Metal(0); 				 		 //Turn metal LED off
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\tMetal Value is: %0.3f V", ADC_SampleValue );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Store metal value
	float_to_bytes( ADC_SampleValue, &MessageBuffer[4] );

	//Send device pair message to controller's FIFO1
	pTxHeader.StdId = 0x0600 | SysInfo.CAN_Address;
	ret=HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, MessageBuffer, &TxMailbox);
	if(ret!=HAL_OK){  SuccessFlag = false; }


	//Print out routine exit message
	if(SuccessFlag==true){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rSensor Measurement Routine SUCCESS - Exiting Routine" );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rSensor Measurement Routine FAILED - Exiting Routine" );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}
*/

}

