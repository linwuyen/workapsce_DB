################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mb_slave/%.obj: ../mb_slave/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="C:/Users/roger_lin/Desktop/C28_49/F280049_100pz_SPI_SLAVE_CLA/peripherals" --include_path="C:/Users/roger_lin/Desktop/C28_49/F280049_100pz_SPI_SLAVE_CLA/mb_slave" --include_path="C:/Users/roger_lin/Desktop/C28_49/F280049_100pz_SPI_SLAVE_CLA" --include_path="C:/Users/roger_lin/Desktop/C28_49/F280049_100pz_SPI_SLAVE_CLA/device" --include_path="C:/ti/c2000/C2000Ware_5_01_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --advice:performance=all --define=DEBUG --define=_FLASH --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="mb_slave/$(basename $(<F)).d_raw" --include_path="C:/Users/roger_lin/Desktop/C28_49/F280049_100pz_SPI_SLAVE_CLA/CPU1_FLASH/syscfg" --obj_directory="mb_slave" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


