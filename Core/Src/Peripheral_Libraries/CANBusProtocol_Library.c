/* Private includes ----------------------------------------------------------------------------*/
#include <Peripheral_Libraries/CANBusProtocol_Library.h>

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern TIM_HandleTypeDef TIMER_TIMEOUT;
extern CAN_HandleTypeDef CANBUS_EXT;

/* External Variable Declarations --------------------------------------------------------------*/
extern CAN_TxHeaderTypeDef pTxHeader;
extern CAN_RxHeaderTypeDef pRxHeader;
extern CAN_FilterTypeDef sFilterConfig;
extern uint32_t TxMailbox;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  uC CANBUS MANAGEMENT AND PROTOCOL FUNCTIONS /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Clear the FIFO buffer of the specified CANBus FIFO
void CAN_ClearFIFO(uint32_t RxFifo){

	//Ensure the FIFO passed is valid
	if( RxFifo==CAN_RX_FIFO0 || RxFifo==CAN_RX_FIFO1 ){

		//Temporary 'clear' variables for the operation
		CAN_RxHeaderTypeDef cRxHeader;
		uint8_t cMessageBuffer[8];
		HAL_StatusTypeDef cRet;

		//Retrieve FIFO messages until the buffer is empty
		while( HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, RxFifo) != 0 ){
			cRet=HAL_CAN_GetRxMessage(&CANBUS_EXT, RxFifo, &cRxHeader, cMessageBuffer);
			if(cRet!=HAL_OK){break;}
		}

	}

}






























uint8_t CAN_ReceiveRoutine(uint8_t *OutputDataArray){

	//Check to see status of receive FIFO
	if(HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO0) == 0){			//If FIFO is empty, return error code
		return 0xAA;
	}
	if(HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO0) > 1){			//If FIFO has more then one message, return error code
		return 0xAB;
	}

	//Function variable declaration
	HAL_StatusTypeDef ret;					//Variable to store status of HAL operations
	int Timeout = 100;					 	//Timeout is set to 100ms
	uint16_t TimerRefValue;			  		//Reference value for timer operations
	uint8_t  TransmitMessage[8]={0};		//Array to send CAN messages
	uint8_t  ReceivedMessage[8]={0};		//Array to receive CAN messages
	uint16_t NumberOfMessages;			    //Number of messages to be received
	uint16_t MessageID;						//ID of the message received
	_Bool ContinueFlag = false;			    //Flag to continue


	//Receive first CAN Message
	ret = HAL_CAN_GetRxMessage(&CANBUS_EXT, CAN_RX_FIFO0, &pRxHeader, ReceivedMessage);
    if ( ret != HAL_OK ) {
    	return 0xAC;
    }

    //Get number of messages to be received
	NumberOfMessages = bytes_to_uint16(&ReceivedMessage[4]);

	//Store data from the first received byte
	OutputDataArray[0] = ReceivedMessage[2];		//Command bytes
	OutputDataArray[1] = ReceivedMessage[3];
	OutputDataArray[2] = ReceivedMessage[4];		//Number of messages to be received
	OutputDataArray[3] = ReceivedMessage[5];

	//Create acknowledgement message and sent it
	TransmitMessage[0] = ReceivedMessage[0];
	TransmitMessage[1] = ReceivedMessage[1];
	TransmitMessage[2] = 0xBA;
	TransmitMessage[3] = 0xAB;

	ret=HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, TransmitMessage, &TxMailbox);
    if ( ret != HAL_OK ) {
    	return 0xAD;
    }

	//Algorithm to read rest of messages to be received and store the data in the output data array
	for(uint16_t MessageIteration = 0; MessageIteration < NumberOfMessages; MessageIteration++ ){

		//Loop setup
		memset(ReceivedMessage, 0 , sizeof(ReceivedMessage));	    //Set array to 0
		memset(TransmitMessage, 0 , sizeof(TransmitMessage));	   	//Set array to 0
		ContinueFlag = false;										//Set continue flag to false


		//Wait for next message
		HAL_TIM_Base_Start(&TIMER_TIMEOUT);
		TimerRefValue = __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT);

		while( ContinueFlag == false){
			if( __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT) - TimerRefValue >= Timeout ){	 	//If time out occurs
				HAL_TIM_Base_Stop(&TIMER_TIMEOUT);
				TIMER_TIMEOUT.Instance -> CNT = 0;
				return 0xAE;
			}
			if( HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO0) == 1 ){
				ContinueFlag = true;
			}
		}

		HAL_TIM_Base_Stop(&TIMER_TIMEOUT);			//Stop timer
		TIMER_TIMEOUT.Instance -> CNT = 0;			//Reset timer


		//Receive CAN Message
		HAL_CAN_GetRxMessage(&CANBUS_EXT, CAN_RX_FIFO0, &pRxHeader, ReceivedMessage);
	    if ( ret != HAL_OK ) {
	    	return 0xAC;
	    }

		//Store read data into array
		MessageID = bytes_to_uint16(&ReceivedMessage[0])-1;
		for(int i=0; i<6; i++){
			OutputDataArray[4+i+(MessageID*6)] = ReceivedMessage[i+2];
		}

		//Create acknowledgment message
		TransmitMessage[0] = ReceivedMessage[0];
		TransmitMessage[1] = ReceivedMessage[1];
		TransmitMessage[2] = 0xBA;
		TransmitMessage[3] = 0xAB;

		//Send acknowledgment message over CAN
		ret=HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, TransmitMessage, &TxMailbox);
	    if ( ret != HAL_OK ) {
	    	return 0xAD;
	    }

	} //Return to top of FOR loop

	return 0xAF;

} //End CANReceiveRoutine function



uint8_t CAN_TransmitRoutine(uint16_t DeviceID, uint16_t ComandBytes, uint8_t *InputDataArray, int ArraySize){

	//Function variable declaration
	HAL_StatusTypeDef ret;					    //Variable to store status of HAL operations
	int Timeout = 100;					 		//Timeout is set to 100ms
	uint16_t TimerRefValue;			  			//Reference value for timer operations
	uint8_t TransmitMessage[8] = {0};		    //Array to send CAN messages
	uint8_t ReceivedMessage[8] = {0};	      	//Array to receive CAN messages
	uint16_t NumberOfMessages;					//Number of messages to be transmitted
	_Bool ContinueFlag = false;				    //Flag to continue or not


	//Set device ID according to input argument
	pTxHeader.StdId = DeviceID;

	//Figure out how many messages will be sent (must be 16 bits or less)
	NumberOfMessages = ArraySize/6;
	if(ArraySize%6 != 0){
		NumberOfMessages=NumberOfMessages+1;
	}else{
		;
	}


	//Create initiation message
	TransmitMessage[0] = 0x00;
	TransmitMessage[1] = 0x00;
	uint16_to_bytes(ComandBytes,      &TransmitMessage[2]);
	uint16_to_bytes(NumberOfMessages, &TransmitMessage[4]);

	//Send initiation message over CAN
	ret = HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, TransmitMessage, &TxMailbox);	  //Transmit first message over CAN
    if ( ret != HAL_OK ) {
    	return 0xBA;
    }


	//Wait for acknowledge from targeted device
	HAL_TIM_Base_Start(&TIMER_TIMEOUT);
	TimerRefValue = __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT);
	while( ContinueFlag == false){
		if( __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT) - TimerRefValue >= Timeout ){	 	//If time out occurs, reset timer, return error code
			HAL_TIM_Base_Stop(&TIMER_TIMEOUT);
			TIMER_TIMEOUT.Instance -> CNT = 0;
			return 0xBB;
		}
		if( HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO0) == 1 ){
			ContinueFlag = true;
		}
	}
	HAL_TIM_Base_Stop(&TIMER_TIMEOUT);			//Stop timer
	TIMER_TIMEOUT.Instance -> CNT = 0;			//Reset timer


	//Receive acknowledgment from FIFO
	HAL_CAN_GetRxMessage(&CANBUS_EXT, CAN_RX_FIFO0, &pRxHeader, ReceivedMessage);
    if ( ret != HAL_OK ) {
    	return 0xBC;
    }

    //Check acknowledgment message for consistency
	uint16_t MessageID = bytes_to_uint16(&ReceivedMessage[0]);	 //Get ACK message ID
	uint16_t ACK = bytes_to_uint16(&ReceivedMessage[2]);	     //Get ACK message data

	if( (MessageID==0x00) && (ACK==0xABBA) ){
		ContinueFlag = true;
	}else{
		ContinueFlag = false;
	  	return 0xBD;
	}

	//Algorithm to write the rest of the messages in the array
	for(uint16_t MessageIteration = 0; MessageIteration < NumberOfMessages; MessageIteration++ ){

		//Loop variable setup
		memset(TransmitMessage, 0 , sizeof(TransmitMessage));	    //Set array to 0
		ContinueFlag = false;										//Set continue flag to false
		MessageID = 0;
		ACK = 0;

		//Create transmit message from input data array
		uint16_to_bytes(MessageIteration+1, TransmitMessage);			//Write message number to first 2 bytes (MessageIteration starts at 0 so +1 increment is needed)
		for(int i=0; i<6; i++){											//Fill rest of message with data from input array
			int InputArrayIndex = MessageIteration*6+i;
			if(InputArrayIndex < ArraySize){
				TransmitMessage[i+2] = InputDataArray[MessageIteration*6+i];
			}else{
				TransmitMessage[i+2] = 0;
			}
		}

		//Transmit message over CAN
		ret=HAL_CAN_AddTxMessage(&CANBUS_EXT, &pTxHeader, TransmitMessage, &TxMailbox);
	    if ( ret != HAL_OK ) {
	    	return 0xBA;
	    }


		//Wait for acknowledge from targeted device
		HAL_TIM_Base_Start(&TIMER_TIMEOUT);
		TimerRefValue = __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT);
		while( ContinueFlag == false){
			if( __HAL_TIM_GET_COUNTER(&TIMER_TIMEOUT) - TimerRefValue >= Timeout ){	 	//If time out occurs, reset timer, return error code
				HAL_TIM_Base_Stop(&TIMER_TIMEOUT);
				TIMER_TIMEOUT.Instance -> CNT = 0;
				return 0xBB;
			}
			if( HAL_CAN_GetRxFifoFillLevel(&CANBUS_EXT, CAN_RX_FIFO0) == 1 ){
				ContinueFlag = true;
			}
		}
		HAL_TIM_Base_Stop(&TIMER_TIMEOUT);			//Stop timer
		TIMER_TIMEOUT.Instance -> CNT = 0;			//Reset timer


		//Receive acknowledgment from FIFO
		ret = HAL_CAN_GetRxMessage(&CANBUS_EXT, CAN_RX_FIFO0, &pRxHeader, ReceivedMessage);
	    if ( ret != HAL_OK ) {
	    	return 0xBC;
	    }

	    //Check acknowledgment message for consistency
		MessageID = bytes_to_uint16(&ReceivedMessage[0]);	 //Get ACK message ID
		ACK = bytes_to_uint16(&ReceivedMessage[2]);	    	 //Get ACK message data

		if( (MessageID == (MessageIteration+1) ) && (ACK==0xABBA) ){
			ContinueFlag = true;
		}else{
			ContinueFlag = false;
		  	return 0xBD;
		}

	} //Return to top of FOR loop

	return 0xBF;

} //End CANTransmitRoutine function


