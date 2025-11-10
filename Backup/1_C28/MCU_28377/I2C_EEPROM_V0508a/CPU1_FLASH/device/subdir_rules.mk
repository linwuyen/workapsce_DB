################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a" --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xs/driverlib" --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a/mb_slave" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a/CPU1_FLASH/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a" --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f2837xs/driverlib" --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a/mb_slave" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=DEBUG --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Cody/C2000/P230502/I2C_EEPROM_V0508a/CPU1_FLASH/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


