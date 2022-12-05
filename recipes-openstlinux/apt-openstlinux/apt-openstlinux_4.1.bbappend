FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:stm32mp2common = "file://packages.openstlinux.st.com.list.stm32mp2common"

do_install:append:stm32mp2common() {
    install ${S}/packages.openstlinux.st.com.list.stm32mp2common ${D}/${sysconfdir}/apt/sources.list.d/packages.openstlinux.st.com.list
}
