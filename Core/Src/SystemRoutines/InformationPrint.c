/* Private includes ----------------------------------------------------------------------------*/
#include "./SystemRoutines/InformationPrintRoutines.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;

/* External Struct Declarations ----------------------------------------------------------------*/
extern sysinfo SysInfo;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Functions to Print System Information to Debug Console  //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Function to print the startup splash message
void StartupPrint_Routine(void){

	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\r\n\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r~~~~~~~~~~ emewCorporation Electrolyte Monitoring System ~~~~~~~~~~" );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r~~~~~~~~~~                   Ion Sensor                  ~~~~~~~~~~" );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

}


//Function to print the sensor information retrieved from the EEPROM IC
void SystemSettingPrint_Routine(void){

	//Print retrieved information
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rSystem Information Retrieved:");
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - Serial Number:\t0x%04X", SysInfo.SerialNumber );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - CAN Address:\t0x%04X", SysInfo.CAN_Address );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - Sensor Type:\t%u", SysInfo.SensorType);
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
	memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\r    - EEPROM Address:\t0x%04X", SysInfo.EEPROM_Address );
	DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);

}
