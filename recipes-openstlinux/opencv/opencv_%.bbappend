PACKAGECONFIG:append = " ${@bb.utils.contains("DISTRO_FEATURES", "opencl", "opencl", "", d)}"
