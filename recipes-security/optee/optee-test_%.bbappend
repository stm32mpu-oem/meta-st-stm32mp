FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# Temporary override, must be reported on meta-st-openstlinux
#TA_DEV_KIT_DIR:stm32mp1common = "${STAGING_INCDIR}/optee/export-user_ta_arm32"
TA_DEV_KIT_DIR:stm32mp2common = "${STAGING_INCDIR}/optee/export-user_ta_arm64"

