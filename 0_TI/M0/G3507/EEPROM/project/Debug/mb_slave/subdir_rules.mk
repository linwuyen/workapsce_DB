################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mb_slave/%.o: ../mb_slave/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/I2C_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_30_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_30_00_03/source" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/mb_slave" -gdwarf-3 -MMD -MP -MF"mb_slave/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


