/* Private includes ----------------------------------------------------------------------------*/
#include <SystemRoutines/SystemInitialization.h>

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern TIM_HandleTypeDef  TIMER_HRT;
//CAN_HandleTypeDef CANBUS_EXT;
//TIM_HandleTypeDef TIMER_TIMEOUT;

/* External Struct Declarations ----------------------------------------------------------------*/
extern sysinfo SysInfo;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;
extern HAL_StatusTypeDef ret;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// System Initialization and Activation Routine ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


_Bool SystemActivation(void){

	_Bool SystemActivationFlag = true;

	//////////////////////////////////////////////////////////
	//////////// INITIAL PRINT MESSAGE ///////////////////////
	//////////////////////////////////////////////////////////

	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1,"\n\n\rStarting subsystem activation sequence:"  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	////////////////////////////////////////////////////////////////
	/////////// Activate all system timers ///////////////////////
	////////////////////////////////////////////////////////////////

	//Start heartbeat timer
	ret = HAL_TIM_Base_Start_IT(&TIMER_HRT);
	if(ret == HAL_OK){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1,"\n\r\t- Heartbeat timer started"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1,"\n\r\t- Heartbeat timer failed to start"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		SystemActivationFlag = false;
	}


	//////////////////////////////////////////////////////////////////////////////
	/////////////// Initialize all outputs to a known state //////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//Set all output pins to the correct state
	Set_LED_HeartBeat(0);
	Set_LED_Indication1(0);
	Set_LED_Indication2(0);
	Set_LED_Indication3(0);
	Set_LED_Indication4(0);
	Set_LED_Indication5(0);
	Set_LED_Metal(0);
	Set_LED_Turbidity(0);
	Set_Flash_CS(1);
	Set_Flash_WP(1);

	//Print sequence load message
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1,"\n\r\t- All output pins set"  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);


	//////////////////////////////////////////////////////////////////////////////
	/////////////// Initialize MCP3421 ADC IC  ///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//Print sequence load message
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer,COMM_SIZE-1, "\n\rInitializing MCP3421 ADC: "  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Reset MCP3421 ADC to a known state
	if( MCP3421_ADC_Reset() == ADC_STATUS_OK ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ACD Reset Passed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ACD Reset Failed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		SystemActivationFlag = false;
	}

	//Configure the MCP3421 ADC's settings
	uint8_t ADC_Config = CONV_ONESHOT | SAMPLE_16BIT | PGA_X1;
	if( MCP3421_ADC_Init(ADC_Config) == ADC_STATUS_OK ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ADC Config Passed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ADC Config Failed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		SystemActivationFlag = false;
	}


	//////////////////////////////////////////////////////////////////////////////
	/////////////// Initialize MCP4726A1T_E DAC IC  //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//Print sequence load message
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer,COMM_SIZE-1, "\n\rInitializing MCP4726A1T_E DAC: "  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//DAC initialization variables
	_Bool EEPROM_ProgramFlag = false;
	uint8_t DAC_ReadBuffer[6] = {0xFF};

	//Read DAC memory to check expected configuration settings
	if( MCP4726A1T_ReadAllMemory(DAC_ReadBuffer) == DAC_STATUS_OK ){

		//Check eeprom config data
		if( (DAC_ReadBuffer[3]&0xDE) == 0xDC ){
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM config verified");
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		}else{
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM config INVALID -> 0x%02X", DAC_ReadBuffer[3] );
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
			EEPROM_ProgramFlag = true;
		}

		//Check volatile config data
		if( (DAC_ReadBuffer[0]&0xDE) == 0xD8 ){
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Volatile memory config verified");
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		}else{
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Volatile memory config INVALID -> 0x%02X", DAC_ReadBuffer[0] );
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
			EEPROM_ProgramFlag = true;
		}

	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- System Read Error during memory verification");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		SystemActivationFlag = false;
	}


	//If DAC config settings are invalid, program all memory
	if( EEPROM_ProgramFlag == true ){
		if(	MCP4726A1T_WriteAllMemory(VREF_BUFF, PD_100K, G_1X, 0 ) != DAC_STATUS_OK ){
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Memory cofig program Failed");
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
			SystemActivationFlag = false;
		}
	}


	//If DAC config has been reprogrammed, re-check the config data
	if( EEPROM_ProgramFlag == true ){

		//Read newly programmed memory from DAC
		memset(DAC_ReadBuffer, 0xFF, 8);
		if( MCP4726A1T_ReadAllMemory(DAC_ReadBuffer) == DAC_STATUS_OK ){

			//Check eeprom config data
			if( (DAC_ReadBuffer[3]&0xDE) == 0xDC ){
				memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM config re-check verified");
				DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
			}else{
				memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM config re-check failed -> 0x%02X", DAC_ReadBuffer[3] );
				DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
				SystemActivationFlag = false;
			}

			//Check volatile config data
			if( (DAC_ReadBuffer[0]&0xDE) == 0xD8 ){
				memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Volatile memory config re-check verified");
				DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
			}else{
				memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Volatile memory config re-check INVALID -> 0x%02X", DAC_ReadBuffer[0] );
				DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
				SystemActivationFlag = false;
			}

		}else{
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC System Read Error during memory re-check verification");
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
			SystemActivationFlag = false;
		}
	}


	//Check POR bit to ensure that the DAC is awake
	if( (DAC_ReadBuffer[0]&0x80) == 0x80 ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC successfully powered on");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC failed to powered on");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		SystemActivationFlag = false;
	}


	//Set the DAC output voltage to 0V so it is in a known state
	if( MCP4726A1T_VoltageOut(0) == DAC_STATUS_OK ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC output voltage reset to know state");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC output voltage failed to reset");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
		SystemActivationFlag = false;
	}



	//////////////////////////////////////////////////////////////////////////////
	/////////////// Load System Information from EEPROM  /////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//Print sequence load message
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer,COMM_SIZE-1, "\n\rInitializing system information from memory: "  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);


	//Read data from the EEPROM IC
	uint8_t EEPROM_ReadArray[7] = {0};
	if( eeprom_Read(0x0000, EEPROM_ReadArray, sizeof(EEPROM_ReadArray) ) == EEPROM_STATUS_OK ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer,COMM_SIZE-1, "\n\r\t- Information successfully retrieved from EEPROM"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer,COMM_SIZE-1, "\n\r\t- Information failed to be retrieved from EEPROM"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		SystemActivationFlag = false;
	}

	//Set system info
	SysInfo.Button1     = false;
	SysInfo.Button2     = false;
	SysInfo.Button3     = false;
	SysInfo.SerialNumber   = bytes_to_uint16(&EEPROM_ReadArray[0]);
	SysInfo.SensorType     = EEPROM_ReadArray[2];
	SysInfo.CAN_Address    = bytes_to_uint16(&EEPROM_ReadArray[3]);
	SysInfo.EEPROM_Address = bytes_to_uint16(&EEPROM_ReadArray[5]);

	//Verify that data read from EEPROM is valid
	if(SysInfo.SerialNumber==0xFFFF || SysInfo.SensorType==0xFF || SysInfo.CAN_Address==0xFFFF || SysInfo.EEPROM_Address==0xFFFF ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM data is invalid -> Memory Must be Programmed"  );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Serial Number:\t0x%04X", SysInfo.SerialNumber );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- CAN Address:\t0x%04X", SysInfo.CAN_Address );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Sensor Type:\t%u", SysInfo.SensorType);
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Address:\t0x%04X", SysInfo.EEPROM_Address );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		SystemActivationFlag = false;
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM data is valid");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}



	//////////////////////////////////////////////////////////////////////////////
	/////////////// Display Sensor Configuration on Indication LEDs //////////////
	//////////////////////////////////////////////////////////////////////////////

	//Set indication lights to show sensor type
	if(SysInfo.SensorType>15){
		Set_LED_Indication5(1);
		Set_LED_Indication4(1);
		Set_LED_Indication3(1);
		Set_LED_Indication2(1);
	}else{
		Set_LED_Indication5( (SysInfo.SensorType>>3)&0x01 );
		Set_LED_Indication4( (SysInfo.SensorType>>2)&0x01 );
		Set_LED_Indication3( (SysInfo.SensorType>>1)&0x01 );
		Set_LED_Indication2( (SysInfo.SensorType>>0)&0x01 );
	}




	return SystemActivationFlag;
}


_Bool InitSystemStruct(void){
	;
	return true;
}


