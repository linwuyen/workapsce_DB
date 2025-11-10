################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-665429601: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.19.0/sysconfig_cli.bat" -s "C:/ti/mspm0_sdk_1_30_00_03/.metadata/product.json" --script "C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/empty.syscfg" -o "." --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-665429601 ../empty.syscfg
device.opt: build-665429601
device.cmd.genlibs: build-665429601
ti_msp_dl_config.c: build-665429601
ti_msp_dl_config.h: build-665429601
Event.dot: build-665429601

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/I2C_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_30_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_30_00_03/source" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/mb_slave" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_1_30_00_03/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/I2C_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_30_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_30_00_03/source" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/mb_slave" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/I2C_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_30_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_30_00_03/source" -I"C:/Users/roger_lin/Desktop/M0/G3507_emu_EEPROM/mb_slave" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


