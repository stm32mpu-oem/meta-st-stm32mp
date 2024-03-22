SUMMARY = "STM32MP2 Firmware examples for CM33"
LICENSE = "Apache-2.0 & MIT & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://License.md;md5=69a1c7467bd46258aa1b1b534832ed7d"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI = "git://github.com/stm32mpu-oem/STM32CubeMP2.git;protocol=https;branch=main"
SRCREV  = "36ec6fc191fc4d9eef755224f7db60dff6788154"

SRC_URI += " \
    file://0001-Add-auto-generated-threadx-files.patch \
    "

PV = "0.3.0"

S = "${WORKDIR}/git"

require recipes-extended/m33projects/m33projects.inc

PROJECTS_LIST = " \
    STM32MP257F-EV1/Demonstrations/USBPD_DRP_UCSI \
"

# WARNING: You MUST put only one project on DEFAULT_COPRO_FIRMWARE per board
# If there is several project defined for the same board while you MUST have issue at runtime
# (not the correct project could be executed).
DEFAULT_COPRO_FIRMWARE = "STM32MP257F-EV1/Demonstrations/USBPD_DRP_UCSI"

# Define default board reference for M33
M33_BOARDS += " STM32MP257F-EV1 "
