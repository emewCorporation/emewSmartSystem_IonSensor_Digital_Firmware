/* Private includes ----------------------------------------------------------------------------*/
#include <Peripheral_Libraries/EEPROM_Library.h>

/* External HAL Variables Declarations ---------------------------------------------------------*/
extern UART_HandleTypeDef UART_COMM;
extern I2C_HandleTypeDef I2C_SYS;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;

/* Global Static Variable Declaration ----------------------------------------------------------*/
static HAL_StatusTypeDef hal_i2c_status;

/* Private Global Variables --------------------------------------------------------------------*/
static const uint16_t DeviceADDR      = 0x50;		//External address of the EEPROM IC


//Datasheet URL: "https://www.digikey.ca/en/products/detail/microchip-technology/24FC16T-I-OT/12177544"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  I2C EEPROM Communication Functions  ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Function to write data to the EEPROM IC
eeprom_status_t eeprom_Write(uint16_t MemADDR, uint8_t* WriteArray, int WriteArraySize){

	//Modify the I2C address value with the 3 most significant bits from MemADDR
	uint16_t DeviceADDR_Modified = DeviceADDR | ((MemADDR>>8)&0x0007) ;

	//Create a write buffer containing the write data and the addressing information
	uint8_t WriteBuffer[1 + WriteArraySize];			//Create array that is the size of the data to be written and the address byte
	WriteBuffer[0] = MemADDR&0x00FF;	  			    //Set the first byte of the array to the EEPROM memory address

	//Fill WriteBuffer with data from WriteArray
	for(int i=1; i< 1 + WriteArraySize; i++ ){
		WriteBuffer[i] = WriteArray[i-1];
	}

	//Write the buffer data to the EEPROM IC
	hal_i2c_status = HAL_I2C_Master_Transmit(&I2C_SYS, (DeviceADDR_Modified<<1), WriteBuffer, sizeof(WriteBuffer), 100);
	HAL_Delay(5);
	if( hal_i2c_status != HAL_OK ){ return EEPROM_ERROR_I2C_WRITE;	}

	//Return success code
	return EEPROM_STATUS_OK;

}


//Function to read data from the EEPROM IC
eeprom_status_t eeprom_Read(uint16_t MemADDR, uint8_t* ReadArray, int ReadArraySize){

	//Modify the I2C address value with the 3 most significant bits from MemADDR
	uint16_t DeviceADDR_Modified = DeviceADDR | ((MemADDR>>8)&0x0007) ;

	//Create a write buffer and store EEPROM read address
	uint8_t WriteBuffer[1] = {MemADDR} ;

	//Read data byte(s) from the EEPROM IC
	hal_i2c_status = HAL_I2C_Master_Transmit(&I2C_SYS, (DeviceADDR_Modified<<1), WriteBuffer, sizeof(WriteBuffer), 100);		//Set the read address in the EEPROM IC
	if( hal_i2c_status != HAL_OK ){
		return EEPROM_ERROR_I2C_WRITE;
	}else{
		hal_i2c_status = HAL_I2C_Master_Receive(&I2C_SYS, (DeviceADDR_Modified<<1)|0x01, ReadArray, ReadArraySize, 100);		//Read data from the EEPROM IC starting at the previously set address
		if( hal_i2c_status != HAL_OK ){	return EEPROM_ERROR_I2C_READ; }
	}

	//Return success code
	return EEPROM_STATUS_OK;

}
