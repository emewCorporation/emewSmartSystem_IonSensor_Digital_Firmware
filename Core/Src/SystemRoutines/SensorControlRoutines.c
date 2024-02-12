/* Private includes ----------------------------------------------------------------------------*/
#include "./SystemRoutines/SensorMeasurementRoutine.h"

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

//static const float OffsetVoltage
static const float ControlResistor = 10;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Ion Measurement and Transmit Routines //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Routine to measure data from the two sensors and send it to the controller
void Transimpedance_TakeMeasurement(float* VoltageReturn, uint8_t SampleNumber){

}



void SensorLED_ActivationControl(uint8_t SensorType, uint8_t SensorState, float SetCurrent ){








}
