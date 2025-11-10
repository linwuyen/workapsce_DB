################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-339027247: ../pinmux.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.18.0/sysconfig_cli.bat" -s "C:/ti/mspm0_sdk_1_20_01_06/.metadata/product.json" --script "C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/pinmux.syscfg" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/device_linker.cmd: build-339027247 ../pinmux.syscfg
syscfg/device.opt: build-339027247
syscfg/device.cmd.genlibs: build-339027247
syscfg/ti_msp_dl_config.c: build-339027247
syscfg/ti_msp_dl_config.h: build-339027247
syscfg/Event.dot: build-339027247
syscfg/: build-339027247

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -gdwarf-3 -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_1_20_01_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


