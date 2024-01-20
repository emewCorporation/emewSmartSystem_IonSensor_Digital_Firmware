################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/SystemRoutines/subdir.mk
-include Core/Src/Support/subdir.mk
-include Core/Src/Peripheral_Libraries/subdir.mk
-include Core/Src/GenericLibraries/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := emewSmartSystem_IonSensor_Digital_Firmware
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
emewSmartSystem_IonSensor_Digital_Firmware.elf \

MAP_FILES += \
emewSmartSystem_IonSensor_Digital_Firmware.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
emewSmartSystem_IonSensor_Digital_Firmware.list \

OBJCOPY_HEX += \
emewSmartSystem_IonSensor_Digital_Firmware.hex \

OBJCOPY_BIN += \
emewSmartSystem_IonSensor_Digital_Firmware.bin \


# All Target
all: main-build

# Main-build Target
main-build: emewSmartSystem_IonSensor_Digital_Firmware.elf secondary-outputs

# Tool invocations
emewSmartSystem_IonSensor_Digital_Firmware.elf emewSmartSystem_IonSensor_Digital_Firmware.map: $(OBJS) $(USER_OBJS) C:\Users\jason\STM32CubeIDE\workspace\emewSmartSystem_IonSensor_Digital_Firmware\STM32F302CBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "emewSmartSystem_IonSensor_Digital_Firmware.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\jason\STM32CubeIDE\workspace\emewSmartSystem_IonSensor_Digital_Firmware\STM32F302CBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="emewSmartSystem_IonSensor_Digital_Firmware.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

emewSmartSystem_IonSensor_Digital_Firmware.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "emewSmartSystem_IonSensor_Digital_Firmware.list"
	@echo 'Finished building: $@'
	@echo ' '

emewSmartSystem_IonSensor_Digital_Firmware.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "emewSmartSystem_IonSensor_Digital_Firmware.hex"
	@echo 'Finished building: $@'
	@echo ' '

emewSmartSystem_IonSensor_Digital_Firmware.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "emewSmartSystem_IonSensor_Digital_Firmware.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout emewSmartSystem_IonSensor_Digital_Firmware.bin emewSmartSystem_IonSensor_Digital_Firmware.elf emewSmartSystem_IonSensor_Digital_Firmware.hex emewSmartSystem_IonSensor_Digital_Firmware.list emewSmartSystem_IonSensor_Digital_Firmware.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
