################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GenericLibraries/DataSerializationLibrary.c 

OBJS += \
./Core/Src/GenericLibraries/DataSerializationLibrary.o 

C_DEPS += \
./Core/Src/GenericLibraries/DataSerializationLibrary.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/GenericLibraries/%.o Core/Src/GenericLibraries/%.su Core/Src/GenericLibraries/%.cyclo: ../Core/Src/GenericLibraries/%.c Core/Src/GenericLibraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-GenericLibraries

clean-Core-2f-Src-2f-GenericLibraries:
	-$(RM) ./Core/Src/GenericLibraries/DataSerializationLibrary.cyclo ./Core/Src/GenericLibraries/DataSerializationLibrary.d ./Core/Src/GenericLibraries/DataSerializationLibrary.o ./Core/Src/GenericLibraries/DataSerializationLibrary.su

.PHONY: clean-Core-2f-Src-2f-GenericLibraries

