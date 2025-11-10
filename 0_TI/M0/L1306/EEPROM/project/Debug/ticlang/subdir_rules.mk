################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ticlang/%.o: ../ticlang/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Ofast -finline-functions -I"C:/Users/roger_lin/Desktop/GW/TI/M0/L1306/EEPROM/project" -I"C:/Users/roger_lin/Desktop/GW/TI/M0/L1306/EEPROM/project/Debug" -I"C:/ti/mspm0_sdk_1_00_00_04/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_00_00_04/source" -I"C:/Users/roger_lin/Desktop/GW/TI/M0/L1306/EEPROM/project/mb_slave" -D__MSPM0L1306__ -gdwarf-3 -MMD -MP -MF"ticlang/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Desktop/GW/TI/M0/L1306/EEPROM/project/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


