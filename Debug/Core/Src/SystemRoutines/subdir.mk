################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/SystemRoutines/CANSetup.c \
../Core/Src/SystemRoutines/InformationPrint.c \
../Core/Src/SystemRoutines/PairingRoutines.c \
../Core/Src/SystemRoutines/SensorControlRoutines.c \
../Core/Src/SystemRoutines/SensorMeasurementRoutine.c \
../Core/Src/SystemRoutines/SystemInitialization.c 

OBJS += \
./Core/Src/SystemRoutines/CANSetup.o \
./Core/Src/SystemRoutines/InformationPrint.o \
./Core/Src/SystemRoutines/PairingRoutines.o \
./Core/Src/SystemRoutines/SensorControlRoutines.o \
./Core/Src/SystemRoutines/SensorMeasurementRoutine.o \
./Core/Src/SystemRoutines/SystemInitialization.o 

C_DEPS += \
./Core/Src/SystemRoutines/CANSetup.d \
./Core/Src/SystemRoutines/InformationPrint.d \
./Core/Src/SystemRoutines/PairingRoutines.d \
./Core/Src/SystemRoutines/SensorControlRoutines.d \
./Core/Src/SystemRoutines/SensorMeasurementRoutine.d \
./Core/Src/SystemRoutines/SystemInitialization.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/SystemRoutines/%.o Core/Src/SystemRoutines/%.su Core/Src/SystemRoutines/%.cyclo: ../Core/Src/SystemRoutines/%.c Core/Src/SystemRoutines/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F302xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-SystemRoutines

clean-Core-2f-Src-2f-SystemRoutines:
	-$(RM) ./Core/Src/SystemRoutines/CANSetup.cyclo ./Core/Src/SystemRoutines/CANSetup.d ./Core/Src/SystemRoutines/CANSetup.o ./Core/Src/SystemRoutines/CANSetup.su ./Core/Src/SystemRoutines/InformationPrint.cyclo ./Core/Src/SystemRoutines/InformationPrint.d ./Core/Src/SystemRoutines/InformationPrint.o ./Core/Src/SystemRoutines/InformationPrint.su ./Core/Src/SystemRoutines/PairingRoutines.cyclo ./Core/Src/SystemRoutines/PairingRoutines.d ./Core/Src/SystemRoutines/PairingRoutines.o ./Core/Src/SystemRoutines/PairingRoutines.su ./Core/Src/SystemRoutines/SensorControlRoutines.cyclo ./Core/Src/SystemRoutines/SensorControlRoutines.d ./Core/Src/SystemRoutines/SensorControlRoutines.o ./Core/Src/SystemRoutines/SensorControlRoutines.su ./Core/Src/SystemRoutines/SensorMeasurementRoutine.cyclo ./Core/Src/SystemRoutines/SensorMeasurementRoutine.d ./Core/Src/SystemRoutines/SensorMeasurementRoutine.o ./Core/Src/SystemRoutines/SensorMeasurementRoutine.su ./Core/Src/SystemRoutines/SystemInitialization.cyclo ./Core/Src/SystemRoutines/SystemInitialization.d ./Core/Src/SystemRoutines/SystemInitialization.o ./Core/Src/SystemRoutines/SystemInitialization.su

.PHONY: clean-Core-2f-Src-2f-SystemRoutines

