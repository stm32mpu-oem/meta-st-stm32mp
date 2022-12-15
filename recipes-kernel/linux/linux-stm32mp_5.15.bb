SUMMARY = "Linux STM32MP Kernel"
SECTION = "kernel"
LICENSE = "GPL-2.0-only"
#LIC_FILES_CHKSUM = "file://COPYING;md5=bbea815ee2795b2f4230826c0c6b8814"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

include linux-stm32mp.inc

LINUX_VERSION = "5.15"
LINUX_SUBVERSION = "67"
LINUX_TARNAME = "linux-${LINUX_VERSION}.${LINUX_SUBVERSION}"
SRC_URI = "https://cdn.kernel.org/pub/linux/kernel/v5.x/${LINUX_TARNAME}.tar.xz;name=kernel"

SRC_URI[kernel.sha256sum] = "da47d9a80b694548835ccb553b6eb1a1f3f5d5cddd9e2bd6f4886b99ca14f940"

SRC_URI += " \
    file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0001-v5.15-stm32mp25-alpha-r1.patch \
    \
    file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0001-arm64-dts-st-remove-SDR104-and-HS200-modes-on-stm32m.patch \
    file://${LINUX_VERSION}/${LINUX_VERSION}.${LINUX_SUBVERSION}/0001-arm64-dts-st-increase-slew-rate-for-eth1-and-eth3-rg.patch \
    "

LINUX_TARGET = "stm32mp2"
LINUX_RELEASE = "alpha-r1"

PV = "${LINUX_VERSION}.${LINUX_SUBVERSION}-${LINUX_TARGET}-${LINUX_RELEASE}"

ARCHIVER_ST_BRANCH = "v${LINUX_VERSION}-${LINUX_TARGET}"
ARCHIVER_ST_REVISION = "v${LINUX_VERSION}-${LINUX_TARGET}-${LINUX_RELEASE}"
ARCHIVER_COMMUNITY_BRANCH = "linux-${LINUX_VERSION}.y"
ARCHIVER_COMMUNITY_REVISION = "v${LINUX_VERSION}.${LINUX_SUBVERSION}"

S = "${WORKDIR}/linux-${LINUX_VERSION}.${LINUX_SUBVERSION}"


# ---------------------------------
# Configure devupstream class usage
# ---------------------------------
BBCLASSEXTEND = "devupstream:target"

SRC_URI:class-devupstream = "git://github.com/STMicroelectronics/linux.git;protocol=https;branch=${ARCHIVER_ST_BRANCH}"
SRCREV:class-devupstream = "0000000000000000000000000000000000000000"

# ---------------------------------
# Configure default preference to manage dynamic selection between tarball and github
# ---------------------------------
STM32MP_SOURCE_SELECTION ?= "tarball"

DEFAULT_PREFERENCE = "${@bb.utils.contains('STM32MP_SOURCE_SELECTION', 'github', '-1', '1', d)}"

# ---------------------------------
# Configure archiver use
# ---------------------------------
include ${@oe.utils.ifelse(d.getVar('ST_ARCHIVER_ENABLE') == '1', 'linux-stm32mp-archiver.inc','')}

# -------------------------------------------------------------
# Defconfig
#
KERNEL_DEFCONFIG        = "defconfig"
KERNEL_CONFIG_FRAGMENTS:stm32mp1common = "${@bb.utils.contains('KERNEL_DEFCONFIG', 'defconfig', '${S}/arch/arm/configs/fragment-01-multiv7_cleanup.config', '', d)}"
KERNEL_CONFIG_FRAGMENTS:stm32mp1common += "${@bb.utils.contains('KERNEL_DEFCONFIG', 'defconfig', '${S}/arch/arm/configs/fragment-02-multiv7_addons.config', '', d)}"
KERNEL_CONFIG_FRAGMENTS:stm32mp1common += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '${WORKDIR}/fragments/${LINUX_VERSION}/fragment-03-systemd.config', '', d)} "
KERNEL_CONFIG_FRAGMENTS:stm32mp1common += "${WORKDIR}/fragments/${LINUX_VERSION}/fragment-04-modules.config"
KERNEL_CONFIG_FRAGMENTS:stm32mp1common += "${@oe.utils.ifelse(d.getVar('KERNEL_SIGN_ENABLE') == '1', '${WORKDIR}/fragments/${LINUX_VERSION}/fragment-05-signature.config','')} "
KERNEL_CONFIG_FRAGMENTS:stm32mp1common += "${@bb.utils.contains('MACHINE_FEATURES', 'nosmp', '${WORKDIR}/fragments/${LINUX_VERSION}/fragment-06-smp.config', '', d)} "


KERNEL_CONFIG_FRAGMENTS:stm32mp2common = "${@bb.utils.contains('KERNEL_DEFCONFIG', 'defconfig', '${S}/arch/arm64/configs/fragment-01-defconfig-cleanup.config', '', d)}"
KERNEL_CONFIG_FRAGMENTS:stm32mp2common += "${@bb.utils.contains('KERNEL_DEFCONFIG', 'defconfig', '${S}/arch/arm64/configs/fragment-02-defconfig-addons.config', '', d)}"
KERNEL_CONFIG_FRAGMENTS:stm32mp2common += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '${WORKDIR}/fragments/${LINUX_VERSION}/fragment-03-systemd.config', '', d)} "
KERNEL_CONFIG_FRAGMENTS:stm32mp2common += "${WORKDIR}/fragments/${LINUX_VERSION}/fragment-04-modules.config"
KERNEL_CONFIG_FRAGMENTS:stm32mp2common += "${@oe.utils.ifelse(d.getVar('KERNEL_SIGN_ENABLE') == '1', '${WORKDIR}/fragments/${LINUX_VERSION}/fragment-05-signature.config','')} "
KERNEL_CONFIG_FRAGMENTS:stm32mp2common += "${@bb.utils.contains('MACHINE_FEATURES', 'nosmp', '${WORKDIR}/fragments/${LINUX_VERSION}/fragment-06-smp.config', '', d)} "


SRC_URI += "file://${LINUX_VERSION}/fragment-03-systemd.config;subdir=fragments"
SRC_URI += "file://${LINUX_VERSION}/fragment-04-modules.config;subdir=fragments"
SRC_URI += "file://${LINUX_VERSION}/fragment-05-signature.config;subdir=fragments"
SRC_URI += "file://${LINUX_VERSION}/fragment-06-smp.config;subdir=fragments"

# Don't forget to add/del for devupstream
SRC_URI:class-devupstream += "file://${LINUX_VERSION}/fragment-03-systemd.config;subdir=fragments"
SRC_URI:class-devupstream += "file://${LINUX_VERSION}/fragment-04-modules.config;subdir=fragments"
SRC_URI:class-devupstream += "file://${LINUX_VERSION}/fragment-05-signature.config;subdir=fragments"
SRC_URI:class-devupstream += "file://${LINUX_VERSION}/fragment-06-smp.config;subdir=fragments"

# -------------------------------------------------------------
# Kernel Args
#
KERNEL_EXTRA_ARGS += "LOADADDR=${ST_KERNEL_LOADADDR}"
