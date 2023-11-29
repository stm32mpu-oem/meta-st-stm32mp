FILESEXTRAPATHS:append := "${THISDIR}/${PN}:"

SRC_URI:append:stm32mp2common = " file://launch_camera_preview_mp25.sh file://check_camera_preview_mp25.sh"
SRC_URI:append:stm32mp2common = " file://011-camera_mp25.yaml"

# sha256sum isp
# 9b7699ba3c1179b378084188e29ed4a3198aec4dce3de9b60fe4896d7c0cd3c4  isp
SRC_URI:append:stm32mp2common = " file://isp"

LICENSE:stm32mp2common = "BSD-3-Clause & Proprietary"
LIC_FILES_CHKSUM:append::stm32mp2common = " file://${OPENSTLINUX_BASE}/files/licenses/ST-Proprietary;md5=7cb1e55a9556c7dd1a3cae09db9cc85f "

do_install:append:stm32mp2common() {
    # install isp utility
    install -m 0755 ${WORKDIR}/isp ${D}${prefix}/local/demo/application/camera/bin
}
