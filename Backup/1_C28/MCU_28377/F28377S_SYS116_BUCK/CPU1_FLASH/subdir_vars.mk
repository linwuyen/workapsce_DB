################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../2837xS_FLASH_lnk_cpu1.cmd 

SYSCFG_SRCS += \
../pinmux.syscfg 

CLA_SRCS += \
../cla_task.cla 

LIB_SRCS += \
C:/ti/c2000/C2000Ware_4_03_00_00/libraries/calibration/hrpwm/f2837xs/lib/SFO_v8_fpu_lib_build_c28_driverlib.lib \
C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xs/driverlib/ccs/Debug/driverlib.lib 

C_SRCS += \
../isr.c \
../main.c \
./syscfg/board.c \
./syscfg/c2000ware_libraries.c 

GEN_FILES += \
./syscfg/board.c \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt \
./syscfg/c2000ware_libraries.c 

CLA_DEPS += \
./cla_task.d 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./isr.d \
./main.d \
./syscfg/board.d \
./syscfg/c2000ware_libraries.d 

GEN_OPTS += \
./syscfg/board.opt \
./syscfg/c2000ware_libraries.opt 

OBJS += \
./cla_task.obj \
./isr.obj \
./main.obj \
./syscfg/board.obj \
./syscfg/c2000ware_libraries.obj 

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
"syscfg\" 

OBJS__QUOTED += \
"cla_task.obj" \
"isr.obj" \
"main.obj" \
"syscfg\board.obj" \
"syscfg\c2000ware_libraries.obj" 

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
"isr.d" \
"main.d" \
"syscfg\board.d" \
"syscfg\c2000ware_libraries.d" 

GEN_FILES__QUOTED += \
"syscfg\board.c" \
"syscfg\board.opt" \
"syscfg\c2000ware_libraries.opt" \
"syscfg\c2000ware_libraries.c" 

CLA_DEPS__QUOTED += \
"cla_task.d" 

C_SRCS__QUOTED += \
"../isr.c" \
"../main.c" \
"./syscfg/board.c" \
"./syscfg/c2000ware_libraries.c" 

SYSCFG_SRCS__QUOTED += \
"../pinmux.syscfg" 


