FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:stm32mp25common = " \
    file://stm32mp25.html \
    file://fps.chart.py \
    "

do_install:append:stm32mp25common() {
    install -d ${D}${datadir}/netdata/web

    install -m 0644 ${WORKDIR}/stm32mp25.html ${D}${datadir}/netdata/web/

    install -d ${D}${libexecdir}/netdata/python.d/
    install -m 0644 ${WORKDIR}/fps.chart.py ${D}${libexecdir}/netdata/python.d/
}

SRC_URI:append:stm32mp25revabcommon = " \
    file://stm32mp25.html \
    file://fps.chart.py \
    "

do_install:append:stm32mp25revabcommon() {
    install -d ${D}${datadir}/netdata/web

    install -m 0644 ${WORKDIR}/stm32mp25.html ${D}${datadir}/netdata/web/

    install -d ${D}${libexecdir}/netdata/python.d/
    install -m 0644 ${WORKDIR}/fps.chart.py ${D}${libexecdir}/netdata/python.d/
}
