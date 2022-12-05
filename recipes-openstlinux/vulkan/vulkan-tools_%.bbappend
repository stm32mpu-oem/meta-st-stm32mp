# Build vkcube from vulkan-tools
EXTRA_OECMAKE:remove = "-DBUILD_CUBE=OFF"
EXTRA_OECMAKE:append = " -DBUILD_CUBE=ON"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append = " file://0001-Enable-vkcube-cross-compilation.patch"

# glslang required to build vkcube shaders
DEPENDS:append = " glslang-native"
EXTRA_OECMAKE:append = " -DGLSLANG_INSTALL_DIR=${STAGING_DIR_NATIVE}"

# Force winsys to Wayland
EXTRA_OECMAKE:append = " -DCUBE_WSI_SELECTION=WAYLAND"

# pkgconfig needed by cmake to look for dependencies (wayland-native and wayland-protocols added)
inherit pkgconfig
PACKAGECONFIG[wayland] = "-DBUILD_WSI_WAYLAND_SUPPORT=ON, -DBUILD_WSI_WAYLAND_SUPPORT=OFF, wayland wayland-native wayland-protocols"
