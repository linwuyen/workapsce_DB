################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../empty.syscfg 

C_SRCS += \
./ti_msp_dl_config.c \
C:/ti/mspm0_sdk_1_30_00_03/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c \
../emu_eeprom.c \
../i2c_eeprom.c \
../main.c \
../tstBlock.c 

GEN_CMDS += \
./device_linker.cmd 

GEN_FILES += \
./device_linker.cmd \
./device.opt \
./ti_msp_dl_config.c 

C_DEPS += \
./ti_msp_dl_config.d \
./startup_mspm0g350x_ticlang.d \
./emu_eeprom.d \
./i2c_eeprom.d \
./main.d \
./tstBlock.d 

GEN_OPTS += \
./device.opt 

OBJS += \
./ti_msp_dl_config.o \
./startup_mspm0g350x_ticlang.o \
./emu_eeprom.o \
./i2c_eeprom.o \
./main.o \
./tstBlock.o 

GEN_MISC_FILES += \
./device.cmd.genlibs \
./ti_msp_dl_config.h \
./Event.dot 

OBJS__QUOTED += \
"ti_msp_dl_config.o" \
"startup_mspm0g350x_ticlang.o" \
"emu_eeprom.o" \
"i2c_eeprom.o" \
"main.o" \
"tstBlock.o" 

GEN_MISC_FILES__QUOTED += \
"device.cmd.genlibs" \
"ti_msp_dl_config.h" \
"Event.dot" 

C_DEPS__QUOTED += \
"ti_msp_dl_config.d" \
"startup_mspm0g350x_ticlang.d" \
"emu_eeprom.d" \
"i2c_eeprom.d" \
"main.d" \
"tstBlock.d" 

GEN_FILES__QUOTED += \
"device_linker.cmd" \
"device.opt" \
"ti_msp_dl_config.c" 

SYSCFG_SRCS__QUOTED += \
"../empty.syscfg" 

C_SRCS__QUOTED += \
"./ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_1_30_00_03/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c" \
"../emu_eeprom.c" \
"../i2c_eeprom.c" \
"../main.c" \
"../tstBlock.c" 


