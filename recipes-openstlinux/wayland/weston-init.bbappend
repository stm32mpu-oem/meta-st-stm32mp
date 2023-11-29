FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://OpenSTLinux_background_1024x600.png"

do_install:append() {
    rm -f ${D}${datadir}/weston/backgrounds/ST13028_Linux_picto_11_1366x768.png
    install -m 0644 ${WORKDIR}/OpenSTLinux_background_1024x600.png ${D}${datadir}/weston/backgrounds/OpenSTLinux_background_1024x600.png
}
