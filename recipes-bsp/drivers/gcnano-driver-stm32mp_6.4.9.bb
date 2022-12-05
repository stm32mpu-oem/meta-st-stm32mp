SUMMARY = "GCNano kernel drivers"
LICENSE = "GPL-1.0-only & MIT"
# Note get md5sum with: $ head -n 53 Makefile | md5sum
LIC_FILES_CHKSUM = "file://Makefile;endline=53;md5=ec60a845a9f75c1c29ffe3007e9d2cb5"

SRC_URI = "git://github.com/PRG-MPU-ALPHA/gcnano-binaries.git;protocol=https;branch=main"
SRCREV = "b6ba29a9719c5d4fe62c66344dabaa8c78afc48f"

GCNANO_TARNAME = "gcnano-driver-${GCNANO_VERSION}"

GCNANO_VERSION = "6.4.9"
GCNANO_SUBVERSION:stm32mp1common = "stm32mp1"
GCNANO_SUBVERSION:stm32mp2common = "stm32mp2"
GCNANO_RELEASE = "r1"

PV = "${GCNANO_VERSION}-${GCNANO_SUBVERSION}-${GCNANO_RELEASE}"

S = "${WORKDIR}/gcnano-driver-${GCNANO_VERSION}"

include gcnano-driver-stm32mp.inc

GCNANO_DRIVER_TARBALL = "${GCNANO_TARNAME}.tar.xz"

DEPENDS += "xz-native"
do_unpack[depends] += "xz-native:do_populate_sysroot"

python () {
    externalsrc = d.getVar('EXTERNALSRC')
    if not externalsrc:
        d.prependVarFlag('do_unpack', 'postfuncs', " do_patch_extract_tarball ")
}

# add to do_patch for untar the tarball
do_patch_extract_tarball() {
    bbnote "tar xf ${WORKDIR}/git/${GCNANO_DRIVER_TARBALL} -C ${WORKDIR}"
    tar xf ${WORKDIR}/git/${GCNANO_DRIVER_TARBALL} -C ${WORKDIR}
}

# ---------------------------------
# Configure archiver use
# ---------------------------------
include ${@oe.utils.ifelse(d.getVar('ST_ARCHIVER_ENABLE') == '1', 'gcnano-driver-stm32mp-archiver.inc','')}
