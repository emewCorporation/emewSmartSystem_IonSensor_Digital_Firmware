/* Private includes ----------------------------------------------------------------------------*/
#include <SystemRoutines/PairingRoutines.h>

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern CAN_HandleTypeDef  CANBUS_EXT;
extern TIM_HandleTypeDef  TIMER_TIMEOUT;

/* External Struct Declarations ----------------------------------------------------------------*/
extern sysinfo SysInfo;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;

extern CAN_TxHeaderTypeDef pTxHeader;			//CAN transmit properties struct
extern CAN_RxHeaderTypeDef pRxHeader;			//CAN receive properties struct
extern uint32_t TxMailbox;						//CAN Mailbox

extern HAL_StatusTypeDef ret;

/* Private Global Variables --------------------------------------------------------------------*/
static const int TimeoutPeriod = 5000;		//Timeout period of 5000ms



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CANBus Pairing System Routines /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Routine to pair a sensor to a controller board when requested by the controller
void ControllerPairRoutine(void){

	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rDevice Pairing Routine Entered"  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Flag to indicate success of the operation
	_Bool SuccessFlag=true;
	_Bool TimeoutFlag=false;

	//Create CAN message with all sensor ID information
	uint8_t MessageBuffer[8] = {0x00};								//Create data buffer
	MessageBuffer[0]=0xA0;											//Store instruction code
	uint16_to_bytes(SysInfo.CAN_Address,  &MessageBuffer[1]);		//Store device CANBus address
	uint16_to_bytes(SysInfo.SerialNumber, &MessageBuffer[3]);		//Store device serial number
	MessageBuffer[5] = SysInfo.SensorType;							//Store device sensor type

	//Send device pair message to controller's FIFO1
	pTxHeader.StdId = 0x0600 | SysInfo.CAN_Address;
	ret=HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, MessageBuffer, &TxMailbox);
	if(ret!=HAL_OK){
		SuccessFlag = false;
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Pairing Failed - Message failed to Send"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Pairing Data Sent"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

	//Setup parameters for the following routine to receive a validation message from the controller
	memset(MessageBuffer,0,8);				//Reset MessageBuffer
	TIMER_TIMEOUT.Instance->CNT = 0;		//Set timeout timer to 0
	HAL_TIM_Base_Start(&TIMER_TIMEOUT);		//Start timeout timer

	//Wait for pair complete message from the controller over FIFO1 to validate operation
	while( SuccessFlag==true && TimeoutFlag==false ){

		//Check to see if a message has arrived over FIFO1
		if( HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO1) != 0 ){
			ret = HAL_CAN_GetRxMessage(&CANBUS_EXT, CAN_RX_FIFO1, &pRxHeader, MessageBuffer);
			if(ret!=HAL_OK){ SuccessFlag = false; }
			break;
		}

		//Check to see if the timeout period has elapsed
		if( __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT) > TimeoutPeriod  ){
			TimeoutFlag=true;
			SuccessFlag=false;
		}

	}

	//Stop timer before routine completion
	HAL_TIM_Base_Stop(&TIMER_TIMEOUT);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Waited %lums for controller confirmation", __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT)  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	TIMER_TIMEOUT.Instance->CNT = 0;

	//Print routine exit message
	if( SuccessFlag==true ){
  		  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rSensor pair SUCCESSFUL - Exiting Routine"  );
  		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		  memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rSensor pair FAILED - Exiting Routine"  );
		  DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

}




//Routine to ping the controller back when requested
void ControllerPingRoutine(void){

    //Print routine enter message
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rSensor Ping Routine Entered"  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Flag to indicate success of the operation
	_Bool SuccessFlag=true;

	//Create CAN message with ping return information
	uint8_t MessageBuffer[8] = {0xA2, 0xCD, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00 };

	//Transmit ping message
	pTxHeader.StdId = 0x0600 | SysInfo.CAN_Address;
	ret=HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, MessageBuffer, &TxMailbox);
	if(ret!=HAL_OK){ SuccessFlag = false; }

	//Print routine exit message
	if(SuccessFlag==true){
  	    memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rSensor Ping SUCCESS - Exiting Routine"  );
  		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
	    memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rSensor Ping FAILED - Exiting Routine"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

}


