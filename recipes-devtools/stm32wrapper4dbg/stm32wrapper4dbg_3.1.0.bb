SUMMARY = "Wrapper for FSBL to debug TF-A U-Boot and bare metal on STM32MP"
SECTION = "devel"
LICENSE = "GPL-2.0-or-later | BSD-3-Clause"
LIC_FILES_CHKSUM = "file://COPYING;md5=7c996e24cb10a869efb08b521b20242f"

SRC_URI = "git://github.com/stm32mpu-oem/stm32wrapper4dbg;protocol=https;branch=main"
SRCREV = "6b14f9d5b54615d974f583308e62c670d8ef60d5"

S = "${WORKDIR}/git"

BBCLASSEXTEND += "native nativesdk"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/stm32wrapper4dbg -t ${D}${bindir}
}
