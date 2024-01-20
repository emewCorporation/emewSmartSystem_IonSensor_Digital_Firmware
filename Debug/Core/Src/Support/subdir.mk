################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Support/ProgramEEPROM.c \
../Core/Src/Support/TestingRoutine.c 

OBJS += \
./Core/Src/Support/ProgramEEPROM.o \
./Core/Src/Support/TestingRoutine.o 

C_DEPS += \
./Core/Src/Support/ProgramEEPROM.d \
./Core/Src/Support/TestingRoutine.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Support/%.o Core/Src/Support/%.su Core/Src/Support/%.cyclo: ../Core/Src/Support/%.c Core/Src/Support/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Support

clean-Core-2f-Src-2f-Support:
	-$(RM) ./Core/Src/Support/ProgramEEPROM.cyclo ./Core/Src/Support/ProgramEEPROM.d ./Core/Src/Support/ProgramEEPROM.o ./Core/Src/Support/ProgramEEPROM.su ./Core/Src/Support/TestingRoutine.cyclo ./Core/Src/Support/TestingRoutine.d ./Core/Src/Support/TestingRoutine.o ./Core/Src/Support/TestingRoutine.su

.PHONY: clean-Core-2f-Src-2f-Support

