
COMPATIBLE_MACHINE = "(stm32mp2common)"

PACKAGE_ARCH = "${MACHINE_ARCH}"

B = "${S}"
# For external source support with devtools
EXTERNALSRC_BUILD:pn-${PN} = "${S}"

DEPENDS += "copro-sdk-native virtual/optee-os tf-m-stm32mp"
# for optee script
DEPENDS += "python3-pyelftools-native python3-pycryptodomex-native"

# Default service for systemd
inherit systemd update-rc.d python3native
SRC_URI += "file://st-m33firmware-load-default.sh"
SRC_URI += "file://st-m33firmware-load.service"
SRC_URI += "file://shutdown-stm32mp2-m33.sh"

# Create specific userfs package
M33_PACKAGE_4USERFS ?= "1"
PACKAGES += "${@bb.utils.contains('M33_PACKAGE_4USERFS', '1', '${PN}-userfs', '', d)}"

# Define default board reference for M33
M33_BOARDS ?= ""

# Init M33 board service install
M33_BOARDS_SERVICE ?= "1"

# Init default copro example to load/execute
DEFAULT_COPRO_FIRMWARE ?= ""

BUILD_CONFIG ?= "Debug"

# Init default installation path
M33_FOLDER ?= "Cube-M33-examples"
M33_INSTALLDIR ?= "${prefix}local"
M33_OUTPUT_4USERFS = "${M33_INSTALLDIR}/${M33_FOLDER}"

export TA_DEV_KIT_DIR="${STAGING_INCDIR}/optee/export-user_ta_arm64"
export SBOOTADDR="0x80000000"
export NSBOOTADDR="0x80100000"

do_install() {
    # Install M33 firmwares, scripts and README in userfs:
    # <userfs>/Cube-M33-examples/${project} (ie STM32MP257F-VALID3/Applications/OpenAMP/OpenAMP_TTY_echo)
    #                        |-- fw_cortex_m33.sh
    #                        |-- README
    #                        |--lib
    #                             |--firmware
    #                                  |-- ELF file for impacted board (ie OpenAMP_TTY_echo_CM33_NonSecure.elf)
    for BIN_MACHINE in ${M33_BOARDS}; do
        for project in ${PROJECTS_LIST} ; do
            if [ "$(echo ${project} | cut -d'/' -f1)" = "${BIN_MACHINE}" ]; then
                BIN_NAME=$(basename ${project})
                PROJ_DIR=$(dirname ${project})
                INSTALL_PROJ=$(echo ${project} | sed 's,/bin/.*,,')
                BIN_NAME_NS=$(echo $BIN_NAME"_CM33_NonSecure")
                BIN_NAME_S=$(echo $BIN_NAME"_CM33_Secure")
                BIN_NAME_NS_S=$(echo $BIN_NAME"_CM33_NonSecure")


                # Install M33 firmwares
                install -d ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}/lib/firmware/
                install -m 0755 ${B}/Projects/${PROJ_DIR}/${BIN_NAME_NS}.elf ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}/lib/firmware/

                # Install sh and README files if any for each example
                if [ -e ${B}/Projects/${PROJ_DIR}/fw_cortex_m33.sh ]; then
                    install -m 0755 ${B}/Projects/${PROJ_DIR}/fw_cortex_m33.sh ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}
                fi
                if [ -e ${S}/Projects/${project}/Remoteproc/README ]; then
                    install -m 0755 ${S}/Projects/${project}/Remoteproc/README ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}
                fi

                # generate signed binaries
                # generate ${BIN_NAME_NS}_sign.bin2
                ${STAGING_BINDIR_NATIVE}/st_copro_firmware_signature.sh \
                        --input-nsecure ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}/lib/firmware/${BIN_NAME_NS}.elf \
                        --signature-key ${TA_DEV_KIT_DIR}/keys/default_rproc.pem \
                        --output ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}/lib/firmware/${BIN_NAME_S}
                # generate ${BIN_NAME_NS_S}_tfm_sign.bin
                ${STAGING_BINDIR_NATIVE}/st_copro_firmware_signature.sh \
                        --input-nsecure ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}/lib/firmware/${BIN_NAME_NS}.elf \
                        --input-secure ${RECIPE_SYSROOT}/firmware/tfm_s.elf \
                        --signature-key ${TA_DEV_KIT_DIR}/keys/default_rproc.pem \
                        --output ${D}${M33_OUTPUT_4USERFS}/${INSTALL_PROJ}/lib/firmware/${BIN_NAME_NS_S}
             fi
         done
    done

    if [ "${M33_BOARDS_SERVICE}" -eq 1 ]; then
        # Install sh script to load/execute DEFAULT_COPRO_FIRMWARE @boot up
        if [ -n "${DEFAULT_COPRO_FIRMWARE}" ]; then
            # Install systemd service
            install -d ${D}${sysconfdir}/init.d/
            install -d ${D}${base_sbindir}/
            install -m 0755 ${WORKDIR}/st-m33firmware-load-default.sh ${D}${sysconfdir}/init.d/st-m33firmware-load-default.sh
            install -m 0755 ${WORKDIR}/st-m33firmware-load-default.sh ${D}${base_sbindir}/st-m33firmware-load-default.sh
            install -m 0755 ${WORKDIR}/shutdown-stm32mp2-m33.sh ${D}${base_sbindir}/shutdown-stm32mp2-m33.sh

            sed -i -e "s:@default_fw@:${DEFAULT_COPRO_FIRMWARE}:" \
            ${D}${sysconfdir}/init.d/st-m33firmware-load-default.sh
            sed -i -e "s:@default_fw@:${DEFAULT_COPRO_FIRMWARE}:" \
            ${D}${base_sbindir}/st-m33firmware-load-default.sh

            sed -i -e "s:@userfs_mount_point@:${M33_OUTPUT_4USERFS}:" \
            ${D}${sysconfdir}/init.d/st-m33firmware-load-default.sh
            sed -i -e "s:@userfs_mount_point@:${M33_OUTPUT_4USERFS}:" \
            ${D}${base_sbindir}/st-m33firmware-load-default.sh
        fi

        # Install systemd service for all machines configurations
        if ${@bb.utils.contains('DISTRO_FEATURES','systemd','true','false',d)}; then
            install -d ${D}${systemd_unitdir}/system
            install -d ${D}${systemd_unitdir}/system-shutdown
            install -m 644 ${WORKDIR}/st-m33firmware-load.service ${D}/${systemd_unitdir}/system
            install -m 755 ${WORKDIR}/shutdown-stm32mp2-m33.sh ${D}/${systemd_unitdir}/system-shutdown
        fi
    fi
}



# -----------------------------------------------------------
# specific for service: start copro m33 firwmare at boot time
SYSTEMD_PACKAGES += " m33projects-stm32mp2 "
SYSTEMD_SERVICE:${PN} = "st-m33firmware-load.service"
SYSTEMD_AUTO_ENABLE:${PN} = "disable"

INITSCRIPT_NAME = "st-m33firmware-load-default.sh"
INITSCRIPT_PARAMS = "stop 22 5 3 ."
# -----------------------------------------------------------

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"

FILES:${PN} += "${nonarch_base_libdir}/firmware ${sysconfdir}/init.d ${systemd_unitdir}/system ${systemd_unitdir}/system-shutdown"
# Configure package split
FILES:${PN} += "${@bb.utils.contains('M33_PACKAGE_4USERFS', '1', '', '${M33_OUTPUT_4USERFS}', d)}"
FILES:${PN}-userfs = "${@bb.utils.contains('M33_PACKAGE_4USERFS', '1', '${M33_OUTPUT_4USERFS}', '', d)}"
RDEPENDS:${PN}-userfs += "busybox"

# Avoid QA issue because binaries are for an Arm architecture but the platform is an AArch64
INSANE_SKIP = "arch"
