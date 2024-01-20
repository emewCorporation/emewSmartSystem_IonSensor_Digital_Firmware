/* Private includes ----------------------------------------------------------------------------*/
#include <Peripheral_Libraries/MCP4726A1T_E.h>

/* #defines for local functions ----------------------------------------------------------------*/
#define DAC_MEMORY_ALL		0
#define DAC_MEMORY_VOLATILE 1

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern I2C_HandleTypeDef I2C_SYS;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;

/* Global Static Variable Declaration ----------------------------------------------------------*/
static HAL_StatusTypeDef hal_i2c_status;

/* Private Global Variables --------------------------------------------------------------------*/
static const uint8_t I2C_GeneralCall	= 0x00;		//I2C General Call Address
static const uint8_t DeviceAddr      	= 0x61;		//External address of the DAC IC

/* Private Function Prototypes ----------------------------------------------------------------*/
dac_status_t MCP4726A1T_WriteMemory(uint8_t MemoryType, uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit, uint16_t DAC_Data);

//Datasheet URL: "https://ww1.microchip.com/downloads/en/DeviceDoc/22272C.pdf"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  High Level DAC Control Functions //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Set the output voltage of the DAC
//Volatile config must have gain = 1 and voltage reference = VREF or results will be unexpected
dac_status_t MCP4726A1T_VoltageOut(float VoltageOutput){

	//Check passed parameters for invalid information
	if(VoltageOutput < 0 || VoltageOutput > 2.048){ return DAC_ERROR_PARAMETER; }

	//Local function variables
	const float VoltageReference	 = 2.048;
	const uint16_t ResistorsInLadder = 4096;
	const uint16_t Gain = 1;

	//Calculate the required DAC register value
	uint16_t DAC_RegValue = (uint16_t) ( (VoltageOutput*ResistorsInLadder)/(Gain*VoltageReference) );

	//Write the DAC register value
	return MCP4726A1T_WriteVolatileDACRegister(PD_NO, DAC_RegValue);

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  Private DAC Memory Access Functions ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Function to perform a generic memory write to either ALL memory or VOLATILE memory
dac_status_t MCP4726A1T_WriteMemory(uint8_t MemoryType, uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit, uint16_t DAC_Data){

	//Check passed parameters for invalid information
	if(MemoryType != DAC_MEMORY_ALL && MemoryType != DAC_MEMORY_VOLATILE){ return DAC_ERROR_PARAMETER; }
	if(VoltRefBits != VDD_UNBUFF && VoltRefBits != VREF_UNBUFF && VoltRefBits != VREF_BUFF ){ return DAC_ERROR_PARAMETER; }
	if(PowerDownBits != PD_NO && PowerDownBits != PD_1K && PowerDownBits != PD_100K && PowerDownBits != PD_500K ){ return DAC_ERROR_PARAMETER; }
	if(GainBit != G_1X && GainBit != G_2X){ return DAC_ERROR_PARAMETER; }
	if(DAC_Data>0x0FFF){ return DAC_ERROR_PARAMETER; }

	//Combined the I2C general call command and the device address information (shift device address by 1 for r/w bit)
	uint16_t I2C_DeviceAddress = (I2C_GeneralCall<<8) | (DeviceAddr<<1);

	//Determine command
	uint8_t MemoryTypeCMD;
	if(MemoryType==0){ MemoryTypeCMD = 0x60; }		//Command for ALL memory
	else{ MemoryTypeCMD = 0x40;}					//Command for VOLATILE memory

	//Format the passed DAC data to big endian
	uint8_t DAC_Data_BigEndian[2];
	DAC_Data = DAC_Data&0x0FFF;
	DAC_Data_BigEndian[0] = (uint8_t)((DAC_Data>>8)&0x00FF);
	DAC_Data_BigEndian[1] = (uint8_t)(DAC_Data&0x00FF);

	//Create a data payload containing the write data
	uint8_t WriteBuffer[3];
	WriteBuffer[0] = MemoryTypeCMD | (VoltRefBits&0x03)<<3 | (PowerDownBits&0x03)<<1 | (0x01&GainBit);
	WriteBuffer[1] = ((DAC_Data_BigEndian[0]<<4)&0xF0) | ((DAC_Data_BigEndian[1]>>4)&0x0F);
	WriteBuffer[2] = ((DAC_Data_BigEndian[1]<<4)&0xF0);

	//Write the buffer data to the DAC IC
	hal_i2c_status = HAL_I2C_Master_Transmit(&I2C_SYS, I2C_DeviceAddress, WriteBuffer, 3, 100);
	if( hal_i2c_status != HAL_OK ){ return DAC_ERROR_I2C_WRITE; }

	//Wait 50ms for max EEPROM programming time
	HAL_Delay(50);

	//Return success code
	return DAC_STATUS_OK;

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  I2C DAC Communication Functions  //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Function to perform a volatile memory write operation
dac_status_t MCP4726A1T_WriteVolatileMemory(uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit, uint16_t DAC_Data){

	return MCP4726A1T_WriteMemory( DAC_MEMORY_VOLATILE, VoltRefBits, PowerDownBits, GainBit, DAC_Data);

}

//Function to perform a write all memory operation
dac_status_t MCP4726A1T_WriteAllMemory(uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit, uint16_t DAC_Data){

	return MCP4726A1T_WriteMemory( DAC_MEMORY_ALL, VoltRefBits, PowerDownBits, GainBit, DAC_Data);

}



//Function to perform a volatile DAC data register write
dac_status_t MCP4726A1T_WriteVolatileDACRegister(uint8_t PowerDownBits, uint16_t DAC_Data){

	//Check passed parameters for invalid information
	if(PowerDownBits != PD_NO && PowerDownBits != PD_1K && PowerDownBits != PD_100K && PowerDownBits != PD_500K ){ return DAC_ERROR_PARAMETER; }
	if(DAC_Data>0x0FFF){ return DAC_ERROR_PARAMETER; }

	//Combined the I2C general call command and the device address information (shift device address by 1 for r/w bit)
	uint16_t I2C_DeviceAddress = (I2C_GeneralCall<<8) | (DeviceAddr<<1);

	//Format the passed DAC data to big endian
	uint8_t DAC_Data_BigEndian[2];
	DAC_Data = DAC_Data&0x0FFF;
	DAC_Data_BigEndian[0] = (uint8_t)((DAC_Data>>8)&0x00FF);
	DAC_Data_BigEndian[1] = (uint8_t)(DAC_Data&0x00FF);

	//Create a data payload containing the write data
	uint8_t WriteBuffer[2];
	WriteBuffer[0] = 0x00 | (PowerDownBits&0x03)<<4 | (DAC_Data_BigEndian[0]&0x0F);
	WriteBuffer[1] = DAC_Data_BigEndian[1];

	//Write the buffer data to the DAC IC
	hal_i2c_status = HAL_I2C_Master_Transmit(&I2C_SYS, I2C_DeviceAddress, WriteBuffer, 2, 100);
	if( hal_i2c_status != HAL_OK ){	return DAC_ERROR_I2C_WRITE;	}

	//Wait 50ms for max EEPROM programming time
	HAL_Delay(50);

	//Return success code
	return DAC_STATUS_OK;

}



//Function to change the configuration settings in volatile memory
dac_status_t MCP4726A1T_WriteVolatileConfiguration(uint8_t VoltRefBits, uint8_t PowerDownBits, uint8_t GainBit){

	//Check passed parameters for invalid information
	if(VoltRefBits != VDD_UNBUFF && VoltRefBits != VREF_UNBUFF && VoltRefBits != VREF_BUFF ){ return DAC_ERROR_PARAMETER; }
	if(PowerDownBits != PD_NO && PowerDownBits != PD_1K && PowerDownBits != PD_100K && PowerDownBits != PD_500K ){ return DAC_ERROR_PARAMETER; }
	if(GainBit != G_1X && GainBit != G_2X){ return DAC_ERROR_PARAMETER; }

	//Combined the I2C general call command and the device address information (shift device address by 1 for r/w bit)
	uint16_t I2C_DeviceAddress = (I2C_GeneralCall<<8) | (DeviceAddr<<1);

	//Create a data payload containing the write data
	uint8_t WriteByte = 0x80 | (VoltRefBits&0x03)<<3 | (PowerDownBits&0x03)<<1 | (0x01&GainBit);

	//Write the buffer data to the DAC IC
	hal_i2c_status = HAL_I2C_Master_Transmit(&I2C_SYS, I2C_DeviceAddress, &WriteByte, 1, 100);
	if(hal_i2c_status != HAL_OK ){ return DAC_ERROR_I2C_WRITE; }

	//Wait 50ms for max EEPROM programming time
	HAL_Delay(50);

	//Return success code
	return DAC_STATUS_OK;

}


//Function to read data from eeprom and volatile DAC memory
//Read array must be 6 bytes in length
dac_status_t MCP4726A1T_ReadAllMemory(uint8_t* ReadArray){

	//Check passed parameters for invalid information
	if(ReadArray == NULL ){ return DAC_ERROR_PARAMETER; }

	//Combined the I2C general call command and the device address information (shift device address by 1 for r/w bit)
	uint16_t I2C_DeviceAddress = (I2C_GeneralCall<<8) | (DeviceAddr<<1);

	//Issue the read command to the DAC and receive the data from memory
	hal_i2c_status = HAL_I2C_Master_Receive(&I2C_SYS, I2C_DeviceAddress|0x01, ReadArray, 6, 100);
	if( hal_i2c_status != HAL_OK ){ return DAC_ERROR_I2C_READ; }

	//Return success code
	return DAC_STATUS_OK;

}
