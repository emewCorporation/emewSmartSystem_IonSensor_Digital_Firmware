################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Peripheral_Libraries/CANBusProtocol_Library.c \
../Core/Src/Peripheral_Libraries/EEPROM_Library.c \
../Core/Src/Peripheral_Libraries/FlashMemory_Library.c \
../Core/Src/Peripheral_Libraries/MCP3421.c \
../Core/Src/Peripheral_Libraries/MCP4726A1T_E.c 

OBJS += \
./Core/Src/Peripheral_Libraries/CANBusProtocol_Library.o \
./Core/Src/Peripheral_Libraries/EEPROM_Library.o \
./Core/Src/Peripheral_Libraries/FlashMemory_Library.o \
./Core/Src/Peripheral_Libraries/MCP3421.o \
./Core/Src/Peripheral_Libraries/MCP4726A1T_E.o 

C_DEPS += \
./Core/Src/Peripheral_Libraries/CANBusProtocol_Library.d \
./Core/Src/Peripheral_Libraries/EEPROM_Library.d \
./Core/Src/Peripheral_Libraries/FlashMemory_Library.d \
./Core/Src/Peripheral_Libraries/MCP3421.d \
./Core/Src/Peripheral_Libraries/MCP4726A1T_E.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Peripheral_Libraries/%.o Core/Src/Peripheral_Libraries/%.su Core/Src/Peripheral_Libraries/%.cyclo: ../Core/Src/Peripheral_Libraries/%.c Core/Src/Peripheral_Libraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Peripheral_Libraries

clean-Core-2f-Src-2f-Peripheral_Libraries:
	-$(RM) ./Core/Src/Peripheral_Libraries/CANBusProtocol_Library.cyclo ./Core/Src/Peripheral_Libraries/CANBusProtocol_Library.d ./Core/Src/Peripheral_Libraries/CANBusProtocol_Library.o ./Core/Src/Peripheral_Libraries/CANBusProtocol_Library.su ./Core/Src/Peripheral_Libraries/EEPROM_Library.cyclo ./Core/Src/Peripheral_Libraries/EEPROM_Library.d ./Core/Src/Peripheral_Libraries/EEPROM_Library.o ./Core/Src/Peripheral_Libraries/EEPROM_Library.su ./Core/Src/Peripheral_Libraries/FlashMemory_Library.cyclo ./Core/Src/Peripheral_Libraries/FlashMemory_Library.d ./Core/Src/Peripheral_Libraries/FlashMemory_Library.o ./Core/Src/Peripheral_Libraries/FlashMemory_Library.su ./Core/Src/Peripheral_Libraries/MCP3421.cyclo ./Core/Src/Peripheral_Libraries/MCP3421.d ./Core/Src/Peripheral_Libraries/MCP3421.o ./Core/Src/Peripheral_Libraries/MCP3421.su ./Core/Src/Peripheral_Libraries/MCP4726A1T_E.cyclo ./Core/Src/Peripheral_Libraries/MCP4726A1T_E.d ./Core/Src/Peripheral_Libraries/MCP4726A1T_E.o ./Core/Src/Peripheral_Libraries/MCP4726A1T_E.su

.PHONY: clean-Core-2f-Src-2f-Peripheral_Libraries

