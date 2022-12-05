FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# Temporary override, must be reported on meta-st-openstlinux
EXTRA_OEMAKE:remove = " TA_DEV_KIT_DIR=${STAGING_INCDIR}/optee/export-user_ta "

EXTRA_OEMAKE:append:stm32mp1common = " TA_DEV_KIT_DIR=${STAGING_INCDIR}/optee/export-user_ta "
EXTRA_OEMAKE:append:stm32mp2common = " TA_DEV_KIT_DIR=${STAGING_INCDIR}/optee/export-user_ta_arm64 "

