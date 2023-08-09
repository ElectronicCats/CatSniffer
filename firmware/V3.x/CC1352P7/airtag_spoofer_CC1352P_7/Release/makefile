################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS

GEN_OPTS__FLAG := @"syscfg/ti_build_config.opt" @"syscfg/ti_ble_app_config.opt" 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./syscfg/ti_ble_config.o" \
"./syscfg/ti_devices_config.o" \
"./syscfg/ti_radio_config.o" \
"./syscfg/ti_drivers_config.o" \
"./syscfg/ti_sysbios_config.o" \
"./Application/airtag_spoofer.o" \
"./Application/rcosc_calibration.o" \
"./Application/util.o" \
"./Drivers/NV/crc.o" \
"./Drivers/NV/nvocmp.o" \
"./NPI/npi_frame_hci.o" \
"./NPI/npi_rxbuf.o" \
"./NPI/npi_task.o" \
"./NPI/npi_tl.o" \
"./NPI/npi_tl_uart.o" \
"./Profiles/devinfoservice.o" \
"./Profiles/simple_gatt_profile.o" \
"./Startup/main.o" \
"./Startup/osal_icall_ble.o" \
"./Startup/rom_init.o" \
"./iCall/icall.o" \
"./iCall/icall_cc2650.o" \
"./iCall/icall_user_config.o" \
"./iCallBLE/ble_user_config.o" \
"./iCallBLE/ble_user_config_stack.o" \
"./iCallBLE/icall_api_lite.o" \
"./iCallBLE/icall_hci_tl.o" \
"../cc13x2x7_cc26x2x7_app_tirtos7.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-lti_utils_build_linker.cmd.genlibs \
-Wl,-l"ti/devices/cc13x2x7_cc26x2x7/driverlib/bin/ticlang/driverlib.lib" \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Application/subdir_vars.mk
-include Drivers/NV/subdir_vars.mk
-include NPI/subdir_vars.mk
-include Profiles/subdir_vars.mk
-include Startup/subdir_vars.mk
-include iCall/subdir_vars.mk
-include iCallBLE/subdir_vars.mk
-include subdir_rules.mk
-include Application/subdir_rules.mk
-include Drivers/NV/subdir_rules.mk
-include NPI/subdir_rules.mk
-include Profiles/subdir_rules.mk
-include Startup/subdir_rules.mk
-include iCall/subdir_rules.mk
-include iCallBLE/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
airtag_spoofer_CC1352P_7.out \

EXE_OUTPUTS__QUOTED += \
"airtag_spoofer_CC1352P_7.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "airtag_spoofer_CC1352P_7.out"

# Tool invocations
airtag_spoofer_CC1352P_7.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" @"C:/Users/eduar/workspace_v12/airtag_spoofer_CC1352P_7/Release/syscfg/ti_ble_app_config.opt" @"C:/Users/eduar/workspace_v12/airtag_spoofer_CC1352P_7/Release/syscfg/ti_build_config.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/config/factory_config.opt"  -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -mthumb -Oz -DDeviceFamily_CC13X2X7 -DFLASH_ROM_BUILD -DNVOCMP_NWSAMEITEM=1 -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -gdwarf-3 -march=armv7e-m -Wl,-m"airtag_spoofer_CC1352P_7.map" -Wl,-i"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source" -Wl,-i"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/kernel/tirtos7/packages" -Wl,-i"C:/Users/eduar/workspace_v12/airtag_spoofer_CC1352P_7/Release/syscfg" -Wl,-i"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/lib" -Wl,--reread_libs -Wl,--define=FLASH_ROM_BUILD=2 -Wl,--diag_suppress=16002-D -Wl,--diag_suppress=10247-D -Wl,--diag_suppress=10325-D -Wl,--diag_suppress=10229-D -Wl,--diag_suppress=16032-D -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="airtag_spoofer_CC1352P_7_linkInfo.xml" -Wl,--rom_model -o "airtag_spoofer_CC1352P_7.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "syscfg\ti_ble_config.o" "syscfg\ti_devices_config.o" "syscfg\ti_radio_config.o" "syscfg\ti_drivers_config.o" "syscfg\ti_sysbios_config.o" "Application\airtag_spoofer.o" "Application\rcosc_calibration.o" "Application\util.o" "Drivers\NV\crc.o" "Drivers\NV\nvocmp.o" "NPI\npi_frame_hci.o" "NPI\npi_rxbuf.o" "NPI\npi_task.o" "NPI\npi_tl.o" "NPI\npi_tl_uart.o" "Profiles\devinfoservice.o" "Profiles\simple_gatt_profile.o" "Startup\main.o" "Startup\osal_icall_ble.o" "Startup\rom_init.o" "iCall\icall.o" "iCall\icall_cc2650.o" "iCall\icall_user_config.o" "iCallBLE\ble_user_config.o" "iCallBLE\ble_user_config_stack.o" "iCallBLE\icall_api_lite.o" "iCallBLE\icall_hci_tl.o" 
	-$(RM) "syscfg\ti_ble_config.d" "syscfg\ti_devices_config.d" "syscfg\ti_radio_config.d" "syscfg\ti_drivers_config.d" "syscfg\ti_sysbios_config.d" "Application\airtag_spoofer.d" "Application\rcosc_calibration.d" "Application\util.d" "Drivers\NV\crc.d" "Drivers\NV\nvocmp.d" "NPI\npi_frame_hci.d" "NPI\npi_rxbuf.d" "NPI\npi_task.d" "NPI\npi_tl.d" "NPI\npi_tl_uart.d" "Profiles\devinfoservice.d" "Profiles\simple_gatt_profile.d" "Startup\main.d" "Startup\osal_icall_ble.d" "Startup\rom_init.d" "iCall\icall.d" "iCall\icall_cc2650.d" "iCall\icall_user_config.d" "iCallBLE\ble_user_config.d" "iCallBLE\ble_user_config_stack.d" "iCallBLE\icall_api_lite.d" "iCallBLE\icall_hci_tl.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmhex.exe -order MS --memwidth=8 --romwidth=8 --intel -o airtag_spoofer_CC1352P_7.hex airtag_spoofer_CC1352P_7.out
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

