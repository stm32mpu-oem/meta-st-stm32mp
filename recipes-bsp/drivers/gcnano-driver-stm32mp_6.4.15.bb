SUMMARY = "GCNano kernel drivers"
LICENSE = "GPL-1.0-only & MIT"
# Note get md5sum with: $ head -n 53 Makefile | md5sum
LIC_FILES_CHKSUM = "file://Makefile;endline=53;md5=85a35fecd70aaa9c4047554f71e1407d"

LIC_FILES_CHKSUM:stm32mp1common = "file://Makefile;endline=53;md5=bdbf89578a2a891bdb74233b3587d75c"
SRC_URI:stm32mp1common = "git://github.com/STMicroelectronics/gcnano-binaries;protocol=https;branch=gcnano-6.4.13-binaries"
SRCREV:stm32mp1common = "5d02efd5cb4cfa85307633891f3cf87550a8bc1d"

SRC_URI:stm32mp2common = "git://github.com/stm32mpu-oem/gcnano-binaries;protocol=https;branch=main"
SRCREV:stm32mp2common = "548f9bf0a6388b9b44658aa77270b1e56d7c1381"

SRC_URI:append:stm32mp25revabcommon = " \
    file://0001-Limit-outstanding-to-1.patch \
    file://0002-NPU-limit-VIP-SRAM-to-first-half-part.patch \
"

SRC_URI:append:stm32mp25common = " \
    file://0001-MP2-Cut-2-for-4GB-allocation-capability.patch \
"

GCNANO_VERSION = "6.4.15"
GCNANO_VERSION:stm32mp1common = "6.4.13"

GCNANO_SUBVERSION:stm32mp1common = "stm32mp1"
GCNANO_SUBVERSION:stm32mp2common = "stm32mp2"
GCNANO_RELEASE = "r1"

PV = "${GCNANO_VERSION}-${GCNANO_SUBVERSION}-${GCNANO_RELEASE}"

S = "${WORKDIR}/git/${BPN}"

include gcnano-driver-stm32mp.inc

# ---------------------------------
# Configure archiver use
# ---------------------------------
include ${@oe.utils.ifelse(d.getVar('ST_ARCHIVER_ENABLE') == '1', 'gcnano-driver-stm32mp-archiver.inc','')}
