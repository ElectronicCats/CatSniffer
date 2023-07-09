################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-279289521: ../airtag_spoofer.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1_16_2/sysconfig_cli.bat" -s "C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/.metadata/product.json" --script "D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang/airtag_spoofer.syscfg" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_ble_config.h: build-279289521 ../airtag_spoofer.syscfg
syscfg/ti_ble_config.c: build-279289521
syscfg/ti_build_config.opt: build-279289521
syscfg/ti_ble_app_config.opt: build-279289521
syscfg/ti_devices_config.c: build-279289521
syscfg/ti_radio_config.c: build-279289521
syscfg/ti_radio_config.h: build-279289521
syscfg/ti_drivers_config.c: build-279289521
syscfg/ti_drivers_config.h: build-279289521
syscfg/ti_utils_build_linker.cmd.genlibs: build-279289521
syscfg/syscfg_c.rov.xs: build-279289521
syscfg/ti_sysbios_config.h: build-279289521
syscfg/ti_sysbios_config.c: build-279289521
syscfg/: build-279289521

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1230/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c @"D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang/Release/syscfg/ti_ble_app_config.opt" @"D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang/Release/syscfg/ti_build_config.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/config/build_components.opt" @"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/config/factory_config.opt"  -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -mthumb -Oz -I"D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang" -I"D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang/Release" -I"D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang/Application" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/controller/cc26xx/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/rom" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/common/cc26xx" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/icall/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/hal/src/target/_common" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/common/cc26xx/npi/stack" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/hal/src/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/heapmgr" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/profiles/dev_info" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/profiles/simple_profile" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/icall/src/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/npi/src" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/osal/src/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/services/src/saddr" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/services/src/sdata" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/common/nv" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/common/cc26xx" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/icall/src" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/common/cc26xx/rcosc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/ble5stack/npi/src/inc" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/devices/cc13x2_cc26x2" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/kernel/tirtos7/packages" -I"C:/ti/simplelink_cc13xx_cc26xx_sdk_7_10_01_24/source/ti/posix/ticlang" -DDeviceFamily_CC13X2 -DFLASH_ROM_BUILD -DNVOCMP_NWSAMEITEM=1 -DHEAPMGR_CONFIG=0x80 -DHEAPMGR_SIZE=0x0 -gdwarf-3 -march=armv7e-m -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"D:/CodeComposer/airtag_spoofer_CC1352P_2_LAUNCHXL_tirtos7_ticlang/Release/syscfg" -std=gnu9x $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


