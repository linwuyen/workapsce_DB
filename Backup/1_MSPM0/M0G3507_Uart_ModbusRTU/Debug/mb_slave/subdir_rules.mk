################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mb_slave/%.o: ../mb_slave/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/cody_chen/CCS12_4_M0G3507/M0G3507_Uart_ModbusRTU" -I"C:/Users/cody_chen/CCS12_4_M0G3507/M0G3507_Uart_ModbusRTU/Debug" -I"C:/ti/mspm0_sdk_1_30_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_30_00_03/source" -gdwarf-3 -MMD -MP -MF"mb_slave/$(basename $(<F)).d_raw" -MT"$(@)" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


