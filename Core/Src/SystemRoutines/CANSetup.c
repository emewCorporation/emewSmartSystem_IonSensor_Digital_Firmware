/* Private includes ----------------------------------------------------------------------------*/
#include <SystemRoutines/CANSetup.h>

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern CAN_HandleTypeDef  CANBUS_EXT;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;
extern HAL_StatusTypeDef ret;

extern uint32_t TxMailbox;						//CAN Mailbox
extern CAN_TxHeaderTypeDef pTxHeader;			//CAN transmit properties struct
extern CAN_RxHeaderTypeDef pRxHeader;			//CAN receive properties struct

/* Private Global Variables --------------------------------------------------------------------*/
static CAN_FilterTypeDef sFilterConfig0;   	 	//CAN filter properties struct 0
static CAN_FilterTypeDef sFilterConfig1;    	//CAN filter properties struct 1



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////    CANBus System Setup and Activation Function ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Setup for transmitting CANbus
void CANSetup(uint16_t CANBusID){

	//Setup transmit header 0 CAN headers
	pTxHeader.StdId = 0x0400;				 	 // 11 bit ID
	pTxHeader.DLC	= 8;						 // Specifies that 8 bytes will be transmitted per message
	pTxHeader.IDE 	= CAN_ID_STD; 			     // Specifies standard ID (11-bit ID)
	pTxHeader.RTR 	= CAN_RTR_DATA; 			 // Specifies data type as "Data Frame"

	//Setup for CAN filter 0 - Used for FIFO0
	sFilterConfig0.FilterBank  		 	= 0;
	sFilterConfig0.FilterMode   		= CAN_FILTERMODE_IDLIST;
	sFilterConfig0.FilterScale  		= CAN_FILTERSCALE_16BIT;
	sFilterConfig0.FilterIdHigh			= (CANBusID&0x01FF)<<5;
	sFilterConfig0.FilterIdLow  		= (0x0000)<<5;
	sFilterConfig0.FilterMaskIdHigh 	= (0x0000)<<5;
	sFilterConfig0.FilterMaskIdLow 		= (0x0000)<<5;
	sFilterConfig0.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig0.FilterActivation 	= ENABLE;

	//Setup for CAN filter 1 - Used for FIFO1
	sFilterConfig1.FilterBank 			= 1;
	sFilterConfig1.FilterMode 			= CAN_FILTERMODE_IDLIST;
	sFilterConfig1.FilterScale 			= CAN_FILTERSCALE_16BIT;
	sFilterConfig1.FilterIdHigh			= ((CANBusID&0x01FF)|0x0200)<<5;
	sFilterConfig1.FilterIdLow 			= (0x0200)<<5;
	sFilterConfig1.FilterMaskIdHigh     = (0x0200)<<5;
	sFilterConfig1.FilterMaskIdLow 	    = (0x0200)<<5;
	sFilterConfig1.FilterFIFOAssignment = CAN_RX_FIFO1;
	sFilterConfig1.FilterActivation 	= ENABLE;


	//Print CANBus Initialization message
    memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rCANBus Initialization Sequence:" );
  	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);


	//Init Config Filter 0
	ret=HAL_CAN_ConfigFilter(&CANBUS_EXT, &sFilterConfig0);
	if( ret != HAL_OK ) {
	    memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - CAN ERROR - Config filter 0 failed to be set" );
	  	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else {
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - Config Filter 0 Set" );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

	//Init Config Filter 1
	ret=HAL_CAN_ConfigFilter(&CANBUS_EXT, &sFilterConfig1);
	if( ret != HAL_OK ) {
	    memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - CAN ERROR - Config Filter 1 Failed" );
	  	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else {
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - Config Filter 1 Set" );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

	//Start Notifications on Receive for FIFO0
	ret=HAL_CAN_ActivateNotification(&CANBUS_EXT, CAN_IT_RX_FIFO0_MSG_PENDING);
	if ( ret != HAL_OK ) {
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - CAN ERROR - Activate FIFO0 Notification Failed"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - Notification on FIFO0 Receive Started" );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}


	//Start Notifications on Receive for FIFO1
		ret=HAL_CAN_ActivateNotification(&CANBUS_EXT, CAN_IT_RX_FIFO1_MSG_PENDING);
		if ( ret != HAL_OK ) {
			memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - CAN ERROR - Activate FIFO1 Notification Failed"  );
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		}else{
			memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - Notification on FIFO1 Receive Started" );
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		}


	//Start the CANBus
	ret=HAL_CAN_Start(&CANBUS_EXT);
	if ( ret != HAL_OK ) {
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rERROR - CANBus Hardware Start Failed"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\rCANBus Hardware Activated"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}


}
