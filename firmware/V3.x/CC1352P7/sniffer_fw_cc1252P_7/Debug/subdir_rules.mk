################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1450491841: ../sniffer.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1230/ccs/utils/sysconfig_1.16.1/sysconfig_cli.bat" -s "C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/.metadata/product.json" -s "C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/.metadata/product.json" --script "C:/Users/eduar/workspace_v12/sniffer_fw/sniffer.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-1450491841 ../sniffer.syscfg
syscfg/ti_drivers_config.c: build-1450491841
syscfg/ti_drivers_config.h: build-1450491841
syscfg/ti_utils_build_linker.cmd.genlibs: build-1450491841
syscfg/syscfg_c.rov.xs: build-1450491841
syscfg/ti_utils_runtime_model.gv: build-1450491841
syscfg/ti_utils_runtime_Makefile: build-1450491841
syscfg/: build-1450491841

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/eduar/workspace_v12/sniffer_fw" --include_path="C:/Users/eduar/workspace_v12/sniffer_fw/Debug" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/devices/cc13x2x7_cc26x2x7/driverlib" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/drivers/dpl" --include_path="C:/Users/eduar/workspace_v12/sniffer_fw/source/smartrf_settings/cc1352p1lp" --include_path="C:/Users/eduar/workspace_v12/sniffer_fw/source/phy" --include_path="C:/Users/eduar/workspace_v12/sniffer_fw/source/link_layer" --include_path="C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=DeviceFamily_CC13X2X7 --define=FW_ID=0x51 -g --c99 --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/eduar/workspace_v12/sniffer_fw/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


