FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

PV = "1.2"

do_install:stm32mpcommon() {
	mkdir -p ${D}${sysconfdir}/st-info.d
	touch ${D}${sysconfdir}/st-info.d/graphics-${PV}
	printf "LIBGLES1=${PREFERRED_PROVIDER_virtual/libgles1}\n" > ${D}${sysconfdir}/st-info.d/graphics-${PV}
}

