################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ti/driverlib/m0p/%.o: ../ti/driverlib/m0p/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -gdwarf-3 -MMD -MP -MF"ti/driverlib/m0p/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/roger_lin/Downloads/M0G3507_BUCK_EEPROM/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


