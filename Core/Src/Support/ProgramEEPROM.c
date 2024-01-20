/* Private includes ----------------------------------------------------------------------------*/
#include "./Support/ProgramEEPROM.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  EEPROM Program Routines //////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Function to write all specified data to the EEPROM IC
void ProgramEEPROM_All(void){

	uint8_t EEPROM_ReadArray[7] = {0};
	uint8_t EEPROM_WriteArray[7] = {0};

	uint16_t  SerialNumber   = 0x001A;
	uint16_t  CAN_Address    = 0x001A;
	uint8_t   SensorType     = 0;
	uint16_t  EEPROM_Address = 0x0007;

	//Print start message
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rMANAUL EEPROM PROGRAM STARTED");
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

	//Create data array from specified information
	uint16_to_bytes(SerialNumber, &EEPROM_WriteArray[0]);
	EEPROM_WriteArray[2] = SensorType;
	uint16_to_bytes(CAN_Address, &EEPROM_WriteArray[3]);
	uint16_to_bytes(EEPROM_Address, &EEPROM_WriteArray[5]);

	//Write data to the EEPROM IC
	eeprom_Write(0x0000, EEPROM_WriteArray, sizeof(EEPROM_WriteArray) );

	//Read data to the EEPROM IC
	eeprom_Read(0x0000, EEPROM_ReadArray, sizeof(EEPROM_ReadArray) );


	//Print retrieved information
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rData retrieved from EEPROM:");
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    Serial Number:\t0x%04X", bytes_to_uint16(&EEPROM_ReadArray[0]) );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    Sensor Type:\t%u", EEPROM_ReadArray[2] );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    CAN Address:\t0x%04X", bytes_to_uint16(&EEPROM_ReadArray[3]) );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    EEPROM Address:\t0x%04X", bytes_to_uint16(&EEPROM_ReadArray[5]) );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

}


//Function to write CANbus Addressing Information
void ProgramEEPROM_CAN(void){

	uint8_t   EEPROM_ReadArray[2] = {0};
	uint8_t   EEPROM_WriteArray[2] = {0};
	uint16_t  CAN_ReadNum 	 = 0;
	uint16_t  CAN_Address    = 0x0007;


	//Write CAN address data to the EEPROM chip
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rWritting CAN Address 0x%04X to EEPROM", CAN_Address );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	uint16_to_bytes(CAN_Address, EEPROM_WriteArray);
	eeprom_Write(0x0003, EEPROM_WriteArray, sizeof(EEPROM_WriteArray) );

	//Recall written CAN address from memory and display to console
	eeprom_Read(0x0003, EEPROM_ReadArray, sizeof(EEPROM_ReadArray) );
	CAN_ReadNum = bytes_to_uint16(EEPROM_ReadArray);
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rReading CAN Address 0x%04X from EEPROM", CAN_ReadNum );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);


}



//Function to write CANbus Addressing Information
void ProgramEEPROM_SensorType(void){

	uint8_t   EEPROM_ReadArray[1] = {0};
	uint8_t   SensorType    	  = 1;

	//Write CAN address data to the EEPROM chip
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rWritting Sensor Type 0x%04X to EEPROM", SensorType );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	eeprom_Write(0x0002, &SensorType, 1 );

	//Recall written CAN address from memory and display to console
	eeprom_Read(0x0002, EEPROM_ReadArray, sizeof(EEPROM_ReadArray) );
	memset(PrintBuffer, '\0', COMM_SIZE); snprintf(PrintBuffer, COMM_SIZE-1, "\n\rReading Sensor Type 0x%04X from EEPROM", EEPROM_ReadArray[0] );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);


}

