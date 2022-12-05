PACKAGE_ARCH = "${MACHINE_ARCH}"

CVE_PRODUCT = "arm:trusted_firmware-m"

inherit python3native deploy

# Baremetal and we bring a compiler below
INHIBIT_DEFAULT_DEPS = "1"

DEPENDS += "cmake-native \
            ninja-native \
            gcc-arm-none-eabi-native \
            python3-cbor2-native \
            python3-click-native \
            python3-cryptography-native \
            python3-pyasn1-native \
            python3-imgtool-native \
            python3-jinja2-native \
            python3-pyyaml-native \
"

S = "${WORKDIR}/git/tfm"
B = "${WORKDIR}/build"

# Build for debug (set TFM_DEBUG to 1 to activate)
TFM_DEBUG ?= "1"

# Platform must be set, ideally in the machine configuration.
TFM_PLATFORM ?= "stm/stm32mp257f_valid3"
python() {
    if not d.getVar("TFM_PLATFORM"):
        raise bb.parse.SkipRecipe("TFM_PLATFORM needs to be set")
}

PACKAGECONFIG ??= ""
# Whether to integrate the test suite
PACKAGECONFIG[test-secure] = "-DTEST_S=ON,-DTEST_S=OFF"
PACKAGECONFIG[test-nonsecure] = "-DTEST_NS=ON,-DTEST_NS=OFF"
PACKAGECONFIG[test-psa] = "-DTEST_PSA_API=ON,-DTEST_PSA_API=OFF"

# Currently we only support using the Arm binary GCC
EXTRA_OECMAKE += "-DTFM_TOOLCHAIN_FILE=${S}/toolchain_GNUARM.cmake"

# Don't let FetchContent download more sources during do_configure
EXTRA_OECMAKE += "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"

# Add platform parameters
EXTRA_OECMAKE += "-DTFM_PLATFORM=${TFM_PLATFORM}"

# Handle TFM_DEBUG parameter
EXTRA_OECMAKE += "${@bb.utils.contains('TFM_DEBUG', '1', '-DCMAKE_BUILD_TYPE=Debug', '', d)}"

# Verbose builds
EXTRA_OECMAKE += "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"

# Path to extra components
EXTRA_OECMAKE += "-DMBEDCRYPTO_PATH=${S}/../mbedtls -DTFM_TEST_REPO_PATH=${S}/../tf-m-tests -DMCUBOOT_PATH=${S}/../mcuboot"

# M33TD
EXTRA_OECMAKE += "${@bb.utils.contains('TFM_M33TDCID', '1', '-DSTM32_M33TDCID=ON', '-DSTM32_M33TDCID=OFF', d)}"

# TFM Profile: small
EXTRA_OECMAKE += "-DTFM_PROFILE=profile_small"

export CMAKE_BUILD_PARALLEL_LEVEL = "${@oe.utils.parallel_make(d, False)}"

# Let the Makefile handle setting up the CFLAGS and LDFLAGS as it is a standalone application
CFLAGS[unexport] = "1"
LDFLAGS[unexport] = "1"
AS[unexport] = "1"
LD[unexport] = "1"


# python3-cryptography needs the legacy provider, so set OPENSSL_MODULES to the
# right path until this is relocated automatically.
export OPENSSL_MODULES="${STAGING_LIBDIR_NATIVE}/ossl-modules"


# TF-M ships patches that it needs applied to mbedcrypto, so apply them
# as part of do_patch.
apply_local_patches() {
    cat ${S}/lib/ext/mbedcrypto/*.patch | patch -p1 -d ${S}/../mbedtls
}
do_patch[postfuncs] += "apply_local_patches"

do_configure[cleandirs] = "${B}"
do_configure() {
    export PATH=$PATH:${STAGING_DATADIR_NATIVE}/gcc-arm-none-eabi/bin
    cmake -GNinja -S ${S} -B ${B} ${EXTRA_OECMAKE} ${PACKAGECONFIG_CONFARGS}
}

# Invoke install here as there's no point in splitting compile from install: the
# first thing the build does is 'install' inside the build tree thus causing a
# rebuild. It also overrides the install prefix to be in the build tree, so you
# can't use the usual install prefix variables.
do_compile() {
    export PATH=$PATH:${STAGING_DATADIR_NATIVE}/gcc-arm-none-eabi/bin
    cmake --build ${B} -- install
}
do_compile[progress] = "outof:^\[(\d+)/(\d+)\]\s+"

do_install() {
    # TODO install headers and static libraries when we know how they're used
    # TODO: need to install only binary needed by platform
    install -d -m 755 ${D}/firmware
    install -m 0644 ${B}/bin/* ${D}/firmware/
}

FILES:${PN} = "/firmware"
SYSROOT_DIRS += "/firmware"

addtask deploy after do_install
do_deploy() {
    install -d ${DEPLOYDIR}/arm-trusted-firmware-m
    cp -rf ${D}/firmware/* ${DEPLOYDIR}/arm-trusted-firmware-m
}

# Build paths are currently embedded
INSANE_SKIP:${PN} += "buildpaths"
INSANE_SKIP:${PN} += "arch"
