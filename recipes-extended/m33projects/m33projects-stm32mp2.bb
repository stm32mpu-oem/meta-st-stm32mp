SUMMARY = "STM32MP2 Firmware examples for CM33"
LICENSE = "Apache-2.0 & MIT & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://License.md;md5=69a1c7467bd46258aa1b1b534832ed7d"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "git://github.com/PRG-MPU-ALPHA/STM32CubeMP2.git;protocol=https;branch=main"
SRCREV  = "371de52d6cc9768eef097783302a4acfd55b0f18"

PV = "0.1.0"

S = "${WORKDIR}/git"

require recipes-extended/m33projects/m33projects.inc

PROJECTS_LIST_VALID = " \
    STM32MP257F-VALID3/Applications/OpenAMP/OpenAMP_TTY_echo/bin/OpenAMP_TTY_echo \
"

PROJECTS_LIST = "${PROJECTS_LIST_VALID}"

# Define default board reference for M33
M33_BOARDS += " STM32MP257F-VALID3 "
