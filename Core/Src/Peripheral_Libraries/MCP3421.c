#include <Peripheral_Libraries/MCP3421.h>

/* Private external variables -------------------------------------------------------*/
extern I2C_HandleTypeDef I2C_SYS;
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;
extern UART_HandleTypeDef UART_COMM;

/* Global Static Variable Declaration ----------------------------------------------------------*/
static HAL_StatusTypeDef hal_i2c_status;

/* Private global variables ---------------------------------------------------------*/
static const uint8_t ADC_DeviceADDR = 0x6B;

//https://www.digikey.ca/en/products/detail/microchip-technology/MCP3421A3T-E-CH/1827892
//https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/22003e.pdf


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////  ADC Read Voltage CONTROL  ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


mcp3421_status_t MCP3421_ADC_ReadVoltage(float* ReturnValue){

	//Toggle the RDY bit to start a new conversion
	uint8_t ConfigValue;
	if( MCP3421_ADC_Read_Config(&ConfigValue) != ADC_STATUS_OK){ return ADC_ERROR_I2C_READ; }
	ConfigValue = 0x80|ConfigValue;
	if( MCP3421_ADC_Write_Config(&ConfigValue) != ADC_STATUS_OK){ return ADC_ERROR_I2C_WRITE; }

	//Determine sample period based on config
	uint16_t SampleTime;
	if( (ConfigValue&0x0C) == 0x00){ SampleTime = 8; }
	else if( (ConfigValue&0x0C) == 0x04){ SampleTime = 18; }
	else if( (ConfigValue&0x0C) == 0x08){ SampleTime = 68;}
	else{ SampleTime = 270; }

	//Wait for the conversion result and read it
	HAL_Delay(SampleTime);
	uint8_t DataBuffer[4];
	if( MCP3421_ADC_Read(DataBuffer, 4) != ADC_STATUS_OK){ return ADC_ERROR_I2C_READ; }

	//Convert the results to a voltage value
	uint16_t CombinedResult = ((DataBuffer[0]<<8)&0xFF00) | ((DataBuffer[1])&0x00FF);
	CombinedResult = CombinedResult&0x7FFF;
	float CalcVal = ((float)CombinedResult)*2.048/(32768-1);
	*ReturnValue = CalcVal;


	//Return operation success
	return ADC_STATUS_OK;

}










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// High Level ADC Control Functions /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Reset the MCP3421A3T's configuration register to its POR value and validate the values
//This is needed because there is no way to force the chip's registers to a POR state, therefore if the system is reset, the IC will maintain its old values
mcp3421_status_t MCP3421_ADC_Reset(void){

	//Power-On-Reset value of the ADC's configuration register
	uint8_t POR_RegValue = 0x10;

	//Write the POR value to the config register
	if( MCP3421_ADC_Write_Config(&POR_RegValue) != ADC_STATUS_OK){ return ADC_ERROR_I2C_WRITE; }

	//Read the config byte from the MCP3421A3T and compare it to the POR value (RDY bit is masked by retrieval function)
	uint8_t ReadConfigByte;
	if( MCP3421_ADC_Read_Config(&ReadConfigByte) != ADC_STATUS_OK){ return ADC_ERROR_I2C_READ; }
	if( ReadConfigByte != POR_RegValue){ return ADC_ERROR_READBACK; }

 	//memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rRead ADC Config Value is 0x%02X", ReadConfigByte );
    //DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	//Return operation success
	return ADC_STATUS_OK;

}



//Initiate the MCP3421A3T's configuration register and validate the values
mcp3421_status_t MCP3421_ADC_Init( uint8_t ConfigValue ){

	//Write the passed config value to the configuration register
	if( MCP3421_ADC_Write_Config(&ConfigValue) != ADC_STATUS_OK){ return ADC_ERROR_I2C_WRITE; }

	//Read the config byte from the MCP3421A3T and compare it to the expected value (RDY bit is masked by retrieval function)
	uint8_t ReadConfigByte;
	if( MCP3421_ADC_Read_Config(&ReadConfigByte) != ADC_STATUS_OK){ return ADC_ERROR_I2C_READ; }
	if( ReadConfigByte != ConfigValue){ return ADC_ERROR_READBACK; }

 	//memset(PrintBuffer, '\0', COMM_SIZE);	snprintf(PrintBuffer, COMM_SIZE-1, "\n\n\rConfig Value is 0x%02X", ReadConfigByte );
    //DebugPrint(VerboseMode, PrintBuffer, COMM_SIZE );

	//Return operation success
	return ADC_STATUS_OK;

}




//Read the data in the MCP3421's configuration register (RDY bit is masked as its volatile)
mcp3421_status_t MCP3421_ADC_Read_Config( uint8_t* ReadByte ){

	//Local function variables
	uint8_t ReadBuffer[4] = {0x00};

	//Read 4-bytes from the MCP3421 assuming 18-bit mode operation
	//This is done as all sample modes repeat the config-byte at the end, so it is known that the 4th byte is the configuration byte across all modes due to the repetition
	if( MCP3421_ADC_Read( ReadBuffer, 4 ) != ADC_STATUS_OK){ return ADC_ERROR_I2C_READ; }

	//Mask the RDY bit and update the value of the passed memory address
	*ReadByte = ReadBuffer[3]&0x7F;

	//Return operation success
	return ADC_STATUS_OK;

}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  I2C ADC Communication Functions  //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Write the passed byte to the MCP3421's configuration register
mcp3421_status_t MCP3421_ADC_Write_Config(uint8_t* WriteArray){

	//Check for invalid parameter conditions
	if(WriteArray==NULL){ return ADC_ERROR_PARAMETER; }

	//Issue the i2c write data command to the ADC with the config data package
	hal_i2c_status = HAL_I2C_Master_Transmit(&I2C_SYS, (ADC_DeviceADDR<<1), WriteArray, 1, 50);
	if( hal_i2c_status != HAL_OK ){	return ADC_ERROR_I2C_WRITE;	}

	//Return success
	HAL_Delay(100);
	return ADC_STATUS_OK;

}


//General purpose read function for the MCP3421 ADC
mcp3421_status_t MCP3421_ADC_Read( uint8_t* ReadArray, uint16_t ReadArraySize){

	//Check for invalid parameter conditions
	if(ReadArray==NULL || ReadArraySize==0){ return ADC_ERROR_PARAMETER; }

	//Issue read data instruction to ADC and read the specified number of bytes from the ADC's memory
	hal_i2c_status = HAL_I2C_Master_Receive(&I2C_SYS, (ADC_DeviceADDR<<1)|0x01, ReadArray, ReadArraySize, 50);
	if( hal_i2c_status != HAL_OK ){	return ADC_ERROR_I2C_READ; }

	//Return success
	return ADC_STATUS_OK;

}
