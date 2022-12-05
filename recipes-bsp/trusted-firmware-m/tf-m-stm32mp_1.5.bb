SUMMARY = "Trusted Firmware for Cortex-M"
DESCRIPTION = "Trusted Firmware-M"
HOMEPAGE = "https://git.trustedfirmware.org/trusted-firmware-m.git"
PROVIDES = "virtual/trusted-firmware-m"

LICENSE = "BSD-3-Clause & Apache-2.0"

LIC_FILES_CHKSUM = "file://license.rst;md5=07f368487da347f3c7bd0fc3085f3afa"

SRC_URI = "git://git.trustedfirmware.org/TF-M/trusted-firmware-m.git;protocol=http;branch=${SRCBRANCH_tfm};name=tfm;destsuffix=git/tfm \
            git://git.trustedfirmware.org/TF-M/tf-m-tests.git;protocol=http;branch=${SRCBRANCH_tfm_test};name=tfm-tests;destsuffix=git/tf-m-tests \
            git://github.com/ARMmbed/mbedtls.git;protocol=https;branch=master;name=mbedtls;destsuffix=git/mbedtls \
            git://github.com/mcu-tools/mcuboot.git;protocol=https;branch=main;name=mcuboot;destsuffix=git/mcuboot \
"

# The required dependencies are documented in tf-m/config/config_default.cmake
# TF-Mv1.6.0
SRCBRANCH_tfm = "release/1.6.x"
SRCREV_tfm = "6fb14a14140f94150f959c88e3b880f48372da06"
# mbedtls-3.0.0
SRCREV_mbedtls = "8df2f8e7b9c7bb9390ac74bb7bace27edca81a2b"
# TF-Mv1.5.0++
SRCBRANCH_tfm_test = "release/1.6.x"
SRCREV_tfm-tests = "c7d80689b93398a4e14800c97223cea5edb6edf1"
# v1.8.0
SRCREV_mcuboot = "29099e1d17f93ae1d09fe945ad191b703aacd3d8"

SRCREV_FORMAT = "tfm"

UPSTREAM_CHECK_GITTAGREGEX = "^TF-Mv(?P<pver>\d+(\.\d+)+)$"

# ST Patches
SRC_URI += " \
    file://0001-v1.5.0-stm32mp25-r3.patch \
"

TF_M_VERSION = "v1.5.0"
TF_M_SUBVERSION = "stm32mp25"
TF_M_RELEASE = "r3"
PV = "${TF_M_VERSION}-${TF_M_SUBVERSION}-${TF_M_RELEASE}"

include tf-m-stm32mp-common.inc

