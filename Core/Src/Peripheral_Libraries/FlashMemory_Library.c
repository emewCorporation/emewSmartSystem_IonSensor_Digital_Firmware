#include "./Peripheral_Libraries/FlashMemory_Library.h"

/* External HAL Variable Declarations --------------------------------------------*/
extern SPI_HandleTypeDef SPI_SYS;
extern UART_HandleTypeDef UART_COMM;

/* External Variable Declarations --------------------------------------------------------------*/
extern char PrintBuffer[COMM_SIZE];
extern uint8_t VerboseMode;

/* Global Static Variable Declaration ----------------------------------------------------------*/
static HAL_StatusTypeDef hal_spi_status;


/* SPI Flash Chip Instruction Set --------------------------------------------*/
static const uint8_t SPI_FLASH_WREN        = 0x06;				//Write enable
static const uint8_t SPI_FLASH_WRDI        = 0x04;				//Write disable
static const uint8_t SPI_FLASH_RDSR1       = 0x05;				//Read status register 1
static const uint8_t SPI_FLASH_RDSR2       = 0x35;				//Read status register 2
static const uint8_t SPI_FLASH_RDSR3       = 0x15;				//Read status register 3
static const uint8_t SPI_FLASH_WRSR1       = 0x01;				//Write status register 1
static const uint8_t SPI_FLASH_WRSR2       = 0x31;				//Write status register 2
static const uint8_t SPI_FLASH_WRSR3       = 0x11;				//Write status register 3
static const uint8_t SPI_FLASH_SEC_ERS     = 0x20;				//Sector erase
static const uint8_t SPI_FLASH_BLK_ERS     = 0x52;				//Block erase
static const uint8_t SPI_FLASH_CHP_ERS     = 0xC7;				//Chip erase
static const uint8_t SPI_FLASH_READ  	   = 0x03;				//Perform read
static const uint8_t SPI_FLASH_WRITE 	   = 0x02;				//Perform write

//https://www.digikey.ca/en/products/detail/winbond-electronics/W25Q128JVSIM/6819721



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Flash Memory Control Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Reads the status register of the flash chip and returns the value
flash_status_t Flash_ReadStatusRegister(uint8_t StatusRegister, uint8_t* ReturnDataByte){

	//Determine which status register to read
    uint8_t SPI_FLASH_RDSR = 0;
    if(StatusRegister==1){ SPI_FLASH_RDSR=SPI_FLASH_RDSR1; }
    if(StatusRegister==2){ SPI_FLASH_RDSR=SPI_FLASH_RDSR2; }
    if(StatusRegister==3){ SPI_FLASH_RDSR=SPI_FLASH_RDSR3; }

    //Issue instruction to the device
    Set_Flash_CS(0);
    hal_spi_status=HAL_SPI_Transmit(&SPI_SYS, (uint8_t*)&SPI_FLASH_RDSR, 1, 100);		//Issue 'read status register' command to bus
    if(hal_spi_status != HAL_OK){														//If operation fails: Issue status, disable CS, leave function
        Set_Flash_CS(1);
		return FLASH_ERROR_SPI_WRITE;
	}

    //Read data from device
    hal_spi_status=HAL_SPI_Receive(&SPI_SYS, ReturnDataByte, 1, 100);	  	//Read incoming data from SPI bus
	Set_Flash_CS(1);														//Raise CS line (Disable Chip)
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_READ; }			//If operation fails: Issue status, disable CS, leave function

	//Return status code
	return FLASH_STATUS_OK;
}


//Write a byte to a status register of the flash chip
flash_status_t Flash_WriteStatusRegister(uint8_t StatusRegister, uint8_t DataByte){

	//Determine which status register to read
    uint8_t SPI_FLASH_WRSR = 0;
    if(StatusRegister==1){ SPI_FLASH_WRSR=SPI_FLASH_WRSR1; }
    if(StatusRegister==2){ SPI_FLASH_WRSR=SPI_FLASH_WRSR2; }
    if(StatusRegister==3){ SPI_FLASH_WRSR=SPI_FLASH_WRSR3; }

    //Create a write buffer by combining MemADDR and the data buffer
	uint8_t WriteBuffer[2];						  	//Create write buffer
	WriteBuffer[0] = SPI_FLASH_WRSR;				//Op-Code
	WriteBuffer[1] = DataByte;						//Data byte

    //Perform WREN instruction
	Set_Flash_CS(0);																	//Set CS pin low
	hal_spi_status=HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WREN, 1, 100);		//Write to SPI EEPROM to enable WREN latch
	Set_Flash_CS(1);																	//Set CS pin high
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, leave function


    //Issue instruction to the device
	Set_Flash_CS(0);													//Enable CS on chip
	hal_spi_status=HAL_SPI_Transmit(&SPI_SYS, WriteBuffer, 2, 100);		//Issue 'read status register' command to bus
	Set_Flash_CS(1);													//Disable CS on chip
    HAL_Delay(20);														//Wait 20ms for the instruction to finish
    if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }		//If operation fails: Issue status, and leave function

	//Disable specified flash chip and return status code
    return FLASH_STATUS_OK;
}



//Read an array of bytes from the SPI EEPROM
flash_status_t Flash_Read(uint32_t MemADDR, uint8_t *ReadArray, int ReadArraySize){

	//Create a write buffer by combining MemADDR and the data buffer
	uint8_t WriteBuffer[4];			  				//Create write buffer to store addressing information
	WriteBuffer[0] = SPI_FLASH_READ;				//OP-Code
	WriteBuffer[1] = (0x00FF0000&MemADDR)>>16;		//MSB of address
	WriteBuffer[2] = (0x0000FF00&MemADDR)>>8;		//Middle byte of address
	WriteBuffer[3] = (0x000000FF&MemADDR);			//LSB of address


	//Issue read page instruction to IC
	Set_Flash_CS(0);																		//Set CS pin to low
	hal_spi_status=HAL_SPI_Transmit(&SPI_SYS, WriteBuffer, sizeof(WriteBuffer), 100);		//Issue read command over SPI with targeted memory address
	if(hal_spi_status != HAL_OK){															//If operation fails: Issue status, reset to default, leave function
		Set_Flash_CS(1);
		return FLASH_ERROR_SPI_WRITE;
	}

	//Read incoming data from SPI bus
	hal_spi_status=HAL_SPI_Receive(&SPI_SYS, ReadArray, ReadArraySize, 100);	//Read data from SPI bus
	Set_Flash_CS(1);															//Set CS pin to low
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_READ; }				//If operation fails: Issue status, reset to default, leave function

	//Set to ensure CS is high before function exit
	return FLASH_STATUS_OK;

}




//Write an array of bytes to the SPI EEPROM
flash_status_t Flash_Write(uint32_t MemADDR, uint8_t *DataArray, int DataArraySize){

	//Create a write buffer by combining MemADDR and the data buffer
	uint8_t WriteBuffer[DataArraySize+4];			//Create write buffer
	WriteBuffer[0] = SPI_FLASH_WRITE;				//OP-Code
	WriteBuffer[1] = (0x00FF0000&MemADDR)>>16;		//MSB of address
	WriteBuffer[2] = (0x0000FF00&MemADDR)>>8;		//Middle byte of address
	WriteBuffer[3] = (0x000000FF&MemADDR);			//LSB of address


	//Fill WriteBuffer with data from DataArray
	for(int i=4; i< 4 + DataArraySize; i++ ){
		WriteBuffer[i] = DataArray[i-4];
	}

	//Perform WREN instruction
	Set_Flash_CS(0);																	//Set CS pin low
	hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WREN, 1, 100);	//Write to SPI EEPROM to enable WREN latch
	Set_Flash_CS(1);																	//Set CS pin high
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, and leave function


	//Perform SPI flash write
	Set_Flash_CS(0);																	//Set CS pin low
	hal_spi_status=HAL_SPI_Transmit(&SPI_SYS, WriteBuffer, sizeof(WriteBuffer), 100);	//Write to SPI EEPROM
	Set_Flash_CS(1);																	//Set CS pin to high
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, and leave function

	//Set to ensure CS is high before function exit
	HAL_Delay(5);																		//Wait 5ms for the instruction to finish
	return FLASH_STATUS_OK;

}



//Erase a (4K-byte) sector of the flash memory
flash_status_t Flash_SectorErase(uint32_t MemADDR){

	  //Create a write buffer by combining MemADDR and the data buffer
	  uint8_t WriteBuffer[4];			  					//Create write buffer
	  WriteBuffer[0] = SPI_FLASH_SEC_ERS;					//OP-Code
	  WriteBuffer[1] = 0x000000FF&(MemADDR>>24);			//MSB of address
	  WriteBuffer[2] = 0x000000FF&(MemADDR>>16);			//Middle of address
	  WriteBuffer[3] = 0x000000FF&MemADDR;					//LSB of address

	  //Issue the WREN instruction
	  Set_Flash_CS(0);																		//Set CS pin low
	  hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WREN, 1, 100);		//Write to SPI EEPROM to enable WREN latch
	  Set_Flash_CS(1);																		//Set CS pin high
	  if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }										//If operation fails: Issue status, reset to default, leave function

	  //Perform subsector erase
	  Set_Flash_CS(0);																		//Set CS pin low
	  hal_spi_status=HAL_SPI_Transmit(&SPI_SYS, WriteBuffer, sizeof(WriteBuffer), 100);		//Write to instruction to SPI flash IC
	  Set_Flash_CS(1);																		//Set CS pin to high
	  if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, leave function

	  //Leave function by returning status
	  return FLASH_STATUS_OK;
}


//Erase a (32KB) block of the flash memory
flash_status_t Flash_BlockErase(uint32_t MemADDR){

	  //Create a write buffer by combining MemADDR and the data buffer
	  uint8_t WriteBuffer[4];			  					//Create write buffer
	  WriteBuffer[0] = SPI_FLASH_BLK_ERS;					//OP-Code
	  WriteBuffer[1] = 0x000000FF&(MemADDR>>24);			//MSB of address
	  WriteBuffer[2] = 0x000000FF&(MemADDR>>16);			//Middle of address
	  WriteBuffer[3] = 0x000000FF&MemADDR;					//LSB of address

	  //Issue the WREN instruction
	  Set_Flash_CS(0);																		//Set CS pin low
	  hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WREN, 1, 100);		//Write to SPI EEPROM to enable WREN latch
	  Set_Flash_CS(1);																		//Set CS pin high
	  if(hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, reset to default, leave function


	  //Perform subsector erase
	  Set_Flash_CS(0);																		//Set CS pin low
	  hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, WriteBuffer, sizeof(WriteBuffer), 100);	//Write to instruction to SPI flash IC
	  Set_Flash_CS(1);																		//Set CS pin to high
	  if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, leave function

	  //Leave function by returning status
	  return FLASH_STATUS_OK;
}


//Issue instruction bulk erase instruction to specified IC
flash_status_t Flash_BulkErase(void){

	  //Enable write enable latch (allow write operations)
	  Set_Flash_CS(0);																		//Set CS pin low (Enable IC)
	  hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WREN, 1, 100);		//Write to SPI EEPROM to enable WREN latch
	  Set_Flash_CS(1);																		//Set CS pin high (Disable IC)
	  if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }				  		//If operation fails: Issue status, reset to default, leave function

	  //Transmit message to perform erase
	  Set_Flash_CS(0);																		//Set CS pin low (Enable IC)
	  hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_CHP_ERS, 1, 100);	//Write to SPI EEPROM to perform bulk erase
	  Set_Flash_CS(1);																		//Set CS pin high (Disable IC)
	  if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }						//If operation fails: Issue status, reset to default, leave function

	  //Return status code
	  return FLASH_STATUS_OK;
}




//Assert the write enable latch of the flash IC
flash_status_t Flash_WREN(void){

    //Perform WREN instruction
	Set_Flash_CS(0);																	//Set CS pin low
	hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WREN, 1, 100);	//Write to SPI EEPROM to enable WREN latch
	Set_Flash_CS(1);																	//Set CS pin high
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }				    	//If operation fails: Issue status, reset to default, leave function

	//Return status code
	return FLASH_STATUS_OK;

}


//De-assert the write enable latch of the flash IC
flash_status_t Flash_WRDI(uint8_t Device){

    //Perform WREN instruction
	Set_Flash_CS(0);																	//Set CS pin low
	hal_spi_status = HAL_SPI_Transmit(&SPI_SYS, (uint8_t *)&SPI_FLASH_WRDI, 1, 100);	//Write to SPI EEPROM to disable WREN latch
	Set_Flash_CS(1);																	//Set CS pin high
	if( hal_spi_status != HAL_OK ){ return FLASH_ERROR_SPI_WRITE; }				    	//If operation fails: Issue status, reset to default, leave function

	//Disable specified flash chip and return status code
	return FLASH_STATUS_OK;
}


