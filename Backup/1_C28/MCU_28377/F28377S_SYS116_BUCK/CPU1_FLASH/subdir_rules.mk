################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.cla $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O4 --opt_for_speed=5 --fp_mode=relaxed --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/peripherals" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xs/driverlib" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/mb_slave" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/eeprom" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/calibration/hrpwm/f2837xs/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/CPU1_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O4 --opt_for_speed=5 --fp_mode=relaxed --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/peripherals" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xs/driverlib" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/mb_slave" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/eeprom" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/calibration/hrpwm/f2837xs/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/CPU1_FLASH/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1361304209: ../pinmux.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.16.2/sysconfig_cli.bat" -s "C:/ti/c2000/C2000Ware_4_03_00_00/.metadata/sdk.json" -d "F2837xS" --script "D:/MCU_28377/F28377S_SYS116_BUCK/pinmux.syscfg" -o "syscfg" --package F2837xS_176PTP --part F2837xS_176PTP --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1361304209 ../pinmux.syscfg
syscfg/board.h: build-1361304209
syscfg/board.cmd.genlibs: build-1361304209
syscfg/board.opt: build-1361304209
syscfg/pinmux.csv: build-1361304209
syscfg/epwm.dot: build-1361304209
syscfg/adc.dot: build-1361304209
syscfg/c2000ware_libraries.cmd.genlibs: build-1361304209
syscfg/c2000ware_libraries.opt: build-1361304209
syscfg/c2000ware_libraries.c: build-1361304209
syscfg/c2000ware_libraries.h: build-1361304209
syscfg/clocktree.h: build-1361304209
syscfg/: build-1361304209

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O4 --opt_for_speed=5 --fp_mode=relaxed --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/peripherals" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xs/driverlib" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/mb_slave" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/eeprom" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/calibration/hrpwm/f2837xs/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="D:/MCU_28377/F28377S_SYS116_BUCK/CPU1_FLASH/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


