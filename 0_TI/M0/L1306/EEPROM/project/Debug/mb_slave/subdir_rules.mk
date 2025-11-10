################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mb_slave/%.o: ../mb_slave/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Ofast -finline-functions -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/I2C_EEPROM" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/mb_slave" -D__MSPM0L1306__ -gdwarf-3 -MMD -MP -MF"mb_slave/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


