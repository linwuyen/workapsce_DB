################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS

GEN_OPTS__FLAG := @"syscfg/device.opt" 
GEN_CMDS__FLAG := -Wl,-l"syscfg/device_linker.cmd" 

ORDERED_OBJS += \
"./emu_eeprom.o" \
"./i2c_eeprom.o" \
"./main.o" \
"./syscfg/ti_msp_dl_config.o" \
"./startup_mspm0l130x_ticlang.o" \
"./tstBlock.o" \
"./I2C_EEPROM/CheckEEPROM.o" \
"./mb_slave/CRC16.o" \
"./mb_slave/ModbusFunction.o" \
"./mb_slave/ModbusSlave.o" \
"./mb_slave/linkVariables.o" \
"./mb_slave/tbmenu.o" \
$(GEN_CMDS__FLAG) \
-Wl,-l"C:/ti/mspm0_sdk_1_20_01_06/source/ti/driverlib/lib/ticlang/m0p/mspm0l11xx_l13xx/driverlib.a" \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include I2C_EEPROM/subdir_vars.mk
-include mb_slave/subdir_vars.mk
-include subdir_rules.mk
-include I2C_EEPROM/subdir_rules.mk
-include mb_slave/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
emulate_EEPROM_with_ModbusU8_DMA.out \

EXE_OUTPUTS__QUOTED += \
"emulate_EEPROM_with_ModbusU8_DMA.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "emulate_EEPROM_with_ModbusU8_DMA.out"

# Tool invocations
emulate_EEPROM_with_ModbusU8_DMA.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -Ofast -finline-functions -D__MSPM0L1306__ -gdwarf-3 -Wl,-m"emulate_EEPROM_with_ModbusU8_DMA.map" -Wl,--stack_size=512 -Wl,-i"C:/Users/roger_lin/Documents/WPM0/emulate_EEPROM_with_ModbusU8_DMA/Debug/syscfg" -Wl,-i"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="emulate_EEPROM_with_ModbusU8_DMA_linkInfo.xml" -Wl,--rom_model -o "emulate_EEPROM_with_ModbusU8_DMA.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "emu_eeprom.o" "i2c_eeprom.o" "main.o" "syscfg\ti_msp_dl_config.o" "startup_mspm0l130x_ticlang.o" "tstBlock.o" "I2C_EEPROM\CheckEEPROM.o" "mb_slave\CRC16.o" "mb_slave\ModbusFunction.o" "mb_slave\ModbusSlave.o" "mb_slave\linkVariables.o" "mb_slave\tbmenu.o" 
	-$(RM) "emu_eeprom.d" "i2c_eeprom.d" "main.d" "syscfg\ti_msp_dl_config.d" "startup_mspm0l130x_ticlang.d" "tstBlock.d" "I2C_EEPROM\CheckEEPROM.d" "mb_slave\CRC16.d" "mb_slave\ModbusFunction.d" "mb_slave\ModbusSlave.d" "mb_slave\linkVariables.d" "mb_slave\tbmenu.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

