inherit eutils

DESCRIPTION="poker hand evaluator"
HOMEPAGE="http://pokersource.org/poker-eval/"
MY_P="${PN}-${PV}.tar.gz"
SRC_URI="http://download.gna.org/pokersource/sources/${MY_P}"
SLOT="0"
LICENSE="GPL-2.1"
KEYWORDS="x86 amd64"
IUSE=""

DEPEND=""

src_unpack() {
	unpack ${MY_P}
	if ls ${FILESDIR}/${PVR}*.patch 2>/dev/null
		then
		for i in ${FILESDIR}/${PVR}*.patch
		  do
		  epatch $i
		done
	fi
}

src_install() {
	einstall INST_LOCATION="${D}/usr" || die "einstall failed"
}
