require gcnano-userland.inc

LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://${GCNANO_TAR_FILENAME}/LICENSE;md5=0e85fa534ee3a1a7f5ccc65cd6e789f9"

DEPENDS += " libdrm wayland "

LIC_FILES_CHKSUM:stm32mp1common = "file://${GCNANO_TAR_FILENAME}/LICENSE;md5=0e85fa534ee3a1a7f5ccc65cd6e789f9"
SRC_URI:stm32mp1common = "git://github.com/STMicroelectronics/gcnano-binaries;protocol=https;branch=gcnano-6.4.13-binaries"
SRCREV:stm32mp1common = "5d02efd5cb4cfa85307633891f3cf87550a8bc1d"

SRC_URI:stm32mp2common = "git://github.com/PRG-MPU-ALPHA/gcnano-binaries;protocol=https;branch=main"
SRCREV:stm32mp2common = "548f9bf0a6388b9b44658aa77270b1e56d7c1381"

PV = "${GCNANO_VERSION}+${GCNANO_DATE}"

GCNANO_VERSION = "6.4.15"
GCNANO_VERSION:stm32mp1common = "6.4.13"

GCNANO_DATE:stm32mp1common = "20230517"
GCNANO_DATE:stm32mp2common = "20231127"

GCNANO_PACKAGECONFIG = "egl gbm glesv1 glesv2 vg"

GCNANO_PACKAGECONFIG:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'openvx', 'vx', '', d)}"
GCNANO_PACKAGECONFIG:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'opencl', 'cl', '', d)}"
GCNANO_PACKAGECONFIG:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'vulkan', 'vulkan', '', d)}"

S = "${WORKDIR}/git"
B = "${S}/${GCNANO_TAR_FILENAME}"

#------------------------------------------
# Overwrite of do_unpack to untar the tarball
python() {
    # Make sure that we're dealing with recipe that enables externalsrc class
    if bb.data.inherits_class('externalsrc', d):
        bb.build.addtask('gcnano_userland_binaries_unpack', 'do_populate_lic', None, d)
        bb.build.addtask('gcnano_userland_binaries_unpack', 'do_configure', None, d)
}

do_gcnano_userland_binaries_unpack[depends] += "xz-native:do_populate_sysroot"
do_gcnano_userland_binaries_unpack() {
    cd ${S}
    rm -rf ${GCNANO_TAR_FILENAME}
    if [ -f "${GCNANO_TAR_FILENAME}.bin" ]; then
        sh ${GCNANO_TAR_FILENAME}.bin --auto-accept
    else
        bbfatal "Missing '${GCNANO_TAR_FILENAME}.bin' file in ${S} folder."
    fi
}

do_unpack[depends] += "xz-native:do_populate_sysroot"
python do_unpack() {
    eula = d.getVar('ACCEPT_EULA_'+d.getVar('MACHINE'))
    eula_file = d.getVar('EULA_FILE_ST')
    machine = d.getVar('MACHINE')
    pkg = d.getVar('PN')
    if eula == None:
        bb.fatal("To use '%s' you need to accept the STMicroelectronics EULA at '%s'. "
                 "Please read it and in case you accept it, write: "
                 "ACCEPT_EULA_%s = \"1\" in your local.conf." % (pkg, eula_file, machine))
    elif eula == '0':
        bb.fatal("To use '%s' you need to accept the STMicroelectronics EULA." % pkg)
    else:
        bb.note("STMicroelectronics EULA has been accepted for '%s'" % pkg)

    try:
        externalsrc = d.getVar('EXTERNALSRC')
        if not externalsrc:
            bb.build.exec_func('base_do_unpack', d)
            bb.build.exec_func('do_gcnano_userland_binaries_unpack', d)
    except:
        raise
}
do_unpack[vardepsexclude] += "EULA_FILE_ST"
