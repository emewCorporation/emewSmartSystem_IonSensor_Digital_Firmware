/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SPIFlashLibrary_H
#define SPIFlashLibrary_H

/* Private includes ----------------------------------------------------------*/
#include <Global_HALL.h>
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Status return codes for the W25Q128JVSIM functions
typedef enum{
	FLASH_STATUS_OK 		=  0,			//Operation success
	FLASH_ERROR_SPI_READ	= -1,			//Error from hal spi read operation
	FLASH_ERROR_SPI_WRITE	= -2,			//Error from hal spi write operation
	FLASH_ERROR_PARAMETER	= -3,			//Passed parameter is invalid
	FLASH_ERROR_UNKNOWN		= -5			//Unknown error
}flash_status_t;


/* Functions prototypes ---------------------------------------------*/
flash_status_t Flash_ReadStatusRegister(uint8_t StatusRegister, uint8_t* ReturnDataByte);
flash_status_t Flash_WriteStatusRegister(uint8_t StatusRegister, uint8_t DataByte);
flash_status_t Flash_Read(uint32_t MemADDR, uint8_t *ReadArray, int ReadArraySize);
flash_status_t Flash_Write(uint32_t MemADDR, uint8_t *DataArray, int DataArraySize);
flash_status_t Flash_SectorErase(uint32_t MemADDR);
flash_status_t Flash_BlockErase(uint32_t MemADDR);
flash_status_t Flash_BulkErase(void);
flash_status_t Flash_WREN(void);
flash_status_t Flash_WRDI(uint8_t Device);



#endif /* SPIFlashLibrary_H */

