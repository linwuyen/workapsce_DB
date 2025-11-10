################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mb_slave/%.obj: ../mb_slave/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O4 --opt_for_speed=5 --fp_mode=relaxed --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --include_path="C:/Users/cody_chen/CCS12_3_0725/F28379D_SCI_MODBUS/peripherals" --include_path="C:/Users/cody_chen/CCS12_3_0725/F28379D_SCI_MODBUS" --include_path="C:/Users/cody_chen/CCS12_3_0725/F28379D_SCI_MODBUS/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xd/driverlib" --include_path="C:/Users/cody_chen/CCS12_3_0725/F28379D_SCI_MODBUS/mb_slave" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/libraries/calibration/hrpwm/f2837xd/include" --advice:performance=all --define=DEBUG --define=CPU1 --define=_FLASH --define=_LAUNCHXL_F28379D --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="mb_slave/$(basename $(<F)).d_raw" --include_path="C:/Users/cody_chen/CCS12_3_0725/F28379D_SCI_MODBUS/LAUNCH_PAD/syscfg" --obj_directory="mb_slave" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


