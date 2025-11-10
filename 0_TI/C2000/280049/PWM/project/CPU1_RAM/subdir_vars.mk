################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../28004x_generic_ram_lnk.cmd 

SYSCFG_SRCS += \
../c2000.syscfg 

LIB_SRCS += \
C:/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28004x/driverlib/ccs/Debug/driverlib.lib 

C_SRCS += \
../ControlPWM.c \
./syscfg/board.c \
./syscfg/c2000ware_libraries.c \
../main.c 

GEN_FILES += \
./syscfg/board.c \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt \
./syscfg/c2000ware_libraries.c 

GEN_MISC_DIRS += \
./syscfg 

C_DEPS += \
./ControlPWM.d \
./syscfg/board.d \
./syscfg/c2000ware_libraries.d \
./main.d 

GEN_OPTS += \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt 

OBJS += \
./ControlPWM.obj \
./syscfg/board.obj \
./syscfg/c2000ware_libraries.obj \
./main.obj 

GEN_MISC_FILES += \
./syscfg/board.h \
./syscfg/board.cmd.genlibs \
./syscfg/pinmux.csv \
./syscfg/epwm.dot \
./syscfg/adc.dot \
./syscfg/c2000ware_libraries.cmd.genlibs \
./syscfg/c2000ware_libraries.h \
./syscfg/clocktree.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg" 

OBJS__QUOTED += \
"ControlPWM.obj" \
"syscfg\board.obj" \
"syscfg\c2000ware_libraries.obj" \
"main.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\board.h" \
"syscfg\board.cmd.genlibs" \
"syscfg\pinmux.csv" \
"syscfg\epwm.dot" \
"syscfg\adc.dot" \
"syscfg\c2000ware_libraries.cmd.genlibs" \
"syscfg\c2000ware_libraries.h" \
"syscfg\clocktree.h" 

C_DEPS__QUOTED += \
"ControlPWM.d" \
"syscfg\board.d" \
"syscfg\c2000ware_libraries.d" \
"main.d" 

GEN_FILES__QUOTED += \
"syscfg\board.c" \
"syscfg\board.opt" \
"syscfg\c2000ware_libraries.opt" \
"syscfg\c2000ware_libraries.c" 

C_SRCS__QUOTED += \
"../ControlPWM.c" \
"./syscfg/board.c" \
"./syscfg/c2000ware_libraries.c" \
"../main.c" 

SYSCFG_SRCS__QUOTED += \
"../c2000.syscfg" 


