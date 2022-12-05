SUMMARY = "Cert_create & Fiptool for fip generation for Trusted Firmware-A"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://license.rst;md5=1dd070c98a281d18d9eefd938729b031"

SRC_URI = "git://github.com/ARM-software/arm-trusted-firmware.git;protocol=https;nobranch=1"
#SRCREV corresponds to v2.6
SRCREV = "a1f02f4f3daae7e21ee58b4c93ec3e46b8f28d15"

SRC_URI += "file://0001-tools-stm32mp25-adaptation.patch \
            file://0002-fix-fiptool-respect-OPENSSL_DIR.patch \
            "

# Mandatory fix to allow feeding password through command line
SRC_URI += "file://0099-tools-allow-to-use-a-root-key-password-from-command-.patch"

DEPENDS += "dtc-native openssl"

S = "${WORKDIR}/git"

COMPATIBLE_HOST:class-target = "null"

EXTRA_OEMAKE:append:class-native = ' PLAT=stm32mp2 '
EXTRA_OEMAKE:append:class-nativesdk = ' PLAT=stm32mp2 '

HOSTCC:class-native = "${BUILD_CC}"
HOSTCC:class-nativesdk = "${CC}"
EXTRA_OEMAKE += "HOSTCC='${HOSTCC}' OPENSSL_DIR='${STAGING_EXECPREFIXDIR}'"
EXTRA_OEMAKE += "certtool fiptool"

do_configure[noexec] = "1"

do_compile:prepend() {
    # This is still needed to have the native fiptool executing properly by
    # setting the RPATH
    sed -e '/^LDLIBS/ s,$, \$\{BUILD_LDFLAGS},' \
        -e '/^INCLUDE_PATHS/ s,$, \$\{BUILD_CFLAGS},' \
        -i ${S}/tools/fiptool/Makefile
    # This is still needed to have the native cert_create executing properly by
    # setting the RPATH
    sed -e '/^LIB_DIR/ s,$, \$\{BUILD_LDFLAGS},' \
        -e '/^INC_DIR/ s,$, \$\{BUILD_CFLAGS},' \
        -i ${S}/tools/cert_create/Makefile
    # specific to stm32mp2
    cp -f ${S}/tools/st/plat_fiptool/plat_fiptool.mk ${S}/plat/st/stm32mp2/
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 \
        ${B}/tools/fiptool/fiptool \
        ${B}/tools/cert_create/cert_create \
        ${D}${bindir}
}

BBCLASSEXTEND += "native nativesdk"
