/* Private includes ----------------------------------------------------------------------------*/
#include "./PrimaryStateMachine.h"

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;

/* External Struct Declarations ----------------------------------------------------------------*/
extern sysinfo SysInfo;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;
extern uint8_t PrimaryState;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  System State Machines /////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Setup for transmitting CANbus
void PrimaryStateMachine(void){

	switch(PrimaryState){

		//Standby - Do Nothing
		case 0x00:
			break;

		//Pairing Routine
		case 0x01:
			ControllerPairRoutine();
			PrimaryState=0x00;
			break;

		//Connection Ping
		case 0x02:
			ControllerPingRoutine();
			PrimaryState=0x00;
			break;

		//Sensor read
		case 0x03:
			SensorMeasurementRoutine();
			PrimaryState=0x00;
			break;

		//Fail case for unimplemented states
		default:
			memset(PrintBuffer, '\0', COMM_SIZE);  snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rInvalid Instruction Received - Transitioning to Standby State"  );
			DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE);
			PrimaryState=0x00;

	}  //End switch statement


}
