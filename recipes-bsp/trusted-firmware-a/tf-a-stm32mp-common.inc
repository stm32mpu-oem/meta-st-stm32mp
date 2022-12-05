FILESEXTRAPATHS:prepend := "${THISDIR}/tf-a-stm32mp:"

SECTION = "bootloaders"

LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://license.rst;md5=1dd070c98a281d18d9eefd938729b031"

CVE_PRODUCT = "arm:trusted_firmware-a"

SRC_URI = "git://github.com/ARM-software/arm-trusted-firmware.git;protocol=https;branch=master"
SRCREV = "a1f02f4f3daae7e21ee58b4c93ec3e46b8f28d15"

SRC_URI += " \
     file://tf-a-st-ddr.tar.xz;subdir=git/;name=fw \
     file://0001-v2.6-stm32mp25-alpha-r1.patch \
     "

SRC_URI[fw.sha256sum] = "307e93f95fb979904f3701d614595af2"

TF_A_VERSION = "v2.6"
TF_A_SUBVERSION = "stm32mp25"
TF_A_RELEASE = "alpha-r1"
PV = "${TF_A_VERSION}-${TF_A_SUBVERSION}-${TF_A_RELEASE}"

ARCHIVER_ST_BRANCH = "${TF_A_VERSION}-${TF_A_SUBVERSION}"
ARCHIVER_ST_REVISION = "${PV}"
ARCHIVER_COMMUNITY_BRANCH = "master"
ARCHIVER_COMMUNITY_REVISION = "${TF_A_VERSION}"

S = "${WORKDIR}/git"

# ---------------------------------
# Configure devupstream class usage
# ---------------------------------
BBCLASSEXTEND = "devupstream:target"

SRC_URI:class-devupstream = "git://github.com/STMicroelectronics/arm-trusted-firmware.git;protocol=https;branch=${ARCHIVER_ST_BRANCH}"
SRCREV:class-devupstream = "0000000000000000000000000000000000000000"

# ---------------------------------
# Configure default preference to manage dynamic selection between tarball and github
# ---------------------------------
STM32MP_SOURCE_SELECTION ?= "tarball"

DEFAULT_PREFERENCE = "${@bb.utils.contains('STM32MP_SOURCE_SELECTION', 'github', '-1', '1', d)}"
