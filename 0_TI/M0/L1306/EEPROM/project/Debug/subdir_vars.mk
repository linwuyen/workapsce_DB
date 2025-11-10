################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../pinmux.syscfg 

C_SRCS += \
../emu_eeprom.c \
../i2c_eeprom.c \
../main.c \
./syscfg/ti_msp_dl_config.c \
C:/ti/mspm0_sdk_1_20_01_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0l130x_ticlang.c \
../tstBlock.c 

GEN_CMDS += \
./syscfg/device_linker.cmd 

GEN_FILES += \
./syscfg/device_linker.cmd \
./syscfg/device.opt \
./syscfg/ti_msp_dl_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./emu_eeprom.d \
./i2c_eeprom.d \
./main.d \
./syscfg/ti_msp_dl_config.d \
./startup_mspm0l130x_ticlang.d \
./tstBlock.d 

GEN_OPTS += \
./syscfg/device.opt 

OBJS += \
./emu_eeprom.o \
./i2c_eeprom.o \
./main.o \
./syscfg/ti_msp_dl_config.o \
./startup_mspm0l130x_ticlang.o \
./tstBlock.o 

GEN_MISC_FILES += \
./syscfg/device.cmd.genlibs \
./syscfg/ti_msp_dl_config.h \
./syscfg/Event.dot 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"emu_eeprom.o" \
"i2c_eeprom.o" \
"main.o" \
"syscfg\ti_msp_dl_config.o" \
"startup_mspm0l130x_ticlang.o" \
"tstBlock.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg\device.cmd.genlibs" \
"syscfg\ti_msp_dl_config.h" \
"syscfg\Event.dot" 

C_DEPS__QUOTED += \
"emu_eeprom.d" \
"i2c_eeprom.d" \
"main.d" \
"syscfg\ti_msp_dl_config.d" \
"startup_mspm0l130x_ticlang.d" \
"tstBlock.d" 

GEN_FILES__QUOTED += \
"syscfg\device_linker.cmd" \
"syscfg\device.opt" \
"syscfg\ti_msp_dl_config.c" 

C_SRCS__QUOTED += \
"../emu_eeprom.c" \
"../i2c_eeprom.c" \
"../main.c" \
"./syscfg/ti_msp_dl_config.c" \
"C:/ti/mspm0_sdk_1_20_01_06/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0l130x_ticlang.c" \
"../tstBlock.c" 

SYSCFG_SRCS__QUOTED += \
"../pinmux.syscfg" 


