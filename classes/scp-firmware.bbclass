# Enable use of scp-firmware shared folder
STAGING_SCPFW_DIR = "${TMPDIR}/work-shared/${MACHINE}/scp-firmware"

do_compile[depends] += "scp-firmware:do_configure"
