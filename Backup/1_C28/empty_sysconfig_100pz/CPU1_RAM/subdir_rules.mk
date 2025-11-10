################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1085261151: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1230/ccs/utils/sysconfig_1.16.1/sysconfig_cli.bat" -s "C:/ti/c2000/C2000Ware_4_03_00_00/.metadata/sdk.json" -d "F28004x" --script "D:/modbus/empty_sysconfig_100pz/c2000.syscfg" -o "syscfg" --package F28004x_100PZ --part F28004x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1085261151 ../c2000.syscfg
syscfg/board.h: build-1085261151
syscfg/board.cmd.genlibs: build-1085261151
syscfg/board.opt: build-1085261151
syscfg/pinmux.csv: build-1085261151
syscfg/c2000ware_libraries.cmd.genlibs: build-1085261151
syscfg/c2000ware_libraries.opt: build-1085261151
syscfg/c2000ware_libraries.c: build-1085261151
syscfg/c2000ware_libraries.h: build-1085261151
syscfg/clocktree.h: build-1085261151
syscfg/: build-1085261151

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/modbus/empty_sysconfig_100pz/peripherals" --include_path="D:/modbus/empty_sysconfig_100pz" --include_path="D:/modbus/empty_sysconfig_100pz/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f28004x/driverlib" --include_path="D:/modbus/empty_sysconfig_100pz/mb_slave" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="D:/modbus/empty_sysconfig_100pz/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.cla $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/modbus/empty_sysconfig_100pz/peripherals" --include_path="D:/modbus/empty_sysconfig_100pz" --include_path="D:/modbus/empty_sysconfig_100pz/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f28004x/driverlib" --include_path="D:/modbus/empty_sysconfig_100pz/mb_slave" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/modbus/empty_sysconfig_100pz/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="D:/modbus/empty_sysconfig_100pz/peripherals" --include_path="D:/modbus/empty_sysconfig_100pz" --include_path="D:/modbus/empty_sysconfig_100pz/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f28004x/driverlib" --include_path="D:/modbus/empty_sysconfig_100pz/mb_slave" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/modbus/empty_sysconfig_100pz/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


