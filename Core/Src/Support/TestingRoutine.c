/* Private includes ----------------------------------------------------------------------------*/
#include "./Support/TestingRoutine.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern CAN_HandleTypeDef CANBUS_EXT;

/* External Struct Declarations ---------------------------------------------------------------*/
extern sysinfo SysInfo;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;

extern HAL_StatusTypeDef ret;

extern CAN_TxHeaderTypeDef pTxHeader0;			//CAN transmit properties struct
extern CAN_RxHeaderTypeDef pRxHeader;			//CAN receive properties struct
extern CAN_FilterTypeDef sFilterConfig;			//CAN filter properties struct
extern uint32_t TxMailbox;						//CAN Mailbox
extern uint8_t  TransmitMessage[8];				//Array to send CAN messages
extern uint8_t  ReceivedMessage[8];				//Array to receive CAN messages



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// PERIPHERAL TESTING ROUTINE ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TestingRoutine(void){

	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r~~~~~~ System Testing Routine ~~~~~~~~~~~"  );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	_Bool TestFlag;

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// EEPROM IC Validation Test //////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//Test variable declaration
	eeprom_status_t eeprom_status;
	uint8_t EEPROM_ReadArray[16];
	uint8_t EEPROM_WriteArray[16];


	//EEPROM IC Validation test message
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEEPROM IC Validity Test:"  );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	//Set values of test page 0x7F0 to factory values of 0xFF
	memset(EEPROM_WriteArray, 0xFF, 16);
	eeprom_status=eeprom_Write(0x07F0, EEPROM_WriteArray, 16);
	if(eeprom_status==EEPROM_STATUS_OK){
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Reset Write Successful"  );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Reset Write Failed -> 0x%02X", eeprom_status );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}

	//Read the reset test page values to ensure it was successful
	memset(EEPROM_ReadArray, 0x00, 16);
	eeprom_status=eeprom_Read(0x07F0, EEPROM_ReadArray, 16);
	if(eeprom_status==EEPROM_STATUS_OK){
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Reset Read Successful"  );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	    //Check that the read buffer values are accurate
	    TestFlag = true;
	    for(int i=0; i<16; i++){
	    	if(EEPROM_ReadArray[i]!=0xFF){ TestFlag = false; }
	    }

	    if(TestFlag==true){
		 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Reset Read Array Value VALID"  );
		    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	    }else{
		 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Reset Read Array Value INVALID"  );
		    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	    }

	}else{
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Reset Read Failed - 0x%02X", eeprom_status );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}


	//Write 16 bytes to the test page in decrementing values to test write operations
	memset(EEPROM_WriteArray, 0x00, 16);
	for(int i=0; i<16; i++){ EEPROM_WriteArray[i] = 15-i; }
	eeprom_status=eeprom_Write(0x07F0, EEPROM_WriteArray, 16);
	if(eeprom_status==EEPROM_STATUS_OK){
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Write Successful"  );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Write Failed -> 0x%02X", eeprom_status );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}


	//Read the test page values to ensure it was successful
	memset(EEPROM_ReadArray, 0x00, 16);
	eeprom_status=eeprom_Read(0x07F0, EEPROM_ReadArray, 16);
	if(eeprom_status==EEPROM_STATUS_OK){
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Read Successful"  );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	    //Check that the read buffer values are accurate
	    TestFlag = true;
	    for(int i=0; i<16; i++){
	    	if(EEPROM_ReadArray[i]!=EEPROM_WriteArray[i]){ TestFlag = false; }
	    }

	    if(TestFlag==true){
		 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Read Array Value VALID"  );
		    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	    }else{
		 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Read Array Value INVALID"  );
		    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	    }

	}else{
	 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- EEPROM Test Page Read Failed -> 0x%02X", eeprom_status );
	    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Flash Memory IC Validation Test ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//Test variable declaration
	uint8_t SPIReadArray[1];

	//Flash Memory IC Validation test message
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rFlash Memory IC Validity Test:"  );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	//Flash Memory Test 1 - Test that SR1 is in default state of 0x00
	Flash_WriteStatusRegister(1, 0x00); 			//Ensure that SRP bit is 0
	Flash_ReadStatusRegister(1, SPIReadArray);
	if(SPIReadArray[0]==0x00){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 1 PASSED -> SR1 is 0x%02X", SPIReadArray[0] );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else if(SPIReadArray[0]==0xFF){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 1 FAILED -> HOLD line asserted - Tests 2, 3, 4 are invalid");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 1 FAILED -> SR1 is 0x%02X - Tests 2, 3, 4 are invalid", SPIReadArray[0] );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}


	//Flash Memory Test 2 - Test SDO, SDI and CS by enabling SRP in SR1
	Flash_WriteStatusRegister( 1, 0x80);
	Flash_ReadStatusRegister( 1, SPIReadArray);
	if(SPIReadArray[0]==0x80){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 2 PASSED -> SRP Enabled");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else if(SPIReadArray[0]==0xFF){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 2 FAILED -> HOLD line asserted");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 2 FAILED -> Data read/write issue");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

	//Flash Memory Test 3 - Test WP by attempting to disable SRP in SR1
	Set_Flash_WP(0);
	Flash_WriteStatusRegister( 1, 0x00);
	Flash_ReadStatusRegister(1, SPIReadArray);
	if(SPIReadArray[0]==0x80){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 3 PASSED -> WP Enabled");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else if(SPIReadArray[0]==0x00){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 3 FAILED -> WP Disabled");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory Test 3 FAILED -> Unknown Error");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}
	Set_Flash_WP(1);


	//Return the SR1 of the flash memory chips to 0x00
	Flash_WriteStatusRegister( 1, 0x00);
	Flash_ReadStatusRegister( 1, SPIReadArray);
	if(SPIReadArray[0]==0x00){
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory -> Returned to default");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Flash Memory -> Failed to returned to default");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// ADC IC Comm Validation Test ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//MCP3421 ADC IC Validation test message
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rMCP3421 ADC Validity Test:"  );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

    //Attempt to reset ADC to POR
    if( MCP3421_ADC_Reset() == ADC_STATUS_OK ){
    	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ADC Reset Passed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
    	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ADC Reset Failed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}

	//Configure the MCP3421 ADC's settings
	uint8_t ADC_Config = CONV_ONESHOT | SAMPLE_16BIT | PGA_X1;
	if( MCP3421_ADC_Init(ADC_Config) == ADC_STATUS_OK ){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ADC Config Passed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- ADC Config Failed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	}



	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// DAC IC Validation Test /////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//MCP3421 ADC IC Validation test message
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rMCP4726A1T_E DAC Validity Test:"  );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

    //Test variables
    dac_status_t DAC_Status;
	uint8_t ReadBuffer[6] = {0x00};
	uint8_t CompareBuffer[6] = {0xDC, 0x0C, 0x80, 0xFC, 0x0C, 0x80};

    //Program eeprom and volatile memory to be in a known and power-down state
	DAC_Status = MCP4726A1T_WriteAllMemory(VREF_BUFF, PD_100K, G_1X, 200 );
	if(DAC_Status != DAC_STATUS_OK){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC Write All Fail -> Status 0x%02X", DAC_Status );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC Write All Success");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

	//Read eeprom and volatile memory back to verify its contents
	DAC_Status = MCP4726A1T_ReadAllMemory(ReadBuffer);
	if(DAC_Status != DAC_STATUS_OK){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC Read All Fail -> Status 0x%02X", DAC_Status );
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC Read All Success");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}

	//Perform memory verification
	_Bool DAC_TestFlag = true;
	for(int i=0; i<6; i++){
		if(ReadBuffer[i]!=CompareBuffer[i]){DAC_TestFlag=false;}
	}

	//If there is a memory mismatch, print out mismatch contents
	if(DAC_TestFlag==true){
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC Memory Compare Test Passed");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	}else{
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- DAC Memory Compare Test FAILED");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t  Memory Dump:");
		DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
		for(int i=0; i<6; i++){
			if(ReadBuffer[i]==CompareBuffer[i]){
				memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t\tPos[%d]: 0x%02X == 0x%02X", i, ReadBuffer[i], CompareBuffer[i] );
				DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
			}else{
				memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t\tPos[%d]: 0x%02X != 0x%02X", i, ReadBuffer[i], CompareBuffer[i] );
				DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
			}
		}
	}



    /////////////////////////////////////////////////////////////////////////////////////////
	//////////////// User Button Validation Test ////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//Test that user BUTTON 1 works
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rPress BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\rBUTTON 1 activated" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	SysInfo.Button1 = false;

	//Test that user BUTTON 2 works
	SysInfo.Button2 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rPress BUTTON 2 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button2 != true );
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\rBUTTON 2 activated" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	SysInfo.Button2 = false;

	//Test that user BUTTON 3 works
	SysInfo.Button3 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rPress BUTTON 3 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button3 != true );
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\rBUTTON 3 activated" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	SysInfo.Button3 = false;


	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// LED Validation Test ////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//Test that blue LED is on
	Set_LED_HeartBeat(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that BLUE LED is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_HeartBeat(false);

	//Test indication LED 1
	Set_LED_Indication1(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that RED LED is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Indication1(false);

	//Test indication LED 2
	Set_LED_Indication2(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that ORANGE LED 1 is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Indication2(false);

	//Test indication LED 3
	Set_LED_Indication3(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that ORANGE LED 2 is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Indication3(false);

	//Test indication LED 4
	Set_LED_Indication4(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that ORANGE LED 3 is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Indication4(false);

	//Test indication LED 5
	Set_LED_Indication5(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that ORANGE LED 4 is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Indication5(false);

	//Test Sensor 1 indication LED
	Set_LED_Metal(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that GREEN Sensor 1 Indication LED is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Metal(false);

	//Test Sensor 2 indication LED
	Set_LED_Turbidity(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that GREEN Sensor 2 Indication LED is ON and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	Set_LED_Turbidity(false);

	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// Sensor LED Control and Activation Test /////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//Activate Sensor 1 LED at 20mA
	MCP4726A1T_WriteVolatileDACRegister(PD_NO, 400 );
	Set_LED_Turbidity(false);
	Set_LED_Metal(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that Sensor 1 LED is being driven at 20mA and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	SysInfo.Button1 = false;
	Set_LED_Metal(false);

	//Activate Sensor 1 LED at 40mA
	MCP4726A1T_WriteVolatileDACRegister(PD_NO, 800 );
	Set_LED_Turbidity(false);
	Set_LED_Metal(true);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that Sensor 1 LED is being driven at 40mA and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	SysInfo.Button1 = false;
	Set_LED_Metal(false);

	//Activate Sensor 2 LED at 20mA
	MCP4726A1T_WriteVolatileDACRegister(PD_NO, 400 );
	Set_LED_Turbidity(true);
	Set_LED_Metal(false);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that Sensor 2 LED is being driven at 20mA and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	SysInfo.Button1 = false;
	Set_LED_Turbidity(false);


	//Activate Sensor 2 LED at 40mA
	MCP4726A1T_WriteVolatileDACRegister(PD_NO, 800 );
	Set_LED_Turbidity(true);
	Set_LED_Metal(false);
	SysInfo.Button1 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rEnsure that Sensor 2 LED is being driven at 40mA and press BUTTON 1 to continue" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );
	while( SysInfo.Button1 != true );
	SysInfo.Button1 = false;
	Set_LED_Turbidity(false);



	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////// ADC IC Validation Test /////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//Print test instructions
	SysInfo.Button1 = false;
	SysInfo.Button2 = false;
 	memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rPress BUTTON 1 to take sample or press BUTTON 2 to leave test" );
    DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

    //Perform test until Button 2 is pressed
    float ADC_Value;
	while( SysInfo.Button2 != true ){

		if(SysInfo.Button1 == true){
			SysInfo.Button1 = false;

			MCP3421_ADC_ReadVoltage(&ADC_Value);
			ADC_Value=ADC_Value*2;
			memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r\t- Voltage Value is: %0.4f V", ADC_Value );
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

		}

	}






	////////////////////////////////////////////
	/////////// Leave Test Routine  ////////////
	////////////////////////////////////////////

	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rPress Button 1 to LEAVE testing mode"  );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	while(SysInfo.Button1 == false);
	SysInfo.Button1 = false;






}
