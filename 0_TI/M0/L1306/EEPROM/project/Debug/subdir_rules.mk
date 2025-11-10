################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Ofast -finline-functions -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/I2C_EEPROM" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/mb_slave" -D__MSPM0L1306__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-248765647: ../pinmux.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.18.0/sysconfig_cli.bat" -s "C:/ti/mspm0_sdk_1_20_01_06/.metadata/product.json" --script "C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/pinmux.syscfg" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/device_linker.cmd: build-248765647 ../pinmux.syscfg
syscfg/device.opt: build-248765647
syscfg/device.cmd.genlibs: build-248765647
syscfg/ti_msp_dl_config.c: build-248765647
syscfg/ti_msp_dl_config.h: build-248765647
syscfg/Event.dot: build-248765647
syscfg/: build-248765647

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Ofast -finline-functions -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/I2C_EEPROM" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/mb_slave" -D__MSPM0L1306__ -gdwarf-3 -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0l130x_ticlang.o: C:/ti/mspm0_sdk_1_20_01_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0l130x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Ofast -finline-functions -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/I2C_EEPROM" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/mb_slave" -D__MSPM0L1306__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


