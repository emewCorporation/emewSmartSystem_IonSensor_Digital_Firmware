/* Private includes ----------------------------------------------------------------------------*/
#include "./GenericLibraries/DataSerializationLibrary.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  Data Serialization Functions  /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Converts float value to a byte array
void float_to_bytes(float InputValue, uint8_t *OutputByteArray){

	  union{
		  float float_variable;
		  uint8_t temp_array[4];
	  }u;

	  u.float_variable =InputValue;
	  memcpy(OutputByteArray, u.temp_array, 4);
}


//Converts byte array into a float
float bytes_to_float(uint8_t *InputByteArray){

	uint8_t TempArray[4] = {InputByteArray[0], InputByteArray[1], InputByteArray[2], InputByteArray[3]};

	float Converted_Value = *(float *)&TempArray;

	return Converted_Value;
}


//Store uint32 value into a byte array in little endian form
void uint32_to_bytes(uint32_t InputValue, uint8_t *OutputByteArray){
	OutputByteArray[0] = (InputValue&0x000000FF);
	OutputByteArray[1] = (InputValue>>8)&0x0000FF00;
	OutputByteArray[2] = (InputValue>>16)&0x00FF0000;
	OutputByteArray[3] = (InputValue>>24)&0xFF000000;
}


//Store little endian byte array into uint32 value
uint32_t bytes_to_uint32(uint8_t *InputByteArray){

	//Parse the bytes into a "workable" value
	uint32_t Converted_Value = ((InputByteArray[3]<<24)&0xFF000000) |
							   ((InputByteArray[2]<<16)&0x00FF0000) |
							   ((InputByteArray[1]<<8)&0x00000FF00) |
							   (InputByteArray[0]&0x000000FF);

	return Converted_Value;

}


//Store uint16 value into a byte array in little endian form
void uint16_to_bytes(uint16_t InputValue, uint8_t *OutputByteArray){
	OutputByteArray[0] = (InputValue&0x00FF);
	OutputByteArray[1] = (InputValue>>8)&0x00FF;
}


//Store little endian byte array into uint16 value
uint16_t bytes_to_uint16(uint8_t *InputByteArray){

	//Parse the bytes into a "workable" value
	uint32_t Converted_Value = ((InputByteArray[1]<<8)&0xFF00) |
								(InputByteArray[0]&0x000FF);

	return Converted_Value;

}
