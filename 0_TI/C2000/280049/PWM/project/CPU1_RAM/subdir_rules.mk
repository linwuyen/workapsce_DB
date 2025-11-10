################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/roger_lin/Desktop/C28_2/PWM_Control" --include_path="C:/Users/roger_lin/Desktop/C28_2/PWM_Control/device" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=CPU1 --c99 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/roger_lin/Desktop/C28_2/PWM_Control/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-941418513: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1260/ccs/utils/sysconfig_1.19.0/sysconfig_cli.bat" --script "C:/Users/roger_lin/Desktop/C28_2/PWM_Control/c2000.syscfg" -o "syscfg" -s "C:/ti/c2000/C2000Ware_5_01_00_00/.metadata/sdk.json" -d "F28004x" --package F28004x_100PZ --part F28004x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-941418513 ../c2000.syscfg
syscfg/board.h: build-941418513
syscfg/board.cmd.genlibs: build-941418513
syscfg/board.opt: build-941418513
syscfg/pinmux.csv: build-941418513
syscfg/epwm.dot: build-941418513
syscfg/adc.dot: build-941418513
syscfg/c2000ware_libraries.cmd.genlibs: build-941418513
syscfg/c2000ware_libraries.opt: build-941418513
syscfg/c2000ware_libraries.c: build-941418513
syscfg/c2000ware_libraries.h: build-941418513
syscfg/clocktree.h: build-941418513
syscfg: build-941418513

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/roger_lin/Desktop/C28_2/PWM_Control" --include_path="C:/Users/roger_lin/Desktop/C28_2/PWM_Control/device" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --advice:performance=all --define=DEBUG --define=CPU1 --c99 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/roger_lin/Desktop/C28_2/PWM_Control/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


