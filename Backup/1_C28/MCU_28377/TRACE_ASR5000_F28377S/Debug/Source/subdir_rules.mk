################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Source/%.obj: ../Source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O4 --opt_for_speed=5 --fp_mode=relaxed --include_path="D:/MCU_28377/TRACE_ASR5000_F28377S" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_common/include" --include_path="D:/MCU_28377/TRACE_ASR5000_F28377S/Include" --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=_FLASH --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Source/$(basename $(<F)).d_raw" --obj_directory="Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Source/%.obj: ../Source/%.cla $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -O4 --opt_for_speed=5 --fp_mode=relaxed --include_path="D:/MCU_28377/TRACE_ASR5000_F28377S" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_common/include" --include_path="D:/MCU_28377/TRACE_ASR5000_F28377S/Include" --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=_FLASH --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Source/$(basename $(<F)).d_raw" --obj_directory="Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


